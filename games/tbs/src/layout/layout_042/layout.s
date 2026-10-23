	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map042
Field_Map042:
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map042, .LField_Map042_METATILES
	map_block Field_Map042, .LField_Map042_SHAPES
	map_block Field_Map042, .LField_Map042_GRID
	map_block Field_Map042, .LField_Map042_ANIMATION
	map_block Field_Map042, .LField_Map042_BLEND
	map_block Field_Map042, .LField_Map042_MARKERS
.LField_Map042_METATILES:
	.incbin "games/tbs/src/layout/layout_042/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map042_SHAPES:
	.incbin "games/tbs/src/layout/layout_042/shapes.table.lz"
	.balign 4, 0
.LField_Map042_GRID:
	.incbin "games/tbs/src/layout/layout_042/grid.parts.lz"
	.balign 4, 0
.LField_Map042_ANIMATION:
	.incbin "games/tbs/src/layout/layout_042/animation.script.lz"
	.balign 4, 0
.LField_Map042_BLEND:
	.incbin "games/tbs/src/layout/layout_039/blend.script.lz"
	.balign 4, 0
.LField_Map042_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set33Palette
Tileset_Set33Palette:
	.incbin "games/tbs/src/graphics/tileset/set_33/palette.gbapal.lz"
	.byte 0xcb, 0x81	@ PACKER: left in the buffer by Field_Map042+0x166
	.global Tileset_Set33TilesA
Tileset_Set33TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_33/tiles_a.4bpp.mtf"
	.byte 0x00, 0x80, 0x57	@ PACKER: left in the buffer by Field_Map042+0x1145
