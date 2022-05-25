# Pong
A Pong clone made with SDL.

Controls:
- Scroll to move the player's paddle (left side).
- Press the space bar to reset.

Build (Terminal & MinGW):
- Add the MinGW/bin folder to your PATH environment variable.
- Run the following in the "Pong" folder: "gcc -Iinclude -Llib -o bin/pong src/main.c -lmingw32 -lSDL2main -lSDL2".
