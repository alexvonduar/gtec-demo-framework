#!/usr/bin/env python3

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

from typing import Any
from typing import List
from typing import Optional
import argparse
from FslBuildGen import Main as MainFlow
#from FslBuildGen import PackageListUtil
#from FslBuildGen.DataTypes import BuildRecipeValidateCommand
#from FslBuildGen.DataTypes import BuildRecipeValidateMethod
from FslBuildGen import PluginSharedValues
from FslBuildGen.Build.BuildVariantConfigUtil import BuildVariantConfigUtil
from FslBuildGen.BuildExternal import RecipeBuilder
from FslBuildGen.BuildExternal.BuilderConfig import BuilderConfig
from FslBuildGen.Config import Config
from FslBuildGen.Context.GeneratorContext import GeneratorContext
#from FslBuildGen.Generator import PluginConfig
#from FslBuildGen.Log import Log
#from FslBuildGen.PackageConfig import PlatformNameString
#from FslBuildGen.PackageFilters import PackageFilters
from FslBuildGen.Tool.AToolAppFlow import AToolAppFlow
from FslBuildGen.Tool.AToolAppFlowFactory import AToolAppFlowFactory
from FslBuildGen.Tool.ToolAppConfig import ToolAppConfig
from FslBuildGen.Tool.ToolAppContext import ToolAppContext
from FslBuildGen.Tool.ToolCommonArgConfig import ToolCommonArgConfig
from FslBuildGen.ToolConfig import ToolConfig


class DefaultValue(object):
    CheckBuildCommands = False
    ForceClaimInstallArea = False
    DryRun = False
    PackageConfigurationType = PluginSharedValues.TYPE_DEFAULT
    PreDeleteBuild = True
    PostDeleteBuild = True
    VoidBuild = False


class LocalToolConfig(ToolAppConfig):
    def __init__(self) -> None:
        super().__init__()

        self.CheckBuildCommands = DefaultValue.CheckBuildCommands
        self.ForceClaimInstallArea = DefaultValue.ForceClaimInstallArea
        self.DryRun = DefaultValue.DryRun
        self.PackageConfigurationType = DefaultValue.PackageConfigurationType
        self.PreDeleteBuild = DefaultValue.PreDeleteBuild
        self.PostDeleteBuild = DefaultValue.PostDeleteBuild
        self.VoidBuild = DefaultValue.VoidBuild


def GetDefaultLocalConfig() -> LocalToolConfig:
    return LocalToolConfig()


