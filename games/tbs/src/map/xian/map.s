	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Xian
Field_Xian:
	map_header 10, 4, 67, 67, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Xian, .LField_Xian_CELL_PICTURES
	map_offset Field_Xian, .LField_Xian_SHAPES
	map_offset Field_Xian, .LField_Xian_GRID
	map_offset Field_Xian, .LField_Xian_ANIMATION
	map_offset Field_Xian, .LField_Xian_BLEND
	map_offset Field_Xian, .LField_Xian_MARKERS
.LField_Xian_CELL_PICTURES:
	.incbin "games/tbs/src/map/xian/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Xian_SHAPES:
	.incbin "games/tbs/src/map/xian/shapes.table.lz"
	.balign 4, 0
.LField_Xian_GRID:
	.incbin "games/tbs/src/map/xian/map.grid.lz"
	.balign 4, 0
.LField_Xian_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_mouth/tileset_anims.script.lz"
	.balign 4, 0
.LField_Xian_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_Xian_MARKERS:
	treasure_spot 27, 20, 100
	treasure_spot 10, 22, 101
	treasure_spots_end
	.balign 4, 0
