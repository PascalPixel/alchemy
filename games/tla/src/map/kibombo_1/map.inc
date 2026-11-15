	.global Field_Kibombo1
Field_Kibombo1:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Kibombo1, .LField_Kibombo1_CELL_PICTURES
	map_block Field_Kibombo1, .LField_Kibombo1_SHAPES
	map_block Field_Kibombo1, .LField_Kibombo1_GRID
	map_block Field_Kibombo1, .LField_Kibombo1_SHAPE_GRID
	map_block Field_Kibombo1, .LField_Kibombo1_ANIMATION
	map_block Field_Kibombo1, .LField_Kibombo1_BLEND
	map_block Field_Kibombo1, .LField_Kibombo1_SCENE
.LField_Kibombo1_CELL_PICTURES:
	.incbin "games/tla/src/map/kibombo_1/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Kibombo1_SHAPES:
	.incbin "games/tla/src/map/kibombo_1/shapes.table.lz"
	.balign 4, 0
.LField_Kibombo1_GRID:
	.incbin "games/tla/src/map/kibombo_1/map.grid.lz"
	.balign 4, 0
.LField_Kibombo1_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_Kibombo1_ANIMATION:
	.incbin "games/tla/src/map/kibombo_1/animation.script.lz"
	.balign 4, 0
.LField_Kibombo1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Kibombo1_SCENE:
	.incbin "games/tla/src/map/kibombo_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Kibombo1Palette
Tileset_Kibombo1Palette:
	.incbin "games/tla/src/graphics/tileset/kibombo_1/palette.gbapal.lz"
	.byte 0x5f, 0x52	@ PACKER: left in the buffer by Field_Kibombo1+0x156
	.global Tileset_Kibombo1TilesA
Tileset_Kibombo1TilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_1/tiles_a.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0xd4, 0x0e, 0x40	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.else
	.byte 0xef, 0x12, 0x40	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.endif
	.global Tileset_Kibombo1TilesB
Tileset_Kibombo1TilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_1/tiles_b.4bpp.mtf"
	.byte 0x8e, 0x16, 0x18	@ PACKER: left in the buffer by Tileset_Kibombo1TilesA+0x2221
	.global Tileset_Kibombo1TilesC
Tileset_Kibombo1TilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_1/tiles_c.4bpp.mtf"
	.global Tileset_Kibombo1TilesD
Tileset_Kibombo1TilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_1/tiles_d.4bpp.mtf"
	.byte 0xe0, 0x75	@ PACKER: left in the buffer by Tileset_Kibombo1TilesC+0x30a
