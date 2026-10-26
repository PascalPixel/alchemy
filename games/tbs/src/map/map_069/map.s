	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map069
Field_Map069:
	map_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map069, .LField_Map069_METATILES
	map_offset Field_Map069, .LField_Map069_SHAPES
	map_offset Field_Map069, .LField_Map069_GRID
	map_offset Field_Map069, .LField_Map069_ANIMATION
	map_offset Field_Map069
	map_offset Field_Map069, .LField_Map069_MARKERS
.LField_Map069_METATILES:
	.incbin "games/tbs/src/map/map_069/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map069_SHAPES:
	.incbin "games/tbs/src/map/map_069/shapes.table.lz"
	.balign 4, 0
.LField_Map069_GRID:
	.incbin "games/tbs/src/map/map_069/map.grid.lz"
	.balign 4, 0
.LField_Map069_ANIMATION:
	.incbin "games/tbs/src/map/map_068/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map069_MARKERS:
	treasure_spot 8, 20, 100
	treasure_spots_end
	.balign 4, 0
