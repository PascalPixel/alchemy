	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout098
Field_Layout098:
	layout_header 0, 0, 56, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout098, .LField_Layout098_METATILES
	layout_offset Field_Layout098, .LField_Layout098_SHAPES
	layout_offset Field_Layout098, .LField_Layout098_GRID
	layout_offset Field_Layout098, .LField_Layout098_ANIMATION
	layout_offset Field_Layout098, .LField_Layout098_BLEND
	layout_offset Field_Layout098, .LField_Layout098_MARKERS
.LField_Layout098_METATILES:
	.incbin "games/tbs/src/layout/layout_098/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout098_SHAPES:
	.incbin "games/tbs/src/layout/layout_098/shapes.table.lz"
	.balign 4, 0
.LField_Layout098_GRID:
	.incbin "games/tbs/src/layout/layout_098/grid.parts.lz"
	.balign 4, 0
.LField_Layout098_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout098_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout098_MARKERS:
	.incbin "games/tbs/src/layout/layout_098/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set53Palette
Tileset_Set53Palette:
	.incbin "games/tbs/src/graphics/tileset/set_53/palette.gbapal.lz"
	.byte 0x14, 0x41, 0xb9	@ PACKER: left in the buffer by Field_Layout098+0x189
	.global Tileset_Set53TilesA
Tileset_Set53TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_53/tiles_a.4bpp.mtf"
	.byte 0x09, 0x02	@ PACKER: left in the buffer by Field_Layout098+0x1a82
	.global Tileset_Set53TilesB
Tileset_Set53TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_53/tiles_b.4bpp.mtf"
