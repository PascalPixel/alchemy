	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_AltinPeakFallingRocks
Field_AltinPeakFallingRocks:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltinPeakFallingRocks, .LField_AltinPeakFallingRocks_CELL_PICTURES
	map_offset Field_AltinPeakFallingRocks, .LField_AltinPeakFallingRocks_SHAPES
	map_offset Field_AltinPeakFallingRocks, .LField_AltinPeakFallingRocks_GRID
	map_offset Field_AltinPeakFallingRocks, .LField_AltinPeakFallingRocks_ANIMATION
	map_offset Field_AltinPeakFallingRocks, .LField_AltinPeakFallingRocks_BLEND
	map_offset Field_AltinPeakFallingRocks, .LField_AltinPeakFallingRocks_MARKERS
.LField_AltinPeakFallingRocks_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin_peak_falling_rocks/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AltinPeakFallingRocks_SHAPES:
	.incbin "games/tbs/src/map/altin_peak_falling_rocks/shapes.table.lz"
	.balign 4, 0
.LField_AltinPeakFallingRocks_GRID:
	.incbin "games/tbs/src/map/altin_peak_falling_rocks/map.grid.lz"
	.balign 4, 0
.LField_AltinPeakFallingRocks_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_south_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltinPeakFallingRocks_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_AltinPeakFallingRocks_MARKERS:
	treasure_spots_end
	.balign 4, 0
