	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map141
Field_Map141:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map141, .LField_Map141_CELL_PICTURES
	map_offset Field_Map141, .LField_Map141_SHAPES
	map_offset Field_Map141, .LField_Map141_GRID
	map_offset Field_Map141, .LField_Map141_ANIMATION
	map_offset Field_Map141, .LField_Map141_BLEND
	map_offset Field_Map141, .LField_Map141_MARKERS
.LField_Map141_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_141/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map141_SHAPES:
	.incbin "games/tbs/src/map/map_141/shapes.table.lz"
	.balign 4, 0
.LField_Map141_GRID:
	.incbin "games/tbs/src/map/map_141/map.grid.lz"
	.balign 4, 0
.LField_Map141_ANIMATION:
	.incbin "games/tbs/src/map/map_029/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map141_BLEND:
	.incbin "games/tbs/src/map/map_139/blend.script.lz"
	.balign 4, 0
.LField_Map141_MARKERS:
	treasure_spots_end
	.balign 4, 0
