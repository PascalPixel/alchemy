# Assets

## Layouts are data

Asset layouts, offsets, widths, counts, pointers, frames and compression plans
belong in maintained data under `games/`, not resource-specific executables.
`alchemy build assets` uses shared typed, record and pointer tables, component
composition, and pixel, text, sample and compression codecs. Preserve
validation of ranges, extents, ordering, padding and reference round trips. A
layout is data; renaming hard-coded game logic does not make a portable codec.

Do not maintain metadata a shared encoder can derive. Zero-skip sprite banks
use PNG frames plus geometry and palette settings in the existing series index;
the encoder generates alignment and the frame pointer directory. Keep
compression recipes only where the encoder cannot yet reproduce the original
stream without them. JSON is the repository's metadata format for registries,
layouts and codec recipes; it is reconstruction metadata, not a claim about
Camelot's files, and moving it to another format would only add a parser.
Runtime tables whose structure is understood become typed C in the unit that
reads them ([SOURCE](SOURCE.md), the Lunpa standard), and editable media stay
PNG, WAV and MIDI.

Localized message source uses UTF-8 GNU gettext PO in `TEXT/<LANGUAGE>.PO`.
The numeric message key is `msgid`, its editable text is `msgstr`, and
`msgctxt "message"` identifies the archive. Named brace commands preserve
runtime controls; unknown glyphs remain explicit tokens. Matching numeric keys
across editions is not proof of matching meaning. The shared encoder derives
Huffman models, character-group headers, message lengths and directories from
the source. Each game's asset build rebuilds its six message archives; exact
text-archive verification does not imply a complete regional ROM build.

## Where inputs live

Editable assets belong beside their owner in the game's `SRC`; there is no
generic `ASSETS` directory. Move area-exclusive maps and graphics beside their
code only after their consumers establish ownership. Shared map containers stay
once under `SRC/FIELD/COMMON`; reused tile banks and palettes in
`SRC/GRAPHICS/COMMON`; sequences, samples and instrument definitions in the
game's `SOUND` (with `SEQUENCE`, `SAMPLE` and `INSTRUMENT` owners, tracked only
once that game's asset manifest rebuilds them); runtime audio C in
`SRC/SOUND`; localized messages and credits in `TEXT`. Do not duplicate shared
resources, invent asset associations, or publish protected extracted inputs.
Use `.gitkeep` only for necessary empty destinations.

The glyph sheet is in `SRC/GRAPHICS/FONT`, with the localization glyph table
beside it: each of its 224 records combines a two-byte advance with 15 two-byte
bitmap rows, so its 7,168 bytes are not all pixels; keep that layout intact.
Mixed item, status, window and menu-image banks are in `SRC/GRAPHICS/TILE`.
Battle-effect tables accompany their consumers in `SRC/BATTLE/DATA`.

Character banks live in `SRC/GRAPHICS/CHARACTER`. Confirmed Japanese ROM names
own `CHAR_<ROMAJI>.PNG` field sheets and `BATTLE_<ROMAJI>.PNG` battle sheets,
with both bank definitions in one character JSON file. Names require message
and runtime descriptor evidence; visual resemblance alone is not enough.
Unproven and shared banks occupy sections of `CHAR_COMMON.PNG`, described once
in `COMMON.JSON`. Pixel indices stay separate from the common palette bank
pool. Run `alchemy build assets --audit-characters OUTPUT` for the Japanese
message and descriptor audit. The common descriptor catalog also feeds
animation and UI consumers; do not split banks by an export label alone.

Additional sprite archives use the same character atlas, with archive offsets
and frame order kept in metadata until runtime associations establish names.
Shared still images, dialogue portraits and eight-bit tile banks use indexed
atlases under `GRAPHICS/COMMON`, with separate palettes. Unknown location and
runtime-role fields stay null. Pixel appearance does not establish a world-map
or battle association. Palette tables preserve all sixteen bits, including the
high bit former RGBA exports stored as alpha. Byte visualizations and JSON
integer streams are not source inputs: type bytes from their consumers, or
register bytes of unknown structure in `SOURCE.JSON` as private inputs
extracted from the verified ROM.

## Private inputs

`games/<game>/SOURCE.JSON` binds map containers, graphics resources and scene
loaders to their physical inputs. Each map owner has one named JSON file of
container sections and one packed binary file; each graphics owner has one
indexed `CHR` sheet whose tile offsets select independent 512-tile banks, and
identical banks share one section. Palette banks and compression recipes have
one common owner. These are reconstruction formats, not evidence of Camelot's
authoring files.

