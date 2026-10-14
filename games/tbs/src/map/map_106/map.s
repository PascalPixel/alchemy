	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map106
Field_Map106:
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map106, .LField_Map106_METATILES
	map_block Field_Map106, .LField_Map106_SHAPES
	map_block Field_Map106, .LField_Map106_GRID
	map_block Field_Map106, .LField_Map106_ANIMATION
	map_block Field_Map106
	map_block Field_Map106, .LField_Map106_MARKERS
.LField_Map106_METATILES:
	.incbin "games/tbs/src/map/map_106/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map106_SHAPES:
	.incbin "games/tbs/src/map/map_106/shapes.table.lz"
	.balign 4, 0
.LField_Map106_GRID:
	.incbin "games/tbs/src/map/map_106/grid.parts.lz"
	.balign 4, 0
.LField_Map106_ANIMATION:
	.incbin "games/tbs/src/map/map_101/animation.script.lz"
	.balign 4, 0
.LField_Map106_MARKERS:
	.incbin "games/tbs/src/map/map_106/markers.table"
	.global Tileset_Palette12
Tileset_Palette12:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_12.gbapal.lz"
	.byte 0x19, 0xe9, 0x78	@ PACKER: left in the buffer by Field_Map106+0x1a5
