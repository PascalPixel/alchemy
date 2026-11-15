	.global Field_YampiDesert1
Field_YampiDesert1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesert1, .LField_YampiDesert1_CELL_PICTURES
	map_block Field_YampiDesert1, .LField_YampiDesert1_SHAPES
	map_block Field_YampiDesert1, .LField_YampiDesert1_GRID
	map_block Field_YampiDesert1, .LField_YampiDesert1_SHAPE_GRID
	map_block Field_YampiDesert1, .LField_YampiDesert1_ANIMATION
	map_block Field_YampiDesert1, .LField_YampiDesert1_BLEND
	map_block Field_YampiDesert1, .LField_YampiDesert1_SCENE
.LField_YampiDesert1_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesert1_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_1/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesert1_GRID:
	.incbin "games/tla/src/map/yampi_desert_1/map.grid.lz"
	.balign 4, 0
.LField_YampiDesert1_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesert1_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_1/animation.script.lz"
	.balign 4, 0
.LField_YampiDesert1_BLEND:
	.incbin "games/tla/src/map/yampi_desert_1/blend.script.lz"
	.balign 4, 0
.LField_YampiDesert1_SCENE:
	.incbin "games/tla/src/map/yampi_desert_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_YampiDesert1Palette
Tileset_YampiDesert1Palette:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_1/palette.gbapal.lz"
	.byte 0x87, 0x37, 0x18	@ PACKER: left in the buffer by Field_YampiDesert1+0x12d
	.global Tileset_YampiDesert1TilesA
Tileset_YampiDesert1TilesA:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_1/tiles_a.4bpp.mtf"
	.byte 0xe6, 0x08	@ PACKER: left in the buffer by Tileset_MadraDrawbridgeTilesA+0x2ab6
	.global Tileset_YampiDesert1TilesB
Tileset_YampiDesert1TilesB:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_1/tiles_b.4bpp.mtf"
	.byte 0xa9, 0x03	@ PACKER: left in the buffer by Tileset_YampiDesert1TilesA+0x1b56
	.global Tileset_YampiDesert1TilesC
Tileset_YampiDesert1TilesC:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_1/tiles_c.4bpp.mtf"
	.byte 0xf5, 0xc0, 0x8e	@ PACKER: left in the buffer by Tileset_YampiDesert1TilesB+0xf45
	.global Tileset_YampiDesert1TilesD
Tileset_YampiDesert1TilesD:
	.incbin "games/tla/src/graphics/tileset/yampi_desert_1/tiles_d.4bpp.mtf"
	.byte 0x6f, 0xd1	@ PACKER: left in the buffer by Tileset_YampiDesert1TilesB+0x1a06
