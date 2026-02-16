#!/usr/bin/env bash
set -euo pipefail

# Always build from the script's directory
cd "$(dirname "$0")"

CXX=${CXX:-g++}

# Compiler flags
CXXFLAGS=(
  -std=c++17
  -O2
  -Wall
  -Wextra
  -pedantic
  -I.
)

# Prefer pkg-config if raylib is installed that way
RAYLIB_CFLAGS=()
RAYLIB_LIBS=()

if command -v pkg-config >/dev/null 2>&1 && pkg-config --exists raylib; then
  # shellcheck disable=SC2207
  RAYLIB_CFLAGS=($(pkg-config --cflags raylib))
  # shellcheck disable=SC2207
  RAYLIB_LIBS=($(pkg-config --libs raylib))
else
  # Fallback (common on many Linux setups)
  RAYLIB_LIBS=(-lraylib -lGL -lm -lpthread -ldl -lrt -lX11)
fi

mkdir -p build

# Compile all .cpp files in the repo root
SRCS=( *.cpp )
OBJS=()

for src in "${SRCS[@]}"; do
  obj="build/${src%.cpp}.o"
  OBJS+=("$obj")
  "$CXX" "${CXXFLAGS[@]}" "${RAYLIB_CFLAGS[@]}" -c "$src" -o "$obj"
done

# Link
"$CXX" "${OBJS[@]}" "${RAYLIB_LIBS[@]}" -o build/chess

echo "Built: build/chess"
