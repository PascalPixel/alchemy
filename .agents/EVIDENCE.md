# Evidence

## The clean room

Use locally held reference ROMs, independently reconstructed material here,
decoded data, verified calls and relocations, and public language, hardware,
compiler and format documentation. Never inspect or copy leaks or another
Golden Sun reconstruction's code, symbols, types, comments or conclusions. The
one exception is Tarpman and Karathan's identification of the compiler and its
flags, which the README credits; nothing else from another Golden Sun project
is a source.
Unrelated projects supply methodology, never target-specific material.

Melee demonstrated two useful techniques: matching near-duplicate functions
from an exact sibling plus decompiler output, and improving decompiler input
with shared types and concrete pointer and union context. Its
[duplicate-family PR](https://github.com/doldecomp/melee/pull/2102) produced
318 matches; its [typed-module example](https://github.com/doldecomp/melee/pull/2076)
used a short compile and diff loop. These are methodological examples, not
sources of Golden Sun code, compiler policy, names or types. Pokeemerald
remains a model for coherent modules, shared headers, explicit link order and
incremental builds.

## Checking against the game

When a picture, palette, map or behavior looks wrong, run the game in an
emulator rather than guessing: request the scene the way the game's own request
routine does and look. Gold Contigo's yellow grass and purple cliffs looked
like a palette bug and are what the game draws.

## One authority per fact

| Fact | Authority |
| --- | --- |
| Owner name and source | `games/<game>/source-paths.json` |
| Compile-local composition and bindings | Translation-unit manifests |
| Code, types and asset layouts | Maintained sources and headers |
| Measured production status | Current build outputs |
| Historical reasoning | Dossiers, never treated as current scores |
| Open work | `TODO.md` |

Historical scores and dossiers are leads, not current facts. A report older
than the commits it describes is not evidence. Source verification produces
fingerprinted receipts; `make coverage` reads those receipts and refuses stale
credit without silently rebuilding or rescoring drafts. See
[COMPLETION](COMPLETION.md) for the scoring authority. Do not edit derived
reports or treat old experiment outputs as build inputs.

A score that improves is not evidence that a change is correct, and a
boundary tested only with the tools available when it was found is weaker
than one retested after the tools improved.
