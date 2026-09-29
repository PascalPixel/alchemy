@ Field_Map241: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map241
Field_Map241:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map241, .LField_Map241_METATILES
	map_block Field_Map241, .LField_Map241_SHAPES
	map_block Field_Map241, .LField_Map241_GRID
	map_block Field_Map241, .LField_Map241_SHAPE_GRID
	map_block Field_Map241, .LField_Map241_ANIMATION
	map_block Field_Map241
	map_block Field_Map241, .LField_Map241_SCENE
.LField_Map241_METATILES:
	.incbin "MAP/MAP_241/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map241_SHAPES:
	.incbin "MAP/MAP_241/SHAPES.table.lz"
	.balign 4, 0
.LField_Map241_GRID:
	.incbin "MAP/MAP_241/GRID.parts.lz"
	.balign 4, 0
.LField_Map241_SHAPE_GRID:
	.incbin "MAP/MAP_241/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map241_ANIMATION:
	.incbin "MAP/MAP_241/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map241_SCENE:
	.incbin "MAP/MAP_241/SCENE.parts.lz"
