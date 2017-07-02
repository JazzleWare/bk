#!/bin/bash

A="a/b/e/l"

if [ -n "$1" ]; then
  A="$1";
fi

BASE="mv"
N=0
while [ -e "$BASE" ]; do
  N=$[$N + 1 ];
  BASE="mv$N"
done

echo "BASE [$BASE]"

N=$[$N+1]
MV="mv$N"
while [ -e "$MV" ]; do
  N=$[$N + 1 ];
  MV="mv$N";
done

echo "$MV" > "$MV"

echo "-m -> moving [$MV] to [$BASE]"; ./bakp -m "$MV" "$BASE"
echo "[$BASE] actually contains $(cat "$BASE")"; rm -i "$BASE" 

echo "--------------------"

echo "$MV" > "$MV"

echo "-l -> linking [$BASE] to [$MV]"; ./bakp -l "$MV" "$BASE"
echo "[$BASE] actually contains $(cat "$BASE")"; rm -i "$BASE" "$MV"

echo "--------------------"

echo "-c -> comparing [hash.c] to [hash.h]"; ./bakp -c hash.c hash.h

echo "--------------------"

echo "  -p -> making [$A]"; ./bakp -p "$A"; echo "RETV: [$?]"

echo "--------------------"

echo "-i -> iterating [.]"; ./bakp -i "."

echo "--------------------"

echo "-h -> hashing [hash.c]"; ./bakp -h hash.c

echo "--------------------"

echo "DONE"

echo "--------------------"

