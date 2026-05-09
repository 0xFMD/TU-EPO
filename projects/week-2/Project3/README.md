# Week 2 — Simple EDID parser

This project about reading and parsing an [EDID (Extended Display Identification Data)](https://en.wikipedia.org/wiki/Extended_Display_Identification_Data) binary file in C. The program reads the binary data from an EDID file, extracts relevant information, and prints this information to the console.

## EDID (Extended Display Identification Data)

EDID is a metadata format for display devices to describe their capabilities to a video source like GPU. It contains information about the display's manufacturer, model, supported resolutions, and more. EDID data is typically stored in a binary format and can be read by software to determine the features of the connected display.

## Linux EDID location

On Linux systems, EDID data can typically be found in the `/sys/class/drm/` directory. Each connected display will have a corresponding subdirectory (e.g, `card0-eDP-1`) that contains an `edid` file. This file can be read to obtain the metadata for that specific display. You can use the following command to read the EDID data for a display:

```bash
cat /sys/class/drm/{selected source}/edid > edid.bin
```
This command reads the EDID data from the specified display and saves it to a file named `edid.bin`, which can then be parsed by the C program to extract the relevant information.

> [!NOTE]
> `selected source` is the specific display you want to read from (e.g, `card1-DP-1`).


## Files

| File                 | Role                                                        |
| -------------------- | ----------------------------------------------------------- |
| [main.c](main.c)     | Main program that defines the EDID parsing logic            |
| [edid.bin](edid.bin) | Binary file containing the EDID data for a specific display |

## Compilation

```bash
gcc main.c -o main
./main ./edid.bin
```

## Output

When you run the program, it will read the `edid.bin` file, parse the data, and print the extracted information to the console:

```
File signature: 0x00FFFFFFFFFFFF00
EDID version: 1.4
Vendor: SHP
Product code: 5400
Serial Number: 0x00000000
Made in: week 45 of 2021
Input type:     Digital
Interface: DisplayPort
Bit depth: 8 bits per color
Size: 37 x 23 cm
Gamma: 2.20
```

## [output comparison from edid-decode cli tool](https://man.archlinux.org/man/edid-decode.1.en)

```
Block 0, Base EDID:
  EDID Structure Version & Revision: 1.4
  Vendor & Product Identification:
    Manufacturer: SHP
    Model: 5400
    Made in: week 45 of 2021
  Basic Display Parameters & Features:
    Digital display
    Bits per primary color channel: 8
    DisplayPort interface
    Maximum image size: 37 cm x 23 cm
    Gamma: 2.20
    Supported color formats: RGB 4:4:4, YCrCb 4:4:4
    Default (sRGB) color space is primary color space
    First detailed timing includes the native pixel format and preferred refresh rate
    Display supports continuous frequencies

```

## References

- [EDID Specs](https://en.wikipedia.org/wiki/Extended_Display_Identification_Data)
- [EDID-decode cli tool](https://man.archlinux.org/man/edid-decode.1.en)
