@ Field_Map226: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map226
Field_Map226:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map226, .LField_Map226_METATILES
	map_block Field_Map226, .LField_Map226_SHAPES
	map_block Field_Map226, .LField_Map226_GRID
	map_block Field_Map226, .LField_Map226_SHAPE_GRID
	map_block Field_Map226, .LField_Map226_ANIMATION
	map_block Field_Map226, .LField_Map226_BLEND
	map_block Field_Map226, .LField_Map226_SCENE
.LField_Map226_METATILES:
	.incbin "MAP/MAP_226/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map226_SHAPES:
	.incbin "MAP/MAP_226/SHAPES.table.lz"
	.balign 4, 0
.LField_Map226_GRID:
	.incbin "MAP/MAP_226/GRID.parts.lz"
	.balign 4, 0
.LField_Map226_SHAPE_GRID:
	.incbin "MAP/MAP_226/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map226_ANIMATION:
	.incbin "MAP/MAP_226/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map226_BLEND:
	.incbin "MAP/MAP_010/BLEND.script.lz"
	.balign 4, 0
.LField_Map226_SCENE:
	.incbin "MAP/MAP_226/SCENE.parts.lz"
