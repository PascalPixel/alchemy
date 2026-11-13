	.global Field_Lemuria
Field_Lemuria:
	map_header 0, 0, 128, 160, 1, 1, 3, 1, 2, 1, 0, 0
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 3, 3, 0x1f, 0x1f
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Lemuria, .LField_Lemuria_CELL_PICTURES
	map_block Field_Lemuria, .LField_Lemuria_SHAPES
	map_block Field_Lemuria, .LField_Lemuria_GRID
	map_block Field_Lemuria, .LField_Lemuria_SHAPE_GRID
	map_block Field_Lemuria, .LField_Lemuria_ANIMATION
	map_block Field_Lemuria, .LField_Lemuria_BLEND
	map_block Field_Lemuria, .LField_Lemuria_SCENE
.LField_Lemuria_CELL_PICTURES:
	.incbin "games/tla/src/map/lemuria/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Lemuria_SHAPES:
	.incbin "games/tla/src/map/lemuria/shapes.table.lz"
	.balign 4, 0
.LField_Lemuria_GRID:
	.incbin "games/tla/src/map/lemuria/map.grid.lz"
	.balign 4, 0
.LField_Lemuria_SHAPE_GRID:
	.incbin "games/tla/src/map/lemuria/shape_grid.plane.lz"
	.balign 4, 0
.LField_Lemuria_ANIMATION:
	.incbin "games/tla/src/map/lemuria/animation.script.lz"
	.balign 4, 0
.LField_Lemuria_BLEND:
	.incbin "games/tla/src/map/lemuria/blend.script.lz"
	.balign 4, 0
.LField_Lemuria_SCENE:
	.incbin "games/tla/src/map/lemuria/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set82Palette
Tileset_Set82Palette:
	.incbin "games/tla/src/graphics/tileset/set_82/palette.gbapal.lz"
	.byte 0x64, 0x05	@ PACKER: left in the buffer by Field_Lemuria+0x1a6
	.global Tileset_Set82TilesA
Tileset_Set82TilesA:
	.incbin "games/tla/src/graphics/tileset/set_82/tiles_a.4bpp.mtf"
	.byte 0xfa, 0x10	@ PACKER: left in the buffer by Field_Lemuria+0x2c1a
	.global Tileset_Set82TilesB
Tileset_Set82TilesB:
	.incbin "games/tla/src/graphics/tileset/set_82/tiles_b.4bpp.mtf"
	.byte 0x0a, 0x1c, 0x4b	@ PACKER: left in the buffer by Field_Lemuria+0x2d31
	.global Tileset_Set82TilesC
Tileset_Set82TilesC:
	.incbin "games/tla/src/graphics/tileset/set_82/tiles_c.4bpp.mtf"
	.byte 0x24	@ PACKER: left in the buffer by Tileset_Set82TilesB+0x1ce7
	.global Tileset_Set82TilesD
Tileset_Set82TilesD:
	.incbin "games/tla/src/graphics/tileset/set_82/tiles_d.4bpp.mtf"
