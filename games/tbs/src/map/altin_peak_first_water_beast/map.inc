	.global Field_AltinPeakFirstWaterBeast
Field_AltinPeakFirstWaterBeast:
	map_header 0, 0, 64, 68, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltinPeakFirstWaterBeast, .LField_AltinPeakFirstWaterBeast_CELL_PICTURES
	map_offset Field_AltinPeakFirstWaterBeast, .LField_AltinPeakFirstWaterBeast_SHAPES
	map_offset Field_AltinPeakFirstWaterBeast, .LField_AltinPeakFirstWaterBeast_GRID
	map_offset Field_AltinPeakFirstWaterBeast, .LField_AltinPeakFirstWaterBeast_ANIMATION
	map_offset Field_AltinPeakFirstWaterBeast, .LField_AltinPeakFirstWaterBeast_BLEND
	map_offset Field_AltinPeakFirstWaterBeast, .LField_AltinPeakFirstWaterBeast_MARKERS
.LField_AltinPeakFirstWaterBeast_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin_peak_first_water_beast/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AltinPeakFirstWaterBeast_SHAPES:
	.incbin "games/tbs/src/map/altin_peak_first_water_beast/shapes.table.lz"
	.balign 4, 0
.LField_AltinPeakFirstWaterBeast_GRID:
	.incbin "games/tbs/src/map/altin_peak_first_water_beast/map.grid.lz"
	.balign 4, 0
.LField_AltinPeakFirstWaterBeast_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_first_water_beast/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltinPeakFirstWaterBeast_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_AltinPeakFirstWaterBeast_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteAltinPeak
Tileset_PaletteAltinPeak:
	.incbin "games/tbs/src/graphics/tileset/palettes/altin_peak.gbapal.lz"
	.byte 0x5d, 0x02, 0x06	@ PACKER: left in the buffer by Field_AltinPeakFirstWaterBeast+0x1b1
	.balign 4, 0