Packed maps, map layers of unknown structure, `CHR` sheets and the common
palette file are private, ignored inputs, even when named `.PNG` or `.JSON`.
Regenerate them with `alchemy build assets --extract-sources roms/tbs-en.gba`
or `alchemy build assets --extract-sources roms/tla-en.gba --target tla-en`;
the reader requires the registered ROM checksum and decoded input hashes. Asset
and full-ROM builds restore absent private inputs from the checksum-verified
local ROM in an isolated temporary workspace, installing only missing files;
`--extract-missing-sources ROM` runs that step explicitly and does not turn
reference bytes into build-time fallback regions. The ordinary build encodes
the sources and compares every stored region with the ROM; `--source-only`
builds without reading the ROM. Never replace uncertain edge regions with
invented blank cells. As pret's `rom_header.s` reserves `.space 156`,
`SRC/SYSTEM/ROM_HEADER.JSON` leaves out the cartridge logo; its bytes are the
private input `SRC/SYSTEM/ROM_HEADER_LOGO.BIN`.

Image round-trip equality proves storage bytes, not the intended picture layout
or colors. Establish frame boundaries, tile ordering and palette selection from
the runtime consumers before rearranging images. A grayscale index palette is
not evidence of monochrome artwork, and a narrow animation strip is not by
itself a bad export. Existing extraction metadata is a hypothesis to check.
The menu-label sheet follows `RenderResource_CreatePair`: two 32×16 OBJ parts
per image, the second at X+32 and tile+8; `frames_per_image` groups adjacent
atlas parts of one compressed image and changes no ROM data.

## Compression

`COMMON/COMPRESSION.JSON` keeps codec metadata and references the shared
`COMPRESSION.TOKENS` control table, which records encoder decisions at decoded
offsets and contains no literal image bytes or copied ROM streams; its format
is documented beside its reader. Recipe rows inherit `recipe_codec`; explicit
codec names override it. `alchemy build assets --compact-plans PLAN` packs
generated plans and verifies their exact inverse before writing;
`--derive-plans PLAN` replaces control streams with deterministic matchers plus
exceptions and compares every affected encoded asset before writing.
The shared predictor is named once in `token_table`; `exceptions: []` means no
overrides and `exceptions: [offset, size, count]` selects a table extent.
Exceptions record only decisions that differ from the rule and keep results
byte-exact; the matchers do not claim to reproduce the original compressor. A
plan may record at most three trailing stream bytes as `lookahead`;
`make verify` rejects longer tails with `alchemy check plan-tails`.

## Review sheets and previews

Derived identification sheets are owned by `SRC/GRAPHICS/REVIEW.JSON`.
`alchemy build assets --review-images out/ROM_LIBRARY_REVIEW` regenerates the
sheets and index from private native inputs, needing no audit output or ROM;
restore missing inputs with `--extract-missing-sources roms/tbs-en.gba` first.
Continue identification by editing the evidence in `REVIEW.JSON`; unknown
palettes, incomplete effect cuts and provisional geometry stay explicit.
`make review-images-check`, part of `make verify`, checks the baseline hash of
sorted filenames, dimensions, pixel indices and resolved RGBA, independent of
PNG compression. Deliberate presentation changes need visual review before
`--review-images OUTPUT --update-baseline`.

Root `PROGRESS.svg` holds the README coverage figure, which carries repository
names and measurements and embeds no font, image, sound or bytes from the game.
Every other preview, render, animation, font or sheet made from game material
goes under `out/<target>/previews` or another stable directory in `out/` and is
never tracked. `make clean-preview` lists generated trees; `make clean` removes
them, never the coverage figure or native inputs.

## Maps and the world assembly

`alchemy build assets --network ROM --target TARGET -o out/<dir>` draws a map
network: every room of a place, how its exits connect, and a report of both.
`--scenes LIST --expand` follows exits out from named scenes until the world
map or a scene whose exits depend on story flags, such as the ship. `--world`
assembles those rooms as one world:

1. Every room is cut out and stands on one plane where its doors meet the
   arrivals they lead to, as nearly as all its links allow. Exits onto the
   world map join places too, pulled together along the direction between
   their world map spots until their rooms touch.
2. Stairs then split rooms into floors three metres apart (a 16-pixel cell is
   a metre), keeping every room's position.
3. On a floor, a room whose every door leads into a larger room it lies inside
   is drawn under it, as a house is under its village. This order is
   temporary until the viewer has controls.

Maps can pack separate areas into one picture, fenced by pure cyan letter-A
tiles; rooms are cut at those fences. Two maps with the same layout and
different palettes are one place in two story states, not a rendering bug. The
output, including its per-floor pictures and the local 3D viewer, holds game
pictures and stays under `out/` ([TRACKING](TRACKING.md)).
