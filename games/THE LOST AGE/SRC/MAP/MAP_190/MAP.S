@ Field_Map190: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map190
Field_Map190:
	map_header 0, 0, 76, 128, 3, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map190, .LField_Map190_METATILES
	map_block Field_Map190, .LField_Map190_SHAPES
	map_block Field_Map190, .LField_Map190_GRID
	map_block Field_Map190, .LField_Map190_SHAPE_GRID
	map_block Field_Map190, .LField_Map190_ANIMATION
	map_block Field_Map190, .LField_Map190_BLEND
	map_block Field_Map190, .LField_Map190_SCENE
.LField_Map190_METATILES:
	.incbin "MAP/MAP_190/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map190_SHAPES:
	.incbin "MAP/MAP_190/SHAPES.table.lz"
	.balign 4, 0
.LField_Map190_GRID:
	.incbin "MAP/MAP_190/GRID.parts.lz"
	.balign 4, 0
.LField_Map190_SHAPE_GRID:
	.incbin "MAP/MAP_190/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map190_ANIMATION:
	.incbin "MAP/MAP_190/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map190_BLEND:
	.incbin "MAP/MAP_190/BLEND.script.lz"
	.balign 4, 0
.LField_Map190_SCENE:
	.incbin "MAP/MAP_190/SCENE.parts.lz"
