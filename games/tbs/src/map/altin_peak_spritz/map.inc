	.global Field_AltinPeakSpritz
Field_AltinPeakSpritz:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltinPeakSpritz, .LField_AltinPeakSpritz_CELL_PICTURES
	map_offset Field_AltinPeakSpritz, .LField_AltinPeakSpritz_SHAPES
	map_offset Field_AltinPeakSpritz, .LField_AltinPeakSpritz_GRID
	map_offset Field_AltinPeakSpritz, .LField_AltinPeakSpritz_ANIMATION
	map_offset Field_AltinPeakSpritz, .LField_AltinPeakSpritz_BLEND
	map_offset Field_AltinPeakSpritz, .LField_AltinPeakSpritz_MARKERS
.LField_AltinPeakSpritz_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin_peak_spritz/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AltinPeakSpritz_SHAPES:
	.incbin "games/tbs/src/map/altin_peak_spritz/shapes.table.lz"
	.balign 4, 0
.LField_AltinPeakSpritz_GRID:
	.incbin "games/tbs/src/map/altin_peak_spritz/map.grid.lz"
	.balign 4, 0
.LField_AltinPeakSpritz_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_south_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltinPeakSpritz_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_AltinPeakSpritz_MARKERS:
	treasure_spot 3, 24, 100
	treasure_spot 16, 49, 101
	treasure_spots_end
	.balign 4, 0
