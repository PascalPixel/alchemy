@ Field_Map189: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map189
Field_Map189:
	map_header 0, 0, 128, 160, 1, 1, 3, 1, 2, 1, 0, 0
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 3, 3, 0x1f, 0x1f
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map189, .LField_Map189_METATILES
	map_block Field_Map189, .LField_Map189_SHAPES
	map_block Field_Map189, .LField_Map189_GRID
	map_block Field_Map189, .LField_Map189_SHAPE_GRID
	map_block Field_Map189, .LField_Map189_ANIMATION
	map_block Field_Map189, .LField_Map189_BLEND
	map_block Field_Map189, .LField_Map189_SCENE
.LField_Map189_METATILES:
	.incbin "MAP/MAP_189/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map189_SHAPES:
	.incbin "MAP/MAP_189/SHAPES.table.lz"
	.balign 4, 0
.LField_Map189_GRID:
	.incbin "MAP/MAP_189/GRID.parts.lz"
	.balign 4, 0
.LField_Map189_SHAPE_GRID:
	.incbin "MAP/MAP_189/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map189_ANIMATION:
	.incbin "MAP/MAP_189/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map189_BLEND:
	.incbin "MAP/MAP_189/BLEND.script.lz"
	.balign 4, 0
.LField_Map189_SCENE:
	.incbin "MAP/MAP_189/SCENE.parts.lz"
