	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_KalayTunnelTreasureRoom
Field_KalayTunnelTreasureRoom:
	map_header 0, 0, 128, 128, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KalayTunnelTreasureRoom, .LField_KalayTunnelTreasureRoom_CELL_PICTURES
	map_offset Field_KalayTunnelTreasureRoom, .LField_KalayTunnelTreasureRoom_SHAPES
	map_offset Field_KalayTunnelTreasureRoom, .LField_KalayTunnelTreasureRoom_GRID
	map_offset Field_KalayTunnelTreasureRoom, .LField_KalayTunnelTreasureRoom_ANIMATION
	map_offset Field_KalayTunnelTreasureRoom, .LField_KalayTunnelTreasureRoom_BLEND
	map_offset Field_KalayTunnelTreasureRoom, .LField_KalayTunnelTreasureRoom_MARKERS
.LField_KalayTunnelTreasureRoom_CELL_PICTURES:
	.incbin "games/tbs/src/map/kalay_tunnel_treasure_room/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_KalayTunnelTreasureRoom_SHAPES:
	.incbin "games/tbs/src/map/kalay_tunnel_treasure_room/shapes.table.lz"
	.balign 4, 0
.LField_KalayTunnelTreasureRoom_GRID:
	.incbin "games/tbs/src/map/kalay_tunnel_treasure_room/map.grid.lz"
	.balign 4, 0
.LField_KalayTunnelTreasureRoom_ANIMATION:
	.incbin "games/tbs/src/map/kalay_tunnel_passage/tileset_anims.script.lz"
	.balign 4, 0
.LField_KalayTunnelTreasureRoom_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_luna_sol_rooms/blend.script.lz"
	.balign 4, 0
.LField_KalayTunnelTreasureRoom_MARKERS:
	treasure_spot 39, 14, 101
	treasure_spot 7, 44, 100
	treasure_spot 39, 44, 101
	treasure_spot 25, 51, 102
	treasure_spot 26, 51, 103
	treasure_spot 25, 53, 104
	treasure_spot 26, 53, 105
	treasure_spots_end
