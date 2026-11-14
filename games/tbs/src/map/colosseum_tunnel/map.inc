	.global Field_ColosseumTunnel
Field_ColosseumTunnel:
	map_header 0, 0, 70, 34, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ColosseumTunnel, .LField_ColosseumTunnel_CELL_PICTURES
	map_offset Field_ColosseumTunnel, .LField_ColosseumTunnel_SHAPES
	map_offset Field_ColosseumTunnel, .LField_ColosseumTunnel_GRID
	map_offset Field_ColosseumTunnel, .LField_ColosseumTunnel_ANIMATION
	map_offset Field_ColosseumTunnel, .LField_ColosseumTunnel_BLEND
	map_offset Field_ColosseumTunnel, .LField_ColosseumTunnel_MARKERS
.LField_ColosseumTunnel_CELL_PICTURES:
	.incbin "games/tbs/src/map/colosseum_tunnel/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ColosseumTunnel_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_ColosseumTunnel_GRID:
	.incbin "games/tbs/src/map/colosseum_tunnel/map.grid.lz"
	.balign 4, 0
.LField_ColosseumTunnel_ANIMATION:
	.incbin "games/tbs/src/map/colosseum_tunnel/tileset_anims.script.lz"
	.balign 4, 0
.LField_ColosseumTunnel_BLEND:
	.incbin "games/tbs/src/map/colosseum_tunnel/blend.script.lz"
	.balign 4, 0
.LField_ColosseumTunnel_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_ColosseumTunnelPalette
Tileset_ColosseumTunnelPalette:
	.incbin "games/tbs/src/graphics/tileset/colosseum_tunnel/palette.gbapal.lz"
	.byte 0x8e, 0x89	@ PACKER: left in the buffer by Field_ColosseumTunnel+0x166
	.global Tileset_ColosseumTunnelTilesA
Tileset_ColosseumTunnelTilesA:
	.incbin "games/tbs/src/graphics/tileset/colosseum_tunnel/tiles_a.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Field_BabiPalace+0x2a13
	.global Tileset_ColosseumTunnelTilesB
Tileset_ColosseumTunnelTilesB:
	.incbin "games/tbs/src/graphics/tileset/colosseum_tunnel/tiles_b.4bpp.mtf"
	.byte 0x8c, 0xba	@ PACKER: left in the buffer by Tileset_ColosseumTunnelTilesA+0x1c3e
	.global Tileset_ColosseumTunnelTilesC
Tileset_ColosseumTunnelTilesC:
	.incbin "games/tbs/src/graphics/tileset/colosseum_tunnel/tiles_c.4bpp.mtf"
	.byte 0xc6	@ PACKER: left in the buffer by Tileset_ColosseumTunnelTilesB+0x4bb
	.global Tileset_ColosseumTunnelTilesD
Tileset_ColosseumTunnelTilesD:
	.incbin "games/tbs/src/graphics/tileset/colosseum_tunnel/tiles_d.4bpp.mtf"
	.byte 0x58	@ PACKER: left in the buffer by Tileset_ColosseumTunnelTilesB+0x1377
	.balign 4, 0
