	.global Field_Kibombo4
Field_Kibombo4:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Kibombo4, .LField_Kibombo4_CELL_PICTURES
	map_block Field_Kibombo4, .LField_Kibombo4_SHAPES
	map_block Field_Kibombo4, .LField_Kibombo4_GRID
	map_block Field_Kibombo4, .LField_Kibombo4_SHAPE_GRID
	map_block Field_Kibombo4, .LField_Kibombo4_ANIMATION
	map_block Field_Kibombo4, .LField_Kibombo4_BLEND
	map_block Field_Kibombo4, .LField_Kibombo4_SCENE
.LField_Kibombo4_CELL_PICTURES:
	.incbin "games/tla/src/map/kibombo_4/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Kibombo4_SHAPES:
	.incbin "games/tla/src/map/kibombo_4/shapes.table.lz"
	.balign 4, 0
.LField_Kibombo4_GRID:
	.incbin "games/tla/src/map/kibombo_4/map.grid.lz"
	.balign 4, 0
.LField_Kibombo4_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_Kibombo4_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_Kibombo4_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Kibombo4_SCENE:
	.incbin "games/tla/src/map/kibombo_4/scene.scene.lz"
	.global Tileset_Kibombo4Palette
Tileset_Kibombo4Palette:
	.incbin "games/tla/src/graphics/tileset/kibombo_4/palette.gbapal.lz"
	.byte 0xd0, 0xe0	@ PACKER: left in the buffer by Field_Kibombo4+0x166
	.global Tileset_Kibombo4TilesA
Tileset_Kibombo4TilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_4/tiles_a.4bpp.mtf"
	.byte 0x0c, 0x09	@ PACKER: left in the buffer by Field_Kibombo4+0x2482
	.global Tileset_Kibombo4TilesB
Tileset_Kibombo4TilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_4/tiles_b.4bpp.mtf"
	.byte 0x0f, 0xbc, 0x0e	@ PACKER: left in the buffer by Tileset_Kibombo4TilesA+0x20e5
	.global Tileset_Kibombo4TilesC
Tileset_Kibombo4TilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_4/tiles_c.4bpp.mtf"
	.global Tileset_Kibombo4TilesD
Tileset_Kibombo4TilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_4/tiles_d.4bpp.mtf"
	.byte 0x7c, 0xfe, 0x81	@ PACKER: left in the buffer by Tileset_Kibombo4TilesC+0x3a1
