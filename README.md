# ⛰️ Alchemy: the Golden Sun decompilation

In Golden Sun, lighting the four Elemental Lighthouses releases Alchemy upon
the world. I’m doing the same thing to the games themselves. **The Broken
Seal** ☀️ and **The Lost Age** ⚓️ are being rewritten, piece by piece, into a
form people can read, change and build on.

## Progress

<img src="https://raw.githubusercontent.com/PascalPixel/alchemy/progress/PROGRESS_CHART.png" width="838" alt="How much of each game's code and data is rebuilt, day by day since 16 July 2026">

<img src="https://raw.githubusercontent.com/PascalPixel/alchemy/progress/PROGRESS.png" width="838" alt="A map of the project's files by size">

Each game has two percentages. Code is how much of the game’s code has been
rewritten so that it rebuilds the original exactly, in all six languages
(Japanese, English, German, Spanish, French and Italian). Data is how much of
everything else on the cartridge (pictures, music, text, tables) is built from
our own source files rather than copied from the original. A piece only counts
once every language that uses it comes out byte for byte identical, and a
game is only 100% done when both numbers are. The Broken Seal comes first;
The Lost Age has just started. Both pictures redraw themselves after every
change.

The chart shows that climb day by day: code as the thick line and data,
measured since 8 October 2026, as the thin one. The colours along the bottom
show which AI models did the work. Sudden drops are honesty checks: when we find
something that only looked finished, it comes back off the count until it’s
done properly. The map below it shows the whole project by size: everything
in light rose is still waiting to be rewritten.

## Why Alchemy?

When Camelot made Golden Sun, they wrote it as instructions a person can read
and then turned those into the unreadable code that sits on the cartridge. The
readable version was never released.

I do this with AI, and a lot of it. The AI does the heavy lifting, and the
original game is the judge.

Once a game can be read, it can be changed. That opens the door to things fans
have wanted for years: Golden Sun running natively on PC, phones and modern
consoles, widescreen and smoother frame rates, quality-of-life fixes, new
translations, and new storylines, quests and Djinn. Alchemy doesn’t do these
things itself. It lays the foundation, and then all of us get to build on it.

## Acknowledgements

_Golden Sun_, its characters, music, art and original code were created by
Camelot Software Planning and published by Nintendo. Alchemy is an unofficial
fan project and is not affiliated with or endorsed by either company.

Thank you to:

- The [r/GoldenSun community](https://www.reddit.com/r/GoldenSun/), for sharing
  Alchemy, cheering it on, and keeping the love for these games alive.
- Tarpman and Karathan, for identifying the compiler and flags Camelot used.
- Coaltergeist, for [camelot-gcc](https://github.com/Coaltergeist/camelot-gcc),
  the compiler Alchemy first built with.
- [pret](https://github.com/pret), whose decompilations set the standard Alchemy
  measures itself against, and whose [agbcc](https://github.com/pret/agbcc)
  Alchemy builds the games’ library code with.

- [Simon Lindholm](https://github.com/simonlindholm/decomp-permuter), for the permuter whose behavior Alchemy reimplements in Rust.
