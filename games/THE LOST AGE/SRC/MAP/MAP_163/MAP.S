@ Field_Map163: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map163
Field_Map163:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map163, .LField_Map163_METATILES
	map_block Field_Map163, .LField_Map163_SHAPES
	map_block Field_Map163, .LField_Map163_GRID
	map_block Field_Map163, .LField_Map163_SHAPE_GRID
	map_block Field_Map163, .LField_Map163_ANIMATION
	map_block Field_Map163
	map_block Field_Map163, .LField_Map163_SCENE
.LField_Map163_METATILES:
	.incbin "MAP/MAP_163/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map163_SHAPES:
	.incbin "MAP/MAP_163/SHAPES.table.lz"
	.balign 4, 0
.LField_Map163_GRID:
	.incbin "MAP/MAP_163/GRID.parts.lz"
	.balign 4, 0
.LField_Map163_SHAPE_GRID:
	.incbin "MAP/MAP_163/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map163_ANIMATION:
	.incbin "MAP/MAP_163/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map163_SCENE:
	.incbin "MAP/MAP_163/SCENE.parts.lz"
