@ Field_Map056: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map056
Field_Map056:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map056, .LField_Map056_METATILES
	map_block Field_Map056, .LField_Map056_SHAPES
	map_block Field_Map056, .LField_Map056_GRID
	map_block Field_Map056, .LField_Map056_SHAPE_GRID
	map_block Field_Map056, .LField_Map056_ANIMATION
	map_block Field_Map056, .LField_Map056_BLEND
	map_block Field_Map056, .LField_Map056_SCENE
.LField_Map056_METATILES:
	.incbin "MAP/MAP_056/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map056_SHAPES:
	.incbin "MAP/MAP_056/SHAPES.table.lz"
	.balign 4, 0
.LField_Map056_GRID:
	.incbin "MAP/MAP_056/GRID.parts.lz"
	.balign 4, 0
.LField_Map056_SHAPE_GRID:
	.incbin "MAP/MAP_056/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map056_ANIMATION:
	.incbin "MAP/MAP_056/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map056_BLEND:
	.incbin "MAP/MAP_056/BLEND.script.lz"
	.balign 4, 0
.LField_Map056_SCENE:
	.incbin "MAP/MAP_056/SCENE.parts.lz"
