	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map131
Field_Map131:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map131, .LField_Map131_CELL_PICTURES
	map_offset Field_Map131, .LField_Map131_SHAPES
	map_offset Field_Map131, .LField_Map131_GRID
	map_offset Field_Map131, .LField_Map131_ANIMATION
	map_offset Field_Map131, .LField_Map131_BLEND
	map_offset Field_Map131, .LField_Map131_MARKERS
.LField_Map131_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_131/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map131_SHAPES:
	.incbin "games/tbs/src/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map131_GRID:
	.incbin "games/tbs/src/map/map_131/map.grid.lz"
	.balign 4, 0
.LField_Map131_ANIMATION:
	.incbin "games/tbs/src/map/map_131/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map131_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map131_MARKERS:
	treasure_spot 28, 36, 100
	treasure_spots_end
	.balign 4, 0
