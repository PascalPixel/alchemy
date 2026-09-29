@ Field_Map148: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map148
Field_Map148:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map148, .LField_Map148_METATILES
	map_block Field_Map148, .LField_Map148_SHAPES
	map_block Field_Map148, .LField_Map148_GRID
	map_block Field_Map148, .LField_Map148_SHAPE_GRID
	map_block Field_Map148, .LField_Map148_ANIMATION
	map_block Field_Map148, .LField_Map148_BLEND
	map_block Field_Map148, .LField_Map148_SCENE
.LField_Map148_METATILES:
	.incbin "MAP/MAP_148/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map148_SHAPES:
	.incbin "MAP/MAP_148/SHAPES.table.lz"
	.balign 4, 0
.LField_Map148_GRID:
	.incbin "MAP/MAP_148/GRID.parts.lz"
	.balign 4, 0
.LField_Map148_SHAPE_GRID:
	.incbin "MAP/MAP_148/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map148_ANIMATION:
	.incbin "MAP/MAP_148/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map148_BLEND:
	.incbin "MAP/MAP_148/BLEND.script.lz"
	.balign 4, 0
.LField_Map148_SCENE:
	.incbin "MAP/MAP_148/SCENE.parts.lz"
