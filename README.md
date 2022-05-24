# Pong
A Pong clone made with SDL. 

Controls:
- Scroll to move the player's paddle (left side).
- Press the space bar to reset.

Build (Sublime Text & MinGW):
- First, go to "tools" in the navigation bar, go to "build system," and then add a new build system. Next, copy-paste the included "sublime-build" file contents into the new file. Finally, go back to "tools" and go to "build with," and click on the name of the new build file.

Build (Terminal & MinGW):
- Run a terminal window in the Pong folder and copy-paste the following: "gcc -Iinclude -Llib -o bin/pong src/main.c -lmingw32 -lSDL2main -lSDL2"
