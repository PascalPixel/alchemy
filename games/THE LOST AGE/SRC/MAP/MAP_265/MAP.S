@ Field_Map265: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map265
Field_Map265:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map265, .LField_Map265_METATILES
	map_block Field_Map265, .LField_Map265_SHAPES
	map_block Field_Map265, .LField_Map265_GRID
	map_block Field_Map265, .LField_Map265_SHAPE_GRID
	map_block Field_Map265, .LField_Map265_ANIMATION
	map_block Field_Map265, .LField_Map265_BLEND
	map_block Field_Map265, .LField_Map265_SCENE
.LField_Map265_METATILES:
	.incbin "MAP/MAP_265/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map265_SHAPES:
	.incbin "MAP/MAP_265/SHAPES.table.lz"
	.balign 4, 0
.LField_Map265_GRID:
	.incbin "MAP/MAP_265/GRID.parts.lz"
	.balign 4, 0
.LField_Map265_SHAPE_GRID:
	.incbin "MAP/MAP_265/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map265_ANIMATION:
	.incbin "MAP/MAP_265/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map265_BLEND:
	.incbin "MAP/MAP_265/BLEND.script.lz"
	.balign 4, 0
.LField_Map265_SCENE:
	.incbin "MAP/MAP_265/SCENE.parts.lz"
