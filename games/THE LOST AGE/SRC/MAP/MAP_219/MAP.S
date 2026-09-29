@ Field_Map219: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map219
Field_Map219:
	map_header 0, 0, 128, 128, 3, 1, 0, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map219, .LField_Map219_METATILES
	map_block Field_Map219, .LField_Map219_SHAPES
	map_block Field_Map219, .LField_Map219_GRID
	map_block Field_Map219, .LField_Map219_SHAPE_GRID
	map_block Field_Map219, .LField_Map219_ANIMATION
	map_block Field_Map219, .LField_Map219_BLEND
	map_block Field_Map219, .LField_Map219_SCENE
.LField_Map219_METATILES:
	.incbin "MAP/MAP_219/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map219_SHAPES:
	.incbin "MAP/MAP_219/SHAPES.table.lz"
	.balign 4, 0
.LField_Map219_GRID:
	.incbin "MAP/MAP_219/GRID.parts.lz"
	.balign 4, 0
.LField_Map219_SHAPE_GRID:
	.incbin "MAP/MAP_219/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map219_ANIMATION:
	.incbin "MAP/MAP_219/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map219_BLEND:
	.incbin "MAP/MAP_219/BLEND.script.lz"
	.balign 4, 0
.LField_Map219_SCENE:
	.incbin "MAP/MAP_219/SCENE.parts.lz"
