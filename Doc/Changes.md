# Latest version

* Ported GLES3.SRGBFramebuffer to Vulkan.SRGBFramebuffer.
* FslBuild now supports "-c install" for some builders.
* Vulkan screenshot support
* Replaced TCLAP with a custom argument parser to reduce executable size.
* **Breaking change** Project.gen now sets RequirePackageCreationYear="true".
  So its now mandatory for all Fsl.gen files to contain a CreationYear.
* Experimental cmake support.
* Ported GLES3.ObjectSelection to Vulkan.ObjectSelection.
* **Breaking change** Now requires C++14.

## Release 5.3.1

* Added Vulkan.NativeWindowTest app.
* Added vulkan wayland improvements.

## Release 5.3.0 or earlier

* Upgraded to Android Studio 3.3.1
* Upgraded to Android NDK R19
* Switched to C++14 from C++11
* Enabled Unit Tests for Yocto builds.
* Updated third-party library OpenCV from 3.4.5 to 4.0.1
* Updated documentation for install the Vulkan sdk to recommend 1.1.92.1.
* Updated third-party library OpenCV from 3.2.0 to 3.4.5
* Updated third-party library fmt from 4.0.0 to 5.3.0
* **Breaking change** Disable third party downloads per default for Yocto builds. To build using an old Yocto release that doesn't come with all third party software you need to add ```--Recipes [*]``` to your command line which will re-enable the download.
* **Deprecation** Vector2+Vector3+Vector4+Matrix all static methods that outputs to a reference now takes the output as the last parameter 
                  just as in MonoGame for consistency existing methods that took it as the first parameter was declared deprecated and 
                  will be removed next release.
* Added more unit tests
* Most OriginalContent directories have been moved to Resources/Source.
* Most large resources are now stored under Resources and then copied to the right locations during content building.
* Added Vulkan.DFSimpleUI101
* Added Vulkan.DFSimpleUI100
* Added Vulkan.DFNativeBatch2D
* Added new Vulkan app template that utilize the new VulkanDemoHost.
* Switched to clang tidy 7.0
* _PreDestruct added to IDemoApp. This allows for better way to do resource cleanup that can use 'virtual' methods and
  allows us to handle exceptions in a controlled way that logs some information before crashing.
* Upgraded to RapidVulkan 1.1.85.0
* Fixed all vulkan 1.1.85.0 validation layer warnings in:
  * DisplacementMapping
  * DynamicTerrainTessellation
  * Gears
  * MeshInstancing
  * TessellationPNTriangles
  * Texturing
  * TexturingArrays
  * TexturingCubeMap
* Fixed all vulkan 1.1.82.1 validation layer warnings in:
  * DevBatch
  * DisplacementMapping
  * Gears
  * MeshInstancing
  * TessellationPNTriangles
  * Texturing
  * TexturingArrays
  * TexturingCubeMap
* DynamicTerrainTessellation
* All events are only logged at -vvvvv. This should make verbose more useful.
* FslBuildNew template ConsoleMinimal, introduces a minimalistic build with no graphics library dependencies.
* VulkanBase third party package introduced. This allows us to removes the window system dependency from some vulkan libraries.
* FslSimpleUI WindowContext now contains a weak pointer to the ui interfaces. This prevents circular dependencies.
* Added new app OpenVX.Stereo.
* CONTRIBUTING.md describes how to contribute.
* Early access to the Helios cross platform camera API.
* Added new app GLES3.CameraDemo
* Added new app GLES3. DFNativeBatchCamera
* Includes a small subset of the unit tests. More will be made available in future release.
* Code cleanup
* FslBuildCheck now supports clang format and clang tidy.
* All code was reformatted with clang format.
* All code had a tidy pass using clang tidy.
* FslBuildCheck parameters renamed.
* FslBuild 2.6+ now checks that all include files are under their correct package name based include path.
  So for a package called "Hamster.Dance.Master" the include files will be under "include/Hamster/Dance/Master"
  This check can de disabled per package with PackageNameBasedIncludePath="false".
