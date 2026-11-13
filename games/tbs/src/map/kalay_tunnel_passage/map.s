	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_KalayTunnelPassage
Field_KalayTunnelPassage:
	map_header 0, 0, 100, 100, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KalayTunnelPassage, .LField_KalayTunnelPassage_CELL_PICTURES
	map_offset Field_KalayTunnelPassage, .LField_KalayTunnelPassage_SHAPES
	map_offset Field_KalayTunnelPassage, .LField_KalayTunnelPassage_GRID
	map_offset Field_KalayTunnelPassage, .LField_KalayTunnelPassage_ANIMATION
	map_offset Field_KalayTunnelPassage, .LField_KalayTunnelPassage_BLEND
	map_offset Field_KalayTunnelPassage, .LField_KalayTunnelPassage_MARKERS
.LField_KalayTunnelPassage_CELL_PICTURES:
	.incbin "games/tbs/src/map/kalay_tunnel_passage/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_KalayTunnelPassage_SHAPES:
	.incbin "games/tbs/src/map/sol_sanctum_eruption/shapes.table.lz"
	.balign 4, 0
.LField_KalayTunnelPassage_GRID:
	.incbin "games/tbs/src/map/kalay_tunnel_passage/map.grid.lz"
	.balign 4, 0
.LField_KalayTunnelPassage_ANIMATION:
	.incbin "games/tbs/src/map/kalay_tunnel_passage/tileset_anims.script.lz"
	.balign 4, 0
.LField_KalayTunnelPassage_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_luna_sol_rooms/blend.script.lz"
	.balign 4, 0
.LField_KalayTunnelPassage_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set63Palette
Tileset_Set63Palette:
	.incbin "games/tbs/src/graphics/tileset/set_63/palette.gbapal.lz"
	.byte 0x4a, 0xf0, 0x09	@ PACKER: left in the buffer by Field_KalayTunnelPassage+0x161
	.global Tileset_Set63TilesA
Tileset_Set63TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_63/tiles_a.4bpp.mtf"
	.byte 0x02, 0x04, 0x30	@ PACKER: left in the buffer by Field_KalayTunnelEntrance+0x29c9
