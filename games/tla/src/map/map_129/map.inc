	.global Field_Map129
Field_Map129:
	map_header 0, 0, 58, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map129, .LField_Map129_CELL_PICTURES
	map_block Field_Map129, .LField_Map129_SHAPES
	map_block Field_Map129, .LField_Map129_GRID
	map_block Field_Map129, .LField_Map129_SHAPE_GRID
	map_block Field_Map129, .LField_Map129_ANIMATION
	map_block Field_Map129, .LField_Map129_BLEND
	map_block Field_Map129, .LField_Map129_SCENE
.LField_Map129_CELL_PICTURES:
	.incbin "games/tla/src/map/map_129/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map129_SHAPES:
	.incbin "games/tla/src/map/map_129/shapes.table.lz"
	.balign 4, 0
.LField_Map129_GRID:
	.incbin "games/tla/src/map/map_129/map.grid.lz"
	.balign 4, 0
.LField_Map129_SHAPE_GRID:
	.incbin "games/tla/src/map/map_129/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map129_ANIMATION:
	.incbin "games/tla/src/map/map_129/animation.script.lz"
	.balign 4, 0
.LField_Map129_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Map129_SCENE:
	.incbin "games/tla/src/map/map_129/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set62Palette
Tileset_Set62Palette:
	.incbin "games/tla/src/graphics/tileset/set_62/palette.gbapal.lz"
	.byte 0x80, 0x0e	@ PACKER: left in the buffer by Field_Map129+0x13a
	.global Tileset_Set62TilesA
Tileset_Set62TilesA:
	.incbin "games/tla/src/graphics/tileset/set_62/tiles_a.4bpp.mtf"
	.byte 0x06, 0x0d, 0x54	@ PACKER: left in the buffer by Field_Map129+0x1505
	.global Tileset_Set62TilesB
Tileset_Set62TilesB:
	.incbin "games/tla/src/graphics/tileset/set_62/tiles_b.4bpp.mtf"
	.global Tileset_Set62TilesC
Tileset_Set62TilesC:
	.incbin "games/tla/src/graphics/tileset/set_62/tiles_c.4bpp.mtf"
	.byte 0x00, 0x5c	@ PACKER: left in the buffer by Tileset_Set62TilesB+0x15e2
	.global Tileset_Set62TilesD
Tileset_Set62TilesD:
	.incbin "games/tla/src/graphics/tileset/set_62/tiles_d.4bpp.mtf"
	.byte 0xa1, 0x4f, 0x64	@ PACKER: left in the buffer by Tileset_Set62TilesC+0x1445
