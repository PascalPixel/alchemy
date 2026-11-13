	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_AltinPeakThirdWaterBeast
Field_AltinPeakThirdWaterBeast:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltinPeakThirdWaterBeast, .LField_AltinPeakThirdWaterBeast_CELL_PICTURES
	map_offset Field_AltinPeakThirdWaterBeast, .LField_AltinPeakThirdWaterBeast_SHAPES
	map_offset Field_AltinPeakThirdWaterBeast, .LField_AltinPeakThirdWaterBeast_GRID
	map_offset Field_AltinPeakThirdWaterBeast, .LField_AltinPeakThirdWaterBeast_ANIMATION
	map_offset Field_AltinPeakThirdWaterBeast, .LField_AltinPeakThirdWaterBeast_BLEND
	map_offset Field_AltinPeakThirdWaterBeast, .LField_AltinPeakThirdWaterBeast_MARKERS
.LField_AltinPeakThirdWaterBeast_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin_peak_third_water_beast/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltinPeakThirdWaterBeast_SHAPES:
	.incbin "games/tbs/src/map/altin_peak_third_water_beast/shapes.table.lz"
	.balign 4, 0
.LField_AltinPeakThirdWaterBeast_GRID:
	.incbin "games/tbs/src/map/altin_peak_third_water_beast/map.grid.lz"
	.balign 4, 0
.LField_AltinPeakThirdWaterBeast_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_rail_tunnel/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltinPeakThirdWaterBeast_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_AltinPeakThirdWaterBeast_MARKERS:
	treasure_spot 16, 7, 101
	treasure_spot 29, 25, 100
	treasure_spots_end
	.balign 4, 0
