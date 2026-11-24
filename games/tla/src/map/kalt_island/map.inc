	.global Field_KaltIsland
Field_KaltIsland:
	map_header 0, 0, 52, 54, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 52, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 54, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KaltIsland, .LField_KaltIsland_CELL_TILES
	map_block Field_KaltIsland, .LField_KaltIsland_SHAPES
	map_block Field_KaltIsland, .LField_KaltIsland_GRID
	map_block Field_KaltIsland, .LField_KaltIsland_SHAPE_GRID
	map_block Field_KaltIsland, .LField_KaltIsland_ANIMATION
	map_block Field_KaltIsland, .LField_KaltIsland_BLEND
	map_block Field_KaltIsland, .LField_KaltIsland_SCENE
.LField_KaltIsland_CELL_TILES:
	.incbin "games/tla/src/map/kalt_island/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_KaltIsland_SHAPES:
	.incbin "games/tla/src/map/kalt_island/shapes.table.lz"
	.balign 4, 0
.LField_KaltIsland_GRID:
	.incbin "games/tla/src/map/kalt_island/map.grid.lz"
	.balign 4, 0
.LField_KaltIsland_SHAPE_GRID:
	.incbin "games/tla/src/map/kalt_island/shape_grid.plane.lz"
	.balign 4, 0
.LField_KaltIsland_ANIMATION:
	.incbin "games/tla/src/map/kalt_island/animation.script.lz"
	.balign 4, 0
.LField_KaltIsland_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KaltIsland_SCENE:
	.incbin "games/tla/src/map/kalt_island/scene.scene.lz"
	.balign 4, 0
	.global Tileset_KaltIslandPalette
Tileset_KaltIslandPalette:
	.incbin "games/tla/src/graphics/tileset/kalt_island/palette.gbapal.lz"
	.byte 0xc0, 0x39	@ PACKER: left in the buffer by Field_KaltIsland+0x166
	.global Tileset_KaltIslandTilesA
Tileset_KaltIslandTilesA:
	.incbin "games/tla/src/graphics/tileset/kalt_island/tiles_a.4bpp.mtf"
	.global Tileset_KaltIslandTilesB
Tileset_KaltIslandTilesB:
	.incbin "games/tla/src/graphics/tileset/kalt_island/tiles_b.4bpp.mtf"
	.global Tileset_KaltIslandTilesC
Tileset_KaltIslandTilesC:
	.incbin "games/tla/src/graphics/tileset/kalt_island/tiles_c.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_KaltIslandTilesD
Tileset_KaltIslandTilesD:
	.incbin "games/tla/src/graphics/tileset/kalt_island/tiles_d.4bpp.mtf"
	.byte 0xe1	@ PACKER: left in the buffer by Tileset_KaltIslandTilesC+0x5db
