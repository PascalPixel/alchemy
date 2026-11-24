	.global Field_TaopoSwampPool
Field_TaopoSwampPool:
	map_header 0, 0, 90, 100, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwampPool, .LField_TaopoSwampPool_CELL_TILES
	map_block Field_TaopoSwampPool, .LField_TaopoSwampPool_SHAPES
	map_block Field_TaopoSwampPool, .LField_TaopoSwampPool_GRID
	map_block Field_TaopoSwampPool, .LField_TaopoSwampPool_SHAPE_GRID
	map_block Field_TaopoSwampPool, .LField_TaopoSwampPool_ANIMATION
	map_block Field_TaopoSwampPool, .LField_TaopoSwampPool_BLEND
	map_block Field_TaopoSwampPool, .LField_TaopoSwampPool_SCENE
.LField_TaopoSwampPool_CELL_TILES:
	.incbin "games/tla/src/map/taopo_swamp_pool/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TaopoSwampPool_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_pool/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwampPool_GRID:
	.incbin "games/tla/src/map/taopo_swamp_pool/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwampPool_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_pool/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwampPool_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_pool/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwampPool_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwampPool_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_pool/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TaopoSwampPoolPalette
Tileset_TaopoSwampPoolPalette:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_pool/palette.gbapal.lz"
	.byte 0xe1, 0x40, 0x88	@ PACKER: left in the buffer by Field_TaopoSwampPool+0x16d
	.global Tileset_TaopoSwampPoolTilesA
Tileset_TaopoSwampPoolTilesA:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_pool/tiles_a.4bpp.mtf"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.global Tileset_TaopoSwampPoolTilesB
Tileset_TaopoSwampPoolTilesB:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_pool/tiles_b.4bpp.mtf"
	.global Tileset_TaopoSwampPoolTilesC
Tileset_TaopoSwampPoolTilesC:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_pool/tiles_c.4bpp.mtf"
	.byte 0x50, 0x22	@ PACKER: left in the buffer by Tileset_TaopoSwampPoolTilesA+0x10d6
	.global Tileset_TaopoSwampPoolTilesD
Tileset_TaopoSwampPoolTilesD:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_pool/tiles_d.4bpp.mtf"
	.byte 0xc1, 0x26, 0x82	@ PACKER: left in the buffer by Tileset_TaopoSwampPoolTilesA+0x1969
