# System Programming Using Linux

A collection of custom Linux utility implementations in C, demonstrating fundamental system programming concepts and command-line utilities.

## Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Compilation](#compilation)
- [Utilities](#utilities)
  - [mypwd - Print Working Directory](#mypwd---print-working-directory)
- [Example Outputs](#example-outputs)

## Overview

This repository contains custom implementations of essential Linux utilities written in C. These implementations showcase system-level programming techniques including file operations, process management, and system calls on Linux platforms.

## Project Structure

```
System_Programming_Using_Linux/
├── README.md
└── custom_utils/
    └── pwd_util/
        ├── mypwd.c          # Source code for custom pwd utility
        └── mypwd            # Compiled executable
```

## Prerequisites

To compile and run the programs in this repository, you need:

- **GCC compiler** (GNU Compiler Collection)
- **Linux/Unix environment** (Linux kernel preferred)
- **Standard C library (libc)**

Install GCC on your system:

```bash
# For Ubuntu/Debian
sudo apt-get install build-essential

# For Fedora/RHEL
sudo yum install gcc

# For macOS (using Homebrew)
brew install gcc
```

## Compilation

### Compile Individual Utilities

To compile any C source file into an executable, use the following GCC command format:

```bash
gcc -o <output_executable> <source_file.c>
```

### Compile All Programs

To compile all utilities at once, navigate to the repository root and execute:

```bash
# Compile mypwd utility
gcc -o custom_utils/pwd_util/mypwd custom_utils/pwd_util/mypwd.c
```

### Compilation Options

For development with debugging symbols:

```bash
gcc -g -o custom_utils/pwd_util/mypwd custom_utils/pwd_util/mypwd.c
```

For optimized builds:

```bash
gcc -O2 -o custom_utils/pwd_util/mypwd custom_utils/pwd_util/mypwd.c
```

## Utilities

### mypwd - Print Working Directory

**Purpose:** Displays the absolute path of the current working directory, mimicking the standard Linux `pwd` utility.

**Source:** `custom_utils/pwd_util/mypwd.c`

**How it works:**
- Uses the `getcwd()` system call to retrieve the current working directory
- Dynamically allocates memory for the path string (passing NULL as the first argument)
- Handles errors gracefully by printing an error message if `getcwd()` fails
- Returns the absolute path of the current directory

**Usage:**

```bash
./custom_utils/pwd_util/mypwd
```

## Example Outputs

### 1. Echo Utility

The standard Linux `echo` command prints text to the terminal:

```bash
$ echo "Hello, System Programming!"
Hello, System Programming!

$ echo "Welcome to Linux"
Welcome to Linux

$ echo -e "Line 1\nLine 2\nLine 3"
Line 1
Line 2
Line 3
```

### 2. Pwd Utility (Custom Implementation)

Our custom `mypwd` utility and the standard `pwd` command:

```bash
$ pwd
/home/user/System_Programming_Using_Linux

$ ./custom_utils/pwd_util/mypwd
/home/user/System_Programming_Using_Linux

$ cd custom_utils/pwd_util && pwd
/home/user/System_Programming_Using_Linux/custom_utils/pwd_util

$ cd custom_utils/pwd_util && ../../mypwd
/home/user/System_Programming_Using_Linux/custom_utils/pwd_util
```

### 3. Cat Utility

The `cat` command displays file contents:

```bash
$ cat mypwd.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    char * buf = getcwd(NULL, 0);
    
    if(buf == NULL)
    {
        printf("Faild to Find Current Working directorys' path");
        exit(-1);
    }
    else
        printf("%s\n", buf);
    return 0;
}

$ cat README.md | head -5
# System Programming Using Linux

A collection of custom Linux utility implementations in C...
```

### 4. Cp Utility

The `cp` command copies files:

```bash
$ cp custom_utils/pwd_util/mypwd.c custom_utils/pwd_util/mypwd_backup.c
$ ls custom_utils/pwd_util/
mypwd  mypwd.c  mypwd_backup.c

$ cat custom_utils/pwd_util/mypwd_backup.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
...

# Copy with verbose output
$ cp -v custom_utils/pwd_util/mypwd.c /tmp/mypwd_copy.c
'custom_utils/pwd_util/mypwd.c' -> '/tmp/mypwd_copy.c'
```

### 5. Mv Utility

The `mv` command moves or renames files:

```bash
$ mv custom_utils/pwd_util/mypwd_backup.c custom_utils/pwd_util/mypwd_old.c
$ ls custom_utils/pwd_util/
mypwd  mypwd.c  mypwd_old.c

# Rename a file
$ mv custom_utils/pwd_util/mypwd_old.c custom_utils/pwd_util/mypwd.c.bak
$ ls custom_utils/pwd_util/
mypwd  mypwd.c  mypwd.c.bak

# Move file to another directory
$ mv custom_utils/pwd_util/mypwd.c.bak /tmp/
$ ls custom_utils/pwd_util/
mypwd  mypwd.c
```

## Running the Programs

After compilation, run any utility:

```bash
# Navigate to the utility directory
cd custom_utils/pwd_util/

# Run the custom pwd utility
./mypwd
```

## License

This project is open source and available for educational purposes.

## Author

Created by Mina-Fathy23

---

**Note:** This repository is designed for learning system programming concepts in Linux. Feel free to extend it with additional utilities or enhancements!
