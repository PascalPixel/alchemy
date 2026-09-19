# What may be tracked

Directive 1 in [AGENTS.md](../AGENTS.md): never commit anything a pret-style
decompilation would not publish.

[pret](https://github.com/pret) publishes decompilations of Pokémon games; its
long public practice is the benchmark, written out here so no one has to know
it. Ask three questions of every file, generated files included:

1. Is it code, build tooling, documentation or metadata that carries no game
   bytes? It may be tracked.
2. Is it a game asset (picture, font sheet, palette, map, sample, music or
   text) in an editable form that this game's build reads to rebuild the ROM
   byte for byte? It may be tracked.
3. Is it anything else made from the game? It may not be tracked.

| May be tracked | Examples here |
| --- | --- |
| Reconstructed C, headers and raw code listings | `SRC/**/*.C`, `INCLUDE/*.H`, `raw/*.s` |
| Names, addresses, sizes and hashes | `source-paths.json`, `SOURCE.JSON` |
| Assets the build reads, in editable form | indexed PNG, WAV, MIDI and message PO catalogs reached from `recon/assets.json` |
| Encoder settings without payload bytes | `COMPRESSION.JSON` |

| May not be tracked | Do this instead |
| --- | --- |
| ROMs, ROM fragments and raw byte dumps, including number arrays, hex and base64 | decode compressed streams and disassemble code stored as data; register what remains as a private input by hash, which the build extracts from the local ROM |
| Build output and patched bytes | build it into ignored `out/` |
| Previews, screenshots, sprite animations, converted fonts, embeds, review sheets and exports for players or other tools | generate them into ignored `out/`, or not at all |
| Assets for a game whose build does not yet rebuild them | add them with the manifest that reads them |
| Leaked, SDK or other third-party material | nothing |
| Compiler, assembler, linker and runtime-library code, patches or diffs to them, and code under any license this repository cannot carry | keep them in their own licensed repositories (the `agbcc` and `agscc` submodules) or build them by bootstrap from pinned official releases; build runtime routines the game linked from those containers |

## Licensed code

This repository carries no license, so it can never hold compiler, assembler,
linker or runtime-library code, a patch or diff to any of them, or any other
code whose license it cannot carry, whatever the file is called or however
little of it there is. Such code lives in its own licensed repository, the
`agbcc` and `agscc` submodules, or is built by bootstrap from pinned official
releases. Routines the original game linked from a compiler runtime, such as
libgcc's soft-float and `_call_via_rN` members, are built at build time from
those containers with the approved compiler and flags and placed at their
recorded links (`games/THE BROKEN SEAL/recon/compiler-runtime.json`); they are
never reconstructed, transcribed or kept here as C, assembly or data.

## Game bytes

Never commit or transmit ROMs, fragments, extracted binaries, objects, ELFs,
compiler output or patches containing reference bytes, and never fill
unfinished code from the reference. Keeping an extracted asset private is
stricter and always allowed. Pages and pictures generated from game material,
such as map renders and the world viewer, stay under ignored `out/`, load
their pictures as files beside them rather than embedding them, and are never
published.

## The gate

`make verify` enforces the mechanical parts with `alchemy check publication
--tree`, which also rejects license markers, patch and diff hunks in any file,
gitlinks other than `agbcc` and `agscc`, embedded data URIs, and JSON byte
values that are not the named `values` of a typed table; passing it does not
make a file allowed. `alchemy check publication --documents` enforces the
document rule in [AGENTS.md](../AGENTS.md), ignored files included.

Each game's asset build fails when a tracked file under `games/<game>/` is
neither read by that build nor named by its review plan, unless it is code,
`recon`, `semantic` or `metrics` metadata, an `asm` registry, or a name
registry. The README coverage figure lives at root `PROGRESS.svg`, outside
the game inputs. Source tracking fails on previews, fonts and exports
anywhere under `games/`, and on extracted inputs no `SOURCE.JSON` registers,
ignored or not.
