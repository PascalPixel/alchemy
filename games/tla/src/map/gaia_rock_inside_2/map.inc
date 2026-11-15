	.global Field_GaiaRockInside2
Field_GaiaRockInside2:
	map_header 0, 0, 130, 112, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GaiaRockInside2, .LField_GaiaRockInside2_CELL_PICTURES
	map_block Field_GaiaRockInside2, .LField_GaiaRockInside2_SHAPES
	map_block Field_GaiaRockInside2, .LField_GaiaRockInside2_GRID
	map_block Field_GaiaRockInside2, .LField_GaiaRockInside2_SHAPE_GRID
	map_block Field_GaiaRockInside2, .LField_GaiaRockInside2_ANIMATION
	map_block Field_GaiaRockInside2, .LField_GaiaRockInside2_BLEND
	map_block Field_GaiaRockInside2, .LField_GaiaRockInside2_SCENE
.LField_GaiaRockInside2_CELL_PICTURES:
	.incbin "games/tla/src/map/gaia_rock_inside_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GaiaRockInside2_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_inside_2/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockInside2_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_2/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockInside2_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockInside2_ANIMATION:
	.incbin "games/tla/src/map/gaia_rock_inside_2/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockInside2_BLEND:
	.incbin "games/tla/src/map/gaia_rock_inside_2/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockInside2_SCENE:
	.incbin "games/tla/src/map/gaia_rock_inside_2/scene.scene.lz"
	.global Tileset_GaiaRockInside2Palette
Tileset_GaiaRockInside2Palette:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_2/palette.gbapal.lz"
	.byte 0x5c, 0x13	@ PACKER: left in the buffer by Field_GaiaRockInside2+0x11e
	.global Tileset_GaiaRockInside2TilesA
Tileset_GaiaRockInside2TilesA:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_2/tiles_a.4bpp.mtf"
	.byte 0x1f	@ PACKER: left in the buffer by Tileset_GaiaRockInside1TilesA+0x1af7
	.global Tileset_GaiaRockInside2TilesB
Tileset_GaiaRockInside2TilesB:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_2/tiles_b.4bpp.mtf"
	.byte 0xa2	@ PACKER: left in the buffer by Tileset_GaiaRockInside2TilesA+0x140b
	.global Tileset_GaiaRockInside2TilesC
Tileset_GaiaRockInside2TilesC:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_2/tiles_c.4bpp.mtf"
	.byte 0x02	@ PACKER: left in the buffer by Tileset_GaiaRockInside2TilesB+0x553
	.global Tileset_GaiaRockInside2TilesD
Tileset_GaiaRockInside2TilesD:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_2/tiles_d.4bpp.mtf"
	.byte 0xb9, 0x40, 0x00	@ PACKER: left in the buffer by Tileset_GaiaRockInside2TilesB+0x639
