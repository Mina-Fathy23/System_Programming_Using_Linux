# System Programming Using Linux

A collection of custom Linux utility implementations in C, demonstrating fundamental system programming concepts and command-line utilities.

## Table of Contents

- [Overview](#overview)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Compilation](#compilation)
- [Utilities](#utilities)
  - [mypwd - Print Working Directory](#mypwd---print-working-directory)
- [Shell Versions](#shell-versions)
- [Example Outputs](#example-outputs)

## Overview

This repository contains custom implementations of essential Linux utilities written in C. These implementations showcase system-level programming techniques including file operations, process management, command parsing, environment variables, dynamic memory management, and file-descriptor manipulation.

## Project Structure

```
System_Programming_Using_Linux/
├── README.md
├── custom_utils/
│   └── pwd_util/
│       ├── mypwd.c          # Source code for custom pwd utility
│       └── mypwd            # Compiled executable
└── shells/                  # Incremental shell implementations
    ├── femto_shell/
    ├── pico_shell/
    ├── nano_shell/
    └── micro_shell/
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

Each shell can be compiled from its directory using the same pattern:

```bash
gcc <shell_source_file.c> -o <shell_executable>
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

## Shell Versions

The repository now includes four versions of a shell. They form an incremental progression, with each version adding a major systems-programming concept to the previous one.

### femto_shell — Parsing and basic built-ins

`femto_shell` is the baseline implementation. It provides a REPL loop with a fixed two-dimensional array tokenizer and supports only two built-ins:

- `echo`
- `exit`

All other commands print `Invalid command`. This version does not create processes or execute external programs.

### pico_shell — Processes and external commands

`pico_shell` extends `femto_shell` with:

- `pwd` and `cd` built-ins
- External command execution through `fork()` and `execvp()`
- `$PATH` command searching provided by `execvp()`
- Parent-side child status collection with `wait()` and `WEXITSTATUS`

It still uses a fixed-size two-dimensional array for arguments and does not provide variables or redirection.

### nano_shell — Dynamic arguments and local variables

`nano_shell` replaces the fixed argument array with heap-allocated, dynamically growing storage using `char **new_argv` together with `malloc()`, `realloc()`, and `free()`. It also introduces a local-variable system:

- `VAR=value` assignment detection through `check_local_var`
- Variable lookup through `find_local_var`
- `$var` expansion inside tokens through `get_special_arg`
- `export` using `putenv`
- `local` to list stored variables

This version uses a `strtok()`-based tokenizer, executes commands with `execvpe()` and a custom environment, guards against excessive arguments, and cleans up heap allocations when exiting.

### micro_shell — I/O redirection and robust cleanup

`micro_shell` builds on `nano_shell` by adding input and output redirection:

- `<` redirects standard input
- `>` redirects standard output
- `2>` redirects standard error

For each command, it saves the original standard file descriptors with `dup()`, opens the requested target with the appropriate flags and mode, and applies redirection with `dup2()`. Redirection tokens are removed from the command arguments into a new filtered array. After every command, `restore_std_fds()` restores the original descriptors.

It also adds robust error-path cleanup, including freeing partially built argument lists and restoring file descriptors when setup fails. File-access failures are reported with messages such as `No such file or directory` and `Permission denied`.

### Shell progression at a glance

| Shell | Main concepts |
| --- | --- |
| `femto_shell` | Parsing, `echo`, and `exit` |
| `pico_shell` | `pwd`/`cd` and external commands via `fork()` + `execvp()` |
| `nano_shell` | Dynamic memory, local variables, `$var` expansion, `export`, and `execvpe()` |
| `micro_shell` | `<`, `>`, and `2>` redirection, file-descriptor save/restore, and error-path cleanup |

## Example Outputs

### 1. myecho - Custom Echo Utility

The custom `myecho` utility prints text to the terminal:

```bash
$ ./myecho Hello, System Programming!
Hello, System Programming!

$ ./myecho Welcome to Linux
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
