	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map087
Field_Map087:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map087, .LField_Map087_CELL_PICTURES
	map_offset Field_Map087, .LField_Map087_SHAPES
	map_offset Field_Map087, .LField_Map087_GRID
	map_offset Field_Map087, .LField_Map087_ANIMATION
	map_offset Field_Map087, .LField_Map087_BLEND
	map_offset Field_Map087, .LField_Map087_MARKERS
.LField_Map087_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_087/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map087_SHAPES:
	.incbin "games/tbs/src/map/map_087/shapes.table.lz"
	.balign 4, 0
.LField_Map087_GRID:
	.incbin "games/tbs/src/map/map_087/map.grid.lz"
	.balign 4, 0
.LField_Map087_ANIMATION:
	.incbin "games/tbs/src/map/map_080/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map087_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map087_MARKERS:
	treasure_spot 35, 5, 100
	treasure_spots_end
	.balign 4, 0
