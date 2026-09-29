@ Field_Map296: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map296
Field_Map296:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map296, .LField_Map296_METATILES
	map_block Field_Map296, .LField_Map296_SHAPES
	map_block Field_Map296, .LField_Map296_GRID
	map_block Field_Map296, .LField_Map296_SHAPE_GRID
	map_block Field_Map296
	map_block Field_Map296
	map_block Field_Map296, .LField_Map296_SCENE
.LField_Map296_METATILES:
	.incbin "MAP/MAP_000/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map296_SHAPES:
	.incbin "MAP/MAP_099/SHAPES.table.lz"
	.balign 4, 0
.LField_Map296_GRID:
	.incbin "MAP/MAP_296/GRID.parts.lz"
	.balign 4, 0
.LField_Map296_SHAPE_GRID:
	.incbin "MAP/MAP_099/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map296_SCENE:
	.incbin "MAP/MAP_000/SCENE.parts.lz"
