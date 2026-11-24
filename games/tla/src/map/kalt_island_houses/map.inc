	.global Field_KaltIslandHouses
Field_KaltIslandHouses:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KaltIslandHouses, .LField_KaltIslandHouses_CELL_TILES
	map_block Field_KaltIslandHouses, .LField_KaltIslandHouses_SHAPES
	map_block Field_KaltIslandHouses, .LField_KaltIslandHouses_GRID
	map_block Field_KaltIslandHouses, .LField_KaltIslandHouses_SHAPE_GRID
	map_block Field_KaltIslandHouses, .LField_KaltIslandHouses_ANIMATION
	map_block Field_KaltIslandHouses, .LField_KaltIslandHouses_BLEND
	map_block Field_KaltIslandHouses, .LField_KaltIslandHouses_SCENE
.LField_KaltIslandHouses_CELL_TILES:
	.incbin "games/tla/src/map/kalt_island_houses/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_KaltIslandHouses_SHAPES:
	.incbin "games/tla/src/map/kalt_island_houses/shapes.table.lz"
	.balign 4, 0
.LField_KaltIslandHouses_GRID:
	.incbin "games/tla/src/map/kalt_island_houses/map.grid.lz"
	.balign 4, 0
.LField_KaltIslandHouses_SHAPE_GRID:
	.incbin "games/tla/src/map/kalt_island_houses/shape_grid.plane.lz"
	.balign 4, 0
.LField_KaltIslandHouses_ANIMATION:
	.incbin "games/tla/src/map/kalt_island_houses/animation.script.lz"
	.balign 4, 0
.LField_KaltIslandHouses_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_KaltIslandHouses_SCENE:
	.incbin "games/tla/src/map/kalt_island_houses/scene.scene.lz"
	.balign 4, 0
	.global Tileset_KaltIslandHousesPalette
Tileset_KaltIslandHousesPalette:
	.incbin "games/tla/src/graphics/tileset/kalt_island_houses/palette.gbapal.lz"
	.global Tileset_KaltIslandHousesTilesA
Tileset_KaltIslandHousesTilesA:
	.incbin "games/tla/src/graphics/tileset/kalt_island_houses/tiles_a.4bpp.mtf"
	.byte 0xb8	@ PACKER: left in the buffer by Tileset_KaltIslandTilesC+0x2677
	.global Tileset_KaltIslandHousesTilesB
Tileset_KaltIslandHousesTilesB:
	.incbin "games/tla/src/graphics/tileset/kalt_island_houses/tiles_b.4bpp.mtf"
	.byte 0xf5, 0x13	@ PACKER: left in the buffer by Tileset_KaltIslandHousesTilesA+0x161e
	.global Tileset_KaltIslandHousesTilesC
Tileset_KaltIslandHousesTilesC:
	.incbin "games/tla/src/graphics/tileset/kalt_island_houses/tiles_c.4bpp.mtf"
	.byte 0xde	@ PACKER: left in the buffer by Tileset_KaltIslandHousesTilesA+0x195f
	.global Tileset_KaltIslandHousesTilesD
Tileset_KaltIslandHousesTilesD:
	.incbin "games/tla/src/graphics/tileset/kalt_island_houses/tiles_d.4bpp.mtf"
