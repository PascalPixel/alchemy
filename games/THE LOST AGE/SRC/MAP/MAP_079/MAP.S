@ Field_Map079: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map079
Field_Map079:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map079, .LField_Map079_METATILES
	map_block Field_Map079, .LField_Map079_SHAPES
	map_block Field_Map079, .LField_Map079_GRID
	map_block Field_Map079, .LField_Map079_SHAPE_GRID
	map_block Field_Map079, .LField_Map079_ANIMATION
	map_block Field_Map079, .LField_Map079_BLEND
	map_block Field_Map079, .LField_Map079_SCENE
.LField_Map079_METATILES:
	.incbin "MAP/MAP_079/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map079_SHAPES:
	.incbin "MAP/MAP_079/SHAPES.table.lz"
	.balign 4, 0
.LField_Map079_GRID:
	.incbin "MAP/MAP_079/GRID.parts.lz"
	.balign 4, 0
.LField_Map079_SHAPE_GRID:
	.incbin "MAP/MAP_079/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map079_ANIMATION:
	.incbin "MAP/MAP_078/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map079_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map079_SCENE:
	.incbin "MAP/MAP_079/SCENE.parts.lz"
