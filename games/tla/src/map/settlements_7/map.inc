	.global Field_Settlements7
Field_Settlements7:
	map_header 0, 0, 52, 54, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 52, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 54, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Settlements7, .LField_Settlements7_CELL_PICTURES
	map_block Field_Settlements7, .LField_Settlements7_SHAPES
	map_block Field_Settlements7, .LField_Settlements7_GRID
	map_block Field_Settlements7, .LField_Settlements7_SHAPE_GRID
	map_block Field_Settlements7, .LField_Settlements7_ANIMATION
	map_block Field_Settlements7, .LField_Settlements7_BLEND
	map_block Field_Settlements7, .LField_Settlements7_SCENE
.LField_Settlements7_CELL_PICTURES:
	.incbin "games/tla/src/map/settlements_7/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Settlements7_SHAPES:
	.incbin "games/tla/src/map/settlements_7/shapes.table.lz"
	.balign 4, 0
.LField_Settlements7_GRID:
	.incbin "games/tla/src/map/settlements_7/map.grid.lz"
	.balign 4, 0
.LField_Settlements7_SHAPE_GRID:
	.incbin "games/tla/src/map/settlements_7/shape_grid.plane.lz"
	.balign 4, 0
.LField_Settlements7_ANIMATION:
	.incbin "games/tla/src/map/settlements_7/animation.script.lz"
	.balign 4, 0
.LField_Settlements7_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Settlements7_SCENE:
	.incbin "games/tla/src/map/settlements_7/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Settlements7Palette
Tileset_Settlements7Palette:
	.incbin "games/tla/src/graphics/tileset/settlements_7/palette.gbapal.lz"
	.byte 0xc0, 0x39	@ PACKER: left in the buffer by Field_Settlements7+0x166
	.global Tileset_Settlements7TilesA
Tileset_Settlements7TilesA:
	.incbin "games/tla/src/graphics/tileset/settlements_7/tiles_a.4bpp.mtf"
	.global Tileset_Settlements7TilesB
Tileset_Settlements7TilesB:
	.incbin "games/tla/src/graphics/tileset/settlements_7/tiles_b.4bpp.mtf"
	.global Tileset_Settlements7TilesC
Tileset_Settlements7TilesC:
	.incbin "games/tla/src/graphics/tileset/settlements_7/tiles_c.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Settlements7TilesD
Tileset_Settlements7TilesD:
	.incbin "games/tla/src/graphics/tileset/settlements_7/tiles_d.4bpp.mtf"
	.byte 0xe1	@ PACKER: left in the buffer by Tileset_Settlements7TilesC+0x5db
