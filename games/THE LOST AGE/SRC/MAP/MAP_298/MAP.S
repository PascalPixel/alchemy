@ Field_Map298: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map298
Field_Map298:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map298, .LField_Map298_METATILES
	map_block Field_Map298, .LField_Map298_SHAPES
	map_block Field_Map298, .LField_Map298_GRID
	map_block Field_Map298, .LField_Map298_SHAPE_GRID
	map_block Field_Map298
	map_block Field_Map298
	map_block Field_Map298, .LField_Map298_SCENE
.LField_Map298_METATILES:
	.incbin "COMMON/MAP/MAP_001/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map298_SHAPES:
	.incbin "MAP/MAP_231/SHAPES.table.lz"
	.balign 4, 0
.LField_Map298_GRID:
	.incbin "MAP/MAP_298/GRID.parts.lz"
	.balign 4, 0
.LField_Map298_SHAPE_GRID:
	.incbin "MAP/MAP_231/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map298_SCENE:
	.incbin "MAP/MAP_000/SCENE.parts.lz"
