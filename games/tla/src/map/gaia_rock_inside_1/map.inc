	.global Field_GaiaRockInside1
Field_GaiaRockInside1:
	map_header 0, 0, 130, 112, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_GaiaRockInside1, .LField_GaiaRockInside1_CELL_PICTURES
	map_block Field_GaiaRockInside1, .LField_GaiaRockInside1_SHAPES
	map_block Field_GaiaRockInside1, .LField_GaiaRockInside1_GRID
	map_block Field_GaiaRockInside1, .LField_GaiaRockInside1_SHAPE_GRID
	map_block Field_GaiaRockInside1, .LField_GaiaRockInside1_ANIMATION
	map_block Field_GaiaRockInside1, .LField_GaiaRockInside1_BLEND
	map_block Field_GaiaRockInside1, .LField_GaiaRockInside1_SCENE
.LField_GaiaRockInside1_CELL_PICTURES:
	.incbin "games/tla/src/map/gaia_rock_inside_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GaiaRockInside1_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_inside_1/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockInside1_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_1/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockInside1_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockInside1_ANIMATION:
	.incbin "games/tla/src/map/gaia_rock_inside_1/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockInside1_BLEND:
	.incbin "games/tla/src/map/gaia_rock_inside_1/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockInside1_SCENE:
	.incbin "games/tla/src/map/gaia_rock_inside_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_GaiaRockInside1Palette
Tileset_GaiaRockInside1Palette:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_1/palette.gbapal.lz"
	.byte 0x18	@ PACKER: left in the buffer by Field_GaiaRockInside1+0x133
	.global Tileset_GaiaRockInside1TilesA
Tileset_GaiaRockInside1TilesA:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_1/tiles_a.4bpp.mtf"
	.byte 0x20	@ PACKER: left in the buffer by Field_GaiaRockInside1+0x1c6b
	.global Tileset_GaiaRockInside1TilesB
Tileset_GaiaRockInside1TilesB:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_1/tiles_b.4bpp.mtf"
	.byte 0x28	@ PACKER: left in the buffer by Tileset_GaiaRockInside1TilesA+0x11b7
	.global Tileset_GaiaRockInside1TilesC
Tileset_GaiaRockInside1TilesC:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_1/tiles_c.4bpp.mtf"
	.global Tileset_GaiaRockInside1TilesD
Tileset_GaiaRockInside1TilesD:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_1/tiles_d.4bpp.mtf"
	.byte 0x23, 0x70, 0x22	@ PACKER: left in the buffer by Tileset_GaiaRockInside1TilesC+0x639
