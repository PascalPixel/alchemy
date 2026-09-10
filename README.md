<p align="center">
  <img src="games/gs1/assets/readme/djinn_101_idle.gif" width="64" height="64" alt="Venus Djinn">
  <img src="games/gs1/assets/readme/djinn_102_idle.gif" width="64" height="64" alt="Jupiter Djinn">
  <img src="games/gs1/assets/readme/djinn_103_idle.gif" width="64" height="64" alt="Mars Djinn">
  <img src="games/gs1/assets/readme/djinn_104_idle.gif" width="64" height="64" alt="Mercury Djinn">
</p>

<h1 align="center">Alchemy</h1>

<p align="center"><strong>Golden Sun 25th Anniversary Preservation Project</strong></p>

Alchemy is an unofficial clean-room reconstruction of _Golden Sun_ and
_Golden Sun: The Lost Age_ for the Game Boy Advance. It preserves all six
Japanese and European-language editions of each game from the original
cartridges, with Japanese as the canonical source base and the English
_Golden Sun_ release as the first complete derived build target.

## Status: 43.92% DONE

![Code](games/gs1/assets/readme/gs1-en-code.svg?v=7b436d674436205e)

![Data](games/gs1/assets/readme/gs1-en-data.svg?v=d6b524e58c957cd7)

## Details

Alchemy rebuilds the games from readable source code, retained assembly, and
named assets while checking the result byte for byte against an approved
cartridge image. The historical editions remain independent from the future
`games/alchemy/` reintegration, so new work cannot weaken their preservation
evidence.

The repository contains one shared source tree per game rather than a copy for
every language. `make targets` checks all twelve edition routes; `make verify`
is the authoritative full-ROM gate for the current `gs1-en` target. Progress
is measured by reconstructed bytes. C compiles through the approved route
to byte-identical output; Drafted is candidate C not yet exact. Retained
assembly earns no DONE credit at all: the standard that marked it was withdrawn
because it did not establish handwritten or third-party origin, so every such
byte now counts as Unknown until a replacement standard admits it. The
classification records are still kept and validated, and the coverage map
publishes their total as `withdrawn_assembly_bytes`. No label claims access to
Camelot's original source.

This is a preservation and decompilation project—not a remake, ROM hack,
emulator, or game distribution. No ROM is included. To build locally or help
with the reconstruction, see [CONTRIBUTING.md](CONTRIBUTING.md), including its
[tooling guide](CONTRIBUTING.md#tooling-index).

## Acknowledgements

_Golden Sun_, its characters, music, art, and original code were created by
Camelot Software Planning and published by Nintendo. Alchemy is an independent
fan preservation project and is not affiliated with or endorsed by either
company.

Thank you to everyone documenting the Game Boy Advance, improving preservation
tooling, testing reconstructed editions, and contributing careful clean-room
work to Alchemy.
