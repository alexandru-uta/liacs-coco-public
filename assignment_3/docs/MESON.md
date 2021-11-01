# Meson
Meson is a modern, speedy, platform-independent, incremental compiler.
In Meson, we use a python-like syntax to define sources, dependencies, generators etc etc.
In this Section, we explain the basics of using Meson.



## Installing Meson
Installing Meson is simple, and requires no root/sudo/su privileges.
See [mesonbuild.com](https://mesonbuild.com/SimpleStart.html#installing-meson) for installation instructions for all popular platforms.
Below, we provide information on installing Meson on Linux machines.


### Installing Meson - Linux
The easiest way to install Meson on Linux is by using pip:
```bash
pip3 install meson ninja --user
```
> **Note**: The `--user` flag installs given packages in user-mode, meaning we do not need superuser privileges.

After installation, the required executables (called `meson` and `ninja`) exist in `~/.local/bin`.
> **Note**: Don't forget to add `~/.local/bin` to your `$PATH` environment variable.
> By doing that, you can execute simply `meson <command>` and `ninja <command>` from your terminal.



## Instantiating Meson
We instantiate this project (only required once) by executing:
```bash
mkdir build
cd build
meson ..
```
Meson initializes itself, generates build files, executables, libraries etc in the `build/` dir.

> **Note**: At this point we expect you to have `flex` and `bison` installed


## Building with Meson
To compile this framework, execute (from `build/`):
```bash
meson
ninja
```

`meson` generates the compilation files with the default configuration options.
All configuration options are available in [`meson-options.txt`](/meson_options.txt).
Print the current configuration using:
```bash
meson configure
```
The options printed at the bottom are defined by this project.

To set options **initially**, use `-Doption=value`, e.g:
```bash
meson -Dwith-intermediate-code=own -Dwith-lexical=own -Dwith-machine-code=own -Dwith-syntax=own -Dwith-tests=true
```

**After initial configuration**, every time you wish to change compilation options, you should regenerate the compilation files using:
```bash
meson -Doption=value... --reconfigure
```
After (re)building the compilation files, (re)compile using `ninja`.



## Configuring Project Options
Each of the 4 main modules (`lexical`, `syntax`, `intermediate-code`, `machine-code`) can be compiled by setting the `with-<module>=true` option, e.g:
```bash
meson -Dwith-lexical=own --reconfigure
```
This command compiles only the library and main executable for the `lexical` module.
To also build the `syntax` module, we use:
```bash
meson -Dwith-lexical=own -Dwith-syntax=own --reconfigure
```
> **Warning**: When we are interested in compiling the `syntax` module,
> we need to compile the `lexical` module as well, because `syntax` requires `lexical`.
> If we forget the `-Dwith-lexical=own` in this situation, we will get compiler errors
> stating that lexical code could not be found.

Every next module requires the previous module(s) to be built as well.

### Prebuilt executables
For some assignments, you will be given a prebuilt library.
We do this, so you can better test your code.
This framework allows you to compile with a prebuilt library instead of building and using your own module.
To build with e.g. a prebuilt `syntax` module, we use:
```bash
meson -Dwith-lexical=own -Dwith-syntax=prebuilt ... --reconfigure
```
This works, provided that `prebuilt/` indeed contains `libsyntax.so`, the prebuilt syntax library.
The related executables (`coco_compiler_syntax` and `libsyntax_test`) are also built, and use the prebuilt library.

If you no longer want to use the prebuilt library and switch back to your own implementations, just use:
```bash
meson -Dwith-lexical=own -Dwith-syntax=own ... --reconfigure
```


### Configuring Test Executables
To compile test executables for all modules set to compile, we use:
```bash
meson -Dwith-lexical=own -Dwith-syntax=own -Dwith-tests=true --reconfigure
```
This command builds the libraries, main executable, *and test executable* for the `lexical` and `syntax` module.



## Executable and Library Locations
Compiled executables and libraries can be found in `build/`. The exact locations for each binary:
* lexical:
   - `build/src/lexical/coco_compiler_lexical`
   - `build/src/lexical/liblexical.so`
   - `build/src/lexical/src/test/liblexical_test`
* syntax:
   - `build/src/syntax/coco_compiler_syntax`
   - `build/src/syntax/libsyntax.so`
   - `build/src/syntax/src/test/libsyntax_test`
* intermediate-code:
   - `build/src/intermediatecode/coco_compiler_intermediatecode`
   - `build/src/intermediatecode/libintermediatecode.so`
   - `build/src/intermediatecode/src/test/libintermediatecode_test`
* machinecode:
   - `build/src/machinecode/coco_compiler_machinecode`
   - `build/src/machinecode/libmachinecode.so`
   - `build/src/machinecode/src/test/libmachinecode_test`

Of course, you have to compile these modules to make the listed binaries available.


## Adding Source Files
Suppose we are working on the `syntax` module, and we want to add an additional `example.h` file.
We don't have to do anything with Meson here to get it compiled.
Instead, we just need to include the header in a part of the project that does get compiled.
> **Note**: It is not allowed to add headers to `/src/<module>/src/main/public/` for any module.

Now suppose we are adding a matching `example.cpp` file for our header.
That file is **not** compiled by Meson, because Meson does not know about it.
We need to add it to the source files for the `syntax` module.
To do so, we update `/src/syntax/src/main/meson.build` to contain the following:
```python
libsyntax_files += files(
   'cpp/example.cpp', # <-------- This line was added to add our new cpp file.
   'cpp/builder/frontendbuilder.cpp',
   'cpp/visitor/coercion/coercion.cpp',
   'cpp/visitor/operator/operator.cpp',
   'cpp/visitor/syntaxvisitor.cpp',
   'cpp/syntax.cpp')
```
This is all we have to do to add a source file for the `syntax` module.
It works equivalently for all other modules.

Students are allowed to make these changes to the build system.


## Cleaning
To clean up object files etcetera, use:
```bash
ninja clean
```


## IDE Integration
Meson, being the 'new cool' player in the field, is often not supported by IDE's,
which only support older, more established compilation systems such as Cmake and Make.

This is a problem easily solved.
After configuring `meson` and compiling once using `ninja`, there is a file called `build/compile_commands.json`.
Many popular IDEs (IntelIJ Clion, VSCode etc) all can open this file as a project.
When doing this, the IDE will understand the Meson project and behave normally.
There are several tricks to make it work even better!

Check out:
1. [Clion](https://blog.jetbrains.com/clion/2021/01/working-with-meson-in-clion-using-compilation-db/)
2. [Meson with Visual Studio and XCode](https://mesonbuild.com/IDE-integration.html)
3. [Meson plugin for Visual Studio](https://marketplace.visualstudio.com/items?itemName=asabil.meson)

Out of these links, we only tested and extensively used the first one, since we develop in Clion.
Let us know how you got Meson to work with your favorite IDE!



## FAQ
1. **Q: I have multiple compilers on my system, and Meson picks the wrong one. How can I let Meson use compiler `X`?**  
   A: Specify the compiler to use with the `CXX` environment variable. Point `CXX` to your compiler, using an absolute path.

Missing a question here? Let us know. We will try to answer it, and we add it here if enough people have this question.