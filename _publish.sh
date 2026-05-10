#!/usr/bin/env bash
set -euo pipefail

# ---- CONFIG (edit these) ----
OUT_NAME="lilc"                 # like OPTS.out_name
INCLUDE_DIR="/usr/include"      # destination include root
LOCAL_INCLUDE_DIR="./include"   # source headers directory

MAKE_LIB=1                      # 1 = install static lib, 0 = skip
LIB_SRC="./build/lilc.a"        # built library path
LIB_DST="/usr/lib/liblilc.a"    # install location
# -----------------------------

dest_dir="${INCLUDE_DIR%/}/${OUT_NAME}"

# Create /usr/include/$OUT_NAME (ignore error if it exists)
mkdir -p "$dest_dir" || true

# Copy headers from ./include -> /usr/include/$OUT_NAME
shopt -s nullglob
for f in "${LOCAL_INCLUDE_DIR%/}"/*; do
  [[ -f "$f" ]] || continue
  sudo cp -f "$f" "$dest_dir/"
done

# Optionally install the library
if [[ "$MAKE_LIB" -eq 1 ]]; then
  sudo mv -f "$LIB_SRC" "$LIB_DST"
fi