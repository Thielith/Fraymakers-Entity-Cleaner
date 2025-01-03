# Fraymakers Entity Cleaner
 Removes any unused object definitions from a Fraymakers .entity file.

## Usage
Define the .entity file to be used with `(-f --file) [path/to/file.entity]`

If you want to change the output location, use the flags `(-o or --output) [path/to/save]`

The program prints this information with `-h` if you ever forget.

## Libraries Used
- [Entity Extractor](https://github.com/Thielith/Fraymakers-Entity-Data-Extractor): For extracting the data from the .entity files
  - [jsoncpp](https://github.com/open-source-parsers/jsoncpp): For parsing the .entity files.
  - [mingw-w64-jsoncpp](https://aur.archlinux.org/packages/mingw-w64-jsoncpp): For compiling the Windows executable version of the program.


## Compiling the Stuff in the Repository
1. Follow the install instructions for the used libraries.
2. Pick your ~~poison~~ platform of choice below.

### Linux
1. Download source files.
    - If you want to make Windows executables, also install [mingw-w64-jsoncpp](https://aur.archlinux.org/packages/mingw-w64-jsoncpp). If you don't know how, just use `yay`
2. Run the included makefile.
    - Note:  It assumes you followed the instructions from the [Entity Extractor](https://github.com/Thielith/Fraymakers-Entity-Data-Extractor) library page and put the libraries and header stuff in `/usr/local/lib` and `/usr/local/include` respectivley.  If you didn't, then modify the paths in the `LOOK_HERE_TOO` makefile variable.
    - To make Windows executables, run `make windows`

### Windows (TBD)

