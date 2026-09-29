@ Field_Map229: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map229
Field_Map229:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map229, .LField_Map229_METATILES
	map_block Field_Map229, .LField_Map229_SHAPES
	map_block Field_Map229, .LField_Map229_GRID
	map_block Field_Map229, .LField_Map229_SHAPE_GRID
	map_block Field_Map229, .LField_Map229_ANIMATION
	map_block Field_Map229
	map_block Field_Map229, .LField_Map229_SCENE
.LField_Map229_METATILES:
	.incbin "MAP/MAP_229/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map229_SHAPES:
	.incbin "MAP/MAP_229/SHAPES.table.lz"
	.balign 4, 0
.LField_Map229_GRID:
	.incbin "MAP/MAP_229/GRID.parts.lz"
	.balign 4, 0
.LField_Map229_SHAPE_GRID:
	.incbin "MAP/MAP_229/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map229_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map229_SCENE:
	.incbin "MAP/MAP_229/SCENE.parts.lz"
