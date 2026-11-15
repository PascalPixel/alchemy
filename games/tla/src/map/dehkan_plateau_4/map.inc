	.global Field_DehkanPlateau4
Field_DehkanPlateau4:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateau4, .LField_DehkanPlateau4_CELL_PICTURES
	map_block Field_DehkanPlateau4, .LField_DehkanPlateau4_SHAPES
	map_block Field_DehkanPlateau4, .LField_DehkanPlateau4_GRID
	map_block Field_DehkanPlateau4, .LField_DehkanPlateau4_SHAPE_GRID
	map_block Field_DehkanPlateau4, .LField_DehkanPlateau4_ANIMATION
	map_block Field_DehkanPlateau4, .LField_DehkanPlateau4_BLEND
	map_block Field_DehkanPlateau4, .LField_DehkanPlateau4_SCENE
.LField_DehkanPlateau4_CELL_PICTURES:
	.incbin "games/tla/src/map/dehkan_plateau_4/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateau4_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_4/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateau4_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_4/map.grid.lz"
	.balign 4, 0
.LField_DehkanPlateau4_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateau4_ANIMATION:
	.incbin "games/tla/src/map/dehkan_plateau_4/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateau4_BLEND:
	.incbin "games/tla/src/map/dehkan_plateau_4/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateau4_SCENE:
	.incbin "games/tla/src/map/dehkan_plateau_4/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DehkanPlateau4Palette
Tileset_DehkanPlateau4Palette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_4/palette.gbapal.lz"
	.byte 0x99, 0xac	@ PACKER: left in the buffer by Field_DehkanPlateau4+0x10a
	.global Tileset_DehkanPlateau4TilesA
Tileset_DehkanPlateau4TilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_4/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x3e	@ PACKER: left in the buffer by Tileset_DehkanPlateau3TilesC+0x1b56
	.global Tileset_DehkanPlateau4TilesB
Tileset_DehkanPlateau4TilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_4/tiles_b.4bpp.mtf"
	.global Tileset_DehkanPlateau4TilesC
Tileset_DehkanPlateau4TilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_4/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_DehkanPlateau4TilesB+0x1f5
	.global Tileset_DehkanPlateau4TilesD
Tileset_DehkanPlateau4TilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_4/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_DehkanPlateau4TilesC+0x1be
