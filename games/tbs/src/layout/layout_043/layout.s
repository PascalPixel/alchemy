	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map043
Field_Map043:
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map043, .LField_Map043_METATILES
	map_block Field_Map043, .LField_Map043_SHAPES
	map_block Field_Map043, .LField_Map043_GRID
	map_block Field_Map043, .LField_Map043_ANIMATION
	map_block Field_Map043, .LField_Map043_BLEND
	map_block Field_Map043, .LField_Map043_MARKERS
.LField_Map043_METATILES:
	.incbin "games/tbs/src/layout/layout_043/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map043_SHAPES:
	.incbin "games/tbs/src/layout/layout_043/shapes.table.lz"
	.balign 4, 0
.LField_Map043_GRID:
	.incbin "games/tbs/src/layout/layout_043/grid.parts.lz"
	.balign 4, 0
.LField_Map043_ANIMATION:
	.incbin "games/tbs/src/layout/layout_043/animation.script.lz"
	.balign 4, 0
.LField_Map043_BLEND:
	.incbin "games/tbs/src/layout/layout_039/blend.script.lz"
	.balign 4, 0
.LField_Map043_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette02
Tileset_Palette02:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_02.gbapal.lz"
	.byte 0xe3, 0x65	@ PACKER: left in the buffer by Field_Map043+0x166
