	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map137
Field_Map137:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map137, .LField_Map137_CELL_PICTURES
	map_offset Field_Map137, .LField_Map137_SHAPES
	map_offset Field_Map137, .LField_Map137_GRID
	map_offset Field_Map137, .LField_Map137_ANIMATION
	map_offset Field_Map137, .LField_Map137_BLEND
	map_offset Field_Map137, .LField_Map137_MARKERS
.LField_Map137_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_137/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map137_SHAPES:
	.incbin "games/tbs/src/map/map_137/shapes.table.lz"
	.balign 4, 0
.LField_Map137_GRID:
	.incbin "games/tbs/src/map/map_137/map.grid.lz"
	.balign 4, 0
.LField_Map137_ANIMATION:
	.incbin "games/tbs/src/map/map_022/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map137_BLEND:
	.incbin "games/tbs/src/map/map_022/blend.script.lz"
	.balign 4, 0
.LField_Map137_MARKERS:
	treasure_spots_end
	.balign 4, 0
