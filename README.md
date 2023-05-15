# XJCO 1921 Coursework 2

### Based On
- Plplot for map drawing
- GTK3.0 for window rendering
- Pure C
- Built with Cmake

### Required
- Linux OS
- Cmake Verison >= 3.0
- Plplot - run following to install
  ```bash
    sudo apt update
    sudo apt install libplplot-dev
  ```

- GTK Version >= 3.0 - run following to install
  ```bash
    sudo apt update
    sudo apt install libgtk-3-dev
  ```

### How to run
**RUN MODE**
In the root directory
```bash
    make
    ./build/main <enter map file path>
```

**TEST MODE**
In the root directory
```bash
    make test
```
You will see the test results output to the terminal

**clean excutable and all temp file**
In the root directory
```bash
make clean
```