@ Field_Map045: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map045
Field_Map045:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map045, .LField_Map045_METATILES
	map_block Field_Map045, .LField_Map045_SHAPES
	map_block Field_Map045, .LField_Map045_GRID
	map_block Field_Map045, .LField_Map045_SHAPE_GRID
	map_block Field_Map045, .LField_Map045_ANIMATION
	map_block Field_Map045, .LField_Map045_BLEND
	map_block Field_Map045, .LField_Map045_SCENE
.LField_Map045_METATILES:
	.incbin "MAP/MAP_045/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map045_SHAPES:
	.incbin "MAP/MAP_045/SHAPES.table.lz"
	.balign 4, 0
.LField_Map045_GRID:
	.incbin "MAP/MAP_045/GRID.parts.lz"
	.balign 4, 0
.LField_Map045_SHAPE_GRID:
	.incbin "MAP/MAP_045/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map045_ANIMATION:
	.incbin "MAP/MAP_045/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map045_BLEND:
	.incbin "MAP/MAP_045/BLEND.script.lz"
	.balign 4, 0
.LField_Map045_SCENE:
	.incbin "MAP/MAP_045/SCENE.parts.lz"
