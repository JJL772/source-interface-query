## source-interface-query

This is a small command-line utility that lets you grab exposed interfaces from Source Engine games. Currently only Linux is supported.

### Building
 
Simply run `make` in the root directory to compile. You'll only need system libraries.

To install, run `sudo make INSTALL_PREFIX=/my/install/prefix`

### Usage

Using the tool is as simple as:
```bash
source-interface-query -i VCvarQuery001 -l /path/to/hl2/bin engine.so
```
If the command succeeds, it will spit out the pointer to the vtable and say `IFACE_OK`
