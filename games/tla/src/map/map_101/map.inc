	.global Field_Map101
Field_Map101:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map101, .LField_Map101_CELL_PICTURES
	map_block Field_Map101, .LField_Map101_SHAPES
	map_block Field_Map101, .LField_Map101_GRID
	map_block Field_Map101, .LField_Map101_SHAPE_GRID
	map_block Field_Map101, .LField_Map101_ANIMATION
	map_block Field_Map101, .LField_Map101_BLEND
	map_block Field_Map101, .LField_Map101_SCENE
.LField_Map101_CELL_PICTURES:
	.incbin "games/tla/src/map/map_101/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map101_SHAPES:
	.incbin "games/tla/src/map/map_101/shapes.table.lz"
	.balign 4, 0
.LField_Map101_GRID:
	.incbin "games/tla/src/map/map_101/map.grid.lz"
	.balign 4, 0
.LField_Map101_SHAPE_GRID:
	.incbin "games/tla/src/map/map_101/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map101_ANIMATION:
	.incbin "games/tla/src/map/map_035/animation.script.lz"
	.balign 4, 0
.LField_Map101_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map101_SCENE:
	.incbin "games/tla/src/map/map_101/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set50Palette
Tileset_Set50Palette:
	.incbin "games/tla/src/graphics/tileset/set_50/palette.gbapal.lz"
	.global Tileset_Set50TilesA
Tileset_Set50TilesA:
	.incbin "games/tla/src/graphics/tileset/set_50/tiles_a.4bpp.mtf"
	.byte 0xf6, 0x09, 0x04	@ PACKER: left in the buffer by Field_Map100+0x1985
	.global Tileset_Set50TilesB
Tileset_Set50TilesB:
	.incbin "games/tla/src/graphics/tileset/set_50/tiles_b.4bpp.mtf"
	.byte 0x1d, 0x94, 0x7f	@ PACKER: left in the buffer by Tileset_Set50TilesA+0x148d
	.global Tileset_Set50TilesC
Tileset_Set50TilesC:
	.incbin "games/tla/src/graphics/tileset/set_50/tiles_c.4bpp.mtf"
	.byte 0xe4	@ PACKER: left in the buffer by Tileset_Set50TilesB+0x467
	.global Tileset_Set50TilesD
Tileset_Set50TilesD:
	.incbin "games/tla/src/graphics/tileset/set_50/tiles_d.4bpp.mtf"
	.byte 0x7f, 0xe0	@ PACKER: left in the buffer by Tileset_Set50TilesC+0x1be
