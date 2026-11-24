	.global Field_MarsLighthouseMarsTower
Field_MarsLighthouseMarsTower:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseMarsTower, .LField_MarsLighthouseMarsTower_CELL_TILES
	map_block Field_MarsLighthouseMarsTower, .LField_MarsLighthouseMarsTower_SHAPES
	map_block Field_MarsLighthouseMarsTower, .LField_MarsLighthouseMarsTower_GRID
	map_block Field_MarsLighthouseMarsTower, .LField_MarsLighthouseMarsTower_SHAPE_GRID
	map_block Field_MarsLighthouseMarsTower, .LField_MarsLighthouseMarsTower_ANIMATION
	map_block Field_MarsLighthouseMarsTower, .LField_MarsLighthouseMarsTower_BLEND
	map_block Field_MarsLighthouseMarsTower, .LField_MarsLighthouseMarsTower_SCENE
.LField_MarsLighthouseMarsTower_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsTower_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsTower_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsTower_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsTower_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsTower_BLEND:
	.incbin "games/tla/src/map/venus_lighthouse/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseMarsTower_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MarsLighthouseMarsTowerPalette
Tileset_MarsLighthouseMarsTowerPalette:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_mars_tower/palette.gbapal.lz"
	.byte 0x14, 0x8f	@ PACKER: left in the buffer by Field_MarsLighthouseMarsTower+0x19a
	.global Tileset_MarsLighthouseMarsTowerTilesA
Tileset_MarsLighthouseMarsTowerTilesA:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_mars_tower/tiles_a.4bpp.mtf"
	.byte 0x10, 0x00	@ PACKER: left in the buffer by Field_ProxHouses+0x22aa
	.global Tileset_MarsLighthouseMarsTowerTilesB
Tileset_MarsLighthouseMarsTowerTilesB:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_mars_tower/tiles_b.4bpp.mtf"
	.byte 0xa6, 0x68, 0x02	@ PACKER: left in the buffer by Tileset_MarsLighthouseMarsTowerTilesA+0x21c5
	.global Tileset_MarsLighthouseMarsTowerTilesC
Tileset_MarsLighthouseMarsTowerTilesC:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_mars_tower/tiles_c.4bpp.mtf"
	.byte 0x10	@ PACKER: left in the buffer by Tileset_MarsLighthouseMarsTowerTilesB+0x16fb
	.global Tileset_MarsLighthouseMarsTowerTilesD
Tileset_MarsLighthouseMarsTowerTilesD:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_mars_tower/tiles_d.4bpp.mtf"
	.byte 0x9e	@ PACKER: left in the buffer by Tileset_MarsLighthouseMarsTowerTilesC+0xb23
