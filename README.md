#

## Folder Structure

`build/`: A special directory that should not be considered part of the source of the project. Used for storing ephemeral build results. must not be checked into source control. If using source control, must be ignored using source control ignore-lists.

`src/`: Main compilable source location. Must be present for projects with compiled components that do not use submodules. In the presence of include/, also contains private headers.

`include/`: Directory for public headers. May be present. May be omitted for projects that do not distinguish between private/public headers. May be omitted for projects that use submodules.

`tests/`: Directory for tests.

`examples/`: Directory for samples and examples.

`external/`: Directory for packages/projects to be used by the project, but not edited as part of the project.

`extras/`: Directory containing extra/optional submodules for the project.

`data/`: Directory containing non-source code aspects of the project. This might include graphics and markup files.

`tools/`: Directory containing development utilities, such as build and refactoring scripts

`docs/`: Directory for project documentation.

`libs/`: Directory for main project submodules.

## Dev

- git clone git://sigrok.org/libserialport
- brew install automake
- brew install libtool

- Driver USB Serial Chipset Prolific: <http://www.prolific.com.tw/US/ShowProduct.aspx?p_id=229&pcid=41>
- Driver USB Serial Chipset CH340: <https://blog.sengotta.net/signed-mac-os-driver-for-winchiphead-ch340-serial-bridge/>

### Build

```bash
cc src/hello.c -o build/hello -lserialport
```

### RUN

./build/hello
