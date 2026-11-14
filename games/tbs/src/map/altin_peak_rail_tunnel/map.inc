	.global Field_AltinPeakRailTunnel
Field_AltinPeakRailTunnel:
	map_header 0, 0, 64, 64, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltinPeakRailTunnel, .LField_AltinPeakRailTunnel_CELL_PICTURES
	map_offset Field_AltinPeakRailTunnel, .LField_AltinPeakRailTunnel_SHAPES
	map_offset Field_AltinPeakRailTunnel, .LField_AltinPeakRailTunnel_GRID
	map_offset Field_AltinPeakRailTunnel, .LField_AltinPeakRailTunnel_ANIMATION
	map_offset Field_AltinPeakRailTunnel, .LField_AltinPeakRailTunnel_BLEND
	map_offset Field_AltinPeakRailTunnel, .LField_AltinPeakRailTunnel_MARKERS
.LField_AltinPeakRailTunnel_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin_peak_rail_tunnel/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AltinPeakRailTunnel_SHAPES:
	.incbin "games/tbs/src/map/altin_peak_rail_tunnel/shapes.table.lz"
	.balign 4, 0
.LField_AltinPeakRailTunnel_GRID:
	.incbin "games/tbs/src/map/altin_peak_rail_tunnel/map.grid.lz"
	.balign 4, 0
.LField_AltinPeakRailTunnel_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_rail_tunnel/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltinPeakRailTunnel_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_AltinPeakRailTunnel_MARKERS:
	treasure_spots_end
	.balign 4, 0
