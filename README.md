# ⛰️ Alchemy: the Golden Sun decompilation

In Golden Sun, lighting the four Elemental Lighthouses releases Alchemy upon
the world. I’m doing the same thing to the games themselves. **The Broken
Seal** ☀️ and **The Lost Age** ⚓️ are being rewritten, piece by piece, into
source code people can read, change and build on, and every piece has to
rebuild the original cartridge exactly.

That means all twelve cartridges: both games in Japanese, English, German,
Spanish, French and Italian. The Japanese releases are the base, and every
other language is written as its differences from them.

## Progress

**☀️ 79.85% · ⚓️ 4.61% (C 1.98 + assembly 1.40 + stubs 1.23)**

<img src="PROGRESS_CHART.png" width="838" alt="DONE by day for The Broken Seal and The Lost Age since 16 July 2026">

<img src="PROGRESS.png" width="838" alt="A map of the project's files by size on disk">

The number is the share of each game’s code that is built from readable
source, counted across all six languages, and only when the rebuilt game is
identical to the original, byte for byte. A language that still borrows a piece
from its cartridge holds the number down until it builds that piece too. The
Lost Age’s bracket splits its figure into C, original assembly, and the tiny
eight-byte stubs that connect one part of the game to another.

The chart keeps every past measurement, The Broken Seal in gold and The Lost
Age in blue, including the two days I made the count stricter. On 28 September
I threw out stored answers and bookkeeping that only looked like progress, so
code now counts only once the game is linked from its source. On 2 October the
meter started counting all six languages instead of English alone, which is
why The Lost Age steps down from 7.17% to 4.61%: its other languages still
copy most of their scene code. Neither drop is lost work. Both are the number
getting harder to fool.

Some matched code still needs a small hint before the old compiler produces
the exact bytes. Every hint is tagged in the source, counted as its own number,
and comes out at the end.

## Why Alchemy?

When Camelot made Golden Sun, they wrote it as instructions a person can read
and then turned those into the unreadable code that sits on the cartridge. The
readable version was never released.

Alchemy works backwards. Piece by piece, it rewrites the game in readable form,
and every piece is checked against the original until the two are identical.
The result isn’t a guess at how Golden Sun works: it is Golden Sun, in a form
people can finally read. I also try to make it look the way Camelot’s own work
might have looked in 2001, down to names taken from the Japanese release:
Sukureta for Kraden, Gerald for Garet.

I do this with AI, and a lot of it. The AI does the heavy lifting and the
compiler is the judge: a piece only counts when the rebuilt game comes out
byte for byte the same as the original, so nothing gets in on charm.

Once a game can be read, it can be changed. That opens the door to things fans
have wanted for years: Golden Sun running natively on PC, phones and modern
consoles, widescreen and smoother frame rates, quality-of-life fixes, new
translations, and new storylines, quests and Djinn. Alchemy doesn’t do these
things itself. It lays the foundation, and then all of us get to build on it.

Alchemy is not a remake, a mod, an emulator or a download of the games. It
doesn’t include the games themselves; you’ll need your own copies. For now,
the best way to help is to share the project and cheer it on. It will open to
outside contributions once both games are complete, and developers can find
the technical details in [AGENTS.md](AGENTS.md).

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
