@ Field_Map018: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map018
Field_Map018:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map018, .LField_Map018_METATILES
	map_block Field_Map018, .LField_Map018_SHAPES
	map_block Field_Map018, .LField_Map018_GRID
	map_block Field_Map018, .LField_Map018_SHAPE_GRID
	map_block Field_Map018, .LField_Map018_ANIMATION
	map_block Field_Map018, .LField_Map018_BLEND
	map_block Field_Map018, .LField_Map018_SCENE
.LField_Map018_METATILES:
	.incbin "MAP/MAP_018/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map018_SHAPES:
	.incbin "MAP/MAP_018/SHAPES.table.lz"
	.balign 4, 0
.LField_Map018_GRID:
	.incbin "MAP/MAP_018/GRID.parts.lz"
	.balign 4, 0
.LField_Map018_SHAPE_GRID:
	.incbin "MAP/MAP_018/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map018_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map018_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map018_SCENE:
	.incbin "MAP/MAP_018/SCENE.parts.lz"
