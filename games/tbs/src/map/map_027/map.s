	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map027
Field_Map027:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map027, .LField_Map027_CELL_PICTURES
	map_offset Field_Map027, .LField_Map027_SHAPES
	map_offset Field_Map027, .LField_Map027_GRID
	map_offset Field_Map027, .LField_Map027_ANIMATION
	map_offset Field_Map027, .LField_Map027_BLEND
	map_offset Field_Map027, .LField_Map027_MARKERS
.LField_Map027_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_027/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map027_SHAPES:
	.incbin "games/tbs/src/map/map_027/shapes.table.lz"
	.balign 4, 0
.LField_Map027_GRID:
	.incbin "games/tbs/src/map/map_027/map.grid.lz"
	.balign 4, 0
.LField_Map027_ANIMATION:
	.incbin "games/tbs/src/map/map_025/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map027_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map027_MARKERS:
	treasure_spots_end
	.balign 4, 0
