	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map089
Field_Map089:
	map_header 0, 0, 55, 68, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map089, .LField_Map089_CELL_PICTURES
	map_offset Field_Map089, .LField_Map089_SHAPES
	map_offset Field_Map089, .LField_Map089_GRID
	map_offset Field_Map089, .LField_Map089_ANIMATION
	map_offset Field_Map089, .LField_Map089_BLEND
	map_offset Field_Map089, .LField_Map089_MARKERS
.LField_Map089_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_089/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map089_SHAPES:
	.incbin "games/tbs/src/map/map_089/shapes.table.lz"
	.balign 4, 0
.LField_Map089_GRID:
	.incbin "games/tbs/src/map/map_089/map.grid.lz"
	.balign 4, 0
.LField_Map089_ANIMATION:
	.incbin "games/tbs/src/map/map_028/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map089_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map089_MARKERS:
	treasure_spot 21, 9, 100
	treasure_spot 4, 15, 110
	treasure_spots_end
	.balign 4, 0
