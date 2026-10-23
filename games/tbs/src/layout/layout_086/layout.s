	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map086
Field_Map086:
	map_header 0, 0, 100, 68, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map086, .LField_Map086_METATILES
	map_block Field_Map086, .LField_Map086_SHAPES
	map_block Field_Map086, .LField_Map086_GRID
	map_block Field_Map086, .LField_Map086_ANIMATION
	map_block Field_Map086, .LField_Map086_BLEND
	map_block Field_Map086, .LField_Map086_MARKERS
.LField_Map086_METATILES:
	.incbin "games/tbs/src/layout/layout_086/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map086_SHAPES:
	.incbin "games/tbs/src/layout/layout_086/shapes.table.lz"
	.balign 4, 0
.LField_Map086_GRID:
	.incbin "games/tbs/src/layout/layout_086/grid.parts.lz"
	.balign 4, 0
.LField_Map086_ANIMATION:
	.incbin "games/tbs/src/layout/layout_086/animation.script.lz"
	.balign 4, 0
.LField_Map086_BLEND:
	.incbin "games/tbs/src/layout/layout_086/blend.script.lz"
	.balign 4, 0
.LField_Map086_MARKERS:
	.incbin "games/tbs/src/layout/layout_086/markers.table"
	.balign 4, 0
	.global Tileset_Palette06
Tileset_Palette06:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_06.gbapal.lz"
	.byte 0x2a, 0x02, 0x98	@ PACKER: left in the buffer by Field_Map086+0x15d
