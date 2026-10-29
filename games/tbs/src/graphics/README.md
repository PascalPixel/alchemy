# Graphics

The images the game shows are kept here as ordinary PNGs anyone can open and
edit: the battle backgrounds, the monsters, the fonts, the
[map](../../glossary.md#map) tiles, the logos and the Title. Every build packs
them into the forms the game reads. [Battle effects](../battle_effect/README.md)
load their sheets from `battle_effects/`, and each map in
[Maps](../map/README.md) places its tiles from `tileset/`. The code that shows
them lives with its bank: the title intro and the credits in
[Menus](../menu/README.md), the decoders beside the code that calls them
([C and assembly](../README.md#c-and-assembly)).

A picture lives here with the assembly file that places it, when that file
places nothing but pictures: their `.incbin` lines and labels, the switches
between editions, alignment and padding, and the bytes Camelot's packing tool
left behind. A file that mixes pictures with other data stays with its bank's
code, and a picture only that file reads sits beside it, as the battle's
advance arrow does beside `battle/element_glow_tables.s`. The headers for
pictures, palettes, sprites and glyphs that several parts of the game read are
in `include/graphics/`.

Each folder holds one kind of picture: the battle backgrounds, the battle
effects, the monsters, a place (`world_map`, `lucky_dice`, `lamakan_desert`,
`title`), or the font, the palettes, the sprites and the windows (`font`,
`palettes`, `sprite`, `ui`).

## How an image is built and placed

The console draws each image from a palette of 16 or 256 colours, and each
pixel is a number into it; a 16-colour image also carries a palette number
saying which of sixteen palettes it uses. The PNGs here are stored the same way, as indexed images
with their own colour list, so an image editor can open them while the game
sees exactly the colours it shipped with.

Each build packs every image into the colours and pixels the game reads,
usually compressed, and places the result on the cartridge under the name the
code uses, most of it after the code, at the end of the cartridge. The placing is a short assembly file in each folder:
`battle_backgrounds/battle_backgrounds.s` places `Battle_GrasslandBackdrop` from
`grassland.gbapal` and the packed `grassland.bitmap.d7`. An image the game asks
for by number is also a row of the
[resource directory](../gamepak/README.md#the-resource-directory-and-table-order):
the grassland is asked for that way both by a battle on open ground and by the
credits. Some packed images are followed by a few bytes
[Camelot](../../glossary.md#camelot)'s packing tool left behind, kept beside
the image with a `PACKER:` note.

A `.tsv` beside an image holds its packing options, such as a font's first
letter. A sprite's `.tsv` in `sprite/` also holds its animations, one line
each, written as the commands the field reads: `picture 55 6` shows
[picture](../../glossary.md#picture) 55 for 6 frames, and `loop`, `play 1` or
`hold` says what follows the last picture: start again, switch to animation 1,
or stay on it.

## Evidence

The folder's name is the credits' "Graphic Arts", the staff who drew these
pictures. `battle_backgrounds`, `battle_effects` and `monster` are the credits'
"Battle Backgrounds", "Battle Effects" and "Monster Design"; `world_map`,
`lucky_dice`, `lucky_wheels` and `colosso` the game's own words;
`lamakan_desert` and `sol_sanctum_statue` the area names of the places they are
drawn in; `title` the Title's scene; `credits` the staff roll itself. The rest
are named in plain English for what they hold.
