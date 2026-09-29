@ Field_Map013: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map013
Field_Map013:
	map_header 0, 0, 110, 110, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map013, .LField_Map013_METATILES
	map_block Field_Map013, .LField_Map013_SHAPES
	map_block Field_Map013, .LField_Map013_GRID
	map_block Field_Map013, .LField_Map013_SHAPE_GRID
	map_block Field_Map013, .LField_Map013_ANIMATION
	map_block Field_Map013, .LField_Map013_BLEND
	map_block Field_Map013, .LField_Map013_SCENE
.LField_Map013_METATILES:
	.incbin "MAP/MAP_013/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map013_SHAPES:
	.incbin "MAP/MAP_013/SHAPES.table.lz"
	.balign 4, 0
.LField_Map013_GRID:
	.incbin "MAP/MAP_013/GRID.parts.lz"
	.balign 4, 0
.LField_Map013_SHAPE_GRID:
	.incbin "MAP/MAP_013/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map013_ANIMATION:
	.incbin "MAP/MAP_013/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map013_BLEND:
	.incbin "MAP/MAP_013/BLEND.script.lz"
	.balign 4, 0
.LField_Map013_SCENE:
	.incbin "MAP/MAP_013/SCENE.parts.lz"
