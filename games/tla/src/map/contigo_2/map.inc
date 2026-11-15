	.global Field_Contigo2
Field_Contigo2:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Contigo2, .LField_Contigo2_CELL_PICTURES
	map_block Field_Contigo2, .LField_Contigo2_SHAPES
	map_block Field_Contigo2, .LField_Contigo2_GRID
	map_block Field_Contigo2, .LField_Contigo2_SHAPE_GRID
	map_block Field_Contigo2, .LField_Contigo2_ANIMATION
	map_block Field_Contigo2, .LField_Contigo2_BLEND
	map_block Field_Contigo2, .LField_Contigo2_SCENE
.LField_Contigo2_CELL_PICTURES:
	.incbin "games/tla/src/map/contigo_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Contigo2_SHAPES:
	.incbin "games/tla/src/map/contigo_2/shapes.table.lz"
	.balign 4, 0
.LField_Contigo2_GRID:
	.incbin "games/tla/src/map/contigo_2/map.grid.lz"
	.balign 4, 0
.LField_Contigo2_SHAPE_GRID:
	.incbin "games/tla/src/map/contigo_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_Contigo2_ANIMATION:
	.incbin "games/tla/src/map/contigo_2/animation.script.lz"
	.balign 4, 0
.LField_Contigo2_BLEND:
	.incbin "games/tla/src/map/daila_interiors_1/blend.script.lz"
	.balign 4, 0
.LField_Contigo2_SCENE:
	.incbin "games/tla/src/map/contigo_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Contigo2Palette
Tileset_Contigo2Palette:
	.incbin "games/tla/src/graphics/tileset/contigo_2/palette.gbapal.lz"
	.byte 0x80, 0x07, 0x98	@ PACKER: left in the buffer by Field_Contigo2+0x1d5
	.global Tileset_Contigo2TilesA
Tileset_Contigo2TilesA:
	.incbin "games/tla/src/graphics/tileset/contigo_2/tiles_a.4bpp.mtf"
	.global Tileset_Contigo2TilesB
Tileset_Contigo2TilesB:
	.incbin "games/tla/src/graphics/tileset/contigo_2/tiles_b.4bpp.mtf"
	.byte 0x5b, 0x44, 0x03	@ PACKER: left in the buffer by Tileset_Contigo2TilesA+0x1679
	.global Tileset_Contigo2TilesC
Tileset_Contigo2TilesC:
	.incbin "games/tla/src/graphics/tileset/contigo_2/tiles_c.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0xff, 0xf3	@ PACKER: left in the buffer by Tileset_Contigo2TilesA+0x1751
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.global Tileset_Contigo2TilesD
Tileset_Contigo2TilesD:
	.incbin "games/tla/src/graphics/tileset/contigo_2/tiles_d.4bpp.mtf"
	.global Tileset_PaletteContigo2
Tileset_PaletteContigo2:
	.incbin "games/tla/src/graphics/tileset/palettes/contigo_2.gbapal.lz"
	.byte 0xa7	@ PACKER: left in the buffer by Tileset_Contigo2TilesD+0x1cb
