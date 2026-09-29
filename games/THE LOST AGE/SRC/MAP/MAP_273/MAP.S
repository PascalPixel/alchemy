@ Field_Map273: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map273
Field_Map273:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map273, .LField_Map273_METATILES
	map_block Field_Map273, .LField_Map273_SHAPES
	map_block Field_Map273, .LField_Map273_GRID
	map_block Field_Map273, .LField_Map273_SHAPE_GRID
	map_block Field_Map273, .LField_Map273_ANIMATION
	map_block Field_Map273
	map_block Field_Map273, .LField_Map273_SCENE
.LField_Map273_METATILES:
	.incbin "MAP/MAP_273/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map273_SHAPES:
	.incbin "MAP/MAP_273/SHAPES.table.lz"
	.balign 4, 0
.LField_Map273_GRID:
	.incbin "MAP/MAP_273/GRID.parts.lz"
	.balign 4, 0
.LField_Map273_SHAPE_GRID:
	.incbin "MAP/MAP_273/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map273_ANIMATION:
	.incbin "MAP/MAP_273/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map273_SCENE:
	.incbin "MAP/MAP_273/SCENE.parts.lz"