* **Breaking change** Isolated the Graphics service from FslDemoHost.Base to FslDemoService/Graphics to 
  allow better dependency control. This allowed us to remove the Graphics service from Console type demos 
  removing their dependency on the Graphics, NativeGraphics and NativeWindow packages which they did not need.
* **Breaking change** Reduced dependencies for the NativeGraphics service.
* Android build now requires Android Studio 3.1 and the Android NDK16b or newer.
* Vulkan enabled for the Yocto Wayland backend.
* All ThirdParty code is now downloaded as needed instead of being included in the repo.
* Assimp 4.1.0 is now support on Android.
* Upgraded Assimp from 3.3.1 to 4.1.0 on Windows and Ubuntu, Yocto is still using 4.
* Removed assimp 3.3.1 source used for Android builds and disabled assimp support for Android
* Windows builds now default to Visual Studio 2017 projects
* **Breaking change**: OpenVX1_0_1 support removed.
* **Breaking change**: GLES3::GLFrameBuffer constructor and reset parameters modified (old not updated code will give a compile error).
* **Breaking change**: GLES2::GLFrameBuffer constructor and reset parameters modified (old not updated code will give a compile error).
* **Breaking change**: Renamed UI::Layout to UI::SimpleLayout in preparation for supporting more complex layouts.
* **Breaking change**: Moved FslDemoHost/EGL/Service/EGLHost/IEGLHostInfo.hpp to FslDemoHost/EGL/Config/Service/IEGLHostInfo.hpp so that all apps can use it without additional dependencies.
* Fixed Android ContentCache loader so it works with format 3 used from forward 5.0.0
* **Breaking change**: EGLStringUtil::GetExtensions moved to EGLUtil::GetExtensions to match the OpenGL ES versions.
* **Breaking change**: IDemoAppRegistry interface simplified, some of its old responsibility was moved to the registation code.
* **Breaking change**: Moved FslDemoApp/Base/Host/DemoAppHostConfigEGL.hpp to FslDemoHost/EGL/Config/DemoAppHostConfigEGL.hpp
  to standardize the location and allow it to utilize EGL types.
* **Breaking change**: Moved FslDemoApp/Vulkan/Config/DemoAppHostConfigVulkanEx.hpp to FslDemoHost/Vulkan/Config/DemoAppHostConfigVulkanEx.hpp
  to standardize the location.
* **Breaking change**: Corrected extension names in fsl.gen files from EXT_geometry_shader to GL_EXT_geometry_shader
* **Breaking change**: Corrected extension names in fsl.gen files from EXT_tessellation_shader to GL_EXT_tessellation_shader
* **Breaking change**: The Camera helper classes was moved from FslGraphics3D.BasicScene to FslGraphics3D.Camera.
* **Breaking change**: IContentManger, IPersistentDataManager and IO::File now uses uint64_t to represent file sizes and relevant types.
* Enabled OpenVX1.1 on ubuntu using the Khronos OpenVX 1.1 sample implementation.
* Switched to OpenVX1.1 on windows using the Khronos OpenVX 1.1 sample implementation.
* Enabled OpenVX1.1 on Yocto builds.
* Converted all OpenVX legacy application to OpenVX 1.1.
* All OpenVX 1.0.1 apps was postfixed with Legacy in their name.
* IDemoAppControl now has the ability to enable/disable mouse capture for the platforms 
  that support it.
* RawMouseMovement introduced. Platform support is limited to Windows at the moment.
* Updated GLM to 0.9.8.5 from 0.9.8.4
* **Breaking change**: All 'shared' demo app libraries renamed to take advantage of sub package support.

  | Old name                             | New name                                    |
  |--------------------------------------|---------------------------------------------|
  | BloomShared                          | Shared.Bloom                                |
  | DFGraphicsBasic2DShared              | Shared.DFGraphicsBasic2D                    |
  | DFNativeBatch2DShared                | Shared.DFNativeBatch2D                      |
  | DFSimpleUI101Shared                  | Shared.DFSimpleUI101                        |
  | EnvironmentMappingShared             | Shared.EnvironmentMapping                   |
  | FractalShaderShared                  | Shared.FractalShader                        |
  | ObjectSelectionShared                | Shared.ObjectSelection                      |
  | VulkanWillemsDemoAppExperimental     | Shared.VulkanWillemsDemoAppExperimental     |
  | VulkanWillemsMeshDemoAppExperimental | Shared.VulkanWillemsMeshDemoAppExperimental |
  | VulkanWindowExperimental             | Shared.VulkanWindowExperimental             |
