@ Field_Map012: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map012
Field_Map012:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map012, .LField_Map012_METATILES
	map_block Field_Map012, .LField_Map012_SHAPES
	map_block Field_Map012, .LField_Map012_GRID
	map_block Field_Map012, .LField_Map012_SHAPE_GRID
	map_block Field_Map012, .LField_Map012_ANIMATION
	map_block Field_Map012, .LField_Map012_BLEND
	map_block Field_Map012, .LField_Map012_SCENE
.LField_Map012_METATILES:
	.incbin "MAP/MAP_012/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map012_SHAPES:
	.incbin "MAP/MAP_012/SHAPES.table.lz"
	.balign 4, 0
.LField_Map012_GRID:
	.incbin "MAP/MAP_012/GRID.parts.lz"
	.balign 4, 0
.LField_Map012_SHAPE_GRID:
	.incbin "MAP/MAP_012/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map012_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map012_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map012_SCENE:
	.incbin "MAP/MAP_012/SCENE.parts.lz"
