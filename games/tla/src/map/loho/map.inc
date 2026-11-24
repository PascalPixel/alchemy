	.global Field_Loho
Field_Loho:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Loho, .LField_Loho_CELL_TILES
	map_block Field_Loho, .LField_Loho_SHAPES
	map_block Field_Loho, .LField_Loho_GRID
	map_block Field_Loho, .LField_Loho_SHAPE_GRID
	map_block Field_Loho, .LField_Loho_ANIMATION
	map_block Field_Loho, .LField_Loho_BLEND
	map_block Field_Loho, .LField_Loho_SCENE
.LField_Loho_CELL_TILES:
	.incbin "games/tla/src/map/loho/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_Loho_SHAPES:
	.incbin "games/tla/src/map/loho/shapes.table.lz"
	.balign 4, 0
.LField_Loho_GRID:
	.incbin "games/tla/src/map/loho/map.grid.lz"
	.balign 4, 0
.LField_Loho_SHAPE_GRID:
	.incbin "games/tla/src/map/loho/shape_grid.plane.lz"
	.balign 4, 0
.LField_Loho_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_Loho_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Loho_SCENE:
	.incbin "games/tla/src/map/loho/scene.scene.lz"
	.global Tileset_LohoPalette
Tileset_LohoPalette:
	.incbin "games/tla/src/graphics/tileset/loho/palette.gbapal.lz"
	.byte 0xa3	@ PACKER: left in the buffer by Field_Loho+0x11f
	.global Tileset_LohoTilesA
Tileset_LohoTilesA:
	.incbin "games/tla/src/graphics/tileset/loho/tiles_a.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_JupiterLighthouseEntranceTilesB+0x25cf
	.global Tileset_LohoTilesB
Tileset_LohoTilesB:
	.incbin "games/tla/src/graphics/tileset/loho/tiles_b.4bpp.mtf"
	.byte 0xc4, 0x0d, 0xef	@ PACKER: left in the buffer by Tileset_LohoTilesA+0x2311
	.global Tileset_LohoTilesC
Tileset_LohoTilesC:
	.incbin "games/tla/src/graphics/tileset/loho/tiles_c.4bpp.mtf"
	.byte 0xf2, 0x00, 0x7f	@ PACKER: left in the buffer by Tileset_LohoTilesB+0x871
	.global Tileset_LohoTilesD
Tileset_LohoTilesD:
	.incbin "games/tla/src/graphics/tileset/loho/tiles_d.4bpp.mtf"
	.byte 0xf5	@ PACKER: left in the buffer by Tileset_LohoTilesC+0x13f
