@ Field_Map152: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map152
Field_Map152:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map152, .LField_Map152_METATILES
	map_block Field_Map152, .LField_Map152_SHAPES
	map_block Field_Map152, .LField_Map152_GRID
	map_block Field_Map152, .LField_Map152_SHAPE_GRID
	map_block Field_Map152, .LField_Map152_ANIMATION
	map_block Field_Map152, .LField_Map152_BLEND
	map_block Field_Map152, .LField_Map152_SCENE
.LField_Map152_METATILES:
	.incbin "MAP/MAP_152/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map152_SHAPES:
	.incbin "MAP/MAP_152/SHAPES.table.lz"
	.balign 4, 0
.LField_Map152_GRID:
	.incbin "MAP/MAP_152/GRID.parts.lz"
	.balign 4, 0
.LField_Map152_SHAPE_GRID:
	.incbin "MAP/MAP_152/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map152_ANIMATION:
	.incbin "MAP/MAP_152/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map152_BLEND:
	.incbin "MAP/MAP_152/BLEND.script.lz"
	.balign 4, 0
.LField_Map152_SCENE:
	.incbin "MAP/MAP_152/SCENE.parts.lz"
