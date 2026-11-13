	.global Field_EasternAlhafra
Field_EasternAlhafra:
	map_header 0, 0, 128, 128, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_EasternAlhafra, .LField_EasternAlhafra_CELL_PICTURES
	map_block Field_EasternAlhafra, .LField_EasternAlhafra_SHAPES
	map_block Field_EasternAlhafra, .LField_EasternAlhafra_GRID
	map_block Field_EasternAlhafra, .LField_EasternAlhafra_SHAPE_GRID
	map_block Field_EasternAlhafra, .LField_EasternAlhafra_ANIMATION
	map_block Field_EasternAlhafra, .LField_EasternAlhafra_BLEND
	map_block Field_EasternAlhafra, .LField_EasternAlhafra_SCENE
.LField_EasternAlhafra_CELL_PICTURES:
	.incbin "games/tla/src/map/eastern_alhafra/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_EasternAlhafra_SHAPES:
	.incbin "games/tla/src/map/eastern_alhafra/shapes.table.lz"
	.balign 4, 0
.LField_EasternAlhafra_GRID:
	.incbin "games/tla/src/map/eastern_alhafra/map.grid.lz"
	.balign 4, 0
.LField_EasternAlhafra_SHAPE_GRID:
	.incbin "games/tla/src/map/eastern_alhafra/shape_grid.plane.lz"
	.balign 4, 0
.LField_EasternAlhafra_ANIMATION:
	.incbin "games/tla/src/map/eastern_alhafra/animation.script.lz"
	.balign 4, 0
.LField_EasternAlhafra_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_EasternAlhafra_SCENE:
	.incbin "games/tla/src/map/eastern_alhafra/scene.scene.lz"
	.balign 4, 0
	.global Tileset_EasternAlhafraPalette
Tileset_EasternAlhafraPalette:
	.incbin "games/tla/src/graphics/tileset/eastern_alhafra/palette.gbapal.lz"
	.byte 0x0d	@ PACKER: left in the buffer by Field_EasternAlhafra+0x147
	.global Tileset_EasternAlhafraTilesA
Tileset_EasternAlhafraTilesA:
	.incbin "games/tla/src/graphics/tileset/eastern_alhafra/tiles_a.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Field_AlhafraManor+0x26af
	.global Tileset_EasternAlhafraTilesB
Tileset_EasternAlhafraTilesB:
	.incbin "games/tla/src/graphics/tileset/eastern_alhafra/tiles_b.4bpp.mtf"
	.balign 4, 0xff	@ PACKER: 0xff fill, not an earlier stream's bytes
	.global Tileset_EasternAlhafraTilesC
Tileset_EasternAlhafraTilesC:
	.incbin "games/tla/src/graphics/tileset/eastern_alhafra/tiles_c.4bpp.mtf"
	.byte 0x8a, 0xbc, 0xc1	@ PACKER: left in the buffer by Tileset_EasternAlhafraTilesB+0xd89
	.global Tileset_EasternAlhafraTilesD
Tileset_EasternAlhafraTilesD:
	.incbin "games/tla/src/graphics/tileset/eastern_alhafra/tiles_d.4bpp.mtf"
