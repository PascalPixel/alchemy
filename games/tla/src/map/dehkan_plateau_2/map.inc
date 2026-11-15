	.global Field_DehkanPlateau2
Field_DehkanPlateau2:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateau2, .LField_DehkanPlateau2_CELL_PICTURES
	map_block Field_DehkanPlateau2, .LField_DehkanPlateau2_SHAPES
	map_block Field_DehkanPlateau2, .LField_DehkanPlateau2_GRID
	map_block Field_DehkanPlateau2, .LField_DehkanPlateau2_SHAPE_GRID
	map_block Field_DehkanPlateau2, .LField_DehkanPlateau2_ANIMATION
	map_block Field_DehkanPlateau2, .LField_DehkanPlateau2_BLEND
	map_block Field_DehkanPlateau2, .LField_DehkanPlateau2_SCENE
.LField_DehkanPlateau2_CELL_PICTURES:
	.incbin "games/tla/src/map/dehkan_plateau_2/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateau2_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_2/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateau2_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_2/map.grid.lz"
	.balign 4, 0
.LField_DehkanPlateau2_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateau2_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateau2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateau2_SCENE:
	.incbin "games/tla/src/map/dehkan_plateau_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DehkanPlateau2Palette
Tileset_DehkanPlateau2Palette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_2/palette.gbapal.lz"
	.byte 0x98	@ PACKER: left in the buffer by Field_DehkanPlateau2+0x14f
	.global Tileset_DehkanPlateau2TilesA
Tileset_DehkanPlateau2TilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_2/tiles_a.4bpp.mtf"
	.global Tileset_DehkanPlateau2TilesB
Tileset_DehkanPlateau2TilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_2/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_DehkanPlateau2TilesA+0xfda
	.global Tileset_DehkanPlateau2TilesC
Tileset_DehkanPlateau2TilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_2/tiles_c.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Field_DehkanPlateau2+0x217f
	.global Tileset_DehkanPlateau2TilesD
Tileset_DehkanPlateau2TilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_2/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_DehkanPlateau2TilesC+0x13f
