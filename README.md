 # Duplex Printer
Virtual printer that adds ecological features to physical printers; features such as multiple pages per sheet, forced monochromatic, and duplex printing.
The goal for this project is to simplify duplex printing on old and cheaper printers that are still in use and may not provide modern functionalities.

DuplexPrinter is a QT5 frontend for GhostScript and for PSUtils, the PostScript utilities by Angus Duggan.

## Platform Support
My original goal was to make this software crossplatform, but chaining the backend components through pipes turned out not to be a feasible software development architecture on Windows. Because of this, instead of working around it, I will only give support for Unix systems.
Currently only Linux has support, but I have hopes to add Mac OS support after the program is complete.
