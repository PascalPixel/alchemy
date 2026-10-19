# Graphics

This folder holds The Broken Seal's pictures and the code that draws the
screens that are mostly pictures: the title intro and the logos, the palette
fade, the credits, and the handwritten decoders that unpack tiles and frames.

Most of it is data. A picture is an indexed PNG beside the `.s` that places
it; the build packs the PNG (and the `.tsv` beside it, where it has one) into
the form the game reads, and the `.s` gives it its label. Edit the PNG and
its palette. A picture both games share is under `games/common/src/graphics`,
which the `.s` here includes.

## Files

| File | What it holds |
| --- | --- |
| `title/intro.c` | the title intro, the Nintendo and Camelot logos, and the palette fade (`PaletteFade_`) |
| `credits/credits.c` | the credits: the backdrops that slide and crossfade, and the scrolling text |
| `text/numwords.c` | the counted nouns the German message decoder writes after a number |
| `text/genders.s` | the grammatical gender of item names, by language |
| `text/decode_symbol.s`, `render/decode.s`, `tile/*.s` | handwritten ARM and Thumb decoders, copiers and a dot product, each proven in its header |
| `battle_fx/` | the battle effects' sheets and masks (`BattleFx_`), loaded by resource id |
| `ui/` | window tiles, the small font, icons and menu sprites |
| `world_map/` | the world map's tiles, palettes and the story frames |
| `title/`, `render/labels.s`, `display/credits.s` | the "To be continued" lettering, the text headings and the credits font |
| `backdrop/`, `monster/`, `sprite/`, `colosso/`, `lamakan/`, `lucky_dice/`, `slots/`, `sol_sanctum_statue/` | the pictures of the place or screen they name |
| `animref*.s` | animation slot tables |

The unit headers are in `games/tbs/include/unit/graphics/`. Link order is in
`games/tbs/objects.tsv`; a picture an effect or scene loads by number is an
entry of the resource directory (`games/tbs/src/system/resource/directory.s`).

## Words used here

- **half**: the credits load their backdrops into one of two halves of BG
  tile memory and the BG palette (`alternate`): 0 is VRAM from 0x06000000 and
  colours 0 to 127, 1 is VRAM from 0x06008000 and colours 128 to 255.
- **component**: one of a colour's red, green and blue levels, 0 to 31. The
  palette fade steps all 1536 components of the 512 BG and sprite colours;
  its `paletteHalf` 1 fades only the BG colours and 2 only the sprites'.
- **tile ring**: the credits' 32 rows of text tiles, sixteen of them visible,
  reused as the text scrolls.

## Prefixes

| Prefix | What it acts on |
| --- | --- |
| `Title_` | the title intro and the logos |
| `PaletteFade_` | the palette fade |
| `DisplayScroll_` | the credits |
| `Graphics_` | tile memory and palettes, as the engine names them elsewhere |
| `Tile_`, `TileMap_`, `BitDecoder_`, `Render_`, `Text_` | the handwritten decoders |
