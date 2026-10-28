	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map058
Field_Map058:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map058, .LField_Map058_CELL_PICTURES
	map_offset Field_Map058, .LField_Map058_SHAPES
	map_offset Field_Map058, .LField_Map058_GRID
	map_offset Field_Map058, .LField_Map058_ANIMATION
	map_offset Field_Map058, .LField_Map058_BLEND
	map_offset Field_Map058, .LField_Map058_MARKERS
.LField_Map058_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_058/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map058_SHAPES:
	.incbin "games/tbs/src/map/map_058/shapes.table.lz"
	.balign 4, 0
.LField_Map058_GRID:
	.incbin "games/tbs/src/map/map_058/map.grid.lz"
	.balign 4, 0
.LField_Map058_ANIMATION:
	.incbin "games/tbs/src/map/map_058/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map058_BLEND:
	.incbin "games/tbs/src/map/map_058/blend.script.lz"
	.balign 4, 0
.LField_Map058_MARKERS:
	treasure_spots_end
	.balign 4, 0
