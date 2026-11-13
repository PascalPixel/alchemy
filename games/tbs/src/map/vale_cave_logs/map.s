	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_ValeCaveLogs
Field_ValeCaveLogs:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ValeCaveLogs, .LField_ValeCaveLogs_CELL_PICTURES
	map_offset Field_ValeCaveLogs, .LField_ValeCaveLogs_SHAPES
	map_offset Field_ValeCaveLogs, .LField_ValeCaveLogs_GRID
	map_offset Field_ValeCaveLogs, .LField_ValeCaveLogs_ANIMATION
	map_offset Field_ValeCaveLogs
	map_offset Field_ValeCaveLogs, .LField_ValeCaveLogs_MARKERS
.LField_ValeCaveLogs_CELL_PICTURES:
	.incbin "games/tbs/src/map/vale_cave_logs/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ValeCaveLogs_SHAPES:
	.incbin "games/tbs/src/map/vale_cave_logs/shapes.table.lz"
	.balign 4, 0
.LField_ValeCaveLogs_GRID:
	.incbin "games/tbs/src/map/vale_cave_logs/map.grid.lz"
	.balign 4, 0
.LField_ValeCaveLogs_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_kite/tileset_anims.script.lz"
	.balign 4, 0
.LField_ValeCaveLogs_MARKERS:
	treasure_spots_end
	.balign 4, 0
