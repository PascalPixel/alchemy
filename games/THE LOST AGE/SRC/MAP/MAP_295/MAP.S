@ Field_Map295: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map295
Field_Map295:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map295, .LField_Map295_METATILES
	map_block Field_Map295, .LField_Map295_SHAPES
	map_block Field_Map295, .LField_Map295_GRID
	map_block Field_Map295, .LField_Map295_SHAPE_GRID
	map_block Field_Map295
	map_block Field_Map295
	map_block Field_Map295, .LField_Map295_SCENE
.LField_Map295_METATILES:
	.incbin "COMMON/MAP/MAP_001/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map295_SHAPES:
	.incbin "MAP/MAP_018/SHAPES.table.lz"
	.balign 4, 0
.LField_Map295_GRID:
	.incbin "MAP/MAP_295/GRID.parts.lz"
	.balign 4, 0
.LField_Map295_SHAPE_GRID:
	.incbin "MAP/MAP_018/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map295_SCENE:
	.incbin "MAP/MAP_000/SCENE.parts.lz"
