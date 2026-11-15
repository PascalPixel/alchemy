	.global Field_DehkanPlateau1
Field_DehkanPlateau1:
	map_header 0, 0, 128, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateau1, .LField_DehkanPlateau1_CELL_PICTURES
	map_block Field_DehkanPlateau1, .LField_DehkanPlateau1_SHAPES
	map_block Field_DehkanPlateau1, .LField_DehkanPlateau1_GRID
	map_block Field_DehkanPlateau1, .LField_DehkanPlateau1_SHAPE_GRID
	map_block Field_DehkanPlateau1, .LField_DehkanPlateau1_ANIMATION
	map_block Field_DehkanPlateau1, .LField_DehkanPlateau1_BLEND
	map_block Field_DehkanPlateau1, .LField_DehkanPlateau1_SCENE
.LField_DehkanPlateau1_CELL_PICTURES:
	.incbin "games/tla/src/map/dehkan_plateau_1/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateau1_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_1/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateau1_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/dehkan_plateau_1/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/dehkan_plateau_1/map.grid.lz"
	.endif
	.balign 4, 0
.LField_DehkanPlateau1_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateau1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateau1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateau1_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/dehkan_plateau_1/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/dehkan_plateau_1/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_DehkanPlateau1Palette
Tileset_DehkanPlateau1Palette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_1/palette.gbapal.lz"
	.byte 0xc2, 0x8d	@ PACKER: left in the buffer by Field_DehkanPlateau1+0x14e
	.global Tileset_DehkanPlateau1TilesA
Tileset_DehkanPlateau1TilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_1/tiles_a.4bpp.mtf"
	.global Tileset_DehkanPlateau1TilesB
Tileset_DehkanPlateau1TilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_1/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_DehkanPlateau1TilesA+0xfda
	.global Tileset_DehkanPlateau1TilesC
Tileset_DehkanPlateau1TilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_1/tiles_c.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Field_DehkanPlateau1+0x217f
	.global Tileset_DehkanPlateau1TilesD
Tileset_DehkanPlateau1TilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_1/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_DehkanPlateau1TilesC+0x13f
