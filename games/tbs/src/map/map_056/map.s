	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map056
Field_Map056:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map056, .LField_Map056_METATILES
	map_offset Field_Map056, .LField_Map056_SHAPES
	map_offset Field_Map056, .LField_Map056_GRID
	map_offset Field_Map056, .LField_Map056_ANIMATION
	map_offset Field_Map056, .LField_Map056_BLEND
	map_offset Field_Map056, .LField_Map056_MARKERS
.LField_Map056_METATILES:
	.incbin "games/tbs/src/map/map_056/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map056_SHAPES:
	.incbin "games/tbs/src/map/map_056/shapes.table.lz"
	.balign 4, 0
.LField_Map056_GRID:
	.incbin "games/tbs/src/map/map_056/map.grid.lz"
	.balign 4, 0
.LField_Map056_ANIMATION:
	.incbin "games/tbs/src/map/map_054/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map056_BLEND:
	.incbin "games/tbs/src/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map056_MARKERS:
	treasure_spot 55, 24, 100
	treasure_spot 24, 34, 101
	treasure_spot 39, 39, 102
	treasure_spots_end
