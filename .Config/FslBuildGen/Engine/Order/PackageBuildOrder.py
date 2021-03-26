#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#****************************************************************************************************************************************************
# Copyright 2020 NXP
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#    * Redistributions of source code must retain the above copyright notice,
#      this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above copyright notice,
#      this list of conditions and the following disclaimer in the documentation
#      and/or other materials provided with the distribution.
#
#    * Neither the name of the NXP. nor the names of
#      its contributors may be used to endorse or promote products derived from
#      this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
# OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#****************************************************************************************************************************************************

from typing import Dict
from typing import List
from typing import Optional
from typing import Tuple
import difflib
from FslBuildGen.DataTypes import AccessType
from FslBuildGen.DataTypes import PackageType
from FslBuildGen.Engine.Order.DependencyGraph import DependencyGraph
from FslBuildGen.Engine.Order.DependencyGraph import DependencyGraphNode
from FslBuildGen.Engine.Order.EvaluationPackage import EvaluationPackage
from FslBuildGen.Engine.Order.EvaluationPackage import FlavorInfo
from FslBuildGen.Engine.Order.Exceptions import PackageDependencyNotFoundException
from FslBuildGen.Engine.Order.Exceptions import PackageFlavorOptionDependencyNotFoundException
from FslBuildGen.Engine.PackageFlavorName import PackageFlavorName
from FslBuildGen.Engine.PackageFlavorOptionName import PackageFlavorOptionName
from FslBuildGen.Engine.Unresolved.UnresolvedBasicPackage import UnresolvedBasicPackage
from FslBuildGen.Engine.Unresolved.UnresolvedPackageName import UnresolvedPackageName
from FslBuildGen.Engine.Unresolved.UnresolvedPackageDependency import UnresolvedPackageDependency
from FslBuildGen.Engine.Unresolved.UnresolvedPackageFlavor import UnresolvedPackageFlavor
from FslBuildGen.Engine.Unresolved.UnresolvedPackageFlavorOption import UnresolvedPackageFlavorOption
from FslBuildGen.Exceptions import GroupedException
from FslBuildGen.Log import Log
from FslBuildGen.PackageConfig import PackageNameMagicString

class LocalVerbosityLevel(object):
    Info = 3
    Debug = 4
    Trace = 5

class ResolveDepRecord(object):
    def __init__(self, unresolved: UnresolvedBasicPackage, resolved: EvaluationPackage) -> None:
        super().__init__()
        self.Unresolved = unresolved
        self.Resolved = resolved


class ConstraintOptionRecord(object):
    def __init__(self, optionName: PackageFlavorOptionName, dependencyStack: List[UnresolvedPackageName]) -> None:
        super().__init__()
        self.OptionName = optionName
        self.DependencyStack = dependencyStack

class ConstraintRecord(object):
    def __init__(self) -> None:
        super().__init__()
        self.LookupDict = dict() # type: Dict[PackageFlavorOptionName, List[ConstraintOptionRecord]]