* **Breaking change**: All scripts now require python3.4. This was done because we found bugs in python2 that are fixed in python3.
* **Breaking change**: Gen files attribute 'NotSupported' was replaced with 'Supported'.
* **Breaking change**: Gen files <UsesFeature> was replaced with <Requirement> and it now supports both feature and extension types and extending other features.
* **Breaking change**: All DemoApp Register methods now takes the application name as the second parameter.
* **Breaking change**: TestService screenshot functionality naming scheme was modified. 
  All numbered screenshots taken are now named '\<prefix>-\<number>.\<extension>' instead of "test-frame-\<number>.\<extension>", 
  "test-screenshot-\<number>.\<extension>" or , "Screenshot-\<number>.\<extension>".
  Manual screenshots taken with F4 now follows the same naming scheme. However if automatic screenshots is enabled with --ScreenshotFrequency 
  the user screenshots are instead saved to "UserScreenshot-\<number>.\<extension>" so they dont conflict with the automatic saves.
* **Breaking change**: Removed FslImageConvert.exe as its no longer needed now that we can save directly to jpg on all platforms.
* **Breaking change**: added command line argument 'ScreenshotFormat' that can be used to chose the format for the screenshot: bmp, jpg, png or tg' (defaults to png)
  The default used to be 'bmp' and it was the only supported format.
* **Breaking change**: Command line argument ScreenshotNameScheme was switched from taking 0,1,2 to a describtive name.
  You now specify:

  | option   | description                                                     |
  |----------|-----------------------------------------------------------------|
  | frame    | The frame number (**default**)                                  |
  | sequence | The screen shot count                                           |
  | exact    | The exact name (this is equal to the prefix + format extension) |
* **Breaking change**: Added command line argument 'ScreenshotNamePrefix' that can be used to chose the screenshot name prefix (defaults to 'Screenshot').
* Added ImageLibrarySTBService which adds basic support for saving bmp, jpg, png or tga.
* **Breaking change**: The low level functionality of FslUtil/OpenCL1_1 was deleted. 
  Chose to use the RapidOpenCL1 library instead as it provides the same functionality.
  This made room for more high level classes in FslUtil/OpenCL1_1 for future improvements. 
* **Breaking change**: The low level functionality of FslUtil/OpenCL1_2 was deleted. 
  Chose to use the RapidOpenCL2 library instead as it provides the same functionality.
  This made room for more high level classes in FslUtil/OpenCL1_2 for future improvements. 
* **Breaking change**: The low level functionality of FslUtil/OpenVX1_1 was deleted. 
  Chose to use the RapidOpenVX library instead as it provides the same functionality.
  This made room for more high level classes in FslUtil/OpenVX1_1 for future improvements. 
* **Breaking change**: The low level functionality of FslUtil/OpenVX1_0_1 was deleted. 
  Chose to use the RapidOpenVX library instead as it provides the same functionality.
  This made room for more high level classes in FslUtil/OpenVX1_0_1 for future improvements. 
* **Breaking change**:OpenCV: Feature OpenCV3.1 was renamed to OpenCV3.
* **Breaking change**:OpenCV: OpenCV3_1 package was renamed to OpenCV3.
* **Breaking change**:OpenCV: The define FSL_ENABLE_OPENCV3_1 was changed to FSL_ENABLE_OPENCV3.
* OpenCV: updated from 3.1 to 3.2 on windows. 
* FslBuildNew.py now supports sanity checking of its templates
* FslUtil/Vulkan1_0 is getting closer to a alpha state.
* VulkanBasicObjects moved into FslUtil/Vulkan1_0
* VulkanExperimental moved into FslUtil/Vulkan1_0
* The low level functionality of FslUtil/Vulkan1_0 was deleted. 
  Chose to use the RapidVulkan library instead as it provides the same functionality.
  This made room for more high level classes in FslUtil/Vulkan. 
  Next step is to merge the experimental libs into it.
