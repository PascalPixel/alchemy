# Sharing source

## Editions

TBS editions share C files and translation units. `INCLUDE/VERSION.H` selects
exactly one of `TBS_EDITION_JA`, `EN`, `DE`, `ES`, `FR` or `IT`; English is the
production default. Use small conditional sections only for measured code
changes. Keep localized text and address differences in bindings and assets,
not copied language trees. A separate module is justified only by
substantially different code, never merely by relocated addresses.

The translation-unit manifest owns optional `editions` entries. Each edition's
`owners` map uses registered function names and records independently reviewed
`address`, complete `extent`, and `source_variant` when code changes. Its
`absolute_symbols` map records named function, data and value bindings with
their `address` and `kind`. Omitted entries keep the normal correspondence
workflow; a source variant needs explicit bindings where call or literal sites
change. Do not use candidate length or candidate call offsets to invent these
facts. Conflicting, unknown, overlapping and invalid declarations are errors.

For one registered overlay owner, compile its shared unit for all six editions:

```sh
./alchemy cross-edition --span 472 --edition-build out/tbs-editions/lunpa-sequence.json resource_3bf:02001e94
```

This report proves only that complete owner, including linked calls, literals
and alignment; it does not prove its neighbors or the whole overlay. Shared
source is compiled before selecting an owner for linking. Declared main-image
variants use `alchemy cross-edition --edition-build out/tbs-editions/owner.json <owner>`.
`--all-overlays --edition-build` compiles once per unit per edition and checks
every member at its regional address and extent. Core similarity is
diagnostic; only zero differences over the complete linked extent is exact.
Keep per-edition results under ignored `out/`; any failed edition makes the
command fail.

## Instances and twins

A module linked into several images is one translation unit with instances.
Each instance names its image and records every member's address and complete
extent, with optional `absolute_symbols`, `compiler_gaps` and `editions`. Calls
and literals are paired from the canonical placement exactly as for editions;
declared entries win. A member name means one function in every instance: a
call reaches the same main-image target or that instance's own owner of that
name. Never copy a source per image or `#define` one image's address names to
another's. `alchemy score --unit <unit-id> --all-instances` proves every owner
in every image; parking one member of an instance parks that whole instance. A
unit whose `data` record links its tables into one listing has no instances.

An exact overlay unit may also declare a `main` instance. It must provide its
own explicit imports, every registered member placement and the same shared
source; no overlay-address fallback or overlay compiler gaps enter that link.
Main-image compilation and verification use those placements. Undeclared
main/overlay source sharing remains an error.

Run `alchemy inspect <owner> --siblings` before and after adopting any owner.
Every binding-equivalent twin it lists joins the same translation unit as an
instance in the same batch; `alchemy adopt`, `alchemy overlay adopt` and
`alchemy check integrate --apply` refuse to finish otherwise. `make verify`
runs `alchemy check siblings`: it rejects `Func_`, `Data_` and `Value_` names
in instanced sources and the headers they include, and reports missed siblings
and copied modules until their waves close.
`--report out/tbs-en/reports/siblings.json` writes every family, with TBS JA
and TLA EN twins as report-only leads.

## Both games

Source that both games compile from the same text lives once under
`games/COMMON/SRC/<module>/`. A file may live there only when both games
compile it byte-exact from that text, proven by each game's check: the TBS
production ROM gate, and for TLA `alchemy check tla-owners roms/tla-en.gba`,
which `make verify` runs whenever that ROM is present. Shared interfaces live
once under `games/COMMON/INCLUDE/<module>/` and contain declarations only;
each game includes them through its own `INCLUDE`. It scores every TLA
main-image owner with a source, including every named member of a declared
unit, over its complete unit extent inside
`games/THE LOST AGE/metrics/executable.json`, and every TLA overlay
owner over its reviewed span from the listing that places it, and refuses a
shared C file no TLA owner compiles. Each game's `source-paths.json` registers it
as `../../COMMON/SRC/<module>/<FILE>.C` with that game's owner and name, and
each game compiles it with its own route and `INCLUDE`. `alchemy check owners`
refuses a shared file that either register omits; the publication gate admits
only nested `.C` source and nested `.H` interfaces there. Everything else stays
in its game.

Sharing a file across the two games requires proved correspondence; the game
boundaries stay until that is shown. Retain ROM sound IDs and storage
relationships without inventing cross-game asset matches.
