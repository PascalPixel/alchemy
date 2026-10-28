	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map055
Field_Map055:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map055, .LField_Map055_CELL_PICTURES
	map_offset Field_Map055, .LField_Map055_SHAPES
	map_offset Field_Map055, .LField_Map055_GRID
	map_offset Field_Map055, .LField_Map055_ANIMATION
	map_offset Field_Map055, .LField_Map055_BLEND
	map_offset Field_Map055, .LField_Map055_MARKERS
.LField_Map055_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_055/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map055_SHAPES:
	.incbin "games/tbs/src/map/map_055/shapes.table.lz"
	.balign 4, 0
.LField_Map055_GRID:
	.incbin "games/tbs/src/map/map_055/map.grid.lz"
	.balign 4, 0
.LField_Map055_ANIMATION:
	.incbin "games/tbs/src/map/map_054/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map055_BLEND:
	.incbin "games/tbs/src/map/map_054/blend.script.lz"
	.balign 4, 0
.LField_Map055_MARKERS:
	treasure_spot 8, 14, 100
	treasure_spot 72, 14, 100
	treasure_spots_end
	.balign 4, 0
