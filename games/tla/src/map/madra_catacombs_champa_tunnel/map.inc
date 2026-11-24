	.global Field_MadraCatacombsChampaTunnel
Field_MadraCatacombsChampaTunnel:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombsChampaTunnel, .LField_MadraCatacombsChampaTunnel_CELL_TILES
	map_block Field_MadraCatacombsChampaTunnel, .LField_MadraCatacombsChampaTunnel_SHAPES
	map_block Field_MadraCatacombsChampaTunnel, .LField_MadraCatacombsChampaTunnel_GRID
	map_block Field_MadraCatacombsChampaTunnel, .LField_MadraCatacombsChampaTunnel_SHAPE_GRID
	map_block Field_MadraCatacombsChampaTunnel, .LField_MadraCatacombsChampaTunnel_ANIMATION
	map_block Field_MadraCatacombsChampaTunnel
	map_block Field_MadraCatacombsChampaTunnel, .LField_MadraCatacombsChampaTunnel_SCENE
.LField_MadraCatacombsChampaTunnel_CELL_TILES:
	.incbin "games/tla/src/map/madra_catacombs_champa_tunnel/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MadraCatacombsChampaTunnel_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_champa_tunnel/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombsChampaTunnel_GRID:
	.incbin "games/tla/src/map/madra_catacombs_champa_tunnel/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombsChampaTunnel_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_champa_tunnel/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombsChampaTunnel_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombsChampaTunnel_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_champa_tunnel/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MadraCatacombsChampaTunnelPalette
Tileset_MadraCatacombsChampaTunnelPalette:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_champa_tunnel/palette.gbapal.lz"
	.byte 0x60, 0x3f, 0xbe	@ PACKER: left in the buffer by Field_MadraCatacombsChampaTunnel+0x115
	.global Tileset_MadraCatacombsChampaTunnelTilesA
Tileset_MadraCatacombsChampaTunnelTilesA:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_champa_tunnel/tiles_a.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Field_MadraRoomsCavernHomes+0x22d3
	.global Tileset_MadraCatacombsChampaTunnelTilesB
Tileset_MadraCatacombsChampaTunnelTilesB:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_champa_tunnel/tiles_b.4bpp.mtf"
	.byte 0x11, 0xa7	@ PACKER: left in the buffer by Tileset_MadraCatacombsChampaTunnelTilesA+0x1c52
	.global Tileset_MadraCatacombsChampaTunnelTilesC
Tileset_MadraCatacombsChampaTunnelTilesC:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_champa_tunnel/tiles_c.4bpp.mtf"
	.byte 0x43	@ PACKER: left in the buffer by Tileset_MadraCatacombsChampaTunnelTilesB+0x643
	.global Tileset_MadraCatacombsChampaTunnelTilesD
Tileset_MadraCatacombsChampaTunnelTilesD:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_champa_tunnel/tiles_d.4bpp.mtf"
	.byte 0x50	@ PACKER: left in the buffer by Tileset_MadraCatacombsChampaTunnelTilesC+0x13f
