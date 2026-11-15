	.global Field_Caverns2
Field_Caverns2:
	map_header 0, 0, 128, 128, 3, 2, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 100, 100, 0, 0, 0xff, 0xff
	map_block Field_Caverns2, .LField_Caverns2_CELL_PICTURES
	map_block Field_Caverns2, .LField_Caverns2_SHAPES
	map_block Field_Caverns2, .LField_Caverns2_GRID
	map_block Field_Caverns2, .LField_Caverns2_SHAPE_GRID
	map_block Field_Caverns2, .LField_Caverns2_ANIMATION
	map_block Field_Caverns2, .LField_Caverns2_BLEND
	map_block Field_Caverns2, .LField_Caverns2_SCENE
.LField_Caverns2_CELL_PICTURES:
	.incbin "games/tla/src/map/caverns_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Caverns2_SHAPES:
	.incbin "games/tla/src/map/caverns_2/shapes.table.lz"
	.balign 4, 0
.LField_Caverns2_GRID:
	.incbin "games/tla/src/map/caverns_2/map.grid.lz"
	.balign 4, 0
.LField_Caverns2_SHAPE_GRID:
	.incbin "games/tla/src/map/caverns_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_Caverns2_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_Caverns2_BLEND:
	.incbin "games/tla/src/map/yampi_desert_1/blend.script.lz"
	.balign 4, 0
.LField_Caverns2_SCENE:
	.incbin "games/tla/src/map/caverns_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Caverns2Palette
Tileset_Caverns2Palette:
	.incbin "games/tla/src/graphics/tileset/caverns_2/palette.gbapal.lz"
	.byte 0x74, 0x6c, 0x5a	@ PACKER: left in the buffer by Field_Caverns2+0x121
	.global Tileset_Caverns2TilesA
Tileset_Caverns2TilesA:
	.incbin "games/tla/src/graphics/tileset/caverns_2/tiles_a.4bpp.mtf"
	.byte 0xe2	@ PACKER: left in the buffer by Tileset_IsletCaveMeldTilesA+0x2afb
	.global Tileset_Caverns2TilesB
Tileset_Caverns2TilesB:
	.incbin "games/tla/src/graphics/tileset/caverns_2/tiles_b.4bpp.mtf"
	.byte 0xe0, 0x28	@ PACKER: left in the buffer by Tileset_Caverns2TilesA+0xdf6
	.global Tileset_Caverns2TilesC
Tileset_Caverns2TilesC:
	.incbin "games/tla/src/graphics/tileset/caverns_2/tiles_c.4bpp.mtf"
	.global Tileset_Caverns2TilesD
Tileset_Caverns2TilesD:
	.incbin "games/tla/src/graphics/tileset/caverns_2/tiles_d.4bpp.mtf"
	.byte 0xc5, 0xde, 0x76	@ PACKER: left in the buffer by Tileset_Caverns2TilesA+0x1839
