@ Field_Map231: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map231
Field_Map231:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map231, .LField_Map231_METATILES
	map_block Field_Map231, .LField_Map231_SHAPES
	map_block Field_Map231, .LField_Map231_GRID
	map_block Field_Map231, .LField_Map231_SHAPE_GRID
	map_block Field_Map231, .LField_Map231_ANIMATION
	map_block Field_Map231
	map_block Field_Map231, .LField_Map231_SCENE
.LField_Map231_METATILES:
	.incbin "MAP/MAP_231/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map231_SHAPES:
	.incbin "MAP/MAP_231/SHAPES.table.lz"
	.balign 4, 0
.LField_Map231_GRID:
	.incbin "MAP/MAP_231/GRID.parts.lz"
	.balign 4, 0
.LField_Map231_SHAPE_GRID:
	.incbin "MAP/MAP_231/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map231_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map231_SCENE:
	.incbin "MAP/MAP_231/SCENE.parts.lz"