class PackageBuildOrder(object):

    @staticmethod
    def ResolveBuildOrder(log: Log, allPackages: List[UnresolvedBasicPackage]) -> List[UnresolvedBasicPackage]:
        log.LogPrintVerbose(LocalVerbosityLevel.Info, "Resolve build order")
        log.PushIndent()
        try:

            finalBuildOrder = [] # type: List[UnresolvedBasicPackage]
            if len(allPackages) <= 0:
                log.LogPrintVerbose(LocalVerbosityLevel.Info, "No packages supplied")
                finalBuildOrder.append(UnresolvedBasicPackage.Create2(UnresolvedPackageName(PackageNameMagicString.TopLevelName), PackageType.TopLevel))
            else:
                evaluationPackages = PackageBuildOrder.__ResolveEvaluationPackages(log, allPackages) # type: List[EvaluationPackage]

                PackageBuildOrder.__ValidateDependencies(log, evaluationPackages)

                log.LogPrintVerbose(LocalVerbosityLevel.Info, "Determining build order for {0} packages".format(len(allPackages)))

                graph, topLevelNode = PackageBuildOrder.__CreateDependencyGraph(log, evaluationPackages)

                buildOrder = graph.DetermineBuildOrder(topLevelNode.Source)
                for entry in buildOrder:
                    finalBuildOrder.append(entry.SourcePackage)
                finalBuildOrder.append(topLevelNode.Source.SourcePackage)

            PackageBuildOrder.__ValidateConstraints(log, finalBuildOrder)
            return finalBuildOrder
        finally:
            log.PopIndent()

    @staticmethod
    def __ValidateDependencies(log: Log, evaluationPackages: List[EvaluationPackage]) -> None:
        log.LogPrintVerbose(LocalVerbosityLevel.Info, "Validating that the dependencies are acyclic")
        exceptionList = None # type: Optional[List[Exception]]
        for package in evaluationPackages:
            try:
                PackageBuildOrder.__ValidateDependenciesFor(package)
            except Exception as ex: # pylint: disable=broad-except
                exceptionList = PackageBuildOrder.__AddException(exceptionList, ex)
                break   # for now just break on the first exception

        PackageBuildOrder.__CheckForExceptions(exceptionList)


    @staticmethod
    def __ValidateDependenciesFor(package: EvaluationPackage) -> None:
        allPackages = PackageBuildOrder.__ExtractTouchedPackageList(package) # type: List[EvaluationPackage]
        graph = DependencyGraph(allPackages)
        graph.DetermineBuildOrder(package)


    @staticmethod
    def __ExtractTouchedPackageList(startPackage: EvaluationPackage) -> List[EvaluationPackage]:
        packageDict = dict()    # type: Dict[UnresolvedPackageName, EvaluationPackage]
        packageDict[startPackage.Name] = startPackage

        packageQueue = []  # type: List[EvaluationPackage]
        packageQueue.append(startPackage)

        while len(packageQueue) > 0:
            currentPackage = packageQueue.pop(0)
            for depRecord in currentPackage.DirectDependencies:
                if depRecord.Package.Name not in packageDict:
                    packageDict[depRecord.Package.Name] = depRecord.Package
                    packageQueue.append(depRecord.Package)
        return list(packageDict.values())

    @staticmethod
    def __ResolveEvaluationPackages(log: Log, allPackages: List[UnresolvedBasicPackage]) -> List[EvaluationPackage]:
        # Build lookup dict
        packageNameToNodeDict = PackageBuildOrder.__CreatePackageNameToPackageDict(log, allPackages)

        log.LogPrintVerbose(LocalVerbosityLevel.Info, "Building evaluation packages")
        # then resolve all direct dependencies
        res = [] # type: List[EvaluationPackage]
        exceptionList = None # type: Optional[List[Exception]]
        for record in packageNameToNodeDict.values():
            package = record.Unresolved

            #packageDependencies = [] # type: List[EvaluationPackage]
            for dep in package.DirectDependencies:
                if dep.Name not in packageNameToNodeDict:
                    exceptionList = PackageBuildOrder.__AddException(exceptionList, PackageBuildOrder.__CreatePackageDependencyNotFoundException(package, dep, allPackages))
                else:
                    depRecord = packageNameToNodeDict[dep.Name] # type: ResolveDepRecord
                    record.Resolved.DirectDependencies.append(EvaluationPackage.DependencyRecord(depRecord.Resolved, None))

            for flavor in package.Flavors:
                for flavorOption in flavor.Options:
                    for dep in flavorOption.DirectDependencies:
                        if dep.Name not in packageNameToNodeDict:
                            exceptionList = PackageBuildOrder.__AddException(exceptionList, PackageBuildOrder.__CreatePackageFlavorOptionDependencyNotFoundException(package, flavor, flavorOption, dep, allPackages))
                        else:
                            depRecord = packageNameToNodeDict[dep.Name]
                            record.Resolved.DirectDependencies.append(EvaluationPackage.DependencyRecord(depRecord.Resolved, FlavorInfo(flavor.Name, flavorOption.Name)))

            record.Resolved.Seal()
            res.append(record.Resolved)

        PackageBuildOrder.__CheckForExceptions(exceptionList)
        return res


    @staticmethod
    def __CreatePackageNameToPackageDict(log: Log, allPackages: List[UnresolvedBasicPackage]) -> Dict[UnresolvedPackageName, ResolveDepRecord]:
        log.LogPrintVerbose(LocalVerbosityLevel.Info, "Building package lookup dict")

        exceptionList = None # type: Optional[List[Exception]]

        # Build lookup dict
        packageNameToPackageDict = dict() # type: Dict[UnresolvedPackageName, ResolveDepRecord]
        for package in allPackages:
            if package.Name in packageNameToPackageDict:
                exceptionList = PackageBuildOrder.__AddException(exceptionList, Exception("PackageName '{0}' defined multiple times".format(package.Name)))

            packageNameToPackageDict[package.Name] = ResolveDepRecord(package, EvaluationPackage(package.Name, package, []))

        PackageBuildOrder.__CheckForExceptions(exceptionList)
        return packageNameToPackageDict


    @staticmethod
    def __CreateDependencyGraph(log: Log, allPackages: List[EvaluationPackage]) -> Tuple[DependencyGraph, DependencyGraphNode]:
        graph = DependencyGraph(allPackages)

        # Add the final top level node so we can determine the global build order
        node = PackageBuildOrder.__CreateTopLevelNode(log, graph)  # type: DependencyGraphNode

        return (graph, node)

    @staticmethod
    def __CreateTopLevelNode(log: Log, graph: DependencyGraph) -> DependencyGraphNode:
        log.LogPrintVerbose(LocalVerbosityLevel.Info, "Creating top level node to determine global build order")

        # Extract the top level nodes
        topLevelPackage = PackageBuildOrder.__CreateTopLevelPackage(log, graph)  # type: EvaluationPackage

        # Add the top level node to the graph with all it dependency edges
        node = graph.AddNode(topLevelPackage)
        graph.AddPackageDirectDependencies(node)
        return node

    @staticmethod
    def __CreateTopLevelPackage(log: Log, graph: DependencyGraph) -> EvaluationPackage:
        # Extract the top level nodes and add them as direct dependencies to a 'top level node' (so we can determine the global build order)
        topLevelNodes = graph.FindNodesWithNoIncomingDependencies() # type: List[DependencyGraphNode]

        if len(topLevelNodes) < 1:
            raise Exception("Internal error, no top level nodes found, so either no packages are available or there is a circular dependency")

        # If there are no top level nodes, that means we are likely to have a circular dependency
        # so we just rely on the circular dependency detection to handle it
        log.LogPrintVerbose(LocalVerbosityLevel.Info, "Main packages: {0}".format(len(topLevelNodes)))

        unresolvedDirectDependencies = []  # type: List[UnresolvedPackageDependency]
        packageDirectDependencies = [] # type: List[EvaluationPackage.DependencyRecord]
        for package in topLevelNodes:
            packageDirectDependencies.append(EvaluationPackage.DependencyRecord(package.Source, None))
            unresolvedDirectDependencies.append(UnresolvedPackageDependency(package.Source.Name, AccessType.Public, ))
        newTopLevel = UnresolvedBasicPackage(UnresolvedPackageName(PackageNameMagicString.TopLevelName), PackageType.TopLevel, unresolvedDirectDependencies, [])
        return EvaluationPackage(newTopLevel.Name, newTopLevel, packageDirectDependencies)


    @staticmethod
    def __CreatePackageDependencyNotFoundException(package: UnresolvedBasicPackage, dep: UnresolvedPackageDependency, debugAllPackages: List[UnresolvedBasicPackage]) -> Exception:
        topCandidates = PackageBuildOrder.__BuildPackageCandidateString(dep.Name, debugAllPackages) # type: str
        return PackageDependencyNotFoundException(package.Name.Value, dep.Name.Value, topCandidates)

    @staticmethod
    def __CreatePackageFlavorOptionDependencyNotFoundException(package: UnresolvedBasicPackage, flavor: UnresolvedPackageFlavor,
                                                               flavorOption: UnresolvedPackageFlavorOption, dep: UnresolvedPackageDependency,
                                                               debugAllPackages: List[UnresolvedBasicPackage]) -> Exception:
        topCandidates = PackageBuildOrder.__BuildPackageCandidateString(dep.Name, debugAllPackages) # type: str
        return PackageFlavorOptionDependencyNotFoundException("Package '{0}' flavor '{1}' option '{2}' dependency to '{3}' not found, did you mean '{4}'".format(package.Name, flavor.Name, flavorOption, dep.Name, topCandidates))

    @staticmethod
    def __BuildPackageCandidateString(unresolvedPackageName: UnresolvedPackageName, debugAllPackages: List[UnresolvedBasicPackage]) -> str:
        ratingList = [] # type: List[Tuple[UnresolvedBasicPackage, float]]

        # Build a possible candidate array
        for possiblePackage in debugAllPackages:
            ratio = difflib.SequenceMatcher(None, unresolvedPackageName.Value, possiblePackage.Name.Value).ratio()
            ratingList.append((possiblePackage, ratio))

        ratingList.sort(key=lambda s: -s[1])

        candidateList = [entry[0].Name.Value for entry in ratingList]
        candidateList = candidateList[0:5]

        topCandidates = ", ".join(candidateList) # type: str
        return topCandidates

    @staticmethod
    def __AddException(exceptionList: Optional[List[Exception]], exception: Exception) -> List[Exception]:
        if exceptionList is None:
            exceptionList = []
        exceptionList.append(exception)
        return exceptionList

    # -----------------------------------------------------------------------------------------------------------------------------------------------

    @staticmethod
    def __ValidateConstraints(log: Log, finalBuildOrder: List[UnresolvedBasicPackage]) -> None:
        log.LogPrintVerbose(LocalVerbosityLevel.Info, "Validating flavor constraints")

        lookupDict = dict() # type: Dict[UnresolvedPackageName, UnresolvedBasicPackage]
        for package in finalBuildOrder:
            lookupDict[package.Name] = package

        depStack = []   # type: List[UnresolvedPackageName]
        for package in finalBuildOrder:
            PackageBuildOrder.__ValidatePackageConstraints(log, lookupDict, depStack, package)


    @staticmethod
    def __ValidatePackageConstraints(log: Log, lookupDict: Dict[UnresolvedPackageName, UnresolvedBasicPackage],
                                     depStack: List[UnresolvedPackageName], package: UnresolvedBasicPackage) -> None:
        if package.Type == PackageType.TopLevel:
            return
        constraintDict = dict() # type: Dict[PackageFlavorName, ConstraintRecord]
        depStack.clear()
        PackageBuildOrder.__ProcessDependencyConstraints(constraintDict, depStack, package, lookupDict)

        errors = [] # type: List[Tuple[str, List[str]]]
        for entry in constraintDict.items():
            if len(entry[1].LookupDict) > 1:
                locationStrings = [] # type: List[str]
                for dep in entry[1].LookupDict.items():
                    for option in dep[1]:
                        depOrderStr = "->".join([depStackEntry.Value for depStackEntry in option.DependencyStack])
                        locationStrings.append("'{0}'='{1}' at {2}".format(entry[0], dep[0], depOrderStr))
                optionStr = " && ".join([keyName.Value for keyName in entry[1].LookupDict.keys()])
                errors.append(("{0}={1}".format(entry[0], optionStr), locationStrings))

        if len(errors) > 0:
            constraintsDesc = ", ".join([error[0] for error in errors]) # type: str
            constraintsLocDesc = ", ".join([", ".join(error[1]) for error in errors]) # type: str
            raise Exception("Mutually exclusive constraints encountered while resolving '{0}' constraints=({1})".format(package.Name, constraintsLocDesc))


    @staticmethod
    def __ProcessDependencyConstraints(constraintDict: Dict[PackageFlavorName, ConstraintRecord],
                                       depStack: List[UnresolvedPackageName], unresolvedPackage: UnresolvedBasicPackage,
                                       packageDict: Dict[UnresolvedPackageName, UnresolvedBasicPackage]) -> None:
        depStack.append(unresolvedPackage.Name)
        for dep in unresolvedPackage.DirectDependencies:
            if len(dep.FlavorConstraints.Selections) > 0:
                for constraintOption in dep.FlavorConstraints.Selections:
                    if constraintOption.Name not in constraintDict:
                        record = ConstraintRecord()
                        constraintDict[constraintOption.Name] = record
                    else:
                        record = constraintDict[constraintOption.Name]

                    if constraintOption.Option not in record.LookupDict:
                        optionRecordList = [] # type: List[ConstraintOptionRecord]
                        record.LookupDict[constraintOption.Option] = optionRecordList
                    else:
                        optionRecordList = record.LookupDict[constraintOption.Option]
                    depList = list(depStack) # type: List[UnresolvedPackageName]
                    depList.reverse()
                    optionRecordList.append(ConstraintOptionRecord(constraintOption.Option, depList))

            if dep.Name not in packageDict:
                raise Exception("Unknown dependency '{0}".format(dep.Name))
            depPackage = packageDict[dep.Name]
            PackageBuildOrder.__ProcessDependencyConstraints(constraintDict, depStack, depPackage, packageDict)
        depStack.pop()

    @staticmethod
    def __CheckForExceptions(exceptionList: Optional[List[Exception]]) -> None:
        if exceptionList is not None and len(exceptionList) > 0:
            if len(exceptionList) > 1:
                raise GroupedException(exceptionList)
            else:
                raise exceptionList[0]
