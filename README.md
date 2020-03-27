## source-interface-query

This is a small command-line utility that lets you grab exposed interfaces from Source Engine games. 

This has been tested with both Source 1 and Source 2. Currently the Windows version is cross compiled using Mingw, so some bugs might be present if you use this on Windows, however, it seems to work great with wine.

### Building
 
Simply run `make` in the root directory to compile. You'll only need system libraries.

To install, run `sudo make INSTALL_PREFIX=/my/install/prefix`

### Usage

Using the tool is as simple as:
```bash
source-interface-query -i VCvarQuery001 -l /path/to/hl2/bin engine.so
```
If the command succeeds, it will spit out the pointer to the vtable and say `IFACE_OK`
