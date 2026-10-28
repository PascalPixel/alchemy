	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map057
Field_Map057:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map057, .LField_Map057_CELL_PICTURES
	map_offset Field_Map057, .LField_Map057_SHAPES
	map_offset Field_Map057, .LField_Map057_GRID
	map_offset Field_Map057, .LField_Map057_ANIMATION
	map_offset Field_Map057, .LField_Map057_BLEND
	map_offset Field_Map057, .LField_Map057_MARKERS
.LField_Map057_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_057/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map057_SHAPES:
	.incbin "games/tbs/src/map/map_057/shapes.table.lz"
	.balign 4, 0
.LField_Map057_GRID:
	.incbin "games/tbs/src/map/map_057/map.grid.lz"
	.balign 4, 0
.LField_Map057_ANIMATION:
	.incbin "games/tbs/src/map/map_054/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map057_BLEND:
	.incbin "games/tbs/src/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map057_MARKERS:
	treasure_spots_end
	.balign 4, 0
