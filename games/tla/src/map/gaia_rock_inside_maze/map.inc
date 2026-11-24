	.global Field_GaiaRockInsideMaze
Field_GaiaRockInsideMaze:
	map_header 0, 0, 130, 112, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_GaiaRockInsideMaze, .LField_GaiaRockInsideMaze_CELL_TILES
	map_block Field_GaiaRockInsideMaze, .LField_GaiaRockInsideMaze_SHAPES
	map_block Field_GaiaRockInsideMaze, .LField_GaiaRockInsideMaze_GRID
	map_block Field_GaiaRockInsideMaze, .LField_GaiaRockInsideMaze_SHAPE_GRID
	map_block Field_GaiaRockInsideMaze, .LField_GaiaRockInsideMaze_ANIMATION
	map_block Field_GaiaRockInsideMaze, .LField_GaiaRockInsideMaze_BLEND
	map_block Field_GaiaRockInsideMaze, .LField_GaiaRockInsideMaze_SCENE
.LField_GaiaRockInsideMaze_CELL_TILES:
	.incbin "games/tla/src/map/gaia_rock_inside_maze/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GaiaRockInsideMaze_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_inside_maze/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockInsideMaze_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_maze/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockInsideMaze_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_maze/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockInsideMaze_ANIMATION:
	.incbin "games/tla/src/map/gaia_rock_inside_maze/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockInsideMaze_BLEND:
	.incbin "games/tla/src/map/gaia_rock_inside_maze/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockInsideMaze_SCENE:
	.incbin "games/tla/src/map/gaia_rock_inside_maze/scene.scene.lz"
	.balign 4, 0
	.global Tileset_GaiaRockInsideMazePalette
Tileset_GaiaRockInsideMazePalette:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_maze/palette.gbapal.lz"
	.byte 0x18	@ PACKER: left in the buffer by Field_GaiaRockInside1+0x133
	.global Tileset_GaiaRockInsideMazeTilesA
Tileset_GaiaRockInsideMazeTilesA:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_maze/tiles_a.4bpp.mtf"
	.byte 0x20	@ PACKER: left in the buffer by Field_GaiaRockInside1+0x1c6b
	.global Tileset_GaiaRockInsideMazeTilesB
Tileset_GaiaRockInsideMazeTilesB:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_maze/tiles_b.4bpp.mtf"
	.byte 0x28	@ PACKER: left in the buffer by Tileset_GaiaRockInsideMazeTilesA+0x11b7
	.global Tileset_GaiaRockInsideMazeTilesC
Tileset_GaiaRockInsideMazeTilesC:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_maze/tiles_c.4bpp.mtf"
	.global Tileset_GaiaRockInsideMazeTilesD
Tileset_GaiaRockInsideMazeTilesD:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_maze/tiles_d.4bpp.mtf"
	.byte 0x23, 0x70, 0x22	@ PACKER: left in the buffer by Tileset_GaiaRockInsideMazeTilesC+0x639