* Renamed FslGraphics_Draft to FslGraphics3D.Procedural
* Renamed FslGraphics3D to FslGraphics3D.BasicScene
* Renamed FslSceneFormat to FslGraphics3D.SceneFormat
* Renamed FslNativeGraphics to FslDemoService.NativeGraphics
* Renamed FslSimpleUI to FslSimpleUI.Base
* Renamed FslSimpleUIApp to FslSimpleUI.App
* Renamed FslNativeWindow to FslNativeWindow.Base
* Renamed FslNativeWindowEGL to FslNativeWindow.EGL
* Renamed FslNativeWindowPlatform to FslNativeWindow.Platform
* Renamed FslNativeWindowVulkan to FslNativeWindow.Vulkan
* Renamed FslDemoApp to FslDemoApp.Base
* Renamed FslDemoAppConsole to FslDemoApp.Console
* Renamed FslDemoAppG2D to FslDemoApp.G2D
* Renamed FslDemoAppGLES2 to FslDemoApp.OpenGLES2
* Renamed FslDemoAppGLES3 to FslDemoApp.OpenGLES3
* Renamed FslDemoAppOpenCL to FslDemoApp.OpenCL
* Renamed FslDemoAppOpenVX to FslDemoApp.OpenVX
* Renamed FslDemoAppStub to FslDemoApp.Stub
* Renamed FslDemoAppVG to FslDemoApp.OpenVG
* Renamed FslDemoAppVulkan to FslDemoApp.Vulkan
* Renamed FslDemoAppWindow to FslDemoApp.Window
* Renamed FslDemoHost to FslDemoHost.Base
* Renamed FslDemoHostConsole to FslDemoHost.Console
* Renamed FslDemoHostEGL to FslDemoHost.EGL
* Renamed FslDemoHostStub to FslDemoHost.Stub
* Renamed FslDemoHostVulkan to FslDemoHost.Vulkan
* Renamed FslDemoHostWindow to FslDemoHost.Window
* Renamed FslUtilOpenVX1_1 to FslUtil.OpenVX1_1
* Renamed FslUtilOpenVX1_0_1 to FslUtil.OpenVX1_0_1
* Renamed FslUtilOpenVX to FslUtil.OpenVX
* Renamed FslUtilOpenCL1_2 to FslUtil.OpenCL1_2
* Renamed FslUtilOpenCL1_1 to FslUtil.OpenCL1_1
* Renamed FslUtilOpenCL to FslUtil.OpenCL
* Renamed FslGraphicsVulkan1_0 to FslUtil.Vulkan1_0
* Renamed FslGraphicsGLES3_1 to FslUtil.OpenGLES3_1
* Renamed FslGraphicsGLES3 to FslUtil.OpenGLES3
* Renamed FslGraphicsGLES2 to FslUtil.OpenGLES2
* Renamed FslGraphicsVG to FslUtil.OpenVG
* Renamed FslGraphicsEGL to FslUtil.EGL
* Updated FslBuild tools to 1.6.0
  * FslBuildNew now uses the same 'engine' as the generators
  * FslBuildNew templates now reside in '.Config\Templates.gen\NewProject'
* Updated Fsl python toolchain to 1.5.5.
  * All FslBuildNew templates moved so they reside under ".Config\Templates.gen\NewProject" instead of "Templates".
  * Internal template improvements.  
* Removed deprecated FslNewDemoProject.
* Updated GLM from 0.9.7.6 to 0.9.8.4
* Updated GLI from 0.8.1.0 to 0.8.2.0
* Release 4.0
* Basic Gamepad support on windows.
* Renamed register demo app header files so all follow the same pattern:
  * RegisterDemoAppG2D.hpp -> RegisterDemoApp.hpp
  * RegisterDemoAppGLES2.hpp -> RegisterDemoApp.hpp
  * RegisterDemoAppGLES3.hpp -> RegisterDemoApp.hpp
  * RegisterDemoAppStub.hpp -> RegisterDemoApp.hpp
