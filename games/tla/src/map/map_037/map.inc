	.global Field_Map037
Field_Map037:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map037, .LField_Map037_CELL_PICTURES
	map_block Field_Map037, .LField_Map037_SHAPES
	map_block Field_Map037, .LField_Map037_GRID
	map_block Field_Map037, .LField_Map037_SHAPE_GRID
	map_block Field_Map037, .LField_Map037_ANIMATION
	map_block Field_Map037, .LField_Map037_BLEND
	map_block Field_Map037, .LField_Map037_SCENE
.LField_Map037_CELL_PICTURES:
	.incbin "games/tla/src/map/map_037/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map037_SHAPES:
	.incbin "games/tla/src/map/map_037/shapes.table.lz"
	.balign 4, 0
.LField_Map037_GRID:
	.incbin "games/tla/src/map/map_037/map.grid.lz"
	.balign 4, 0
.LField_Map037_SHAPE_GRID:
	.incbin "games/tla/src/map/map_037/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map037_ANIMATION:
	.incbin "games/tla/src/map/map_035/animation.script.lz"
	.balign 4, 0
.LField_Map037_BLEND:
	.incbin "games/tla/src/map/map_035/blend.script.lz"
	.balign 4, 0
.LField_Map037_SCENE:
	.incbin "games/tla/src/map/map_037/scene.scene.lz"
	.global Tileset_Set20Palette
Tileset_Set20Palette:
	.incbin "games/tla/src/graphics/tileset/set_20/palette.gbapal.lz"
	.global Tileset_Set20TilesA
Tileset_Set20TilesA:
	.incbin "games/tla/src/graphics/tileset/set_20/tiles_a.4bpp.mtf"
	.global Tileset_Set20TilesB
Tileset_Set20TilesB:
	.incbin "games/tla/src/graphics/tileset/set_20/tiles_b.4bpp.mtf"
	.global Tileset_Set20TilesC
Tileset_Set20TilesC:
	.incbin "games/tla/src/graphics/tileset/set_20/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_Set20TilesB+0x1f5
	.global Tileset_Set20TilesD
Tileset_Set20TilesD:
	.incbin "games/tla/src/graphics/tileset/set_20/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_Set20TilesC+0x1be
