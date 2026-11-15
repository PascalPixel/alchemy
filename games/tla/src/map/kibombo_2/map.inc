	.global Field_Kibombo2
Field_Kibombo2:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Kibombo2, .LField_Kibombo2_CELL_PICTURES
	map_block Field_Kibombo2, .LField_Kibombo2_SHAPES
	map_block Field_Kibombo2, .LField_Kibombo2_GRID
	map_block Field_Kibombo2, .LField_Kibombo2_SHAPE_GRID
	map_block Field_Kibombo2, .LField_Kibombo2_ANIMATION
	map_block Field_Kibombo2, .LField_Kibombo2_BLEND
	map_block Field_Kibombo2, .LField_Kibombo2_SCENE
.LField_Kibombo2_CELL_PICTURES:
	.incbin "games/tla/src/map/kibombo_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Kibombo2_SHAPES:
	.incbin "games/tla/src/map/kibombo_2/shapes.table.lz"
	.balign 4, 0
.LField_Kibombo2_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/kibombo_2/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/kibombo_2/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Kibombo2_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_Kibombo2_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_Kibombo2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Kibombo2_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/kibombo_2/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/kibombo_2/scene.scene.lz"
	.endif
	.global Tileset_Kibombo2Palette
Tileset_Kibombo2Palette:
	.incbin "games/tla/src/graphics/tileset/kibombo_2/palette.gbapal.lz"
	.byte 0x3c, 0x88, 0x03	@ PACKER: left in the buffer by Field_Kibombo2+0x11d
	.global Tileset_Kibombo2TilesA
Tileset_Kibombo2TilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_2/tiles_a.4bpp.mtf"
	.byte 0x5b	@ PACKER: left in the buffer by Tileset_Kibombo1TilesA+0x28ab
	.global Tileset_Kibombo2TilesB
Tileset_Kibombo2TilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_2/tiles_b.4bpp.mtf"
	.byte 0xc8, 0xf7	@ PACKER: left in the buffer by Tileset_Kibombo2TilesA+0x1f92
	.global Tileset_Kibombo2TilesC
Tileset_Kibombo2TilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_2/tiles_c.4bpp.mtf"
	.byte 0xad	@ PACKER: left in the buffer by Tileset_Kibombo2TilesB+0x101f
	.global Tileset_Kibombo2TilesD
Tileset_Kibombo2TilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_2/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_Kibombo2TilesC+0x13f
