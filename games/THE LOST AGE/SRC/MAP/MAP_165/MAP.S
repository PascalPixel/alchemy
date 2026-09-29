@ Field_Map165: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map165
Field_Map165:
	map_header 0, 0, 128, 128, 2, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 164, 4, 4, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map165, .LField_Map165_METATILES
	map_block Field_Map165, .LField_Map165_SHAPES
	map_block Field_Map165, .LField_Map165_GRID
	map_block Field_Map165, .LField_Map165_SHAPE_GRID
	map_block Field_Map165, .LField_Map165_ANIMATION
	map_block Field_Map165, .LField_Map165_BLEND
	map_block Field_Map165, .LField_Map165_SCENE
.LField_Map165_METATILES:
	.incbin "MAP/MAP_165/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map165_SHAPES:
	.incbin "MAP/MAP_165/SHAPES.table.lz"
	.balign 4, 0
.LField_Map165_GRID:
	.incbin "MAP/MAP_165/GRID.parts.lz"
	.balign 4, 0
.LField_Map165_SHAPE_GRID:
	.incbin "MAP/MAP_165/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map165_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map165_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map165_SCENE:
	.incbin "MAP/MAP_165/SCENE.parts.lz"
