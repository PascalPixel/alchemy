@ Field_Map147: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map147
Field_Map147:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map147, .LField_Map147_METATILES
	map_block Field_Map147, .LField_Map147_SHAPES
	map_block Field_Map147, .LField_Map147_GRID
	map_block Field_Map147, .LField_Map147_SHAPE_GRID
	map_block Field_Map147, .LField_Map147_ANIMATION
	map_block Field_Map147, .LField_Map147_BLEND
	map_block Field_Map147, .LField_Map147_SCENE
.LField_Map147_METATILES:
	.incbin "MAP/MAP_147/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map147_SHAPES:
	.incbin "MAP/MAP_147/SHAPES.table.lz"
	.balign 4, 0
.LField_Map147_GRID:
	.incbin "MAP/MAP_147/GRID.parts.lz"
	.balign 4, 0
.LField_Map147_SHAPE_GRID:
	.incbin "MAP/MAP_147/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map147_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map147_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map147_SCENE:
	.incbin "MAP/MAP_147/SCENE.parts.lz"
