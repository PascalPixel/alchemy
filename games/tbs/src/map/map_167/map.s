	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map167
Field_Map167:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map167, .LField_Map167_METATILES
	map_offset Field_Map167, .LField_Map167_SHAPES
	map_offset Field_Map167, .LField_Map167_GRID
	map_offset Field_Map167, .LField_Map167_ANIMATION
	map_offset Field_Map167, .LField_Map167_BLEND
	map_offset Field_Map167, .LField_Map167_MARKERS
.LField_Map167_METATILES:
	.incbin "games/tbs/src/map/map_167/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map167_SHAPES:
	.incbin "games/tbs/src/map/map_167/shapes.table.lz"
	.balign 4, 0
.LField_Map167_GRID:
	.incbin "games/tbs/src/map/map_167/map.grid.lz"
	.balign 4, 0
.LField_Map167_ANIMATION:
	.incbin "games/tbs/src/map/map_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map167_BLEND:
	.incbin "games/tbs/src/map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map167_MARKERS:
	treasure_spot 27, 47, 110
	treasure_spot 57, 49, 100
	treasure_spots_end
	.balign 4, 0