class ToolFlowBuildExternal(AToolAppFlow):
    #def __init__(self, toolAppContext: ToolAppContext) -> None:
    #    super().__init__(toolAppContext)


    def ProcessFromCommandLine(self, args: Any, currentDirPath: str, toolConfig: ToolConfig, userTag: Optional[object]) -> None:
        # Process the input arguments here, before calling the real work function
        localToolConfig = LocalToolConfig()

        # Configure the ToolAppConfig part
        localToolConfig.SetToolAppConfigValues(self.ToolAppContext.ToolAppConfig)

        # Configure the local part
        localToolConfig.CheckBuildCommands = args.CheckBuildCommands
        localToolConfig.ForceClaimInstallArea = args.ForceClaimInstallArea
        localToolConfig.PackageConfigurationType = args.type
        localToolConfig.PreDeleteBuild = not args.DontPreDeleteBuild
        localToolConfig.PostDeleteBuild = not args.DontPostDeleteBuild
        localToolConfig.VoidBuild = args.VoidBuild

        self.Process(currentDirPath, toolConfig, localToolConfig)


    def Process(self, currentDirPath: str, toolConfig: ToolConfig, localToolConfig: LocalToolConfig) -> None:
        config = Config(self.Log, toolConfig, localToolConfig.PackageConfigurationType,
                        localToolConfig.BuildVariantsDict, localToolConfig.AllowDevelopmentPlugins)

        packageFilters = localToolConfig.BuildPackageFilters

        buildVariantConfig = BuildVariantConfigUtil.GetBuildVariantConfig(localToolConfig.BuildVariantsDict)
        platform = self.ToolAppContext.PluginConfigContext.GetGeneratorPluginById(localToolConfig.PlatformName, localToolConfig.Generator,
                                                                                  buildVariantConfig, config.ToolConfig.DefaultPackageLanguage,
                                                                                  config.ToolConfig.CMakeConfiguration,
                                                                                  localToolConfig.GetUserCMakeConfig(), False)
        theFiles = [] # type: List[str]
        if not localToolConfig.VoidBuild:
            theFiles = MainFlow.DoGetFiles(config, toolConfig.GetMinimalConfig(), currentDirPath, localToolConfig.Recursive)
        else:
            self.Log.LogPrintVerbose(1, "Doing a void build")
        generatorContext = GeneratorContext(config, self.ErrorHelpManager, packageFilters.RecipeFilterManager, config.ToolConfig.Experimental, platform)
        packages = MainFlow.DoGetPackages(generatorContext, config, theFiles, packageFilters)
        #packages = DoExperimentalGetRecipes(generatorContext, config, [])
        #topLevelPackage = PackageListUtil.GetTopLevelPackage(packages)


        builderConfig = BuilderConfig()
        builderConfig.Settings.PreDeleteBuild = localToolConfig.PreDeleteBuild
        builderConfig.Settings.PostDeleteBuild = localToolConfig.PostDeleteBuild
        builderConfig.Settings.CheckBuildCommands = localToolConfig.CheckBuildCommands
        builderConfig.Settings.ForceClaimInstallArea = localToolConfig.ForceClaimInstallArea
        builderConfig.Settings.BuildThreads = localToolConfig.BuildThreads

        RecipeBuilder.BuildPackages(self.Log, config.SDKPath, config.IsDryRun, config.ToolConfig, generatorContext, builderConfig, packages)

        #topLevelPackage = PackageListUtil.GetTopLevelPackage(packages)
        #for package in topLevelPackage.ResolvedExperimentalRecipeBuildOrder:
        #    print("{0}".format(package.Name))
        #    packageRecipe = package.ResolvedDirectExperimentalRecipe
        #    pipeline = packageRecipe.Pipeline
        #    validation = packageRecipe.ValidateInstallation
        #    print("- {0}".format(packageRecipe.Name))
        #    if pipeline is not None:
        #        for command in pipeline.CommandList:
        #            print("  - {0}".format(command.CommandName))
        #            for joinCommand in command.JoinCommandList:
        #                print("    - {0}".format(joinCommand.CommandName))
        #    if validation is not None:
        #        for command in validation.CommandList:
        #            if command.CommandType == BuildRecipeValidateCommand.EnvironmentVariable:
        #                print("  - {0}".format(command.CommandName))
        #            elif command.CommandType == BuildRecipeValidateCommand.Path:
        #                if command.Method == BuildRecipeValidateMethod.IsDirectory:
        #                    print("  - {0} '{1}' is directory".format(command.CommandName, command.Name))
        #                elif command.Method == BuildRecipeValidateMethod.IsFile:
        #                    print("  - {0} '{1}' is file".format(command.CommandName, command.Name))
        #                elif command.Method == BuildRecipeValidateMethod.Exists:
        #                    print("  - {0} '{1}' exists".format(command.CommandName, command.Name))
        #                else:
        #                    print("  - {0} '{1}' unknown".format(command.CommandName, command.Name))
        #            elif command.CommandType == BuildRecipeValidateCommand.FindFileInPath:
        #                print("  - {0}".format(command.CommandName))
        #            elif command.CommandType == BuildRecipeValidateCommand.FindExecutableFileInPath:
        #                print("  - {0}".format(command.CommandName))
        #            elif command.CommandType == BuildRecipeValidateCommand.AddHeaders:
        #                print("  - {0}".format(command.CommandName))
        #            elif command.CommandType == BuildRecipeValidateCommand.AddLib:
        #                print("  - {0}".format(command.CommandName))
        #            elif command.CommandType == BuildRecipeValidateCommand.AddDLL:
        #                print("  - {0}".format(command.CommandName))
        #            elif command.CommandType == BuildRecipeValidateCommand.AddTool:
        #                print("  - {0}".format(command.CommandName))
        #            else:
        #                print("  - Unexpected '{0}'".format(command.CommandName))



class ToolAppFlowFactory(AToolAppFlowFactory):
    #def __init__(self) -> None:
    #    pass


    def GetTitle(self) -> str:
        return 'FslBuildExternal'


    def GetToolCommonArgConfig(self) -> ToolCommonArgConfig:
        argConfig = ToolCommonArgConfig()
        argConfig.AddPlatformArg = True
        argConfig.AddGeneratorSelection = True
        argConfig.ProcessRemainingArgs = False
        #argConfig.AllowVSVersion = True
        argConfig.AllowForceClaimInstallArea = True
        argConfig.SupportBuildTime = True
        argConfig.AddBuildThreads = True
        argConfig.AllowRecursive = True
        return argConfig


    def AddCustomArguments(self, parser: argparse.ArgumentParser, toolConfig: ToolConfig, userTag: Optional[object]) -> None:
        parser.add_argument('-t', '--type', default=DefaultValue.PackageConfigurationType, choices=[PluginSharedValues.TYPE_DEFAULT, 'sdk'], help='Select generator type')
        parser.add_argument('--DontPreDeleteBuild', action='store_true', help='The build temporary directory will not be deleted before starting a build. Only use this if you know what you are doing')
        parser.add_argument('--DontPostDeleteBuild', action='store_true', help='On successfull build dont delete the build directory')
        parser.add_argument('--CheckBuildCommands', action='store_true', help='Check that all build commands are available')
        parser.add_argument('--VoidBuild', action='store_true', help='Build a empty package (a void package)')


    def Create(self, toolAppContext: ToolAppContext) -> AToolAppFlow:
        return ToolFlowBuildExternal(toolAppContext)
