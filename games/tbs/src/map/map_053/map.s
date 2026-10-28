	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map053
Field_Map053:
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 70, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map053, .LField_Map053_CELL_PICTURES
	map_offset Field_Map053, .LField_Map053_SHAPES
	map_offset Field_Map053, .LField_Map053_GRID
	map_offset Field_Map053, .LField_Map053_ANIMATION
	map_offset Field_Map053, .LField_Map053_BLEND
	map_offset Field_Map053, .LField_Map053_MARKERS
.LField_Map053_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_053/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map053_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map053_GRID:
	.incbin "games/tbs/src/map/map_053/map.grid.lz"
	.balign 4, 0
.LField_Map053_ANIMATION:
	.incbin "games/tbs/src/map/map_053/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map053_BLEND:
	.incbin "games/tbs/src/map/map_053/blend.script.lz"
	.balign 4, 0
.LField_Map053_MARKERS:
	treasure_spots_end
	.balign 4, 0
