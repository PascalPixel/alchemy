@ Field_Map299: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map299
Field_Map299:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map299, .LField_Map299_METATILES
	map_block Field_Map299, .LField_Map299_SHAPES
	map_block Field_Map299, .LField_Map299_GRID
	map_block Field_Map299, .LField_Map299_SHAPE_GRID
	map_block Field_Map299
	map_block Field_Map299
	map_block Field_Map299, .LField_Map299_SCENE
.LField_Map299_METATILES:
	.incbin "MAP/MAP_299/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map299_SHAPES:
	.incbin "MAP/MAP_299/SHAPES.table.lz"
	.balign 4, 0
.LField_Map299_GRID:
	.incbin "MAP/MAP_299/GRID.parts.lz"
	.balign 4, 0
.LField_Map299_SHAPE_GRID:
	.incbin "MAP/MAP_299/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map299_SCENE:
	.incbin "MAP/MAP_299/SCENE.parts.lz"
