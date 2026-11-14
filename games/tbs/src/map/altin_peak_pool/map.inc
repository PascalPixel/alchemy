	.global Field_AltinPeakPool
Field_AltinPeakPool:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltinPeakPool, .LField_AltinPeakPool_CELL_PICTURES
	map_offset Field_AltinPeakPool, .LField_AltinPeakPool_SHAPES
	map_offset Field_AltinPeakPool, .LField_AltinPeakPool_GRID
	map_offset Field_AltinPeakPool, .LField_AltinPeakPool_ANIMATION
	map_offset Field_AltinPeakPool, .LField_AltinPeakPool_BLEND
	map_offset Field_AltinPeakPool, .LField_AltinPeakPool_MARKERS
.LField_AltinPeakPool_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin_peak_pool/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AltinPeakPool_SHAPES:
	.incbin "games/tbs/src/map/altin_peak_pool/shapes.table.lz"
	.balign 4, 0
.LField_AltinPeakPool_GRID:
	.incbin "games/tbs/src/map/altin_peak_pool/map.grid.lz"
	.balign 4, 0
.LField_AltinPeakPool_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_rail_tunnel/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltinPeakPool_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_AltinPeakPool_MARKERS:
	treasure_spot 3, 37, 100
	treasure_spots_end
	.balign 4, 0
