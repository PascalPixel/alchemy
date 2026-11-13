	.global Field_OseniaCliffs
Field_OseniaCliffs:
	map_header 0, 0, 74, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 74, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_OseniaCliffs, .LField_OseniaCliffs_CELL_PICTURES
	map_block Field_OseniaCliffs, .LField_OseniaCliffs_SHAPES
	map_block Field_OseniaCliffs, .LField_OseniaCliffs_GRID
	map_block Field_OseniaCliffs, .LField_OseniaCliffs_SHAPE_GRID
	map_block Field_OseniaCliffs, .LField_OseniaCliffs_ANIMATION
	map_block Field_OseniaCliffs, .LField_OseniaCliffs_BLEND
	map_block Field_OseniaCliffs, .LField_OseniaCliffs_SCENE
.LField_OseniaCliffs_CELL_PICTURES:
	.incbin "games/tla/src/map/osenia_cliffs/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_OseniaCliffs_SHAPES:
	.incbin "games/tla/src/map/osenia_cliffs/shapes.table.lz"
	.balign 4, 0
.LField_OseniaCliffs_GRID:
	.incbin "games/tla/src/map/osenia_cliffs/map.grid.lz"
	.balign 4, 0
.LField_OseniaCliffs_SHAPE_GRID:
	.incbin "games/tla/src/map/osenia_cliffs/shape_grid.plane.lz"
	.balign 4, 0
.LField_OseniaCliffs_ANIMATION:
	.incbin "games/tla/src/map/osenia_cliffs/animation.script.lz"
	.balign 4, 0
.LField_OseniaCliffs_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_OseniaCliffs_SCENE:
	.incbin "games/tla/src/map/osenia_cliffs/scene.scene.lz"
	.balign 4, 0
	.global Tileset_OseniaCliffsPalette
Tileset_OseniaCliffsPalette:
	.incbin "games/tla/src/graphics/tileset/osenia_cliffs/palette.gbapal.lz"
	.global Tileset_OseniaCliffsTilesA
Tileset_OseniaCliffsTilesA:
	.incbin "games/tla/src/graphics/tileset/osenia_cliffs/tiles_a.4bpp.mtf"
	.byte 0x61	@ PACKER: left in the buffer by Field_OseniaCliffs+0x1a2b
	.global Tileset_OseniaCliffsTilesB
Tileset_OseniaCliffsTilesB:
	.incbin "games/tla/src/graphics/tileset/osenia_cliffs/tiles_b.4bpp.mtf"
	.byte 0x5a, 0x14	@ PACKER: left in the buffer by Tileset_Set29TilesC+0x2336
	.global Tileset_OseniaCliffsTilesC
Tileset_OseniaCliffsTilesC:
	.incbin "games/tla/src/graphics/tileset/osenia_cliffs/tiles_c.4bpp.mtf"
	.byte 0x9d	@ PACKER: left in the buffer by Tileset_OseniaCliffsTilesB+0x11af
	.global Tileset_OseniaCliffsTilesD
Tileset_OseniaCliffsTilesD:
	.incbin "games/tla/src/graphics/tileset/osenia_cliffs/tiles_d.4bpp.mtf"
	.byte 0x53, 0xb6, 0x22	@ PACKER: left in the buffer by Tileset_OseniaCliffsTilesB+0x1921
