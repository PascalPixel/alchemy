	.global Field_IsletCaveHalls
Field_IsletCaveHalls:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_IsletCaveHalls, .LField_IsletCaveHalls_CELL_TILES
	map_block Field_IsletCaveHalls, .LField_IsletCaveHalls_SHAPES
	map_block Field_IsletCaveHalls, .LField_IsletCaveHalls_GRID
	map_block Field_IsletCaveHalls, .LField_IsletCaveHalls_SHAPE_GRID
	map_block Field_IsletCaveHalls, .LField_IsletCaveHalls_ANIMATION
	map_block Field_IsletCaveHalls, .LField_IsletCaveHalls_BLEND
	map_block Field_IsletCaveHalls, .LField_IsletCaveHalls_SCENE
.LField_IsletCaveHalls_CELL_TILES:
	.incbin "games/tla/src/map/islet_cave_halls/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_IsletCaveHalls_SHAPES:
	.incbin "games/tla/src/map/islet_cave_halls/shapes.table.lz"
	.balign 4, 0
.LField_IsletCaveHalls_GRID:
	.incbin "games/tla/src/map/islet_cave_halls/map.grid.lz"
	.balign 4, 0
.LField_IsletCaveHalls_SHAPE_GRID:
	.incbin "games/tla/src/map/islet_cave_halls/shape_grid.plane.lz"
	.balign 4, 0
.LField_IsletCaveHalls_ANIMATION:
	.incbin "games/tla/src/map/islet_cave_halls/animation.script.lz"
	.balign 4, 0
.LField_IsletCaveHalls_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_IsletCaveHalls_SCENE:
	.incbin "games/tla/src/map/islet_cave_halls/scene.scene.lz"
	.balign 4, 0
	.global Tileset_IsletCaveHallsPalette
Tileset_IsletCaveHallsPalette:
	.incbin "games/tla/src/graphics/tileset/islet_cave_halls/palette.gbapal.lz"
	.byte 0x47, 0x90	@ PACKER: left in the buffer by Field_IsletCaveHalls+0x166
	.global Tileset_IsletCaveHallsTilesA
Tileset_IsletCaveHallsTilesA:
	.incbin "games/tla/src/graphics/tileset/islet_cave_halls/tiles_a.4bpp.mtf"
	.byte 0xf8	@ PACKER: left in the buffer by Field_LohoRooms+0x2fbf
	.global Tileset_IsletCaveHallsTilesB
Tileset_IsletCaveHallsTilesB:
	.incbin "games/tla/src/graphics/tileset/islet_cave_halls/tiles_b.4bpp.mtf"
	.byte 0xc2, 0x04, 0x38	@ PACKER: left in the buffer by Tileset_IsletCaveHallsTilesA+0x126d
	.global Tileset_IsletCaveHallsTilesC
Tileset_IsletCaveHallsTilesC:
	.incbin "games/tla/src/graphics/tileset/islet_cave_halls/tiles_c.4bpp.mtf"
	.global Tileset_IsletCaveHallsTilesD
Tileset_IsletCaveHallsTilesD:
	.incbin "games/tla/src/graphics/tileset/islet_cave_halls/tiles_d.4bpp.mtf"
	.byte 0x81, 0x03	@ PACKER: left in the buffer by Tileset_IsletCaveHallsTilesA+0x1ac2
