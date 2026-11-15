	.global Field_DehkanPlateau3
Field_DehkanPlateau3:
	map_header 0, 0, 128, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateau3, .LField_DehkanPlateau3_CELL_PICTURES
	map_block Field_DehkanPlateau3, .LField_DehkanPlateau3_SHAPES
	map_block Field_DehkanPlateau3, .LField_DehkanPlateau3_GRID
	map_block Field_DehkanPlateau3, .LField_DehkanPlateau3_SHAPE_GRID
	map_block Field_DehkanPlateau3, .LField_DehkanPlateau3_ANIMATION
	map_block Field_DehkanPlateau3, .LField_DehkanPlateau3_BLEND
	map_block Field_DehkanPlateau3, .LField_DehkanPlateau3_SCENE
.LField_DehkanPlateau3_CELL_PICTURES:
	.incbin "games/tla/src/map/dehkan_plateau_3/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateau3_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_3/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateau3_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_3/map.grid.lz"
	.balign 4, 0
.LField_DehkanPlateau3_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateau3_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateau3_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateau3_SCENE:
	.incbin "games/tla/src/map/dehkan_plateau_3/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DehkanPlateau3Palette
Tileset_DehkanPlateau3Palette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_3/palette.gbapal.lz"
	.global Tileset_DehkanPlateau3TilesA
Tileset_DehkanPlateau3TilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_3/tiles_a.4bpp.mtf"
	.global Tileset_DehkanPlateau3TilesB
Tileset_DehkanPlateau3TilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_3/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_DehkanPlateau3TilesA+0xfda
	.global Tileset_DehkanPlateau3TilesC
Tileset_DehkanPlateau3TilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_3/tiles_c.4bpp.mtf"
	.byte 0xd4	@ PACKER: left in the buffer by Field_DehkanPlateau3+0x217f
	.global Tileset_DehkanPlateau3TilesD
Tileset_DehkanPlateau3TilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_3/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_DehkanPlateau3TilesC+0x13f
