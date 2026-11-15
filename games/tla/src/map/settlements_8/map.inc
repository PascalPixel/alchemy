	.global Field_Settlements8
Field_Settlements8:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Settlements8, .LField_Settlements8_CELL_PICTURES
	map_block Field_Settlements8, .LField_Settlements8_SHAPES
	map_block Field_Settlements8, .LField_Settlements8_GRID
	map_block Field_Settlements8, .LField_Settlements8_SHAPE_GRID
	map_block Field_Settlements8, .LField_Settlements8_ANIMATION
	map_block Field_Settlements8, .LField_Settlements8_BLEND
	map_block Field_Settlements8, .LField_Settlements8_SCENE
.LField_Settlements8_CELL_PICTURES:
	.incbin "games/tla/src/map/settlements_8/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Settlements8_SHAPES:
	.incbin "games/tla/src/map/settlements_8/shapes.table.lz"
	.balign 4, 0
.LField_Settlements8_GRID:
	.incbin "games/tla/src/map/settlements_8/map.grid.lz"
	.balign 4, 0
.LField_Settlements8_SHAPE_GRID:
	.incbin "games/tla/src/map/settlements_8/shape_grid.plane.lz"
	.balign 4, 0
.LField_Settlements8_ANIMATION:
	.incbin "games/tla/src/map/settlements_8/animation.script.lz"
	.balign 4, 0
.LField_Settlements8_BLEND:
	.incbin "games/tla/src/map/daila_interiors_houses/blend.script.lz"
	.balign 4, 0
.LField_Settlements8_SCENE:
	.incbin "games/tla/src/map/settlements_8/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Settlements8Palette
Tileset_Settlements8Palette:
	.incbin "games/tla/src/graphics/tileset/settlements_8/palette.gbapal.lz"
	.global Tileset_Settlements8TilesA
Tileset_Settlements8TilesA:
	.incbin "games/tla/src/graphics/tileset/settlements_8/tiles_a.4bpp.mtf"
	.byte 0xb8	@ PACKER: left in the buffer by Tileset_Settlements7TilesC+0x2677
	.global Tileset_Settlements8TilesB
Tileset_Settlements8TilesB:
	.incbin "games/tla/src/graphics/tileset/settlements_8/tiles_b.4bpp.mtf"
	.byte 0xf5, 0x13	@ PACKER: left in the buffer by Tileset_Settlements8TilesA+0x161e
	.global Tileset_Settlements8TilesC
Tileset_Settlements8TilesC:
	.incbin "games/tla/src/graphics/tileset/settlements_8/tiles_c.4bpp.mtf"
	.byte 0xde	@ PACKER: left in the buffer by Tileset_Settlements8TilesA+0x195f
	.global Tileset_Settlements8TilesD
Tileset_Settlements8TilesD:
	.incbin "games/tla/src/graphics/tileset/settlements_8/tiles_d.4bpp.mtf"
