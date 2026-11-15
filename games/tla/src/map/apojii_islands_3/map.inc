	.global Field_ApojiiIslands3
Field_ApojiiIslands3:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ApojiiIslands3, .LField_ApojiiIslands3_CELL_PICTURES
	map_block Field_ApojiiIslands3, .LField_ApojiiIslands3_SHAPES
	map_block Field_ApojiiIslands3, .LField_ApojiiIslands3_GRID
	map_block Field_ApojiiIslands3, .LField_ApojiiIslands3_SHAPE_GRID
	map_block Field_ApojiiIslands3, .LField_ApojiiIslands3_ANIMATION
	map_block Field_ApojiiIslands3, .LField_ApojiiIslands3_BLEND
	map_block Field_ApojiiIslands3, .LField_ApojiiIslands3_SCENE
.LField_ApojiiIslands3_CELL_PICTURES:
	.incbin "games/tla/src/map/apojii_islands_3/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ApojiiIslands3_SHAPES:
	.incbin "games/tla/src/map/apojii_islands_3/shapes.table.lz"
	.balign 4, 0
.LField_ApojiiIslands3_GRID:
	.incbin "games/tla/src/map/apojii_islands_3/map.grid.lz"
	.balign 4, 0
.LField_ApojiiIslands3_SHAPE_GRID:
	.incbin "games/tla/src/map/apojii_islands_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_ApojiiIslands3_ANIMATION:
	.incbin "games/tla/src/map/yallam_interiors/animation.script.lz"
	.balign 4, 0
.LField_ApojiiIslands3_BLEND:
	.incbin "games/tla/src/map/daila_interiors_1/blend.script.lz"
	.balign 4, 0
.LField_ApojiiIslands3_SCENE:
	.incbin "games/tla/src/map/apojii_islands_3/scene.scene.lz"
	.global Tileset_ApojiiIslands3Palette
Tileset_ApojiiIslands3Palette:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_3/palette.gbapal.lz"
	.byte 0x17, 0x03, 0xcc	@ PACKER: left in the buffer by Field_ApojiiIslands3+0x1c5
	.global Tileset_ApojiiIslands3TilesA
Tileset_ApojiiIslands3TilesA:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_3/tiles_a.4bpp.mtf"
	.byte 0x3e, 0xe0, 0xde	@ PACKER: left in the buffer by Field_ApojiiIslands3+0x1989
	.global Tileset_ApojiiIslands3TilesB
Tileset_ApojiiIslands3TilesB:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_3/tiles_b.4bpp.mtf"
	.byte 0xa8, 0x53	@ PACKER: left in the buffer by Tileset_ApojiiIslands3TilesA+0x112a
	.global Tileset_ApojiiIslands3TilesC
Tileset_ApojiiIslands3TilesC:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_3/tiles_c.4bpp.mtf"
	.byte 0x72, 0x1c	@ PACKER: left in the buffer by Tileset_ApojiiIslands3TilesB+0xbae
	.global Tileset_ApojiiIslands3TilesD
Tileset_ApojiiIslands3TilesD:
	.incbin "games/tla/src/graphics/tileset/apojii_islands_3/tiles_d.4bpp.mtf"
