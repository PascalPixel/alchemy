@ Field_Map294: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map294
Field_Map294:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map294, .LField_Map294_METATILES
	map_block Field_Map294, .LField_Map294_SHAPES
	map_block Field_Map294, .LField_Map294_GRID
	map_block Field_Map294, .LField_Map294_SHAPE_GRID
	map_block Field_Map294
	map_block Field_Map294
	map_block Field_Map294, .LField_Map294_SCENE
.LField_Map294_METATILES:
	.incbin "MAP/MAP_294/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map294_SHAPES:
	.incbin "MAP/MAP_294/SHAPES.table.lz"
	.balign 4, 0
.LField_Map294_GRID:
	.incbin "MAP/MAP_294/GRID.parts.lz"
	.balign 4, 0
.LField_Map294_SHAPE_GRID:
	.incbin "MAP/MAP_294/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map294_SCENE:
	.incbin "MAP/MAP_294/SCENE.parts.lz"
