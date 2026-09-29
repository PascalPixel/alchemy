@ Field_Map244: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map244
Field_Map244:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map244, .LField_Map244_METATILES
	map_block Field_Map244, .LField_Map244_SHAPES
	map_block Field_Map244, .LField_Map244_GRID
	map_block Field_Map244, .LField_Map244_SHAPE_GRID
	map_block Field_Map244, .LField_Map244_ANIMATION
	map_block Field_Map244, .LField_Map244_BLEND
	map_block Field_Map244, .LField_Map244_SCENE
.LField_Map244_METATILES:
	.incbin "MAP/MAP_244/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map244_SHAPES:
	.incbin "MAP/MAP_244/SHAPES.table.lz"
	.balign 4, 0
.LField_Map244_GRID:
	.incbin "MAP/MAP_244/GRID.parts.lz"
	.balign 4, 0
.LField_Map244_SHAPE_GRID:
	.incbin "MAP/MAP_244/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map244_ANIMATION:
	.incbin "MAP/MAP_244/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map244_BLEND:
	.incbin "MAP/MAP_244/BLEND.script.lz"
	.balign 4, 0
.LField_Map244_SCENE:
	.incbin "MAP/MAP_244/SCENE.parts.lz"
