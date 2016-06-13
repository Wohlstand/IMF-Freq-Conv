# IMF-Freq-Conv
A tiny tool which converts a chunk delays in the IMF (id Software Music Format) between 280Hz, 560hz and 700Hz tempo speeds.

# Compiling
To compile utility you just need to use a compiler command

## Linux / Mac OS X / FreeBSD
With GCC
```bash
gcc imfreqc.c -o imfreqc
```
With CLang
```bash
clang imfreqc.c -o imfreqc
```
## Windows

With GCC (MinGW or cygwin)
```bash
gcc imfreqc.c -o imfreqc
```

With CL (Visual Studio, you must open Visual Studio command environment or open cl.exe directly from it's folder)
```cmd
cl imfreqc.c /link /out:imfreqc.exe
```

# Usage
```text
Syntax: imflench <SourceFrequency> <oldfile>.imf <DestFrequency> <newfile>.imf

SourceFrequency - number 280 560 or 700
DestFrequency   - number 280 560 or 700

------------------------------------------------------------------------------
Supported frequencies:

------------------------------------------------------------------------------
  280Hz - Duke Nukem II
  560Hz - Bio Menace, Commander Keen, Cosmo's Cosmic Adventures, Monster Bash
  700Hz - Blake Stone, Major Stryker, Operation Body Count, Wolfenstein 3-D

------------------------------------------------------------------------------
```
