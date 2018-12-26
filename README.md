# HuntTheWumpus
Minimal Hunt-The-Wumpus game

## Compilation
Compile the single source file using your C compiler. Run the resulting
executable.

## The Wumpus
See the main [Wumpus Wikipedia Page](https://en.wikipedia.org/wiki/Hunt_the_Wumpus)
for more information.

The Wumpus is a monster living in a set of rooms that are placed on the
surface of a [dodecahedron](https://en.wikipedia.org/wiki/Regular_dodecahedron),
aligned at the vertices of the dodecahedron.

## Layout
There are twenty named rooms, with each room connected to three other 
rooms. You're the intrepid adventurer in search of this magical beast.
Unfortunately there are dangers in the way:

- Two of the rooms have bottomless pits. Entering these rooms result in
  your immediate death.
- Two of these rooms have giant bats. Entering a room with a giant bat
  will result in the bat picking you up and dropping you in a random room.
  The bat will then go and roost in a different random room.
- Finally, there is the Wumpus itself, a massive creature with suckers on
  its feet (that prevent it from falling into pits) and an appetite for
  adventurers. Also, due to the huge size of the Wumpus the superbats are
  unable to carry it off when it enters their rooms. Entering the room
  with the Wumpus results in your immediate death.

## Hunting the Wumpus
You have unlimited *crooked arrows*, which can follow the curve of the
dodcahedron if necessary. Your arrows, when fired, can pass through up to
five rooms of your choice (provided they are all adjacent).

Be warned, if your arrow misses the Wumpus, it will be alerted and will
move to an adjacent room.

## Development
You can watch the initial development [on youtube](https://youtu.be/dYDu9Hrg7yM).
I screencasted the 2.5 hours it took to write this game, and then doubled
the speed of the video.
