# Mars Pathplanning

DIANA has finally managed to build and operate its first Mars-based crewed station.
Our crew has been sent to Mars to execute soil analysis and investigate about possible traces of past life.

But there's a problem. Our crew members are all very very lazy and can't be bothered to leave the habitat for any reason. After all, they're all only volunteers and they don't get paid anything. Luckily, they can count their ol' pal, ARDITO the rover, and on its marvelous capabilities.

They also count on you to help ARDITO complete its mission.

Thanks to the images coming from _**DIOMIO** (**DI**ANA **O**rbiter for **M**ars **I**mprobable **O**bservations)_ they found out **nine** points of interest around their base in which it could be interesting to execute soil analysis. You need to instruct ARDITO about the path to reach all marked points and then come back to the base as soon as possible as our crew members are also very anxious and won't go to sleep until their precious rover is back safe and sound.


## Software Requirements
In the file ```points.txt``` you will find the coordinates of the points, one for each line, in the form ```[x], [y]``` (you can assume the unit of measure as meters). The first point is the starting point ```0.0, 0.0```. Each point is implicitly identified by its position in the file, starting from point 0 (so that at the 3rd line you will have point n°2, at the 4th line you have point n°3 and so on up until point n°9).

Write a program, in whichever language you prefer, which reads from ```points.txt``` (do not hardcode the points in the code) to compute the shortest possible path which starts at the starting point, reaches all of the requested coordinates and ends back to the starting point. Assume no obstacles on the terrain (ARDITO is really tough, it has not fear to traverse on rocks and craters).

## Requested Output
Your program should output (to terminal or to a file) the final order of the points to be reached as well as the final distance traveled. For example:

```[0 1 2 3 4 5 6 7 8 9] 1000 m```

Since we expect to receive a closed loop, for simplicity, shifted or inverted solutions will be considered equivalent, so the above example will be considered equivalent to ```[5 6 7 8 9 0 1 2 3 4] 1000 m``` and to ```[8 7 6 5 4 3 2 1 0 9] 1000 m``` and so on.



