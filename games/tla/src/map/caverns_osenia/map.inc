	.global Field_CavernsOsenia
Field_CavernsOsenia:
	map_header 0, 0, 128, 128, 3, 2, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 100, 100, 0, 0, 0xff, 0xff
	map_block Field_CavernsOsenia, .LField_CavernsOsenia_CELL_TILES
	map_block Field_CavernsOsenia, .LField_CavernsOsenia_SHAPES
	map_block Field_CavernsOsenia, .LField_CavernsOsenia_GRID
	map_block Field_CavernsOsenia, .LField_CavernsOsenia_SHAPE_GRID
	map_block Field_CavernsOsenia, .LField_CavernsOsenia_ANIMATION
	map_block Field_CavernsOsenia, .LField_CavernsOsenia_BLEND
	map_block Field_CavernsOsenia, .LField_CavernsOsenia_SCENE
.LField_CavernsOsenia_CELL_TILES:
	.incbin "games/tla/src/map/caverns_osenia/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_CavernsOsenia_SHAPES:
	.incbin "games/tla/src/map/caverns_osenia/shapes.table.lz"
	.balign 4, 0
.LField_CavernsOsenia_GRID:
	.incbin "games/tla/src/map/caverns_osenia/map.grid.lz"
	.balign 4, 0
.LField_CavernsOsenia_SHAPE_GRID:
	.incbin "games/tla/src/map/caverns_osenia/shape_grid.plane.lz"
	.balign 4, 0
.LField_CavernsOsenia_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_CavernsOsenia_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_CavernsOsenia_SCENE:
	.incbin "games/tla/src/map/caverns_osenia/scene.scene.lz"
	.balign 4, 0
	.global Tileset_CavernsOseniaPalette
Tileset_CavernsOseniaPalette:
	.incbin "games/tla/src/graphics/tileset/caverns_osenia/palette.gbapal.lz"
	.byte 0x74, 0x6c, 0x5a	@ PACKER: left in the buffer by Field_Caverns2+0x121
	.global Tileset_CavernsOseniaTilesA
Tileset_CavernsOseniaTilesA:
	.incbin "games/tla/src/graphics/tileset/caverns_osenia/tiles_a.4bpp.mtf"
	.byte 0xe2	@ PACKER: left in the buffer by Tileset_IsletCaveHallsTilesA+0x2afb
	.global Tileset_CavernsOseniaTilesB
Tileset_CavernsOseniaTilesB:
	.incbin "games/tla/src/graphics/tileset/caverns_osenia/tiles_b.4bpp.mtf"
	.byte 0xe0, 0x28	@ PACKER: left in the buffer by Tileset_CavernsOseniaTilesA+0xdf6
	.global Tileset_CavernsOseniaTilesC
Tileset_CavernsOseniaTilesC:
	.incbin "games/tla/src/graphics/tileset/caverns_osenia/tiles_c.4bpp.mtf"
	.global Tileset_CavernsOseniaTilesD
Tileset_CavernsOseniaTilesD:
	.incbin "games/tla/src/graphics/tileset/caverns_osenia/tiles_d.4bpp.mtf"
	.byte 0xc5, 0xde, 0x76	@ PACKER: left in the buffer by Tileset_CavernsOseniaTilesA+0x1839
