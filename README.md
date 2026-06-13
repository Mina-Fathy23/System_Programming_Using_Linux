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
gcc <source_file.c> -o <output_executable>
```

### Compile All Programs

To compile all utilities at once, navigate to the repository root and execute:

```bash
# Compile mypwd utility
gcc custom_utils/pwd_util/mypwd.c -o custom_utils/pwd_util/mypwd
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

### 1. myecho - Custom Echo Utility

The custom `myecho` utility prints text to the terminal:

```bash
$ ./myecho "Hello, System Programming!"
Hello, System Programming!

$ ./myecho "Welcome to Linux"
Welcome to Linux
```

### 2. mypwd - Custom Print Working Directory Utility

Our custom `mypwd` utility displays the current working directory:

```bash
$ ./custom_utils/pwd_util/mypwd
/home/user/System_Programming_Using_Linux

$ cd custom_utils/pwd_util && ./mypwd
/home/user/System_Programming_Using_Linux/custom_utils/pwd_util
```

### 3. mycat - Custom Cat Utility

The `mycat` utility displays file contents:

```bash
$ ./mycat mypwd.c
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

$ ./mycat README.md
# System Programming Using Linux

A collection of custom Linux utility implementations in C...
```

### 4. mycp - Custom Copy Utility

The `mycp` utility copies files:

```bash
$ ./mycp custom_utils/pwd_util/mypwd.c custom_utils/pwd_util/mypwd_backup.c
$ ls custom_utils/pwd_util/
mypwd  mypwd.c  mypwd_backup.c

$ ./mycat custom_utils/pwd_util/mypwd_backup.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
...
```

### 5. mymv - Custom Move Utility

The `mymv` utility moves or renames files:

```bash
$ ./mymv custom_utils/pwd_util/mypwd_backup.c custom_utils/pwd_util/mypwd_old.c
$ ls custom_utils/pwd_util/
mypwd  mypwd.c  mypwd_old.c

$ ./mymv custom_utils/pwd_util/mypwd_old.c custom_utils/pwd_util/mypwd.c.bak
$ ls custom_utils/pwd_util/
mypwd  mypwd.c  mypwd.c.bak
```

## Running the Programs

After compilation, run any utility:

```bash
# Navigate to the utility directory
cd custom_utils/pwd_util/

# Run the custom pwd utility
./mypwd
```

## Author

Created by Mina-Fathy23

---

**Note:** This repository is designed for learning system programming concepts in Linux. Feel free to extend it with additional utilities or enhancements!
