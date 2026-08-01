## CMake build fails debug

### Error 1:

- The build failed because your Linux system is missing the X11 Screen Saver development package (XSCRNSAVER),
- which SDL3 requires for its X11 video driver components.
- You can fix this either by installing the missing package on your operating system or by telling CMake to skip that specific feature.

- **Fix Option 1**: Install the Dependency (Recommended)Depending on your Linux distribution,
- run the corresponding command to install the required development package:
  Ubuntu / Debian / Mint:

```bash
sudo apt-get update
sudo apt-get install libxss-dev
```

### Error 2

- The configuration failed again because your system is missing the X11 Testing Extension development package (XTEST),
- which SDL3 requires for synthesizing input events under X11.The actual system package name for this extension is libXtst.
- You can resolve this error by either installing the missing library or explicitly bypassing it.

- **Fix Option 1**: Install the Dependency (Recommended)
- Run the appropriate command below based on your Linux operating system to pull in the development headers:
- Ubuntu / Debian / Mint:

```bash
sudo apt-get update
sudo apt-get install libxtst-dev
```

### Error 3
- [ISSUE] when editing the cpp file the cute.h lib is no where to be found , which fails the cmake and comilation.
- When compiling the source file the project cannot find the cute frameowork headers files. 
- [The Problem: Missing Static Library
The error No rule to make target 'cute/libcute.a' means the Cute Framework wasn't built as a static library. 
This happens because the framework's CMake configuration didn't properly create the library target.]

- **solution**:
- 1. create c_cpp_properties.json file with content below
- 2. 
.vscode/c_cpp_properties.json
```json {
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "${workspaceFolder}/libs/cute_framework/include"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/g++",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}```

.vscode/tasks.json

```json 
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "build",
            "type": "shell",
            "command": "cd ${workspaceFolder}/build && make && ./RacingGame_one.lnx",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": []
        }
    ]
}
```
- 3. Update the cmake list to this minimal version
```bash

cmake_minimum_required(VERSION 3.14)
project(RacingGame_one LANGUAGES C CXX)

set(CUTE_FRAMEWORK_STATIC ON)

add_subdirectory(libs/cute_framework)

add_executable(RacingGame_one src/main.cpp)
target_link_libraries(RacingGame_one cute)

```
- 3. restart the vscode window 
