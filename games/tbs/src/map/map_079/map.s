	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map079
Field_Map079:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map079, .LField_Map079_CELL_PICTURES
	map_offset Field_Map079, .LField_Map079_SHAPES
	map_offset Field_Map079, .LField_Map079_GRID
	map_offset Field_Map079, .LField_Map079_ANIMATION
	map_offset Field_Map079, .LField_Map079_BLEND
	map_offset Field_Map079, .LField_Map079_MARKERS
.LField_Map079_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_079/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map079_SHAPES:
	.incbin "games/tbs/src/map/map_079/shapes.table.lz"
	.balign 4, 0
.LField_Map079_GRID:
	.incbin "games/tbs/src/map/map_079/map.grid.lz"
	.balign 4, 0
.LField_Map079_ANIMATION:
	.incbin "games/tbs/src/map/map_078/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map079_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map079_MARKERS:
	treasure_spot 56, 8, 100
	treasure_spots_end
	.balign 4, 0
