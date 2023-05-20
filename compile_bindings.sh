#!/bin/bash

BUILD_DIR="build/"
PYTHON_DIR="src/visualization"

MAKE_TARGET="dcel_binds"
PIP_CMD="./bin/pip"

WHEEL_DIR="../../dist"
FILE_NAME="dcel_binds-0.0.1-cp311-cp311-linux_x86_64.whl"

# Go into the build directory and apply cmake settings, then build the bindings
cd "$BUILD_DIR" || exit 255
cmake ..
make "$MAKE_TARGET"

printf '\n\n%s\n\n' "Built C++ shared object, now going to build the wheel file"

# Go back into the root and run setup.py
cd ..
python3 setup.py bdist_wheel

printf '\n\n%s\n\n' "Built wheel file, now installing into python venv"

cd "$PYTHON_DIR" || exit 255

"$PIP_CMD" install "$WHEEL_DIR/$FILE_NAME" --force-reinstall

printf '\n\n%s\n\n' "Successfully reinstalled dcel bindings"
