	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map070
Field_Map070:
	map_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map070, .LField_Map070_CELL_PICTURES
	map_offset Field_Map070, .LField_Map070_SHAPES
	map_offset Field_Map070, .LField_Map070_GRID
	map_offset Field_Map070, .LField_Map070_ANIMATION
	map_offset Field_Map070
	map_offset Field_Map070, .LField_Map070_MARKERS
.LField_Map070_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_070/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map070_SHAPES:
	.incbin "games/tbs/src/map/map_070/shapes.table.lz"
	.balign 4, 0
.LField_Map070_GRID:
	.incbin "games/tbs/src/map/map_070/map.grid.lz"
	.balign 4, 0
.LField_Map070_ANIMATION:
	.incbin "games/tbs/src/map/map_068/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map070_MARKERS:
	treasure_spot 43, 2, 100
	treasure_spots_end
	.balign 4, 0
