# cotton

![cotton](imgs/cotton.png)

cotton is a tiny virtual computer that lives in your windows - it was written in C99 and has 64 KiB of memory and runs programs written in the cot programming language. 

cotton was originally based on a CHIP-8 emulator, but has since evolved into its own thing somewhat inspired by [uxn](https://100r.co/site/uxn.html) (check it out!! its an amazing project!!) and the concept of a small, easy-to-understand virtual computer.

cotton is a passion pet project and while i do get alot of suggestions via my friends, i am the only and main developer of it, so, don't expect anything grandiouse or of perfect quality, at the end of theday this is all a big learning project that im taking at my own pace!! xP

## building cotton,

```
meson setup build
ninja -C build
```

(make sure you also have sdl installed or else windows wont open wawawaw!!)

## running cotton,

```
cotton <file.cot>
```

## docs

before v0.2, i used to have everything about the cot programming language and cotton's tooling on this md file, but, as i have now made docs for cotton and it's ecosystem, i strongly suggest that anyone who wants to read the docs gets [mandoc](https://mandoc.bsd.lv/) (should be called packaged as mandoc on your distro of choice, probably :P) to do so!! 

## cotton's mascot

![ton](imgs/ton.png)

this is cotton's mascot!! his name is ton!! he was drawn by me using cotton's colour palette!! say hi to ton!! :3

## credits

i'd like to thank [this guide](https://austinmorlan.com/posts/chip8_emulator/) for helping me figure out the initial base for cotton, even if 90% of it is gone by now, it helped me learn alot of stuff that i am now applying for cotton, awesome stuff ^^

i'd also like to thank [chld](https://srcdump.net/chld/) for making a C port of eikimaker.go AND currently working on a haiku backend for cotton - both arent merged to the repo yet but im still very grateful for both :)

i'd like to thank [emilia](https://srcdump.net/emilia9/) for the patches to add *BSD support and fix the compiler warning for the comments :D

and also [pita](https://pita.im/), [lotte](https://srcdump.net/lotte/), [merc](https://merc.4plt.ch/) and [shrub](https://shrub.industries/) (in no particular order) for all of the cool suggestions and guidance for cotton as of the release of v0.2, thank you so much!! :D
