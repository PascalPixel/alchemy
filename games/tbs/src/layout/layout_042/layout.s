	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout042
Field_Layout042:
	layout_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout042, .LField_Layout042_METATILES
	layout_offset Field_Layout042, .LField_Layout042_SHAPES
	layout_offset Field_Layout042, .LField_Layout042_GRID
	layout_offset Field_Layout042, .LField_Layout042_ANIMATION
	layout_offset Field_Layout042, .LField_Layout042_BLEND
	layout_offset Field_Layout042, .LField_Layout042_MARKERS
.LField_Layout042_METATILES:
	.incbin "games/tbs/src/layout/layout_042/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout042_SHAPES:
	.incbin "games/tbs/src/layout/layout_042/shapes.table.lz"
	.balign 4, 0
.LField_Layout042_GRID:
	.incbin "games/tbs/src/layout/layout_042/map.grid.lz"
	.balign 4, 0
.LField_Layout042_ANIMATION:
	.incbin "games/tbs/src/layout/layout_042/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout042_BLEND:
	.incbin "games/tbs/src/layout/layout_039/blend.script.lz"
	.balign 4, 0
.LField_Layout042_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set33Palette
Tileset_Set33Palette:
	.incbin "games/tbs/src/graphics/tileset/set_33/palette.gbapal.lz"
	.byte 0xcb, 0x81	@ PACKER: left in the buffer by Field_Layout042+0x166
	.global Tileset_Set33TilesA
Tileset_Set33TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_33/tiles_a.4bpp.mtf"
	.byte 0x00, 0x80, 0x57	@ PACKER: left in the buffer by Field_Layout042+0x1145
