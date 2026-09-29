@ Field_Map240: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map240
Field_Map240:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map240, .LField_Map240_METATILES
	map_block Field_Map240, .LField_Map240_SHAPES
	map_block Field_Map240, .LField_Map240_GRID
	map_block Field_Map240, .LField_Map240_SHAPE_GRID
	map_block Field_Map240, .LField_Map240_ANIMATION
	map_block Field_Map240
	map_block Field_Map240, .LField_Map240_SCENE
.LField_Map240_METATILES:
	.incbin "MAP/MAP_240/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map240_SHAPES:
	.incbin "MAP/MAP_240/SHAPES.table.lz"
	.balign 4, 0
.LField_Map240_GRID:
	.incbin "MAP/MAP_240/GRID.parts.lz"
	.balign 4, 0
.LField_Map240_SHAPE_GRID:
	.incbin "MAP/MAP_240/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map240_ANIMATION:
	.incbin "MAP/MAP_240/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map240_SCENE:
	.incbin "MAP/MAP_240/SCENE.parts.lz"
