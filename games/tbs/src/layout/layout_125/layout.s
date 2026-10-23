	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map125
Field_Map125:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map125, .LField_Map125_METATILES
	map_block Field_Map125, .LField_Map125_SHAPES
	map_block Field_Map125, .LField_Map125_GRID
	map_block Field_Map125, .LField_Map125_ANIMATION
	map_block Field_Map125, .LField_Map125_BLEND
	map_block Field_Map125, .LField_Map125_MARKERS
.LField_Map125_METATILES:
	.incbin "games/tbs/src/layout/layout_125/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map125_SHAPES:
	.incbin "games/tbs/src/layout/layout_125/shapes.table.lz"
	.balign 4, 0
.LField_Map125_GRID:
	.incbin "games/tbs/src/layout/layout_125/grid.parts.lz"
	.balign 4, 0
.LField_Map125_ANIMATION:
	.incbin "games/tbs/src/layout/layout_125/animation.script.lz"
	.balign 4, 0
.LField_Map125_BLEND:
	.incbin "games/tbs/src/layout/layout_125/blend.script.lz"
	.balign 4, 0
.LField_Map125_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette29
Tileset_Palette29:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_29.gbapal.lz"
	.byte 0x9d, 0xe1, 0xfd	@ PACKER: left in the buffer by Field_Map125+0x189
