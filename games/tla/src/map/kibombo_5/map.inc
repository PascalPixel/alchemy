	.global Field_Kibombo5
Field_Kibombo5:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Kibombo5, .LField_Kibombo5_CELL_PICTURES
	map_block Field_Kibombo5, .LField_Kibombo5_SHAPES
	map_block Field_Kibombo5, .LField_Kibombo5_GRID
	map_block Field_Kibombo5, .LField_Kibombo5_SHAPE_GRID
	map_block Field_Kibombo5, .LField_Kibombo5_ANIMATION
	map_block Field_Kibombo5, .LField_Kibombo5_BLEND
	map_block Field_Kibombo5, .LField_Kibombo5_SCENE
.LField_Kibombo5_CELL_PICTURES:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/kibombo_5/cell_pictures_ja.delta1.lz"
	.else
	.incbin "games/tla/src/map/kibombo_5/cell_pictures.delta1.lz"
	.endif
	.balign 4, 0
.LField_Kibombo5_SHAPES:
	.incbin "games/tla/src/map/kibombo_2/shapes.table.lz"
	.balign 4, 0
.LField_Kibombo5_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/kibombo_5/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/kibombo_5/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Kibombo5_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_Kibombo5_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_Kibombo5_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Kibombo5_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/kibombo_5/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/kibombo_5/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_Kibombo5Palette
Tileset_Kibombo5Palette:
	.incbin "games/tla/src/graphics/tileset/kibombo_5/palette.gbapal.lz"
	.byte 0x3c, 0x78, 0xf0	@ PACKER: left in the buffer by Field_Kibombo5+0x165
	.global Tileset_Kibombo5TilesA
Tileset_Kibombo5TilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_5/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_Kibombo4+0x27f7
	.global Tileset_Kibombo5TilesB
Tileset_Kibombo5TilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_5/tiles_b.4bpp.mtf"
	.byte 0x08	@ PACKER: left in the buffer by Tileset_Kibombo5TilesA+0x2033
	.global Tileset_Kibombo5TilesC
Tileset_Kibombo5TilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_5/tiles_c.4bpp.mtf"
	.global Tileset_Kibombo5TilesD
Tileset_Kibombo5TilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_5/tiles_d.4bpp.mtf"
	.byte 0xe0, 0x7c	@ PACKER: left in the buffer by Tileset_Kibombo5TilesC+0xbae