* The FSL_REGISTER_VULKAN_DEMO and FSL_REGISTER_VULKAN_DEMO_EX macro are not supported anymore. Create a ConfigureDemoAppEnvironment function instead.
* The FSL_REGISTER_OPENVX_DEMO and FSL_REGISTER_OPENVX_DEMO_EX macro are not supported anymore. Create a ConfigureDemoAppEnvironment function instead.
* The FSL_REGISTER_OPENVG_DEMO and FSL_REGISTER_OPENVG_DEMO_EX macro are not supported anymore. Create a ConfigureDemoAppEnvironment function instead.
* The FSL_REGISTER_OPENCL_DEMO and FSL_REGISTER_OPENCL_DEMO_EX macro are not supported anymore. Create a ConfigureDemoAppEnvironment function instead.
* The FSL_REGISTER_CONSOLE_DEMO and FSL_REGISTER_CONSOLE_DEMO_EX macro are not supported anymore. Create a ConfigureDemoAppEnvironment function instead.
* The FSL_REGISTER_OPENGLES3_DEMO and FSL_REGISTER_OPENGLES3_EX macro are not supported anymore. Create a ConfigureDemoAppEnvironment function instead.
* The FSL_REGISTER_OPENGLES2_DEMO and FSL_REGISTER_OPENGLES2_EX macro are not supported anymore. Create a ConfigureDemoAppEnvironment function instead.
* The FSL_REGISTER_WINDOW_DEMO and FSL_REGISTER_WINDOW_DEMO_EX macro are not supported anymore. Create a ConfigureDemoAppEnvironment function instead.
* DevIL updated from 1.7.8 to 1.8.0 on windows.
* Android ArmeAbi is no longer supported as its C++ library doesn't contain std::exception_ptr
* Visual Studio 2015 is now the default environment under windows.
* Visual Studio projects now utilize the full package name instead of the short package name. 
  So apps are called GLES2.S06_Texturing. This causes less conflicts when using sub packages.
* Example of building and running a command on a GLES2 project, this will take a screenshot after five frames and exit, it will also uses
  a fixed update time to ensure reproducible output.
    FslBuild.py --ForAllExe "(EXE) --Window [0,0,1920,1080] --ScreenshotFrequency 5 --ExitAfterFrame 5 --ForceUpdateTime 16666"
  On Windows the environment variables can be specified to control the GLES2 emulator:
    %FSLBUILD_VARIANT_FSL_GLES_NAME% = the name of the GLES2 emulator (equal to what can be specified to .StartProject.bat)
    %FSLBUILD_FEATURE_OPENGLES2% = Extra parameters for .StartProject.bat.
  Can be used to control the 
* FslBuild added EarlyAccess tag to all API's that are currently released in early access mode.
* FslBuild added argument:
  --RequireFeatures The list of features that are required for a executable to be build. For example [OpenGLES2] to build all executables that use OpenGLES2.
  The requirement is added on top of any existing feature requirements.
* FslBuildNew introduced, it is a replacement for FslNewDemoProject.py. It's simpler to extend with new templates and
  its package aware.
* FslBuildGen --Variants was renamed to --Variants, to make it more like FslBuild even though it only handles build variants
* Android vulkan builds also detects and handles conflicts with Vulkan and armeabi
  if 'all' are used the 'armeabi' is removed automatically as its not supported.
  if 'armabi' is the only ANDROID_ABI then a error is raised
* Android builds now have a ANDROID_ABI variant that can be set
  ANDROID_ABI=all,armeabi,armeabi-v7a,arm64-v8a,x86,x86_64,mips,mips64
  See FslBuildGen --ListVariants and --Variants
  See FslBuild --ListVariants and --Variants
* Android projects are now generated to the path specified in the environment variable
  FSL_GRAPHICS_SDK_ANDROID_PROJECT_DIR. If it's not defined the 'prepare' script
  sets it to a default location.
* Android builds now use gradle and CMake.
* Android builds now requires android-ndk-r12b and Android 7.0 (API 24) SDK Platform
* Vulkan is now available in early access.
* Assimp updated to 3.3.1
* Window now use the reference implementation of OpenVG.
* Windows is now a x64 build instead of a x86.
* A new WindowSystem implementation has replaced the old EGLBackend system.
  Use WindowSystem instead of EGLBackend now.
