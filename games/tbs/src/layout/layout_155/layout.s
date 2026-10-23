	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map155
Field_Map155:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map155, .LField_Map155_METATILES
	map_block Field_Map155, .LField_Map155_SHAPES
	map_block Field_Map155, .LField_Map155_GRID
	map_block Field_Map155, .LField_Map155_ANIMATION
	map_block Field_Map155, .LField_Map155_BLEND
	map_block Field_Map155, .LField_Map155_MARKERS
.LField_Map155_METATILES:
	.incbin "games/tbs/src/layout/layout_155/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map155_SHAPES:
	.incbin "games/tbs/src/layout/layout_155/shapes.table.lz"
	.balign 4, 0
.LField_Map155_GRID:
	.incbin "games/tbs/src/layout/layout_155/grid.parts.lz"
	.balign 4, 0
.LField_Map155_ANIMATION:
	.incbin "games/tbs/src/layout/layout_054/animation.script.lz"
	.balign 4, 0
.LField_Map155_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Map155_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette43
Tileset_Palette43:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_43.gbapal.lz"
	.byte 0xec, 0x5c	@ PACKER: left in the buffer by Field_Map155+0x17e
