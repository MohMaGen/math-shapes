# math shapes

![preview](./preview/preview2.gif)

## Keybindings

### General
- Key P to hide/reveal UI.
- Key M to toggle camera rotation.
- Key 5 to reset camera rotation.

### Parameters
- Mouse wheel to change parameter's value.
- Mouse wheel with Ctrl to change parameters's value more precisely.
- Left Mouse Button to toggle multiplication by sin of time.


## Compilation
- Init adn update [raylib](https://github.com/raysan5/raylib) submodule:
    ```bash
    git submodule init
    git submodule update
    ```
- Then make [raylib](https://github.com/raysan5/raylib):
    ```bash
    make -C raylib/src/
    ```
- The run build script: (You can change compiler in this script to whatever you want)
    ```bash
    ./build.sh
    ```
