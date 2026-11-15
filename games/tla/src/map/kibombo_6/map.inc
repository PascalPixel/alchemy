	.global Field_Kibombo6
Field_Kibombo6:
	map_header 0, 0, 128, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Kibombo6, .LField_Kibombo6_CELL_PICTURES
	map_block Field_Kibombo6, .LField_Kibombo6_SHAPES
	map_block Field_Kibombo6, .LField_Kibombo6_GRID
	map_block Field_Kibombo6, .LField_Kibombo6_SHAPE_GRID
	map_block Field_Kibombo6, .LField_Kibombo6_ANIMATION
	map_block Field_Kibombo6, .LField_Kibombo6_BLEND
	map_block Field_Kibombo6, .LField_Kibombo6_SCENE
.LField_Kibombo6_CELL_PICTURES:
	.incbin "games/tla/src/map/kibombo_6/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Kibombo6_SHAPES:
	.incbin "games/tla/src/map/kibombo_6/shapes.table.lz"
	.balign 4, 0
.LField_Kibombo6_GRID:
	.incbin "games/tla/src/map/kibombo_6/map.grid.lz"
	.balign 4, 0
.LField_Kibombo6_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_Kibombo6_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_Kibombo6_BLEND:
	.incbin "games/tla/src/map/gabomba_statue_arrows/blend.script.lz"
	.balign 4, 0
.LField_Kibombo6_SCENE:
	.incbin "games/tla/src/map/kibombo_6/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Kibombo6Palette
Tileset_Kibombo6Palette:
	.incbin "games/tla/src/graphics/tileset/kibombo_6/palette.gbapal.lz"
	.global Tileset_Kibombo6TilesA
Tileset_Kibombo6TilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_6/tiles_a.4bpp.mtf"
	.byte 0x00, 0x04	@ PACKER: left in the buffer by Field_Kibombo6+0x3d2
	.global Tileset_Kibombo6TilesB
Tileset_Kibombo6TilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_6/tiles_b.4bpp.mtf"
	.byte 0x4d, 0xcb, 0x82	@ PACKER: left in the buffer by Tileset_GabombaStatuePinionTilesA+0x1479
	.global Tileset_Kibombo6TilesC
Tileset_Kibombo6TilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_6/tiles_c.4bpp.mtf"
	.global Tileset_Kibombo6TilesD
Tileset_Kibombo6TilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_6/tiles_d.4bpp.mtf"
	.byte 0xdc	@ PACKER: left in the buffer by Tileset_Kibombo6TilesC+0x13f
