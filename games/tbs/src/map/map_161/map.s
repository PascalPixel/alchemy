	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map161
Field_Map161:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map161, .LField_Map161_CELL_PICTURES
	map_offset Field_Map161, .LField_Map161_SHAPES
	map_offset Field_Map161, .LField_Map161_GRID
	map_offset Field_Map161, .LField_Map161_ANIMATION
	map_offset Field_Map161, .LField_Map161_BLEND
	map_offset Field_Map161, .LField_Map161_MARKERS
.LField_Map161_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_161/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map161_SHAPES:
	.incbin "games/tbs/src/map/map_161/shapes.table.lz"
	.balign 4, 0
.LField_Map161_GRID:
	.incbin "games/tbs/src/map/map_161/map.grid.lz"
	.balign 4, 0
.LField_Map161_ANIMATION:
	.incbin "games/tbs/src/map/map_161/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map161_BLEND:
	.incbin "games/tbs/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map161_MARKERS:
	treasure_spot 4, 3, 203
	treasure_spot 5, 3, 203
	treasure_spot 31, 3, 102
	treasure_spot 31, 4, 102
	treasure_spot 7, 17, 205
	treasure_spot 8, 17, 205
	treasure_spot 9, 17, 204
	treasure_spot 10, 17, 204
	treasure_spots_end
	.balign 4, 0
