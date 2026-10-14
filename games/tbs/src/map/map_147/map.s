	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map147
Field_Map147:
	map_header 0, 0, 64, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 128, 0, 20, 20, -121, 35, 0x1f, 0x1f
	map_block Field_Map147, .LField_Map147_METATILES
	map_block Field_Map147, .LField_Map147_SHAPES
	map_block Field_Map147, .LField_Map147_GRID
	map_block Field_Map147, .LField_Map147_ANIMATION
	map_block Field_Map147
	map_block Field_Map147, .LField_Map147_MARKERS
.LField_Map147_METATILES:
	.incbin "games/tbs/src/map/map_147/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map147_SHAPES:
	.incbin "games/tbs/src/map/map_147/shapes.table.lz"
	.balign 4, 0
.LField_Map147_GRID:
	.incbin "games/tbs/src/map/map_147/grid.parts.lz"
	.balign 4, 0
.LField_Map147_ANIMATION:
	.incbin "games/tbs/src/map/map_147/animation.script.lz"
	.balign 4, 0
.LField_Map147_MARKERS:
	.incbin "games/tbs/src/map/map_147/markers.table"
	.balign 4, 0
	.global Tileset_Palette39
Tileset_Palette39:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_39.gbapal.lz"
