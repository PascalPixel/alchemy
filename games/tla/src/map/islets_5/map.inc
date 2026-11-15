	.global Field_Islets5
Field_Islets5:
	map_header 0, 0, 58, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Islets5, .LField_Islets5_CELL_PICTURES
	map_block Field_Islets5, .LField_Islets5_SHAPES
	map_block Field_Islets5, .LField_Islets5_GRID
	map_block Field_Islets5, .LField_Islets5_SHAPE_GRID
	map_block Field_Islets5, .LField_Islets5_ANIMATION
	map_block Field_Islets5, .LField_Islets5_BLEND
	map_block Field_Islets5, .LField_Islets5_SCENE
.LField_Islets5_CELL_PICTURES:
	.incbin "games/tla/src/map/islets_5/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Islets5_SHAPES:
	.incbin "games/tla/src/map/islets_5/shapes.table.lz"
	.balign 4, 0
.LField_Islets5_GRID:
	.incbin "games/tla/src/map/islets_5/map.grid.lz"
	.balign 4, 0
.LField_Islets5_SHAPE_GRID:
	.incbin "games/tla/src/map/islets_5/shape_grid.plane.lz"
	.balign 4, 0
.LField_Islets5_ANIMATION:
	.incbin "games/tla/src/map/islets_5/animation.script.lz"
	.balign 4, 0
.LField_Islets5_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Islets5_SCENE:
	.incbin "games/tla/src/map/islets_5/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Islets5Palette
Tileset_Islets5Palette:
	.incbin "games/tla/src/graphics/tileset/islets_5/palette.gbapal.lz"
	.byte 0x80, 0x0e	@ PACKER: left in the buffer by Field_Islets5+0x13a
	.global Tileset_Islets5TilesA
Tileset_Islets5TilesA:
	.incbin "games/tla/src/graphics/tileset/islets_5/tiles_a.4bpp.mtf"
	.byte 0x06, 0x0d, 0x54	@ PACKER: left in the buffer by Field_Islets5+0x1505
	.global Tileset_Islets5TilesB
Tileset_Islets5TilesB:
	.incbin "games/tla/src/graphics/tileset/islets_5/tiles_b.4bpp.mtf"
	.global Tileset_Islets5TilesC
Tileset_Islets5TilesC:
	.incbin "games/tla/src/graphics/tileset/islets_5/tiles_c.4bpp.mtf"
	.byte 0x00, 0x5c	@ PACKER: left in the buffer by Tileset_Islets5TilesB+0x15e2
	.global Tileset_Islets5TilesD
Tileset_Islets5TilesD:
	.incbin "games/tla/src/graphics/tileset/islets_5/tiles_d.4bpp.mtf"
	.byte 0xa1, 0x4f, 0x64	@ PACKER: left in the buffer by Tileset_Islets5TilesC+0x1445
