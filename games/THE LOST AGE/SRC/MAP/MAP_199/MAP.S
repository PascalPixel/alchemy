@ Field_Map199: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map199
Field_Map199:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map199, .LField_Map199_METATILES
	map_block Field_Map199, .LField_Map199_SHAPES
	map_block Field_Map199, .LField_Map199_GRID
	map_block Field_Map199, .LField_Map199_SHAPE_GRID
	map_block Field_Map199, .LField_Map199_ANIMATION
	map_block Field_Map199
	map_block Field_Map199, .LField_Map199_SCENE
.LField_Map199_METATILES:
	.incbin "MAP/MAP_199/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map199_SHAPES:
	.incbin "MAP/MAP_199/SHAPES.table.lz"
	.balign 4, 0
.LField_Map199_GRID:
	.incbin "MAP/MAP_199/GRID.parts.lz"
	.balign 4, 0
.LField_Map199_SHAPE_GRID:
	.incbin "MAP/MAP_199/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map199_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map199_SCENE:
	.incbin "MAP/MAP_199/SCENE.parts.lz"
