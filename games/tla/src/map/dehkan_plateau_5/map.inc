	.global Field_DehkanPlateau5
Field_DehkanPlateau5:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateau5, .LField_DehkanPlateau5_CELL_PICTURES
	map_block Field_DehkanPlateau5, .LField_DehkanPlateau5_SHAPES
	map_block Field_DehkanPlateau5, .LField_DehkanPlateau5_GRID
	map_block Field_DehkanPlateau5, .LField_DehkanPlateau5_SHAPE_GRID
	map_block Field_DehkanPlateau5, .LField_DehkanPlateau5_ANIMATION
	map_block Field_DehkanPlateau5, .LField_DehkanPlateau5_BLEND
	map_block Field_DehkanPlateau5, .LField_DehkanPlateau5_SCENE
.LField_DehkanPlateau5_CELL_PICTURES:
	.incbin "games/tla/src/map/dehkan_plateau_5/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateau5_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_5/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateau5_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_5/map.grid.lz"
	.balign 4, 0
.LField_DehkanPlateau5_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_5/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateau5_ANIMATION:
	.incbin "games/tla/src/map/dehkan_plateau_4/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateau5_BLEND:
	.incbin "games/tla/src/map/dehkan_plateau_4/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateau5_SCENE:
	.incbin "games/tla/src/map/dehkan_plateau_5/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DehkanPlateau5Palette
Tileset_DehkanPlateau5Palette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_5/palette.gbapal.lz"
	.global Tileset_DehkanPlateau5TilesA
Tileset_DehkanPlateau5TilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_5/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x3e	@ PACKER: left in the buffer by Tileset_DehkanPlateau4TilesA+0x1b56
	.global Tileset_DehkanPlateau5TilesB
Tileset_DehkanPlateau5TilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_5/tiles_b.4bpp.mtf"
	.global Tileset_DehkanPlateau5TilesC
Tileset_DehkanPlateau5TilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_5/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_DehkanPlateau5TilesB+0x1f5
	.global Tileset_DehkanPlateau5TilesD
Tileset_DehkanPlateau5TilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_5/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_DehkanPlateau5TilesC+0x1be
