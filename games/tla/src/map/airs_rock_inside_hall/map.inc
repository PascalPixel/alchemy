	.global Field_AirsRockInsideHall
Field_AirsRockInsideHall:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInsideHall, .LField_AirsRockInsideHall_CELL_TILES
	map_block Field_AirsRockInsideHall, .LField_AirsRockInsideHall_SHAPES
	map_block Field_AirsRockInsideHall, .LField_AirsRockInsideHall_GRID
	map_block Field_AirsRockInsideHall, .LField_AirsRockInsideHall_SHAPE_GRID
	map_block Field_AirsRockInsideHall, .LField_AirsRockInsideHall_ANIMATION
	map_block Field_AirsRockInsideHall, .LField_AirsRockInsideHall_BLEND
	map_block Field_AirsRockInsideHall, .LField_AirsRockInsideHall_SCENE
.LField_AirsRockInsideHall_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_inside_hall/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AirsRockInsideHall_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_hall/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInsideHall_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_hall/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInsideHall_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_hall/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInsideHall_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_hall/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInsideHall_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInsideHall_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_hall/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AirsRockInsideHallPalette
Tileset_AirsRockInsideHallPalette:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_hall/palette.gbapal.lz"
	.byte 0x94, 0x75	@ PACKER: left in the buffer by Field_AirsRockInsideHall+0x14a
	.global Tileset_AirsRockInsideHallTilesA
Tileset_AirsRockInsideHallTilesA:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_hall/tiles_a.4bpp.mtf"
	.byte 0x09	@ PACKER: left in the buffer by Field_AirsRockInsideHall+0x1e83
	.global Tileset_AirsRockInsideHallTilesB
Tileset_AirsRockInsideHallTilesB:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_hall/tiles_b.4bpp.mtf"
	.byte 0x7f, 0x04	@ PACKER: left in the buffer by Tileset_AirsRockInsideHallTilesA+0x15a6
	.global Tileset_AirsRockInsideHallTilesC
Tileset_AirsRockInsideHallTilesC:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_hall/tiles_c.4bpp.mtf"
	.byte 0x04, 0x40, 0xd8	@ PACKER: left in the buffer by Field_AirsRockInsideHall+0x1fa9
	.global Tileset_AirsRockInsideHallTilesD
Tileset_AirsRockInsideHallTilesD:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_hall/tiles_d.4bpp.mtf"
	.byte 0xe3, 0xf4	@ PACKER: left in the buffer by Tileset_AirsRockInsideHallTilesC+0xf6e
