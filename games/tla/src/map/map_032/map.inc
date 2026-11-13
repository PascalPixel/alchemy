	.global Field_Map032
Field_Map032:
	map_header 0, 0, 128, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map032, .LField_Map032_CELL_PICTURES
	map_block Field_Map032, .LField_Map032_SHAPES
	map_block Field_Map032, .LField_Map032_GRID
	map_block Field_Map032, .LField_Map032_SHAPE_GRID
	map_block Field_Map032, .LField_Map032_ANIMATION
	map_block Field_Map032, .LField_Map032_BLEND
	map_block Field_Map032, .LField_Map032_SCENE
.LField_Map032_CELL_PICTURES:
	.incbin "games/tla/src/map/map_032/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map032_SHAPES:
	.incbin "games/tla/src/map/map_032/shapes.table.lz"
	.balign 4, 0
.LField_Map032_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_032/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/map_032/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Map032_SHAPE_GRID:
	.incbin "games/tla/src/map/map_032/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map032_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map032_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map032_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_032/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/map_032/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_Set15Palette
Tileset_Set15Palette:
	.incbin "games/tla/src/graphics/tileset/set_15/palette.gbapal.lz"
	.byte 0xc2, 0x8d	@ PACKER: left in the buffer by Field_Map032+0x14e
	.global Tileset_Set15TilesA
Tileset_Set15TilesA:
	.incbin "games/tla/src/graphics/tileset/set_15/tiles_a.4bpp.mtf"
	.global Tileset_Set15TilesB
Tileset_Set15TilesB:
	.incbin "games/tla/src/graphics/tileset/set_15/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_Set15TilesA+0xfda
	.global Tileset_Set15TilesC
Tileset_Set15TilesC:
	.incbin "games/tla/src/graphics/tileset/set_15/tiles_c.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Field_Map032+0x217f
	.global Tileset_Set15TilesD
Tileset_Set15TilesD:
	.incbin "games/tla/src/graphics/tileset/set_15/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_Set15TilesC+0x13f
