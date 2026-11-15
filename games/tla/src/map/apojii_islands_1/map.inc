	.global Field_ApojiiIslands1
Field_ApojiiIslands1:
	map_header 0, 0, 88, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ApojiiIslands1, .LField_ApojiiIslands1_CELL_PICTURES
	map_block Field_ApojiiIslands1, .LField_ApojiiIslands1_SHAPES
	map_block Field_ApojiiIslands1, .LField_ApojiiIslands1_GRID
	map_block Field_ApojiiIslands1, .LField_ApojiiIslands1_SHAPE_GRID
	map_block Field_ApojiiIslands1, .LField_ApojiiIslands1_ANIMATION
	map_block Field_ApojiiIslands1, .LField_ApojiiIslands1_BLEND
	map_block Field_ApojiiIslands1, .LField_ApojiiIslands1_SCENE
.LField_ApojiiIslands1_CELL_PICTURES:
	.incbin "games/tla/src/map/apojii_islands_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ApojiiIslands1_SHAPES:
	.incbin "games/tla/src/map/apojii_islands_1/shapes.table.lz"
	.balign 4, 0
.LField_ApojiiIslands1_GRID:
	.incbin "games/tla/src/map/apojii_islands_1/map.grid.lz"
	.balign 4, 0
.LField_ApojiiIslands1_SHAPE_GRID:
	.incbin "games/tla/src/map/apojii_islands_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_ApojiiIslands1_ANIMATION:
	.incbin "games/tla/src/map/apojii_islands_1/animation.script.lz"
	.balign 4, 0
.LField_ApojiiIslands1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ApojiiIslands1_SCENE:
	.incbin "games/tla/src/map/apojii_islands_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ApojiiIslands1Palette
Tileset_ApojiiIslands1Palette:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_1/palette.gbapal.lz"
	.byte 0x78, 0x9f	@ PACKER: left in the buffer by Field_ApojiiIslands1+0x156
	.global Tileset_ApojiiIslands1TilesA
Tileset_ApojiiIslands1TilesA:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_1/tiles_a.4bpp.mtf"
	.global Tileset_ApojiiIslands1TilesB
Tileset_ApojiiIslands1TilesB:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_1/tiles_b.4bpp.mtf"
	.byte 0xa3	@ PACKER: left in the buffer by Tileset_ApojiiIslands1TilesA+0x1ab7
	.global Tileset_ApojiiIslands1TilesC
Tileset_ApojiiIslands1TilesC:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_1/tiles_c.4bpp.mtf"
	.byte 0x0f, 0x74, 0xf6	@ PACKER: left in the buffer by Tileset_ApojiiIslands1TilesB+0x1881
	.global Tileset_ApojiiIslands1TilesD
Tileset_ApojiiIslands1TilesD:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_1/tiles_d.4bpp.mtf"
	.byte 0x6d, 0x93, 0xa2	@ PACKER: left in the buffer by Field_ApojiiIslands1+0x1d25
