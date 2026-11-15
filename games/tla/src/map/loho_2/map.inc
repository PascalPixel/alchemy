	.global Field_Loho2
Field_Loho2:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Loho2, .LField_Loho2_CELL_PICTURES
	map_block Field_Loho2, .LField_Loho2_SHAPES
	map_block Field_Loho2, .LField_Loho2_GRID
	map_block Field_Loho2, .LField_Loho2_SHAPE_GRID
	map_block Field_Loho2, .LField_Loho2_ANIMATION
	map_block Field_Loho2, .LField_Loho2_BLEND
	map_block Field_Loho2, .LField_Loho2_SCENE
.LField_Loho2_CELL_PICTURES:
	.incbin "games/tla/src/map/loho_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Loho2_SHAPES:
	.incbin "games/tla/src/map/loho_2/shapes.table.lz"
	.balign 4, 0
.LField_Loho2_GRID:
	.incbin "games/tla/src/map/loho_2/map.grid.lz"
	.balign 4, 0
.LField_Loho2_SHAPE_GRID:
	.incbin "games/tla/src/map/loho_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_Loho2_ANIMATION:
	.incbin "games/tla/src/map/loho_2/animation.script.lz"
	.balign 4, 0
.LField_Loho2_BLEND:
	.incbin "games/tla/src/map/daila_interiors_1/blend.script.lz"
	.balign 4, 0
.LField_Loho2_SCENE:
	.incbin "games/tla/src/map/loho_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Loho2Palette
Tileset_Loho2Palette:
	.incbin "games/tla/src/graphics/tileset/loho_2/palette.gbapal.lz"
	.global Tileset_Loho2TilesA
Tileset_Loho2TilesA:
	.incbin "games/tla/src/graphics/tileset/loho_2/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_Loho2+0x14fb
	.global Tileset_Loho2TilesB
Tileset_Loho2TilesB:
	.incbin "games/tla/src/graphics/tileset/loho_2/tiles_b.4bpp.mtf"
	.byte 0xb5	@ PACKER: left in the buffer by Field_Loho2+0x1bc7
	.global Tileset_Loho2TilesC
Tileset_Loho2TilesC:
	.incbin "games/tla/src/graphics/tileset/loho_2/tiles_c.4bpp.mtf"
	.byte 0x95, 0x3b	@ PACKER: left in the buffer by Tileset_Loho2TilesB+0x1b7a
	.global Tileset_Loho2TilesD
Tileset_Loho2TilesD:
	.incbin "games/tla/src/graphics/tileset/loho_2/tiles_d.4bpp.mtf"
	.byte 0x82, 0x94, 0x06	@ PACKER: left in the buffer by Tileset_Loho2TilesC+0x449
