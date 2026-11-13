	.global Field_Map117
Field_Map117:
	map_header 0, 0, 128, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map117, .LField_Map117_CELL_PICTURES
	map_block Field_Map117, .LField_Map117_SHAPES
	map_block Field_Map117, .LField_Map117_GRID
	map_block Field_Map117, .LField_Map117_SHAPE_GRID
	map_block Field_Map117, .LField_Map117_ANIMATION
	map_block Field_Map117, .LField_Map117_BLEND
	map_block Field_Map117, .LField_Map117_SCENE
.LField_Map117_CELL_PICTURES:
	.incbin "games/tla/src/map/map_117/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map117_SHAPES:
	.incbin "games/tla/src/map/map_117/shapes.table.lz"
	.balign 4, 0
.LField_Map117_GRID:
	.incbin "games/tla/src/map/map_117/map.grid.lz"
	.balign 4, 0
.LField_Map117_SHAPE_GRID:
	.incbin "games/tla/src/map/map_117/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map117_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map117_BLEND:
	.incbin "games/tla/src/map/map_113/blend.script.lz"
	.balign 4, 0
.LField_Map117_SCENE:
	.incbin "games/tla/src/map/map_117/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set58Palette
Tileset_Set58Palette:
	.incbin "games/tla/src/graphics/tileset/set_58/palette.gbapal.lz"
	.global Tileset_Set58TilesA
Tileset_Set58TilesA:
	.incbin "games/tla/src/graphics/tileset/set_58/tiles_a.4bpp.mtf"
	.byte 0x00, 0x04	@ PACKER: left in the buffer by Field_Map117+0x3d2
	.global Tileset_Set58TilesB
Tileset_Set58TilesB:
	.incbin "games/tla/src/graphics/tileset/set_58/tiles_b.4bpp.mtf"
	.byte 0x4d, 0xcb, 0x82	@ PACKER: left in the buffer by Tileset_Set57TilesA+0x1479
	.global Tileset_Set58TilesC
Tileset_Set58TilesC:
	.incbin "games/tla/src/graphics/tileset/set_58/tiles_c.4bpp.mtf"
	.global Tileset_Set58TilesD
Tileset_Set58TilesD:
	.incbin "games/tla/src/graphics/tileset/set_58/tiles_d.4bpp.mtf"
	.byte 0xdc	@ PACKER: left in the buffer by Tileset_Set58TilesC+0x13f
