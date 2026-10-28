	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map072
Field_Map072:
	map_header 10, 4, 67, 67, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map072, .LField_Map072_CELL_PICTURES
	map_offset Field_Map072, .LField_Map072_SHAPES
	map_offset Field_Map072, .LField_Map072_GRID
	map_offset Field_Map072, .LField_Map072_ANIMATION
	map_offset Field_Map072, .LField_Map072_BLEND
	map_offset Field_Map072, .LField_Map072_MARKERS
.LField_Map072_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_072/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map072_SHAPES:
	.incbin "games/tbs/src/map/map_072/shapes.table.lz"
	.balign 4, 0
.LField_Map072_GRID:
	.incbin "games/tbs/src/map/map_072/map.grid.lz"
	.balign 4, 0
.LField_Map072_ANIMATION:
	.incbin "games/tbs/src/map/map_028/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map072_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map072_MARKERS:
	treasure_spot 27, 20, 100
	treasure_spot 10, 22, 101
	treasure_spots_end
	.balign 4, 0
