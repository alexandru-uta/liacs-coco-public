# Debugging
While working on this project, you might find yourself in a situation 
where your application displays unexpected behavior, or  has a `segfault` and halts.
In these situations, it is good to know how to debug your code.

By default, all binaries are compiled with the `-g` (debugging) flag.
You can use `gdb` to debug all your applications.


### IDE Debugging
Some IDEs provide their own or additional debugging tools.
Those tools normally use `-g`-provided flags and should not need anything extra.

Our own experience with Clion: Debugging works well, exactly as normal.

If you have any questions or things to share about your debugging experiences, let us know.