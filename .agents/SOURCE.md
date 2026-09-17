# Source

## Layout

Each game has one physical root: `games/THE BROKEN SEAL` (build IDs `tbs-ja`,
`tbs-en` and the other edition-qualified targets) and `games/THE LOST AGE`
(`tla-` build IDs). TLA keeps its current layout until it is independently
mapped. These directory choices are our reconstruction, not recovered Camelot
names. Keep a shallow module tree: no folder per function, and no sorting of
whole scenes into actor, dialogue or story categories. A scene's actors,
dialogue and events belong with its area or loadable module. Moving a file does
not change its translation unit, overlay identity, compiler route or credit.

TBS `SRC/` responsibilities:

| Folder | Responsibility |
| --- | --- |
| `SYSTEM/` | Startup, scheduling, memory, input, save, link and resource loading. |
| `LIB/` | Game library support and shared math routines; compiler runtime is built from its container, never kept here. |
| `GRAPHICS/` | Display, palette, animation, tile, text, window and icon rendering. |
| `SOUND/` | Sound and music player code, not the sound assets. |
| `GAME/` | Shared character, party, inventory, item, ability, Djinn and flag rules. |
| `FIELD/COMMON/` | Shared map, camera, object, event and script runtime. |
| `FIELD/<location-or-scene>/` | Distinct loadable area modules and their exclusive assets. |
| `FIELD/COMMON/<locations>/` | Distinct loadable modules serving multiple areas. |
| `BATTLE/` | Battle rules, presentation, motion and effect modules. |
| `MENU/` | Menu interaction, shops, inns and selection screens. |
| `DEBUG/` | Debug facilities. |

Shared headers belong in `INCLUDE`. Use `COMMON` for genuinely common runtime
code and interfaces, not a generic `SHARED` catch-all; common engine code has
one owner. Each overlay has its own workspace. A separately loaded overlay used
by several locations still has its own named directory and compilation
identity; its `FIELD/COMMON/<locations>/` directory groups those overlays, not
merged engine code. Area code belongs in the directory the `atlas_destination`
column of `games/THE BROKEN SEAL/locations.tsv` records; one location may have
several separately compiled overlays, and distinct overlay address spaces are
never merged because they share a location. Remove empty directories left by
moves.

`locations.tsv` keeps the ROM-backed English location assignment of all 96 TBS
overlays; its header records the scene table, location rules and message
selection the engine uses, and the debug scene label is message
`0xa07 + scene`. Preserve entry-specific and shared-location assignments; a
default title-screen fallback is not a place. These are English display labels,
not recovered Japanese folder names. Filenames and function names alone do not
prove an assignment: the historical `BATTLE_EFFECT_TAIL.JSON` and
`BATTLE_EFFECT_RULES.JSON` names do not make those tables battle effects.
Establish new locations through resource, map or call-site evidence, and never
invent a place to fill a folder.

Keep classification metadata outside source folders; provisional-source records
live in `games/THE BROKEN SEAL/semantic/provisional-source.json`. Overlay
assembly and compression recipes belong in `raw/overlays/`, battle assembly in
`raw/battle/`. Raw `.byte`, `.2byte`, `.4byte` and `.word` data in overlay
listings is tolerated retained assembly, not a place for new data: the
Makefile's `OVERLAY_DATA_DIRECTIVES_MAX` records its total, and `make verify`
fails when the total grows.

## Names

Use modules and evidenced jobs, not one file per address. Merge related C only
when shared compilation stays exact. No address filenames, ordinal placeholders,
resource-number folders, codenames or invented geography.

Folder labels are uppercase ASCII: a short romaji prefix from the Japanese ROM
location name, repeated so related folders sort together, and one short area
word, such as `HAIDIA_MURA`, `HAIDIA_HEYA`, `RUNPA_DOU`, `RUNPA_JO`. Use the
Japanese ROM's name rather than its English localization, shorten romaji
consistently, and record the decoded label and any abbreviation in the owner
evidence. Use `MURA` for an evidenced village, `MACHI` for a town, `HEYA` for
interiors and `DOU` for a cave; a reused area can add `SAI` for a return scene.
Inspect maps and scene behavior before choosing an area suffix. Shared
overlays name the locations they serve. Resource identities and addresses
belong in metadata, not folder labels. These are working names, not recovered
historical directory names.

