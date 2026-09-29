@ Field_Map069: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map069
Field_Map069:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map069, .LField_Map069_METATILES
	map_block Field_Map069, .LField_Map069_SHAPES
	map_block Field_Map069, .LField_Map069_GRID
	map_block Field_Map069, .LField_Map069_SHAPE_GRID
	map_block Field_Map069, .LField_Map069_ANIMATION
	map_block Field_Map069, .LField_Map069_BLEND
	map_block Field_Map069, .LField_Map069_SCENE
.LField_Map069_METATILES:
	.incbin "MAP/MAP_069/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map069_SHAPES:
	.incbin "MAP/MAP_038/SHAPES.table.lz"
	.balign 4, 0
.LField_Map069_GRID:
	.incbin "MAP/MAP_069/GRID.parts.lz"
	.balign 4, 0
.LField_Map069_SHAPE_GRID:
	.incbin "MAP/MAP_069/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map069_ANIMATION:
	.incbin "MAP/MAP_062/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map069_BLEND:
	.incbin "MAP/MAP_060/BLEND.script.lz"
	.balign 4, 0
.LField_Map069_SCENE:
	.incbin "MAP/MAP_069/SCENE.parts.lz"
