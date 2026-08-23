# shitty_raytracer

A quick and dirty raytracer. Casts rays at two spheres, shades them with a
single diffuse sun light, and writes the result to a 1920x1080 PNG. Nothing
fancy, but it renders.

## Build

Generate with `cmake -GNinja -Bbuild .` and build with `cmake --build build`.

## Run

Run `./build/rt_circle`. This generates a file called `rt_circle.png`. Open it
however you like and admire your two very round spheres.
