	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map065
Field_Map065:
	map_header 0, 0, 72, 106, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 106, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map065, .LField_Map065_METATILES
	map_offset Field_Map065, .LField_Map065_SHAPES
	map_offset Field_Map065, .LField_Map065_GRID
	map_offset Field_Map065, .LField_Map065_ANIMATION
	map_offset Field_Map065, .LField_Map065_BLEND
	map_offset Field_Map065, .LField_Map065_MARKERS
.LField_Map065_METATILES:
	.incbin "games/tbs/src/map/map_065/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map065_SHAPES:
	.incbin "games/tbs/src/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map065_GRID:
	.incbin "games/tbs/src/map/map_065/map.grid.lz"
	.balign 4, 0
.LField_Map065_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map065_BLEND:
	.incbin "games/tbs/src/map/map_063/blend.script.lz"
	.balign 4, 0
.LField_Map065_MARKERS:
	treasure_spot 11, 8, 100
	treasure_spots_end
	.balign 4, 0
