#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#****************************************************************************************************************************************************
# Copyright 2017 NXP
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

#from typing import Dict
from typing import List
from typing import Optional
from FslBuildGen.Log import Log
from FslBuildGen import IOUtil
from FslBuildGen.BuildExternal.DataTypes import RecipeType
from FslBuildGen.BuildExternal.PipelineCommand import PipelineCommand
from FslBuildGen.BuildExternal.PipelineCommandBuilder import PipelineCommandBuilder
from FslBuildGen.BuildExternal.PackageExperimentalRecipe import PackageExperimentalRecipe
#from FslBuildGen.DataTypes import BuildRecipePipelineCommand
from FslBuildGen.Packages.Package import Package
#from FslBuildGen.PackageConfig import PlatformNameString

class Pipeline(object):
    def __init__(self, log: Log, builder: PipelineCommandBuilder, sourcePackage: Package, sourceRecipe: PackageExperimentalRecipe) -> None:
        super().__init__()
        self._Log = log
        self.SourcePackage = sourcePackage
        self.SourceRecipe = sourceRecipe
        self.SourcePipeline = sourceRecipe.Pipeline
        self.InstallPath = sourceRecipe.ResolvedInstallLocation
        self.BuildPath = builder.GetBuildPath(sourceRecipe)
        self.CommandList = self.__CreateCommandList(builder, self.SourcePackage, self.SourceRecipe)

    def __CreateCommandList(self, builder: PipelineCommandBuilder, sourcePackage: Package, sourceRecipe: PackageExperimentalRecipe) -> List[PipelineCommand]:
        if sourceRecipe.Pipeline is None:
            raise Exception("Invalid recipe")

        #isAndroidBuild = (sourcePackage.ResolvedPlatform is not None and sourcePackage.ResolvedPlatform.Name == PlatformNameString.ANDROID)

        builder.Begin(sourcePackage, sourceRecipe)
        #if not isAndroidBuild:
        for sourceCommand in sourceRecipe.Pipeline.CommandList:
            builder.Add(sourceCommand, False)
        #else:
        #    # We handle cmake differently in android builds for now
        #    # This is until we get a proper solution that will allow us to prebuild libraries
        #    for index, sourceCommand in enumerate(sourceRecipe.Pipeline.CommandList):
        #        skip = False
        #        if sourceCommand.CommandType == BuildRecipePipelineCommand.CMakeBuild:
        #            # Validate our pipeline assumptions that allow CMakeBuild to work on android
        #            if index != (len(sourceRecipe.Pipeline.CommandList)-1):
        #                raise Exception("CMakeBuild can only be the last entry in a android package {0} recipe".format(sourcePackage.Name))
        #            elif sourceCommand.JoinCommandList is not None and len(sourceCommand.JoinCommandList) > 0:
        #                raise Exception("CMakeBuild in a android package {0} recipe can not contian join commands".format(sourcePackage.Name))
        #            skip = True
        #        builder.Add(sourceCommand, skip)
        return builder.End()

class RecipeRecord(object):
    def __init__(self, log: Log, builder: PipelineCommandBuilder, sourcePackage: Package) -> None:
        super().__init__()

        if sourcePackage.ResolvedDirectExperimentalRecipe is None:
            raise Exception("No build recipe in package {0}".format(sourcePackage.Name))

        #if sourcePackage.Type != PackageType.ExperimentalRecipe:
        #    raise Exception("Unsupported package type encountered: {0} in package {1}".format(sourcePackage.Type, sourcePackage.Name))

        self.SourcePackage = sourcePackage
        self.SourceRecipe = sourcePackage.ResolvedDirectExperimentalRecipe
        self.Pipeline = self.__TryCreatePipeline(log, builder, sourcePackage, self.SourceRecipe)


    def __TryCreatePipeline(self, log: Log, builder: PipelineCommandBuilder,
                            sourcePackage: Package, sourceRecipe: PackageExperimentalRecipe) -> Optional[Pipeline]:
        if sourceRecipe.Pipeline is None:
            return None
        self.__RemoveInvalidInstallation(log, sourceRecipe)
        return Pipeline(log, builder, sourcePackage, sourceRecipe)


    def __RemoveInvalidInstallation(self, log: Log, sourceRecipe: PackageExperimentalRecipe) -> None:
        if sourceRecipe is None or sourceRecipe.ResolvedInstallLocation is None or sourceRecipe.Pipeline is None:
            return

        # the external installs dont have a associated pipeline so this should be safe
        # but we check the build type as well just to be safe
        if IOUtil.IsDirectory(sourceRecipe.ResolvedInstallLocation.ResolvedPath):
            if sourceRecipe.Type != RecipeType.Build:
                log.DoPrintWarning("The sourceRecipe type was not of the expected type, aborting delete to be safe")
                return

            log.LogPrint("Removing invalid content at '{0}'".format(sourceRecipe.ResolvedInstallLocation.ResolvedPath))
            IOUtil.SafeRemoveDirectoryTree(sourceRecipe.ResolvedInstallLocation.ResolvedPath)
