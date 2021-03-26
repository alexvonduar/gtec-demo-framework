﻿#!/usr/bin/env python3
#****************************************************************************************************************************************************
#* BSD 3-Clause License
#*
#* Copyright (c) 2015, Mana Battery
#* All rights reserved.
#*
#* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#*
#* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
#*    documentation and/or other materials provided with the distribution.
#* 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
#*    software without specific prior written permission.
#*
#* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
#* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
#* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#****************************************************************************************************************************************************

from typing import Any
from typing import Dict
from typing import List
from typing import Tuple
from TPConvert import IOUtil
from TPConvert.FormatPlugin import FormatPlugin
from TPConvert.TexturePackerObjects import TexturePackerAtlas
from TPConvert.TexturePackerObjects import TexturePackerFrame
from TPConvert.FormatPluginBinaryTAUtil import *


class FormatPluginBinaryTA2(FormatPlugin):
    def __init__(self) -> None:
        super().__init__("bta2")

    def Process(self, atlas: TexturePackerAtlas, outputFilename: str) -> None:
        pathInfo = self.__BuildPathDirectory(atlas.Entries)
        self.__DebugPaths(atlas.Entries, pathInfo)

        entryDict = {}  # type: Dict[str, Tuple[int,str,str]]
        for pathEntry in pathInfo[1]:
            entryDict[pathEntry[2]+pathEntry[1]] = pathEntry

        entryList = []                  # type: List[int]
        AddHeader(entryList, 0x1001);
        # make room for a number of bytes written entry and store the offset where it was written so we can pacth it later
        offset = len(entryList)
        AddUInt32(entryList, 0);

        self.__AddPathList(entryList, pathInfo[0])

        AddEncodedUInt32(entryList, len(atlas.Entries))
        for entry in atlas.Entries:
            rectX = entry.Frame.X - entry.SpriteSourceSize.X
            rectY = entry.Frame.Y - entry.SpriteSourceSize.Y
            rectWidth = entry.SourceSize.Width
            rectHeight = entry.SourceSize.Height
            self.__AddEntry(entryList, rectX, rectY, rectWidth, rectHeight, entry.Frame.X, entry.Frame.Y, entry.Frame.Width, entry.Frame.Height, 
                            entryDict[entry.FullFilenameWithoutExt])

        # Write the number of bytes that were written to the extended header
        # -4 because we dont count the 'size' entry
        bytesWritten = len(entryList) - offset - 4
        SetUInt32(entryList, offset, bytesWritten)

        content = bytearray(entryList)
        outputFilename = '{0}.{1}'.format(outputFilename, 'bta')
        IOUtil.WriteBinaryFileIfChanged(outputFilename, content)


    def __DebugReconstructFilename(self, paths: List[Tuple[int,str]], file: Tuple[int,str,str]) -> str:
        if file[0] <= 0:
            return file[1]
        return self.__DebugReconstructFilenameNow(paths, file[0]-1) + '/' + file[1]

    def __DebugReconstructFilenameNow(self, paths: List[Tuple[int,str]], pathIndex: int) -> str:
        if paths[pathIndex][0] <= 0:
            return paths[pathIndex][1]
        return self.__DebugReconstructFilenameNow(paths, paths[pathIndex][0]-1) + '/' + paths[pathIndex][1]


    def __DebugPaths(self, entries: List[TexturePackerFrame], pathInfo: Tuple[List[Tuple[int,str]], List[Tuple[int,str,str]]]) -> None:
        reconstructed = []
        for file in pathInfo[1]:
            reconstructed.append(self.__DebugReconstructFilename(pathInfo[0], file))

        reconstructedSet = set()
        for reconstructedEntry in reconstructed:
            reconstructedSet.add(reconstructedEntry)

        for entry in entries:
            if not entry.FullFilenameWithoutExt in reconstructedSet:
                raise Exception("NotFound %s" % (entry.FullFilenameWithoutExt));


    def __BuildPathDirectory(self, entries: List[TexturePackerFrame]) -> Tuple[List[Tuple[int,str]], List[Tuple[int,str,str]]]:
        # Build path tree using dicts
        entryDict = {}  # type: Dict[str, Dict[str, Any]]
        for entry in entries:
            currentDict = entryDict
            pathElements = entry.FullFilenameWithoutExt.split('/')
            for element in pathElements:
                if not element in currentDict:
                    newDict = {} # type: Dict[str, Dict[str, Any]]
                    currentDict[element] = newDict
                    currentDict = newDict
                else:
                    currentDict = currentDict[element]
        listDirs = []       # type: List[Tuple[int,str]]
        listFiles = []      # type: List[Tuple[int,str,str]]
        # Offset the indices by one so that 'zero' can mean the root
        self.__BuildPathList(listDirs, listFiles, entryDict, 0, 1, "")
        #print listDirs
        return (listDirs, listFiles)


    def __BuildPathList(self, listDirs: List[Tuple[int,str]], listFiles: List[Tuple[int,str,str]], 
                        entryDict: Dict[str, Dict[str, Any]], parentIndex: int, startIndex: int, parentName: str) -> int:
        totalIndex = startIndex
        for item in entryDict.items():
            if len(item[1]) == 0:
                listFiles.append((parentIndex, item[0], parentName))
            else:
                listDirs.append((parentIndex, item[0]))
                totalIndex = totalIndex + 1

        count2 = startIndex
        for items in entryDict.items():
            if len(items[1]) > 0:
                totalIndex = self.__BuildPathList(listDirs, listFiles, items[1], count2, totalIndex, parentName + items[0] + "/" )
                count2 = count2 + 1
        return totalIndex


    def __AddPathList(self, entryList: List[int], pathList: List[Tuple[int,str]]) -> None:
        AddEncodedUInt32(entryList, len(pathList))
        for entry in pathList:
            AddEncodedUInt32(entryList, entry[0])
            AddString(entryList, entry[1])

    def __AddEntry(self, entryList: List[int], srcRectX: int, srcRectY: int, srcRectWidth: int, srcRectHeight: int, 
                   trimmedRectX: int, trimmedRectY: int, trimmedRectWidth: int, trimmedRectHeight: int, pathInfo: Tuple[int,str,str]) -> None:
        AddRectangle(entryList, srcRectX, srcRectY, srcRectWidth, srcRectHeight)
        AddRectangle(entryList, trimmedRectX, trimmedRectY, trimmedRectWidth, trimmedRectHeight)
        AddEncodedUInt32(entryList, pathInfo[0])
        AddString(entryList, pathInfo[1])