* QNX should now build out of the box, but its experimental.
* FslBuild --ForAllExe support.
  * Run A command for all executables with access to various build information
    (EXE) = the full path to the executable.
    (EXE_NAME) = name of the executable.
    (EXE_PATH) = the executables dir. 
    (PACKAGE_PATH) = full path to package
    (CONTENT_PATH) = full path to package content directory
  For example on windows to take a screenshot of all executables supporting a set of features:
    FslBuild.py --ForAllExe "WinScreenshot.bat (EXE)" -t sdk --UseFeatures [EGL,OpenGLES2,OpenGLES3,OpenGLES3.1,OpenVG,OpenCL,OpenCL1.1,OpenCL1.2]
* OpenCV support was split into OpenCV2_4 and OpenCV3_0 due to different libs and breaking changes.
* New Texture class and full support for loading it and creating it in GLTexture with the same level of format
  conversion support as we have for Bitmaps. The new texture class supports 1d, 2d, 3d, cubemaps with mipmaps,
  layers and compression. We also now have full ktx and dds loading support.
* Moved all the FsGraphics bitmap related files into their own directory.
  * FslGraphics/Bitmap            -> FslGraphics/Bitmap/Bitmap
  * FslGraphics/BitmapClearMethod -> FslGraphics/Bitmap/BitmapClearMethod
  * FslGraphics/BitmapOrigin      -> FslGraphics/Bitmap/BitmapOrigin
  * FslGraphics/BitmapUtil        -> FslGraphics/Bitmap/BitmapUtil
  * FslGraphics/RawBitmap         -> FslGraphics/Bitmap/RawBitmap
  * FslGraphics/RawBitmapEx       -> FslGraphics/Bitmap/RawBitmapEx
  * FslGraphics/RawBitmapUtil     -> FslGraphics/Bitmap/RawBitmapUtil
  * FslGraphics/RawCubeBitmap     -> FslGraphics/Bitmap/RawCubeBitmap
  * FslGraphics/RawCubeBitmapEx   -> FslGraphics/Bitmap/RawCubeBitmapEx
* FslBuildCheck can now do some code style sanity checks and even repair a bit of the common mistakes.
  --ScanSource = Scan source and check for common issues.
  --Repair     = If used in combination with --ScanSource the tool will attempt to fix common mistakes, 
                 beware this modifies your source files so use it at your own risk!
  --DryRun     = No files will be created.
* PixelFormat was updated to support all vulkan formats. 
  The formats was also made more explicit in their description which unfortunately meant the old formats had to be renamed.

    | Old name          | New name                   | Is supported by Vulkan |
    |-------------------|----------------------------|------------------------|
    | RGB888            | R8G8B8_UNORM               | Y                      |
    | BGR888            | B8G8R8_UNORM               | Y                      |
    | RGBA8888          | R8G8B8A8_UNORM             | Y                      |
    | BGRA8888          | B8G8R8A8_UNORM             | Y                      |
    | Vector4           | R32G32B32A32_SFLOAT        | Y                      |
    | HalfVector4       | R16G16B16A16_SFLOAT        | Y                      |
    | ALPHA8            | EX_ALPHA8_UNORM            | N                      |
    | LUMINANCE8        | EX_LUMINANCE8_UNORM        | N                      |
    | LUMINANCE_ALPHA88 | EX_LUMINANCE8_ALPHA8_UNORM | N                      |

  Beware the new name listed here is just a suggestion that will match well with how
  GLES2+3 treated the data.

* Environment variable FSLDEMOAPP_PREFERRED_WINDOW_RESOLUTION can now be set to the preferred window resolution. 
  It will be used if no --Window parameter is set, it also accepts the same values as --Window does.
* Renamed FslConfigCheck to FslBuildCheck and allowed it to utilize the FslBuildGen package.
  Introduced FslBuildContent which will be the new content pipeline for building Content.
  The FslBuildContent code automatically be executed for the build generators that dont support it.
