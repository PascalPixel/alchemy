@ Field_Map204: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map204
Field_Map204:
	map_header 0, 0, 128, 64, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map204, .LField_Map204_METATILES
	map_block Field_Map204, .LField_Map204_SHAPES
	map_block Field_Map204, .LField_Map204_GRID
	map_block Field_Map204, .LField_Map204_SHAPE_GRID
	map_block Field_Map204, .LField_Map204_ANIMATION
	map_block Field_Map204, .LField_Map204_BLEND
	map_block Field_Map204, .LField_Map204_SCENE
.LField_Map204_METATILES:
	.incbin "MAP/MAP_204/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map204_SHAPES:
	.incbin "MAP/MAP_204/SHAPES.table.lz"
	.balign 4, 0
.LField_Map204_GRID:
	.incbin "MAP/MAP_204/GRID.parts.lz"
	.balign 4, 0
.LField_Map204_SHAPE_GRID:
	.incbin "MAP/MAP_204/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map204_ANIMATION:
	.incbin "MAP/MAP_204/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map204_BLEND:
	.incbin "MAP/MAP_204/BLEND.script.lz"
	.balign 4, 0
.LField_Map204_SCENE:
	.incbin "MAP/MAP_204/SCENE.parts.lz"
