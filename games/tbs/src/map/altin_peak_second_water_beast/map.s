	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_AltinPeakSecondWaterBeast
Field_AltinPeakSecondWaterBeast:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltinPeakSecondWaterBeast, .LField_AltinPeakSecondWaterBeast_CELL_PICTURES
	map_offset Field_AltinPeakSecondWaterBeast, .LField_AltinPeakSecondWaterBeast_SHAPES
	map_offset Field_AltinPeakSecondWaterBeast, .LField_AltinPeakSecondWaterBeast_GRID
	map_offset Field_AltinPeakSecondWaterBeast, .LField_AltinPeakSecondWaterBeast_ANIMATION
	map_offset Field_AltinPeakSecondWaterBeast, .LField_AltinPeakSecondWaterBeast_BLEND
	map_offset Field_AltinPeakSecondWaterBeast, .LField_AltinPeakSecondWaterBeast_MARKERS
.LField_AltinPeakSecondWaterBeast_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin_peak_second_water_beast/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AltinPeakSecondWaterBeast_SHAPES:
	.incbin "games/tbs/src/map/altin_peak_second_water_beast/shapes.table.lz"
	.balign 4, 0
.LField_AltinPeakSecondWaterBeast_GRID:
	.incbin "games/tbs/src/map/altin_peak_second_water_beast/map.grid.lz"
	.balign 4, 0
.LField_AltinPeakSecondWaterBeast_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_rail_tunnel/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltinPeakSecondWaterBeast_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_AltinPeakSecondWaterBeast_MARKERS:
	treasure_spot 56, 8, 100
	treasure_spots_end
	.balign 4, 0
