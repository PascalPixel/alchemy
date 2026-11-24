	.global Field_SouthwestAttekaIslet
Field_SouthwestAttekaIslet:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SouthwestAttekaIslet, .LField_SouthwestAttekaIslet_CELL_TILES
	map_block Field_SouthwestAttekaIslet, .LField_SouthwestAttekaIslet_SHAPES
	map_block Field_SouthwestAttekaIslet, .LField_SouthwestAttekaIslet_GRID
	map_block Field_SouthwestAttekaIslet, .LField_SouthwestAttekaIslet_SHAPE_GRID
	map_block Field_SouthwestAttekaIslet, .LField_SouthwestAttekaIslet_ANIMATION
	map_block Field_SouthwestAttekaIslet, .LField_SouthwestAttekaIslet_BLEND
	map_block Field_SouthwestAttekaIslet, .LField_SouthwestAttekaIslet_SCENE
.LField_SouthwestAttekaIslet_CELL_TILES:
	.incbin "games/tla/src/map/southwest_atteka_islet/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_SouthwestAttekaIslet_SHAPES:
	.incbin "games/tla/src/map/southwest_atteka_islet/shapes.table.lz"
	.balign 4, 0
.LField_SouthwestAttekaIslet_GRID:
	.incbin "games/tla/src/map/southwest_atteka_islet/map.grid.lz"
	.balign 4, 0
.LField_SouthwestAttekaIslet_SHAPE_GRID:
	.incbin "games/tla/src/map/southwest_atteka_islet/shape_grid.plane.lz"
	.balign 4, 0
.LField_SouthwestAttekaIslet_ANIMATION:
	.incbin "games/tla/src/map/southwest_atteka_islet/animation.script.lz"
	.balign 4, 0
.LField_SouthwestAttekaIslet_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_SouthwestAttekaIslet_SCENE:
	.incbin "games/tla/src/map/southwest_atteka_islet/scene.scene.lz"
	.balign 4, 0
	.global Tileset_SouthwestAttekaIsletPalette
Tileset_SouthwestAttekaIsletPalette:
	.incbin "games/tla/src/graphics/tileset/southwest_atteka_islet/palette.gbapal.lz"
	.byte 0xfe, 0x07	@ PACKER: left in the buffer by Field_SouthwestAttekaIslet+0x17e
	.global Tileset_SouthwestAttekaIsletTilesA
Tileset_SouthwestAttekaIsletTilesA:
	.incbin "games/tla/src/graphics/tileset/southwest_atteka_islet/tiles_a.4bpp.mtf"
	.global Tileset_SouthwestAttekaIsletTilesB
Tileset_SouthwestAttekaIsletTilesB:
	.incbin "games/tla/src/graphics/tileset/southwest_atteka_islet/tiles_b.4bpp.mtf"
	.byte 0x9d, 0x69	@ PACKER: left in the buffer by Tileset_SouthwestAttekaIsletTilesA+0x1e02
	.global Tileset_SouthwestAttekaIsletTilesC
Tileset_SouthwestAttekaIsletTilesC:
	.incbin "games/tla/src/graphics/tileset/southwest_atteka_islet/tiles_c.4bpp.mtf"
	.byte 0x84	@ PACKER: left in the buffer by Tileset_SouthwestAttekaIsletTilesB+0x1d5b
	.global Tileset_SouthwestAttekaIsletTilesD
Tileset_SouthwestAttekaIsletTilesD:
	.incbin "games/tla/src/graphics/tileset/southwest_atteka_islet/tiles_d.4bpp.mtf"
