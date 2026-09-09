# cotton

![cotton](imgs/cotton.png)

cotton is a small stack machine for the cot programming language (a concatenative language inspired by Lua*, Forth and Uxntal) written in C99 . 

cotton originally started as a slightly modified CHIP8 emulator, then was repurposed into being a fantasy computer inspired by [uxn](https://100r.co/site/uxn.html) but has ultimately ended up turning into what you're seeing right now :) - the old version of the project can be found on the 'old-cotton' branch of this repo, and, as you may be able to guess, its effectively dead and wont get updated anymore, being there purely for archival purposes xP

cotton is a passion hobby project being made solely by me, so, dont expect anything grandiose or of high quality - even then, suggestions and ideas are always welcome!! :D

## building cotton,

```
meson setup build
ninja -C build
```

## running cotton,

```
cotton <file.cot>
```

## docs

current docs are a bit outdated, will work on them soon enough

## cotton's mascot

![ton](imgs/ton.png)

this is cotton's mascot!! his name is ton!! he was drawn by me using old cotton's own colour palette!! say hi to ton!! :3

## to-do list for (cot)ton

honestly i dont have alot to write here for the time being, mostly because im afraid to set high expectations that i can't achieve, however:

* Repurpose the current code into my new idea for the project and run something [X] (got a little base for tonight and it does work so wooo)
* Organise the codebase a bit bc i can see the interpreter getting quite full like on old cotton [ ]
* Update the current docs [ ]
* One day figure out how to optionally compile cot files into uxntal files (might be too ambitious but one can dream, right :<) [ ]
