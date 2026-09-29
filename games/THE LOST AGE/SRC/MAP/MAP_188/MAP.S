@ Field_Map188: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map188
Field_Map188:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map188, .LField_Map188_METATILES
	map_block Field_Map188, .LField_Map188_SHAPES
	map_block Field_Map188, .LField_Map188_GRID
	map_block Field_Map188, .LField_Map188_SHAPE_GRID
	map_block Field_Map188, .LField_Map188_ANIMATION
	map_block Field_Map188
	map_block Field_Map188, .LField_Map188_SCENE
.LField_Map188_METATILES:
	.incbin "MAP/MAP_188/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map188_SHAPES:
	.incbin "MAP/MAP_188/SHAPES.table.lz"
	.balign 4, 0
.LField_Map188_GRID:
	.incbin "MAP/MAP_188/GRID.parts.lz"
	.balign 4, 0
.LField_Map188_SHAPE_GRID:
	.incbin "MAP/MAP_188/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map188_ANIMATION:
	.incbin "MAP/MAP_188/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map188_SCENE:
	.incbin "MAP/MAP_188/SCENE.parts.lz"
