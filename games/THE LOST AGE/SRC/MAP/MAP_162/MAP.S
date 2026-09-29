@ Field_Map162: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map162
Field_Map162:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map162, .LField_Map162_METATILES
	map_block Field_Map162, .LField_Map162_SHAPES
	map_block Field_Map162, .LField_Map162_GRID
	map_block Field_Map162, .LField_Map162_SHAPE_GRID
	map_block Field_Map162, .LField_Map162_ANIMATION
	map_block Field_Map162
	map_block Field_Map162, .LField_Map162_SCENE
.LField_Map162_METATILES:
	.incbin "MAP/MAP_162/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map162_SHAPES:
	.incbin "MAP/MAP_162/SHAPES.table.lz"
	.balign 4, 0
.LField_Map162_GRID:
	.incbin "MAP/MAP_162/GRID.parts.lz"
	.balign 4, 0
.LField_Map162_SHAPE_GRID:
	.incbin "MAP/MAP_162/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map162_ANIMATION:
	.incbin "MAP/MAP_162/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map162_SCENE:
	.incbin "MAP/MAP_162/SCENE.parts.lz"
