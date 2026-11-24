	.global Field_MarsLighthouseHall
Field_MarsLighthouseHall:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseHall, .LField_MarsLighthouseHall_CELL_TILES
	map_block Field_MarsLighthouseHall, .LField_MarsLighthouseHall_SHAPES
	map_block Field_MarsLighthouseHall, .LField_MarsLighthouseHall_GRID
	map_block Field_MarsLighthouseHall, .LField_MarsLighthouseHall_SHAPE_GRID
	map_block Field_MarsLighthouseHall, .LField_MarsLighthouseHall_ANIMATION
	map_block Field_MarsLighthouseHall, .LField_MarsLighthouseHall_BLEND
	map_block Field_MarsLighthouseHall, .LField_MarsLighthouseHall_SCENE
.LField_MarsLighthouseHall_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_hall/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseHall_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_hall/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseHall_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_hall/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseHall_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_hall/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseHall_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseHall_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseHall_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_hall/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MarsLighthouseHallPalette
Tileset_MarsLighthouseHallPalette:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_hall/palette.gbapal.lz"
	.byte 0x05, 0x18	@ PACKER: left in the buffer by Field_MarsLighthouseHall+0x1b2
	.global Tileset_MarsLighthouseHallTilesA
Tileset_MarsLighthouseHallTilesA:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_hall/tiles_a.4bpp.mtf"
	.byte 0x23, 0x44, 0x28	@ PACKER: left in the buffer by Tileset_MarsLighthouseAerieTilesA+0x223d
	.global Tileset_MarsLighthouseHallTilesB
Tileset_MarsLighthouseHallTilesB:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_hall/tiles_b.4bpp.mtf"
	.byte 0x8e	@ PACKER: left in the buffer by Tileset_MarsLighthouseHallTilesA+0xb23
