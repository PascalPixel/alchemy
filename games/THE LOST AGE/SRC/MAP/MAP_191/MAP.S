@ Field_Map191: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map191
Field_Map191:
	map_header 0, 0, 128, 112, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map191, .LField_Map191_METATILES
	map_block Field_Map191, .LField_Map191_SHAPES
	map_block Field_Map191, .LField_Map191_GRID
	map_block Field_Map191, .LField_Map191_SHAPE_GRID
	map_block Field_Map191, .LField_Map191_ANIMATION
	map_block Field_Map191, .LField_Map191_BLEND
	map_block Field_Map191, .LField_Map191_SCENE
.LField_Map191_METATILES:
	.incbin "MAP/MAP_191/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map191_SHAPES:
	.incbin "MAP/MAP_191/SHAPES.table.lz"
	.balign 4, 0
.LField_Map191_GRID:
	.incbin "MAP/MAP_191/GRID.parts.lz"
	.balign 4, 0
.LField_Map191_SHAPE_GRID:
	.incbin "MAP/MAP_191/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map191_ANIMATION:
	.incbin "MAP/MAP_191/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map191_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map191_SCENE:
	.incbin "MAP/MAP_191/SCENE.parts.lz"
