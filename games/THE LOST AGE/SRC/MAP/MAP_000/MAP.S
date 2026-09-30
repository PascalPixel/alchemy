@ Field_Map000: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map000
Field_Map000:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map000, .LField_Map000_METATILES
	map_block Field_Map000, .LField_Map000_SHAPES
	map_block Field_Map000, .LField_Map000_GRID
	map_block Field_Map000, .LField_Map000_SHAPE_GRID
	map_block Field_Map000
	map_block Field_Map000
	map_block Field_Map000, .LField_Map000_SCENE
.LField_Map000_METATILES:
	.incbin "COMMON/MAP/MAP_001/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map000_SHAPES:
	.incbin "COMMON/MAP/MAP_001/SHAPES.table.lz"
	.balign 4, 0
.LField_Map000_GRID:
	.incbin "MAP/MAP_000/GRID.parts.lz"
	.balign 4, 0
.LField_Map000_SHAPE_GRID:
	.incbin "MAP/MAP_000/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map000_SCENE:
	.incbin "MAP/MAP_000/SCENE.parts.lz"
