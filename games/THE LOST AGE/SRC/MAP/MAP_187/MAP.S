@ Field_Map187: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map187
Field_Map187:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map187, .LField_Map187_METATILES
	map_block Field_Map187, .LField_Map187_SHAPES
	map_block Field_Map187, .LField_Map187_GRID
	map_block Field_Map187, .LField_Map187_SHAPE_GRID
	map_block Field_Map187, .LField_Map187_ANIMATION
	map_block Field_Map187
	map_block Field_Map187, .LField_Map187_SCENE
.LField_Map187_METATILES:
	.incbin "MAP/MAP_187/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map187_SHAPES:
	.incbin "MAP/MAP_187/SHAPES.table.lz"
	.balign 4, 0
.LField_Map187_GRID:
	.incbin "MAP/MAP_187/GRID.parts.lz"
	.balign 4, 0
.LField_Map187_SHAPE_GRID:
	.incbin "MAP/MAP_187/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map187_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map187_SCENE:
	.incbin "MAP/MAP_187/SCENE.parts.lz"
