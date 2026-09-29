@ Field_Map293: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map293
Field_Map293:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map293, .LField_Map293_METATILES
	map_block Field_Map293, .LField_Map293_SHAPES
	map_block Field_Map293, .LField_Map293_GRID
	map_block Field_Map293, .LField_Map293_SHAPE_GRID
	map_block Field_Map293
	map_block Field_Map293
	map_block Field_Map293, .LField_Map293_SCENE
.LField_Map293_METATILES:
	.incbin "MAP/MAP_293/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map293_SHAPES:
	.incbin "MAP/MAP_293/SHAPES.table.lz"
	.balign 4, 0
.LField_Map293_GRID:
	.incbin "MAP/MAP_293/GRID.parts.lz"
	.balign 4, 0
.LField_Map293_SHAPE_GRID:
	.incbin "MAP/MAP_293/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map293_SCENE:
	.incbin "MAP/MAP_293/SCENE.parts.lz"
