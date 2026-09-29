@ Field_Map085: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map085
Field_Map085:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map085, .LField_Map085_METATILES
	map_block Field_Map085, .LField_Map085_SHAPES
	map_block Field_Map085, .LField_Map085_GRID
	map_block Field_Map085, .LField_Map085_SHAPE_GRID
	map_block Field_Map085, .LField_Map085_ANIMATION
	map_block Field_Map085, .LField_Map085_BLEND
	map_block Field_Map085, .LField_Map085_SCENE
.LField_Map085_METATILES:
	.incbin "MAP/MAP_085/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map085_SHAPES:
	.incbin "MAP/MAP_085/SHAPES.table.lz"
	.balign 4, 0
.LField_Map085_GRID:
	.incbin "MAP/MAP_085/GRID.parts.lz"
	.balign 4, 0
.LField_Map085_SHAPE_GRID:
	.incbin "MAP/MAP_085/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map085_ANIMATION:
	.incbin "MAP/MAP_085/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map085_BLEND:
	.incbin "MAP/MAP_085/BLEND.script.lz"
	.balign 4, 0
.LField_Map085_SCENE:
	.incbin "MAP/MAP_085/SCENE.parts.lz"
