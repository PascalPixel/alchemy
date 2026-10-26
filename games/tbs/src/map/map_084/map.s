	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map084
Field_Map084:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map084, .LField_Map084_METATILES
	map_offset Field_Map084, .LField_Map084_SHAPES
	map_offset Field_Map084, .LField_Map084_GRID
	map_offset Field_Map084, .LField_Map084_ANIMATION
	map_offset Field_Map084, .LField_Map084_BLEND
	map_offset Field_Map084, .LField_Map084_MARKERS
.LField_Map084_METATILES:
	.incbin "games/tbs/src/map/map_084/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map084_SHAPES:
	.incbin "games/tbs/src/map/map_084/shapes.table.lz"
	.balign 4, 0
.LField_Map084_GRID:
	.incbin "games/tbs/src/map/map_084/map.grid.lz"
	.balign 4, 0
.LField_Map084_ANIMATION:
	.incbin "games/tbs/src/map/map_080/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map084_BLEND:
	.incbin "games/tbs/src/map/map_025/blend.script.lz"
	.balign 4, 0
.LField_Map084_MARKERS:
	treasure_spot 6, 27, 100
	treasure_spots_end
	.balign 4, 0
