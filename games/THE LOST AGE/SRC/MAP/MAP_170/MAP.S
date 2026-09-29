@ Field_Map170: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map170
Field_Map170:
	map_header 0, 0, 130, 112, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map170, .LField_Map170_METATILES
	map_block Field_Map170, .LField_Map170_SHAPES
	map_block Field_Map170, .LField_Map170_GRID
	map_block Field_Map170, .LField_Map170_SHAPE_GRID
	map_block Field_Map170, .LField_Map170_ANIMATION
	map_block Field_Map170, .LField_Map170_BLEND
	map_block Field_Map170, .LField_Map170_SCENE
.LField_Map170_METATILES:
	.incbin "MAP/MAP_170/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map170_SHAPES:
	.incbin "MAP/MAP_170/SHAPES.table.lz"
	.balign 4, 0
.LField_Map170_GRID:
	.incbin "MAP/MAP_170/GRID.parts.lz"
	.balign 4, 0
.LField_Map170_SHAPE_GRID:
	.incbin "MAP/MAP_170/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map170_ANIMATION:
	.incbin "MAP/MAP_170/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map170_BLEND:
	.incbin "MAP/MAP_170/BLEND.script.lz"
	.balign 4, 0
.LField_Map170_SCENE:
	.incbin "MAP/MAP_170/SCENE.parts.lz"
