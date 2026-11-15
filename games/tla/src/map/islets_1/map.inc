	.global Field_Islets1
Field_Islets1:
	map_header 0, 0, 50, 128, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, -1, -3, 0x3f, 0x3f
	map_block Field_Islets1, .LField_Islets1_CELL_PICTURES
	map_block Field_Islets1, .LField_Islets1_SHAPES
	map_block Field_Islets1, .LField_Islets1_GRID
	map_block Field_Islets1, .LField_Islets1_SHAPE_GRID
	map_block Field_Islets1, .LField_Islets1_ANIMATION
	map_block Field_Islets1, .LField_Islets1_BLEND
	map_block Field_Islets1, .LField_Islets1_SCENE
.LField_Islets1_CELL_PICTURES:
	.incbin "games/tla/src/map/islets_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Islets1_SHAPES:
	.incbin "games/tla/src/map/islets_1/shapes.table.lz"
	.balign 4, 0
.LField_Islets1_GRID:
	.incbin "games/tla/src/map/islets_1/map.grid.lz"
	.balign 4, 0
.LField_Islets1_SHAPE_GRID:
	.incbin "games/tla/src/map/islets_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_Islets1_ANIMATION:
	.incbin "games/tla/src/map/islets_1/animation.script.lz"
	.balign 4, 0
.LField_Islets1_BLEND:
	.incbin "games/tla/src/map/islets_1/blend.script.lz"
	.balign 4, 0
.LField_Islets1_SCENE:
	.incbin "games/tla/src/map/islets_1/scene.scene.lz"
	.global Tileset_Islets1Palette
Tileset_Islets1Palette:
	.incbin "games/tla/src/graphics/tileset/islets_1/palette.gbapal.lz"
	.byte 0x11, 0x98, 0x70	@ PACKER: left in the buffer by Field_Islets1+0x171
	.global Tileset_Islets1TilesA
Tileset_Islets1TilesA:
	.incbin "games/tla/src/graphics/tileset/islets_1/tiles_a.4bpp.mtf"
	.byte 0x4d, 0x25	@ PACKER: left in the buffer by Tileset_GabombaCatacombs1TilesA+0x2a36
	.global Tileset_Islets1TilesB
Tileset_Islets1TilesB:
	.incbin "games/tla/src/graphics/tileset/islets_1/tiles_b.4bpp.mtf"
	.byte 0x02	@ PACKER: left in the buffer by Tileset_Islets1TilesA+0x28cf
	.global Tileset_Islets1TilesC
Tileset_Islets1TilesC:
	.incbin "games/tla/src/graphics/tileset/islets_1/tiles_c.4bpp.mtf"
	.global Tileset_Islets1TilesD
Tileset_Islets1TilesD:
	.incbin "games/tla/src/graphics/tileset/islets_1/tiles_d.4bpp.mtf"
	.byte 0xf3	@ PACKER: left in the buffer by Tileset_Islets1TilesB+0x27d3
