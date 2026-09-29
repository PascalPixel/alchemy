@ Field_Map237: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map237
Field_Map237:
	map_header 0, 0, 88, 104, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map237, .LField_Map237_METATILES
	map_block Field_Map237, .LField_Map237_SHAPES
	map_block Field_Map237, .LField_Map237_GRID
	map_block Field_Map237, .LField_Map237_SHAPE_GRID
	map_block Field_Map237, .LField_Map237_ANIMATION
	map_block Field_Map237
	map_block Field_Map237, .LField_Map237_SCENE
.LField_Map237_METATILES:
	.incbin "MAP/MAP_237/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map237_SHAPES:
	.incbin "MAP/MAP_237/SHAPES.table.lz"
	.balign 4, 0
.LField_Map237_GRID:
	.incbin "MAP/MAP_237/GRID.parts.lz"
	.balign 4, 0
.LField_Map237_SHAPE_GRID:
	.incbin "MAP/MAP_237/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map237_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map237_SCENE:
	.incbin "MAP/MAP_237/SCENE.parts.lz"
