# Publication policy

This repository follows the practical source-tree boundary established by
[`pret/pokeemerald`](https://github.com/pret/pokeemerald). This is an
operational publication rule, not a claim that precedent creates a licence or
guarantees that a host or rights holder will agree with every publication.

The boundary is based on a file's role, not on the fact that reconstruction
began with a private ROM. `pokeemerald` publicly tracks reconstructed C and
assembly, map JSON, source PNG graphics and palettes, and canonical binary map
sources such as metatiles. It ignores the built ROM, object and ELF files,
generated hardware graphics encodings, dumps, and build directories.

## Publishable source

- Reconstructed C and assembly, including byte-verified code that has not yet
  been decompiled to C.
- Scripts, text, tables, symbols, addresses, ranges, checksums, manifests, and
  semantic JSON required to understand or rebuild the game.
- Canonical source assets such as PNGs, palettes, audio sources, maps,
  tilemaps, metatiles, and animation data.
- Deterministic encoder descriptions and payload-free compression plans that
  turn those source forms back into the verified ROM representation.
- A narrowly scoped binary source format when it is the canonical named input
  for a reconstructed subsystem, is actively consumed by the build, and is
  not merely an opaque slice copied from the ROM.

Tracked assets must be authoritative build inputs, not disposable extraction
output. When a coherent semantic source replaces a provisional atlas or raw
representation, the superseded file is removed.

## Never publish

- `baserom.gba`, a built `.gba`, or any other playable ROM image.
- Copied ROM gaps, committed `.incbin` payloads, or raw ROM ranges kept only to
  make an incomplete reconstruction link.
- Opaque bulk extraction dumps whose only function is to substitute for ROM
  bytes and which have not been promoted to a canonical source format.
- The private compiler bundle, toolchains, native dependencies, or other
  redistributable packages not authored as part of this repository.
- Object files, archives, ELFs, logs, generated linker output, analysis dumps,
  disassembler/compiler output, work directories, caches, and generated
  previews.
- Private ROM content or generated private artifacts sent to network tools.

## Commit gate

Before every commit, inspect all staged paths. A staged file passes only if it
is a canonical source input or independently authored generic tooling and is
needed by the tracked build or documentation. Verify that no forbidden file
above is staged, then run the relevant byte-identical build checks. Repository
size alone is not the boundary, but redundant and superseded assets must not
accumulate.

Useful upstream examples are `pokeemerald`'s
[`data/tilesets/primary/general`](https://github.com/pret/pokeemerald/tree/master/data/tilesets/primary/general),
which tracks `tiles.png`, palettes, `metatiles.bin`, and
`metatile_attributes.bin`; its semantic
[`LittlerootTown/map.json`](https://github.com/pret/pokeemerald/blob/master/data/maps/LittlerootTown/map.json);
and its [`.gitignore`](https://github.com/pret/pokeemerald/blob/master/.gitignore),
which excludes ROMs, compiled graphics encodings, object and ELF files, dumps,
and build output.
