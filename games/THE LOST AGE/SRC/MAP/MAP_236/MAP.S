@ Field_Map236: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map236
Field_Map236:
	map_header 0, 0, 128, 64, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map236, .LField_Map236_METATILES
	map_block Field_Map236, .LField_Map236_SHAPES
	map_block Field_Map236, .LField_Map236_GRID
	map_block Field_Map236, .LField_Map236_SHAPE_GRID
	map_block Field_Map236, .LField_Map236_ANIMATION
	map_block Field_Map236, .LField_Map236_BLEND
	map_block Field_Map236, .LField_Map236_SCENE
.LField_Map236_METATILES:
	.incbin "MAP/MAP_236/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map236_SHAPES:
	.incbin "MAP/MAP_236/SHAPES.table.lz"
	.balign 4, 0
.LField_Map236_GRID:
	.incbin "MAP/MAP_236/GRID.parts.lz"
	.balign 4, 0
.LField_Map236_SHAPE_GRID:
	.incbin "MAP/MAP_236/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map236_ANIMATION:
	.incbin "MAP/MAP_236/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map236_BLEND:
	.incbin "MAP/MAP_236/BLEND.script.lz"
	.balign 4, 0
.LField_Map236_SCENE:
	.incbin "MAP/MAP_236/SCENE.parts.lz"
