@ Field_Map287: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map287
Field_Map287:
	map_header 0, 0, 128, 128, 3, 2, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 100, 100, 0, 0, 0xff, 0xff
	map_block Field_Map287, .LField_Map287_METATILES
	map_block Field_Map287, .LField_Map287_SHAPES
	map_block Field_Map287, .LField_Map287_GRID
	map_block Field_Map287, .LField_Map287_SHAPE_GRID
	map_block Field_Map287, .LField_Map287_ANIMATION
	map_block Field_Map287, .LField_Map287_BLEND
	map_block Field_Map287, .LField_Map287_SCENE
.LField_Map287_METATILES:
	.incbin "MAP/MAP_287/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map287_SHAPES:
	.incbin "MAP/MAP_287/SHAPES.table.lz"
	.balign 4, 0
.LField_Map287_GRID:
	.incbin "MAP/MAP_287/GRID.parts.lz"
	.balign 4, 0
.LField_Map287_SHAPE_GRID:
	.incbin "MAP/MAP_287/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map287_ANIMATION:
	.incbin "MAP/MAP_287/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map287_BLEND:
	.incbin "MAP/MAP_287/BLEND.script.lz"
	.balign 4, 0
.LField_Map287_SCENE:
	.incbin "MAP/MAP_287/SCENE.parts.lz"
