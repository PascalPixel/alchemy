@ Field_Map043: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map043
Field_Map043:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map043, .LField_Map043_METATILES
	map_block Field_Map043, .LField_Map043_SHAPES
	map_block Field_Map043, .LField_Map043_GRID
	map_block Field_Map043, .LField_Map043_SHAPE_GRID
	map_block Field_Map043, .LField_Map043_ANIMATION
	map_block Field_Map043, .LField_Map043_BLEND
	map_block Field_Map043, .LField_Map043_SCENE
.LField_Map043_METATILES:
	.incbin "MAP/MAP_043/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map043_SHAPES:
	.incbin "MAP/MAP_043/SHAPES.table.lz"
	.balign 4, 0
.LField_Map043_GRID:
	.incbin "MAP/MAP_043/GRID.parts.lz"
	.balign 4, 0
.LField_Map043_SHAPE_GRID:
	.incbin "MAP/MAP_043/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map043_ANIMATION:
	.incbin "MAP/MAP_043/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map043_BLEND:
	.incbin "MAP/MAP_043/BLEND.script.lz"
	.balign 4, 0
.LField_Map043_SCENE:
	.incbin "MAP/MAP_043/SCENE.parts.lz"