* FslGraphicsGLES3v1 renamed to FslGraphicsGLES3_1 to follow the convention of the rest of the packages.
* FslBuild.py now support all platforms!
* FslBuild.py now support
  --BuildTime    = Time the build and log it at the end of the build.
  --ListVariants = List all variant and their options.
  --Variants     = specify a list of variants and their configuration
                   for example
                   "--Variants [config=Debug,FSL_GLES_NAME=vivante]" or
                   "--Variants [config=Debug,WindowSystem=X11]"
                   This means that we have build system independent way of
                   doing this so it works on all platforms.
* FslNativeWindow, FslNativeWindowEGL, FslNativeWindowPlatform created in preparation for Vulkan support (and future platforms)
* FslBuild.py now has experimental windows support.
* FslUtilOpenCL StringHelper.hpp was replaced by DebugStrings.hpp.
* FslUtilOpenVX
  * All GetHandle methods was renamed to Get. Since this library was marked 'early access' no deprecation cycle was needed.
  * Was split into version specific libraries (FSLUtilOpenVX is generic, FslUtilOpenVX1_0_1, FslUtilOpenVX1_1)
* FslUtilOpenCL
  * StringHelper.pp was replaced by DebugStrings.hpp
  * All GetHandle methods was renamed to Get. Since this library was marked 'early access' no deprecation cycle was needed.
  * Program was renamed to ProgramEx and replaced with another Program RAII class.
  * Was split into version specific libraries (FSLUtilOpenCL is generic, FslUtilOpenCL1_1, FslUtilOpenCL1_2)
* Legacy style struct enums was converted to C++11 enum class
  * BitmapClearMethod::Enum was changed to BitmapClearMethod
  * BlendState::Enum was changed to BlendState
  * ChunkType::Enum was changed to ChunkType
  * ConfigControl::Enum was changed to ConfigControl
  * ContentMonitorResultCommand was changed to ContentMonitorResultCommand.
  * DemoState::Enum was changed to DemoState
  * EventType::Enum was changed to EventType
  * OptimizationFlag::Enum was changed to OptimizationFlag.
  * PixelFormat::Enum was changed to PixelFormat.
  * PrimitiveType::Enum was changed to PrimitiveType.
  * SearchOptions::Enum was changed to SearchOptions.
  * StrideRequirement::Enum was changed to StrideRequirement.
  * TimeStepMode::Enum was changed to TimeStepMode.
  * Texture2DFilterHint::Enum was changed to Texture2DFilterHint.
  * VertexElementFormat::Enum was changed to VertexElementFormat.
  * VertexElementUsage::Enum was changed to VertexElementUsage.  
  * WindowMode::Enum was changed to WindowMode
* The build system was updated.
  * All build.sh type scripts was replaced by the FslBuild.py script.
    See Readme_FslBuild.txt. Seriously read that document! :)
  * All build documentation was updated to reflect this change.
  * Its recommended to run a clean on your git checkout, but be careful :)
  * QNX is available as a 'in-development' generator, it can be enabled by adding the '--dev' parameter
  * All build output locations were made more consistent
    * Ubuntu builds used to write to the "obj" directory it now writes to "build/Ubuntu"
    * Windows builds used to build to the "bin" directory it now writes to "build/Windows"
    * Yocto builds used to build to the "obj" directory it now writes to "build/Yocto"
    * QNX builds used to build to the "build_qnx" directory it now writes to "build/QNX/"
  * Build/project files are no longer submitted to GIT.
    Instead you are required to run FslBuild or FslBuildGen to generate them.
  * The generated Visual Studio project files do not detect changes to the build environment automatically.
    So its your job to run FslBuildGen when you change it by for example doing a "git pull"
    * The build can now be queried for a list of required features by using
       ```FslBuild.py --ListFeatures```
    * The build can be constrained by a list of available features. For example to build the entire SDK
     using the features EGL, OpenGLES2, OpenCL do this:
       ```FslBuild.py -t sdk --UseFeatures [EGL,OpenGLES2,OpenCL]```
    * For now the old build.sh files are still generated but they are located in the build/<platform' folder as a backup.
* Early access to new DemoApp types were added:
  * OpenCL
  * OpenVX
  However since this is early access they are not supported on all platforms nor are the interfaces
  considered stable.
