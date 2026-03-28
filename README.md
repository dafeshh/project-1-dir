<div align="center">

<br/>


**A faithful C++ reimplementation of the Windows `DIR` command using the Win32 API**

<br/>

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=c%2B%2B&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=flat-square&logo=windows&logoColor=white)
![Win32](https://img.shields.io/badge/API-Win32-5C2D91?style=flat-square&logo=microsoft&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green?style=flat-square)
![Build](https://img.shields.io/badge/Build-MinGW%20%7C%20MSVC-orange?style=flat-square)

<br/>

</div>

---

## Overview

`timtenfile` replicates the core behavior of the native Windows `dir` command — built from scratch using raw Win32 APIs. It supports attribute filtering, recursive traversal, sorting, multiple display modes, and volume information — all through a familiar CLI interface.

---

## Features

- 📁 List files and directories with **size**, **timestamp**, and **attribute** display
- 🔍 **Attribute filtering** via `/A` — filter by directory, hidden, system, read-only, archive, reparse point, offline, and more
- 🌳 **Recursive traversal** with `/S` — walk entire directory trees
- 📄 **Bare format** with `/B` — output filenames only, perfect for scripting
- 💽 **Volume info** — serial number and free disk space
- ⚠️ **Invalid switch detection** with meaningful error messages

---

## Requirements

| Requirement | Details |
|---|---|
| OS | Windows (any modern version) |
| Compiler | MinGW (g++) or MSVC |
| SDK | Windows SDK |

---

## Build

### MSVC

```bash
cl timtenfile.cpp /Fe:timtenfile.exe
```

---

## Usage

```
timtenfile [drive:][path][filename] [switches]
```

### Examples

```bash
# List files in the current directory
timtenfile

# List files in a specific path
timtenfile C:\Users\John\Documents

# List only directories
timtenfile /AD

# List hidden files
timtenfile /AH

# Bare format (filenames only)
timtenfile /B

# Recursive listing of all subdirectories
timtenfile /S

# Combine switches — bare format + recursive
timtenfile /B /S

# List read-only files
timtenfile /AR

# Show help
timtenfile /?
```

---

## Supported Switches

### Attribute Filters (`/A`)

| Switch | Description |
|--------|-------------|
| `/A`   | Display files with specified attributes |
| `/AD`  | Directories only |
| `/AH`  | Hidden files |
| `/AS`  | System files |
| `/AR`  | Read-only files |
| `/AA`  | Files ready for archiving |
| `/AL`  | Reparse points (junctions/symlinks) |
| `/AI`  | Not content indexed files |
| `/AO`  | Offline files |

### Display Options

| Switch | Description |
|--------|-------------|
| `/B`   | Bare format — filenames only, no header or summary |


### Traversal & Pagination

| Switch | Description |
|--------|-------------|
| `/S`   | Recurse into all subdirectories |



> **Tip:** Switches can be combined freely, e.g. `/B /S /AH` for a bare recursive listing of hidden files.

---

## Output Format

Default output follows the standard `dir` layout:

```
 Volume in drive C is Windows-SSD
 Volume Serial Number is ABCD-1234

 Directory of C:\some\path

03/28/2026  10:30    <DIR>          folder_name
03/27/2026  08:15              4,096 file.txt
               1 File(s)              4,096 bytes
               1 Dir(s)   50,000,000,000 bytes free
```

With `/B` (bare format):

```
folder_name
file.txt
```

---

## Win32 APIs Used

| API | Purpose |
|-----|---------|
| `FindFirstFile` / `FindNextFile` | Enumerate files and directories |
| `FindClose` | Release file search handle |
| `GetCurrentDirectory` | Retrieve the current working directory |
| `GetVolumeInformation` | Retrieve volume name and serial number |
| `GetDiskFreeSpaceEx` | Query available disk space |
| `FileTimeToSystemTime` | Convert `FILETIME` to human-readable `SYSTEMTIME` |

---

## Project Structure

```
.
└── timtenfile.cpp      # Full implementation — single translation unit
```

---

## License

This project is released under the [MIT License](LICENSE).

---

<div align="center">

Built with C++ · Powered by Win32 · Inspired by the classic Windows CLI

</div>