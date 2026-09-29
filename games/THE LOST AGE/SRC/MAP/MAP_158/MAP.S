@ Field_Map158: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map158
Field_Map158:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map158, .LField_Map158_METATILES
	map_block Field_Map158, .LField_Map158_SHAPES
	map_block Field_Map158, .LField_Map158_GRID
	map_block Field_Map158, .LField_Map158_SHAPE_GRID
	map_block Field_Map158, .LField_Map158_ANIMATION
	map_block Field_Map158, .LField_Map158_BLEND
	map_block Field_Map158, .LField_Map158_SCENE
.LField_Map158_METATILES:
	.incbin "MAP/MAP_158/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map158_SHAPES:
	.incbin "MAP/MAP_158/SHAPES.table.lz"
	.balign 4, 0
.LField_Map158_GRID:
	.incbin "MAP/MAP_158/GRID.parts.lz"
	.balign 4, 0
.LField_Map158_SHAPE_GRID:
	.incbin "MAP/MAP_158/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map158_ANIMATION:
	.incbin "MAP/MAP_158/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map158_BLEND:
	.incbin "MAP/MAP_158/BLEND.script.lz"
	.balign 4, 0
.LField_Map158_SCENE:
	.incbin "MAP/MAP_158/SCENE.parts.lz"
