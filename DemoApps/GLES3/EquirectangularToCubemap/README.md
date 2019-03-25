<!-- #AG_DEMOAPP_HEADER_BEGIN# -->
# EquirectangularToCubemap
<img src="Example.jpg" height="135px">

<!-- #AG_DEMOAPP_HEADER_END# -->
<!-- #AG_BRIEF_BEGIN# -->
Convert a [equirectangular map](https://en.wikipedia.org/wiki/Equirectangular_projection) to a cubemap using OpenGL ES3.
<!-- #AG_BRIEF_END# -->

Inspired by:
- https://learnopengl.com/PBR/IBL/Diffuse-irradiance
- http://paulbourke.net/miscellaneous/cubemaps/

The app can be controlled using:

Key          | Result
-------------|----------------------------------
A            | Strafe left
W            | Strafe right
S            | Move forward
D            | Move backward
Right Mouse  | Mouse look (hold it)
Middle mouse | Reset camera

<!-- #AG_DEMOAPP_COMMANDLINE_ARGUMENTS_BEGIN# -->

Command line arguments':

Argument                       |Description                                                                                                                                                          |Source
-------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------|---------------
-s, --Scene \<arg>             |Select the scene to run (0 to 1)                                                                                                                                     |Demo
--DisplayId \<arg>             |DisplayId \<number>                                                                                                                                                  |DemoHost
--EGLAlphaSize \<arg>          |Force EGL_ALPHA_SIZE to the given value                                                                                                                              |DemoHost
--EGLBlueSize \<arg>           |Force EGL_BLUE_SIZE to the given value                                                                                                                               |DemoHost
--EGLDepthSize \<arg>          |Force EGL_DEPTH_SIZE to the given value                                                                                                                              |DemoHost
--EGLGreenSize \<arg>          |Force EGL_GREEN_SIZE to the given value                                                                                                                              |DemoHost
--EGLLogConfig                 |Output the EGL config to the log                                                                                                                                     |DemoHost
--EGLLogConfigs \<arg>         |Output the supported configurations to the log. 0=Off, 1=All, 2=HDR. Don't confuse this with LogConfig.                                                              |DemoHost
--EGLLogExtensions             |Output the EGL extensions to the log                                                                                                                                 |DemoHost
--EGLRedSize \<arg>            |Force EGL_RED_SIZE to the given value                                                                                                                                |DemoHost
--Window \<arg>                |Window mode [left,top,width,height]                                                                                                                                  |DemoHost
--AppFirewall                  |Enable the app firewall, reporting crashes on-screen instead of exiting                                                                                              |DemoHostManager
--ContentMonitor               |Monitor the Content directory for changes and restart the app on changes. WARNING: Might not work on all platforms and it might impact app performance (experimental)|DemoHostManager
--ExitAfterDuration \<arg>     |Exit after the given duration has passed. The value can be specified in seconds or milliseconds. For example 10s or 10ms.                                            |DemoHostManager
--ExitAfterFrame \<arg>        |Exit after the given number of frames has been rendered                                                                                                              |DemoHostManager
--ForceUpdateTime \<arg>       |Force the update time to be the given value in microseconds (can be useful when taking a lot of screen-shots). If 0 this option is disabled                          |DemoHostManager
--LogStats                     |Log basic rendering stats (this is equal to setting LogStatsMode to latest)                                                                                          |DemoHostManager
--LogStatsMode \<arg>          |Set the log stats mode, more advanced version of LogStats. Can be disabled, latest, average                                                                          |DemoHostManager
--ScreenshotFormat \<arg>      |Chose the format for the screenshot: bmp, jpg, png or tga (defaults to png)                                                                                          |DemoHostManager
--ScreenshotFrequency \<arg>   |Create a screenshot at the given frame frequency                                                                                                                     |DemoHostManager
--ScreenshotNamePrefix \<arg>  |Chose the screenshot name prefix (defaults to 'Screenshot')                                                                                                          |DemoHostManager
--ScreenshotNameScheme \<arg>  |Chose the screenshot name scheme: frame, sequence or exact (defaults to frame)                                                                                       |DemoHostManager
--Stats                        |Display basic frame profiling stats                                                                                                                                  |DemoHostManager
--Profiler.AverageEntries \<arg>|The number of frames used to calculate the average frame-time. Defaults to: 60                                                                                       |ProfilerService
--ghelp \<arg>                 |Display option groups: all, demo or host                                                                                                                             |base
-h, --help                     |Display options                                                                                                                                                      |base
-v, --verbose                  |Enable verbose output                                                                                                                                                |base
<!-- #AG_DEMOAPP_COMMANDLINE_ARGUMENTS_END# -->
