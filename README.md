# DIR Command — Windows CLI Reimplementation

A C++ reimplementation of the Windows `DIR` command using the Win32 API. This project replicates the core behavior of the native `dir` command, supporting multiple switches and attribute filters.

---

## Features

- List files and directories in a given path
- Display file size, last modified time, and file attributes
- Support for multiple command-line switches (`/A`, `/B`, `/S`, `/O`, `/W`, `/P`, `/Q`, `/R`, `/T`, `/X`, `/L`, `/N`, `/D`, `/C`, `/?`)
- Attribute filtering via `/A` (Directory, Hidden, System, Read-only, Archive, Reparse Point, Offline, Not Content Indexed)
- Bare format listing with `/B`
- Recursive subdirectory traversal with `/S`
- Volume serial number and free disk space display
- Invalid switch detection and error reporting

---

## Requirements

- Windows OS
- C++ compiler with Win32 API support (e.g., MinGW, MSVC)
- Windows SDK

---

## Build

### Using g++ (MinGW)

```bash
g++ timtenfile.cpp -o timtenfile.exe -lkernel32
```

### Using MSVC

```bash
cl timtenfile.cpp /Fe:timtenfile.exe
```

---

## Usage

```
timtenfile [drive:][path][filename] [/A[[:]attributes]] [/B] [/C] [/D] [/L] [/N]
          [/O[[:]sortorder]] [/P] [/Q] [/R] [/S] [/T[[:]timefield]] [/W] [/X] [/4]
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

# Bare format + recursive
timtenfile /B /S

# List files with read-only attribute
timtenfile /AR

# Show help
timtenfile /?
```

---

## Supported Switches

| Switch | Description |
|--------|-------------|
| `/A`   | Display files with specified attributes |
| `/AD`  | Directories only |
| `/AH`  | Hidden files only |
| `/AS`  | System files only |
| `/AR`  | Read-only files only |
| `/AA`  | Files ready for archiving |
| `/AL`  | Reparse points (junctions) only |
| `/AI`  | Not content indexed files |
| `/AO`  | Offline files |
| `/B`   | Bare format — filenames only, no header or summary |
| `/S`   | Recurse into all subdirectories |
| `/?`   | Display help information |

> Note: Switches can be combined, e.g. `/B /S` for a bare recursive listing.

---

## Output Format

Default listing output follows the standard `dir` format:

```
 Volume in drive C is Windows-SSD
 Volume Serial Number is XXXX-XXXX

 Directory of C:\some\path

03/28/2026  10:30    <DIR>               folder_name
03/27/2026  08:15              4,096 file.txt
               1 File(s)          4,096 bytes
               1 Dir(s)   50,000,000,000 bytes free
```



---

## Key Win32 APIs Used

| API | Purpose |
|-----|---------|
| `FindFirstFile` / `FindNextFile` | Enumerate files and directories |
| `FindClose` | Release file search handle |
| `GetCurrentDirectory` | Retrieve the current working directory |
| `GetVolumeInformation` | Retrieve volume name and serial number |
| `GetDiskFreeSpaceEx` | Query available disk space |
| `FileTimeToSystemTime` | Convert `FILETIME` to human-readable `SYSTEMTIME` |


