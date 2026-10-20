# Graphics

This folder holds The Broken Seal's pictures and the code that draws the
screens that are mostly pictures: the title intro and the logos, the palette
fade, the credits, and the handwritten decoders that unpack tiles and frames.

Most of it is data. A picture is an indexed PNG beside the `.s` that places
it; the build packs the PNG (and the `.tsv` beside it, where it has one) into
the form the game reads, and the `.s` gives it its label. Edit the PNG and
its palette.

See also: [Battle effects](../effect/README.md), which load the sheets in
`battle_fx/`, and [Maps](../map/README.md), whose `map.s` files place the
tile sets.

## Files

| File | What it holds |
| --- | --- |
| `title/intro.c` | the title intro, the Nintendo and Camelot logos, and the palette fade (`PaletteFade_`) |
| `credits/credits.c` | the credits: the backdrops that slide and crossfade, and the scrolling text |
| `text/numwords.c` | the counted nouns the German message decoder writes after a number |
| `text/genders.s` | the grammatical gender of item names, by language |
| `text/decode_symbol.s`, [`render/decode.s`](render/decode.s), `tile/*.s` | handwritten ARM and Thumb decoders, copiers and a dot product, each proven in its header |
| `battle_fx/` | the battle effects' sheets and masks (`BattleFx_`), loaded by resource id |
| `ui/` | window tiles, the small font, icons and menu sprites |
| `world_map/` | the world map's tiles, palettes and the story frames |
| `title/`, `render/labels.s`, `display/credits.s` | the "To be continued" lettering, the text headings and the credits font |
| `backdrop/`, `monster/`, `sprite/`, `colosso/`, `lamakan/`, `lucky_dice/`, `slots/`, `sol_sanctum_statue/` | the pictures of the place or screen they name |
| `animref*.s` | animation slot tables |

The unit headers are in `games/tbs/include/unit/graphics/`. Link order is in
`games/tbs/objects.tsv`; a picture an effect or scene loads by number is an
entry of the resource directory (`games/tbs/src/system/resource/directory.s`).

## Words

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

The comment markers are in [Matching](../MATCHING.md).

## Design

### The credits crossfade between two halves

The credits show a sequence of battle backdrops, each fading into the next.
Both backdrops must be on screen at once during a fade, so the credits split
the 256-colour BG tile memory and the BG palette into two halves: BG3 draws
from tiles at 0x06000000 with colours 0 to 127, BG2 from tiles at 0x06008000
with colours 128 to 255 (`Graphics_LoadCharacterBlockAndPalette` offsets the
second half's pixel values by 128). `DisplayScroll_RunSlideshow` loads each
backdrop into the half that is not showing, then steps the blend weights
between BG2 and BG3 over 16 steps of 4 frames, and holds. The halves swap
roles from one backdrop to the next, so no backdrop is ever loaded while it is
being shown.

The scrolling text uses a ring of 32 rows of tiles of which 16 are visible.
A line is drawn into the row 16 ahead of the top as the text scrolls
(`DisplayScroll_RenderEnteringLine`), so only lines about to show are drawn,
and the tile memory needed does not grow with the length of the credits.

The credits run with the engine's usual frame mostly switched off: they reset
the task table, stop the VBlank's OAM copy (`gOamCopyEnabled`) and send their
own 128 sprites from a heap buffer. Its pointer occupies the otherwise idle
flash-library cell `gFlashNumRemainingBytes`; the other borrowed cells hold
the scroll position and end-of-text state.

## Evidence and open questions

The pictures are named for what they show, from the scenes and effects that
load them; the decoders for the format each unpacks, proven in their headers;
the credits' and title's functions from what they draw.

Still unknown:

- XXX: why the credits borrow flash-library cells for their state instead
  of dedicated globals.
