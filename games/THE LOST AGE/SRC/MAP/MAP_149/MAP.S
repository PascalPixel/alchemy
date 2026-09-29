@ Field_Map149: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map149
Field_Map149:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map149, .LField_Map149_METATILES
	map_block Field_Map149, .LField_Map149_SHAPES
	map_block Field_Map149, .LField_Map149_GRID
	map_block Field_Map149, .LField_Map149_SHAPE_GRID
	map_block Field_Map149, .LField_Map149_ANIMATION
	map_block Field_Map149, .LField_Map149_BLEND
	map_block Field_Map149, .LField_Map149_SCENE
.LField_Map149_METATILES:
	.incbin "MAP/MAP_149/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map149_SHAPES:
	.incbin "MAP/MAP_149/SHAPES.table.lz"
	.balign 4, 0
.LField_Map149_GRID:
	.incbin "MAP/MAP_149/GRID.parts.lz"
	.balign 4, 0
.LField_Map149_SHAPE_GRID:
	.incbin "MAP/MAP_149/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map149_ANIMATION:
	.incbin "MAP/MAP_149/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map149_BLEND:
	.incbin "MAP/MAP_148/BLEND.script.lz"
	.balign 4, 0
.LField_Map149_SCENE:
	.incbin "MAP/MAP_149/SCENE.parts.lz"
