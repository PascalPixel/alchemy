	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_KalayTunnelEntrance
Field_KalayTunnelEntrance:
	map_header 0, 0, 72, 72, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KalayTunnelEntrance, .LField_KalayTunnelEntrance_CELL_PICTURES
	map_offset Field_KalayTunnelEntrance, .LField_KalayTunnelEntrance_SHAPES
	map_offset Field_KalayTunnelEntrance, .LField_KalayTunnelEntrance_GRID
	map_offset Field_KalayTunnelEntrance, .LField_KalayTunnelEntrance_ANIMATION
	map_offset Field_KalayTunnelEntrance, .LField_KalayTunnelEntrance_BLEND
	map_offset Field_KalayTunnelEntrance, .LField_KalayTunnelEntrance_MARKERS
.LField_KalayTunnelEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/kalay_tunnel_entrance/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KalayTunnelEntrance_SHAPES:
	.incbin "games/tbs/src/map/kalay_tunnel_entrance/shapes.table.lz"
	.balign 4, 0
.LField_KalayTunnelEntrance_GRID:
	.incbin "games/tbs/src/map/kalay_tunnel_entrance/map.grid.lz"
	.balign 4, 0
.LField_KalayTunnelEntrance_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_mouth/tileset_anims.script.lz"
	.balign 4, 0
.LField_KalayTunnelEntrance_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_KalayTunnelEntrance_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette37
Tileset_Palette37:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_37.gbapal.lz"