Native game names use uppercase folders, basenames and extensions throughout
`SRC`, `INCLUDE`, `SOUND`, `TEXT`, `PREVIEW` and `SOURCE.JSON`: `.C`, `.H`,
`.JSON`, `.PNG`, `.BIN`, `.MID`, `.WAV` and `.TSV`. Repository and tooling
registries keep their established spellings. Uppercase is a project
convention, not a recovered historical spelling.

Keep function addresses in the owner register. `Func_`, `Data_` and
`Value_<address>` identifiers are legacy spellings: forbidden in instanced
units, in the headers they include and, once migrated, in every `COMMON` and
`LIB` source. Overlay code calls main-image functions by their registered
names; the build binds each call to that overlay's import veneer.

Use neutral Japanese commercial C vocabulary appropriate to 2000–2001:
`Subsystem_VerbObject`, short locals such as `pos`, `cnt`, `tbl`, `buf` and
`work`. Unknown roles remain `unk`, `field_<offset>` or raw offsets; `pad`
means proved padding. No fan lore, fake Japanese, modern framework terminology
or comments impersonating lost source. Names are reading aids, not recovered
historical spellings.

Shared interfaces and evidenced types belong in headers; local constraints
belong beside the relevant expression. Keep source ordinary C89 and preserve
observable behavior, including bugs.

## The Lunpa standard

Matching C is the floor. The target for every module is directive 2: the source
Camelot most likely had on disk. `SRC/FIELD/COMMON/KUUPUAPPU_RUNPA` (the jail
between the Vault and Lunpa) and `SRC/FIELD/COMMON/RUNPA_SUHARA` (Lunpa's
interiors) are the reference; read them before converting another module.

1. **Names carry game meaning.** Take them from evidence: message text in
   `TEXT/MESSAGE_ARCHIVE.JSON` and the ids that select it, call sites, how the
   engine consumes the data, and sibling modules. The same engine call keeps
   one name everywhere. No `Func_`, `Data_` or `Value_` names and no mechanical
   names such as `Actor8`, `Mode10`, `unk6` or `filler`; where no evidence
   exists, choose a neutral descriptive name.
2. **Types, not offsets.** Real structs, enums and named constants. No raw
   pointer arithmetic and no literal addresses in code; actor ids, sounds,
   message ids and flags are symbolic.
3. **Coherent files.** One file per responsibility, prototypes in headers and
   shared vocabulary in `INCLUDE` (`FIELD_SCENE.H`, `FIELD_EVENT.H` and the
   `*_IDS.H` enums). No alias `#define` layers, per-file source bindings,
   `*_BODY.INC` wrappers or `#ifdef` variant scaffolding. Scripts call the
   engine through the inline services in `FIELD_EVENT.H`, which reproduce the
   separate argument loads of every call; the unit's `absolute_symbols` bind
   the `Engine_` entry points those services forward to, and those names win
   over the shared alias map. A call into another unit of the same overlay
   binds the callee's register alias to its runtime address.
4. **Data is C.** Tables and scene data become typed `const` data built with
   macros derived from the engine code that reads them, in the same unit. The
   unit's `data` record links its `.rodata` into the listing's
   `AlchemyData_<address>` placeholder, so the overlay listing keeps only
   veneer includes, placeholders and alignment. Bytes whose structure no
   consumer proves stay as directives on the folder's allowlist in
   `recon/showcase.json`, and the change reports them.
5. **Comments describe the game.** Addresses, pool words, registers, compiler
   reasons and probes belong in `recon/en/dossiers.json`.
6. **Byte-exact after every edit.** Rescore with `alchemy score --unit <id>`;
   `alchemy overlay audit` stays clean, and DONE never drops except by
   withdrawing a match that needed a trick ([COMPILER](COMPILER.md)).

`make verify` runs `alchemy check showcase`. It fails when a folder registered
in `games/THE BROKEN SEAL/recon/showcase.json`, or a header it includes,
contains address names, alias defines, `*_BODY.INC` includes, literal
`0x02`/`0x03`/`0x08` addresses in C, or register, pool or spill comments, when
per-file bindings alias its names, or when its overlay listing holds code or
data directives beyond the allowlist. Register a folder once it meets this
standard; never unregister one to pass the gate.
