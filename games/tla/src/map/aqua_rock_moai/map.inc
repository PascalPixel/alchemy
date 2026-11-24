	.global Field_AquaRockMoai
Field_AquaRockMoai:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockMoai, .LField_AquaRockMoai_CELL_TILES
	map_block Field_AquaRockMoai, .LField_AquaRockMoai_SHAPES
	map_block Field_AquaRockMoai, .LField_AquaRockMoai_GRID
	map_block Field_AquaRockMoai, .LField_AquaRockMoai_SHAPE_GRID
	map_block Field_AquaRockMoai, .LField_AquaRockMoai_ANIMATION
	map_block Field_AquaRockMoai, .LField_AquaRockMoai_BLEND
	map_block Field_AquaRockMoai, .LField_AquaRockMoai_SCENE
.LField_AquaRockMoai_CELL_TILES:
	.incbin "games/tla/src/map/aqua_rock_moai/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_AquaRockMoai_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_moai/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockMoai_GRID:
	.incbin "games/tla/src/map/aqua_rock_moai/map.grid.lz"
	.balign 4, 0
.LField_AquaRockMoai_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_moai/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockMoai_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_moai/animation.script.lz"
	.balign 4, 0
.LField_AquaRockMoai_BLEND:
	.incbin "games/tla/src/map/aqua_rock_moai/blend.script.lz"
	.balign 4, 0
.LField_AquaRockMoai_SCENE:
	.incbin "games/tla/src/map/aqua_rock_moai/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AquaRockMoaiPalette
Tileset_AquaRockMoaiPalette:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_moai/palette.gbapal.lz"
	.byte 0x55, 0x71, 0xd4	@ PACKER: left in the buffer by Field_AquaRockMoai+0x161
	.global Tileset_AquaRockMoaiTilesA
Tileset_AquaRockMoaiTilesA:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_moai/tiles_a.4bpp.mtf"
	.byte 0xd1, 0x92, 0x40	@ PACKER: left in the buffer by Field_AquaRockMoai+0x26dd
	.global Tileset_AquaRockMoaiTilesB
Tileset_AquaRockMoaiTilesB:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_moai/tiles_b.4bpp.mtf"
	.global Tileset_AquaRockMoaiTilesC
Tileset_AquaRockMoaiTilesC:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_moai/tiles_c.4bpp.mtf"
	.byte 0x7c, 0x3c, 0x07	@ PACKER: left in the buffer by Tileset_AquaRockMoaiTilesB+0x118d
	.global Tileset_AquaRockMoaiTilesD
Tileset_AquaRockMoaiTilesD:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_moai/tiles_d.4bpp.mtf"
	.byte 0x8c, 0x03	@ PACKER: left in the buffer by Tileset_AquaRockMoaiTilesB+0x1572
