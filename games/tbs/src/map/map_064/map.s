	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map064
Field_Map064:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map064, .LField_Map064_CELL_PICTURES
	map_offset Field_Map064, .LField_Map064_SHAPES
	map_offset Field_Map064, .LField_Map064_GRID
	map_offset Field_Map064, .LField_Map064_ANIMATION
	map_offset Field_Map064, .LField_Map064_BLEND
	map_offset Field_Map064, .LField_Map064_MARKERS
.LField_Map064_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_064/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map064_SHAPES:
	.incbin "games/tbs/src/map/map_064/shapes.table.lz"
	.balign 4, 0
.LField_Map064_GRID:
	.incbin "games/tbs/src/map/map_064/map.grid.lz"
	.balign 4, 0
.LField_Map064_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map064_BLEND:
	.incbin "games/tbs/src/map/map_063/blend.script.lz"
	.balign 4, 0
.LField_Map064_MARKERS:
	treasure_spots_end
	.balign 4, 0
