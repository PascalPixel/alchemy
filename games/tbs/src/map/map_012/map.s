	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map012
Field_Map012:
	map_header 0, 0, 104, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 104, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map012, .LField_Map012_CELL_PICTURES
	map_offset Field_Map012, .LField_Map012_SHAPES
	map_offset Field_Map012, .LField_Map012_GRID
	map_offset Field_Map012, .LField_Map012_ANIMATION
	map_offset Field_Map012, .LField_Map012_BLEND
	map_offset Field_Map012, .LField_Map012_MARKERS
.LField_Map012_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_012/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map012_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map012_GRID:
	.incbin "games/tbs/src/map/map_012/map.grid.lz"
	.balign 4, 0
.LField_Map012_ANIMATION:
	.incbin "games/tbs/src/map/map_012/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map012_BLEND:
	.incbin "games/tbs/src/map/map_012/blend.script.lz"
	.balign 4, 0
.LField_Map012_MARKERS:
	treasure_spot 15, 10, 100
	treasure_spots_end
	.balign 4, 0
