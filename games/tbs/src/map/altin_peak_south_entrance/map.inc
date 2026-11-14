	.global Field_AltinPeakSouthEntrance
Field_AltinPeakSouthEntrance:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltinPeakSouthEntrance, .LField_AltinPeakSouthEntrance_CELL_PICTURES
	map_offset Field_AltinPeakSouthEntrance, .LField_AltinPeakSouthEntrance_SHAPES
	map_offset Field_AltinPeakSouthEntrance, .LField_AltinPeakSouthEntrance_GRID
	map_offset Field_AltinPeakSouthEntrance, .LField_AltinPeakSouthEntrance_ANIMATION
	map_offset Field_AltinPeakSouthEntrance, .LField_AltinPeakSouthEntrance_BLEND
	map_offset Field_AltinPeakSouthEntrance, .LField_AltinPeakSouthEntrance_MARKERS
.LField_AltinPeakSouthEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin_peak_south_entrance/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AltinPeakSouthEntrance_SHAPES:
	.incbin "games/tbs/src/map/altin_peak_south_entrance/shapes.table.lz"
	.balign 4, 0
.LField_AltinPeakSouthEntrance_GRID:
	.incbin "games/tbs/src/map/altin_peak_south_entrance/map.grid.lz"
	.balign 4, 0
.LField_AltinPeakSouthEntrance_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_south_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltinPeakSouthEntrance_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_AltinPeakSouthEntrance_MARKERS:
	treasure_spots_end
	.balign 4, 0
