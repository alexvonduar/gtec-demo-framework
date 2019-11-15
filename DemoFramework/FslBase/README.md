# FslBase

The base library that everything in the framework is build upon.

| Type        | Description                                                   |
|-------------|---------------------------------------------------------------|
| Bits        | Common bit manipulation                                       |
| IO          | Platform independent IO operations missing from C++14 and STL |
| Log         | A very basic logging system                                   |
| Math        | Basic math                                                    |
| String      | Common string functionality missing from C++14 and STL        |
| System      | More 'system' level code missing from C++14 and STL           |
| Transition  | Simple transition value classes, useful for simple animation  |


## Platform configuration defines

| Name                           | Description                                                              |
|--------------------------------|--------------------------------------------------------------------------|
| FSLBASE_THREAD_BACKEND_PTHREAD | Disable the standard C++11 thread implementation and use pthread instead |
