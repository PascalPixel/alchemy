	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map066
Field_Map066:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map066, .LField_Map066_CELL_PICTURES
	map_offset Field_Map066, .LField_Map066_SHAPES
	map_offset Field_Map066, .LField_Map066_GRID
	map_offset Field_Map066, .LField_Map066_ANIMATION
	map_offset Field_Map066, .LField_Map066_BLEND
	map_offset Field_Map066, .LField_Map066_MARKERS
.LField_Map066_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_066/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map066_SHAPES:
	.incbin "games/tbs/src/map/map_066/shapes.table.lz"
	.balign 4, 0
.LField_Map066_GRID:
	.incbin "games/tbs/src/map/map_066/map.grid.lz"
	.balign 4, 0
.LField_Map066_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map066_BLEND:
	.incbin "games/tbs/src/map/map_063/blend.script.lz"
	.balign 4, 0
.LField_Map066_MARKERS:
	treasure_spot 17, 5, 100
	treasure_spots_end
	.balign 4, 0
