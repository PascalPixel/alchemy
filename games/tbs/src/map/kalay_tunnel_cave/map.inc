	.global Field_KalayTunnelCave
Field_KalayTunnelCave:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KalayTunnelCave, .LField_KalayTunnelCave_CELL_PICTURES
	map_offset Field_KalayTunnelCave, .LField_KalayTunnelCave_SHAPES
	map_offset Field_KalayTunnelCave, .LField_KalayTunnelCave_GRID
	map_offset Field_KalayTunnelCave, .LField_KalayTunnelCave_ANIMATION
	map_offset Field_KalayTunnelCave, .LField_KalayTunnelCave_BLEND
	map_offset Field_KalayTunnelCave, .LField_KalayTunnelCave_MARKERS
.LField_KalayTunnelCave_CELL_PICTURES:
	.incbin "games/tbs/src/map/kalay_tunnel_cave/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_KalayTunnelCave_SHAPES:
	.incbin "games/tbs/src/map/kalay_tunnel_cave/shapes.table.lz"
	.balign 4, 0
.LField_KalayTunnelCave_GRID:
	.incbin "games/tbs/src/map/kalay_tunnel_cave/map.grid.lz"
	.balign 4, 0
.LField_KalayTunnelCave_ANIMATION:
	.incbin "games/tbs/src/map/kalay_tunnel_cave/tileset_anims.script.lz"
	.balign 4, 0
.LField_KalayTunnelCave_BLEND:
	.incbin "games/tbs/src/map/bilibin_treasury/blend.script.lz"
	.balign 4, 0
.LField_KalayTunnelCave_MARKERS:
	treasure_spot 24, 19, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteKalayTunnelCave
Tileset_PaletteKalayTunnelCave:
	.incbin "games/tbs/src/graphics/tileset/palettes/kalay_tunnel_cave.gbapal.lz"
	.balign 4, 0
