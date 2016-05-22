# Compiling ROOT

A quick guide to compiling [ROOT](https://root.cern.ch/) based projects

===

## Reason I don't like using CINT

* Inconsistent behaviour in `.L file.cc` and `.L file.cc+`
* Loading multiple source code files is difficult:
   * All-in-one headers (needlessly long compile time)
   * Structure-less `rootLogOn.C` files

-----

*We should use ROOT like a C++ library*!

---

### Short answer

```
g++ $(root-config --cflags --glibs) main.cc
```

If you understand why this works, you can handle your analysis project better!


===

## Compiling vs. Linking

---

### A nice structure for a mini project:
* `main.cc`:
   * Contains only high level functions

* `file_1.cc`, `file_2.cc`...
   * Order functions in files by relevance

```c++
// Prototyping....
ChannelMgr signal, bg, data;
int main(){
   ReadNtupleFiles( signal , "signal_file.root" );  // See file read.cc
   PrintSummaryTable( bg );  // See file print.cc
   RunChiSqaureReco( data ); // See file reco.cc
   RunTrackFitReco( data );  // See file reco.cc
   PlotValidatations( signal, bg, data );  // See file plot.cc
}
```
---

### Compiling multiple files

Simple command
```
g++ main.cc read.cc print.cc ....
# g++ *.cc
```

-------

Problems

* Change one, compile everything.
* With classes, we might need multiple main functions!

---

### Shared Object files

**Compiling** - Turning human readable text into machine readable commands

> What about repeated/similar machine code? Like standard functions printf(), sqrt()... ?

* Mark command files to find commands in other files!
* Compiled files don't know where to find the missing commands!

---

**Linking** - Changing compiled marks to concrete file position
   * *UNIX* - `.so` (shared object)
   * *WINDOWS* - `.dll` (dynamically linked libraries)

---

**Binary** - Completely linked command files with **initiate** and **terminate** commands

---

### Why our simple command works?

When `g++` sees a `main` function, it assumes you want to compile, link and complete a binary file for you!

---

### Why do we want this?

To share code functionality, you only need to share:
  * the shared object file it self
  * what is inside it

---

### Making shared object files

Compiling (produces multiple .o files)
```
g++ -c -fPIC *.cc
```

Linking and merging (define filename yourself)
```
g++ --shared file_1.o file_2.o class.o  -o libShared.so
```
Making Binary (note filename!)
```
g++ -L. -lShared main.o   -o a.out
g++ -L. -lShared main_2.o -o a_2.out
```

---

### Shared Object file dependencies

```
g++ --shared file_4.cc -o libNewShared.so
g++ -L. -lNewShared main.cc  ## Compile error
```

Shared object files can be made with .o files and other Shared object files
```
g++ --shared file_4.cc libShared.so -o libNewShared.so
g++ -L. -lNewShared main.cc  ## OK!!
```

===

### Using shared objects files
```
g++ -L<path> -l<filename> -l<filename_2>
```
* `<filename>`- name of shared object filename   
   * omitts prefix `lib` and post fix `.so`)

* `<path>`- Where `g++` should try to look for .so files
   * `echo $LD_LIBRARY_PATH` for default path!

-------

Collection of `.so` file(s) `.so` constitutes a **library**.

The most common library used: `libstdc++.so`


---

## Importance of `#include`


* We require need to know that contents of a shared object file to be able to use it
* Header files that are included in both library source and main function ensures both sides use the function in the same form!

```
g++ -I<path>
```
Tells what paths the `include` should try to find the files!

===

## ROOT as C++ Library

---

Armed with this knowledge, we just need to use find what options we need to put in `g++`!

```
>>> whereis root
/usr/include/root /usr/lib/root ...

>>> ls /usr/lib/root/
libHistFactory.so libHistPainter.so libCore.so  ...
```

------

```
g++ -I/usr/include/root -L/usr/lib/root -l???? main.cc
```
---

### A problem - which libraries???

Even if you do guess right (or simply add all of them), you will likely get a compile error

------

ROOT is compiled with additional flags!
  * Must include multi-threading
  * Uses C++11 standard

---

### Get Root to tell it to you!
```
root-config --cflags --libs #--glibs for GUI related work
```

Copy and paste results?

---

### Subshell expansion
```
g++ $(root-config --cflags --libs) main.cc
```
-------
Run command
```
root-config --cflags --libs
```
and insert output into command
```
g++ <> main.cc
```

===

## Limitations of shared objects files

* Must be understood by native compiler/shared object files
   * Unix/Windows incompatibility
   * Linux/OSX incompatibility
   * Distribution incompatibility

* Linked files must be in same location

---

## Solution

* Always compile from source
* Use uniform structure
   * Unify linux distribution (Scientific Linux)
* Unify compile tools
   * CMS Software



===

### A look at automation

* Track what commands to type
* avoid repeated compilation

---

### Introduction to make and makefiles
`make <target>` - Looks in directory for a file named Makefile to see how to compile target.

In Makefile - A list of things to compile

```
<target>:<dependencies>
   commands need to create target
```

---

### Advantages of using make

* Keeps track of what commands to run
* Only runs command if dependencies have updates
* Define variables

------
Only mention in passing:

* Globbing
* Computer track dependencies
* Calling other make files to link projects together

---

### RAW make files are difficult to write!!

* New advanced tool:
   * cmake
   * CMSSW

===

## Compiling in CMSSW

```
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_7_6_3_patch2
cd CMSSW_7_6_3_patch2/src
cmsrel
```

---

### Structure of CMSSW source codes

```
src
├── Package
    └── SubPackage
        ├── bin       <= files with main function goes here
        ├── interface <= All header file here
        ├── src       <- All other source code
```
Everything in `Package/SubPackage/src` will be compile into unique library.

---

### Specifying library dependencies - src

Write `BuildFile.xml`, and for each required library, write:
```
<use name="Package/Subpackage"/>
```
At end of file add
```
<export><lib name="1"/></export>
```

---

-------
Special packages
```
<use name="root"/>
<use name="roofit"/>
<use name="boost"/>
```
-------
Note packages found in the [CMSSW source code](https://github.com/cms-sw/cmssw)
could be included though you cannot see them
```
<use name="TopQuarkAnalysis/TopHitFit"/>
```


---

### Building a binary

In `bin` write a `BuildFile.xml`

  * Add required library like above
  * For each binary requrested, add

```
<bin name="executablename" file="your_main_file.cc"/>
```

-------

Compile using `scram -b`

-------

Compiled binary appear in
```
CMSSW_X_X_X/bin/slcX_amd64_gccXXX/
```

===

# Thanks for Listening!
### Happy coding!
