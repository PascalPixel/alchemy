	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map095
Field_Map095:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map095, .LField_Map095_CELL_PICTURES
	map_offset Field_Map095, .LField_Map095_SHAPES
	map_offset Field_Map095, .LField_Map095_GRID
	map_offset Field_Map095, .LField_Map095_ANIMATION
	map_offset Field_Map095, .LField_Map095_BLEND
	map_offset Field_Map095, .LField_Map095_MARKERS
.LField_Map095_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_095/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map095_SHAPES:
	.incbin "games/tbs/src/map/map_095/shapes.table.lz"
	.balign 4, 0
.LField_Map095_GRID:
	.incbin "games/tbs/src/map/map_095/map.grid.lz"
	.balign 4, 0
.LField_Map095_ANIMATION:
	.incbin "games/tbs/src/map/map_094/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map095_BLEND:
	.incbin "games/tbs/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map095_MARKERS:
	treasure_spots_end
	.balign 4, 0
