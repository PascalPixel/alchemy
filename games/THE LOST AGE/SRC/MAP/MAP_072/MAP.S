@ Field_Map072: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map072
Field_Map072:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map072, .LField_Map072_METATILES
	map_block Field_Map072, .LField_Map072_SHAPES
	map_block Field_Map072, .LField_Map072_GRID
	map_block Field_Map072, .LField_Map072_SHAPE_GRID
	map_block Field_Map072, .LField_Map072_ANIMATION
	map_block Field_Map072
	map_block Field_Map072, .LField_Map072_SCENE
.LField_Map072_METATILES:
	.incbin "MAP/MAP_072/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map072_SHAPES:
	.incbin "MAP/MAP_072/SHAPES.table.lz"
	.balign 4, 0
.LField_Map072_GRID:
	.incbin "MAP/MAP_072/GRID.parts.lz"
	.balign 4, 0
.LField_Map072_SHAPE_GRID:
	.incbin "MAP/MAP_072/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map072_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map072_SCENE:
	.incbin "MAP/MAP_072/SCENE.parts.lz"
