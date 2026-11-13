	.global Field_Map202
Field_Map202:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map202, .LField_Map202_CELL_PICTURES
	map_block Field_Map202, .LField_Map202_SHAPES
	map_block Field_Map202, .LField_Map202_GRID
	map_block Field_Map202, .LField_Map202_SHAPE_GRID
	map_block Field_Map202, .LField_Map202_ANIMATION
	map_block Field_Map202, .LField_Map202_BLEND
	map_block Field_Map202, .LField_Map202_SCENE
.LField_Map202_CELL_PICTURES:
	.incbin "games/tla/src/map/map_202/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map202_SHAPES:
	.incbin "games/tla/src/map/map_202/shapes.table.lz"
	.balign 4, 0
.LField_Map202_GRID:
	.incbin "games/tla/src/map/map_202/map.grid.lz"
	.balign 4, 0
.LField_Map202_SHAPE_GRID:
	.incbin "games/tla/src/map/map_202/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map202_ANIMATION:
	.incbin "games/tla/src/map/map_202/animation.script.lz"
	.balign 4, 0
.LField_Map202_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map202_SCENE:
	.incbin "games/tla/src/map/map_202/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set88Palette
Tileset_Set88Palette:
	.incbin "games/tla/src/graphics/tileset/set_88/palette.gbapal.lz"
	.byte 0xfe, 0x07	@ PACKER: left in the buffer by Field_Map202+0x17e
	.global Tileset_Set88TilesA
Tileset_Set88TilesA:
	.incbin "games/tla/src/graphics/tileset/set_88/tiles_a.4bpp.mtf"
	.global Tileset_Set88TilesB
Tileset_Set88TilesB:
	.incbin "games/tla/src/graphics/tileset/set_88/tiles_b.4bpp.mtf"
	.byte 0x9d, 0x69	@ PACKER: left in the buffer by Tileset_Set88TilesA+0x1e02
	.global Tileset_Set88TilesC
Tileset_Set88TilesC:
	.incbin "games/tla/src/graphics/tileset/set_88/tiles_c.4bpp.mtf"
	.byte 0x84	@ PACKER: left in the buffer by Tileset_Set88TilesB+0x1d5b
	.global Tileset_Set88TilesD
Tileset_Set88TilesD:
	.incbin "games/tla/src/graphics/tileset/set_88/tiles_d.4bpp.mtf"
