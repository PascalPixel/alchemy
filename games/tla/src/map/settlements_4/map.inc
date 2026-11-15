	.global Field_Settlements4
Field_Settlements4:
	map_header 0, 0, 128, 64, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Settlements4, .LField_Settlements4_CELL_PICTURES
	map_block Field_Settlements4, .LField_Settlements4_SHAPES
	map_block Field_Settlements4, .LField_Settlements4_GRID
	map_block Field_Settlements4, .LField_Settlements4_SHAPE_GRID
	map_block Field_Settlements4, .LField_Settlements4_ANIMATION
	map_block Field_Settlements4, .LField_Settlements4_BLEND
	map_block Field_Settlements4, .LField_Settlements4_SCENE
.LField_Settlements4_CELL_PICTURES:
	.incbin "games/tla/src/map/settlements_4/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Settlements4_SHAPES:
	.incbin "games/tla/src/map/settlements_4/shapes.table.lz"
	.balign 4, 0
.LField_Settlements4_GRID:
	.incbin "games/tla/src/map/settlements_4/map.grid.lz"
	.balign 4, 0
.LField_Settlements4_SHAPE_GRID:
	.incbin "games/tla/src/map/settlements_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_Settlements4_ANIMATION:
	.incbin "games/tla/src/map/settlements_4/animation.script.lz"
	.balign 4, 0
.LField_Settlements4_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Settlements4_SCENE:
	.incbin "games/tla/src/map/settlements_4/scene.scene.lz"
	.global Tileset_Settlements4Palette
Tileset_Settlements4Palette:
	.incbin "games/tla/src/graphics/tileset/settlements_4/palette.gbapal.lz"
	.global Tileset_Settlements4TilesA
Tileset_Settlements4TilesA:
	.incbin "games/tla/src/graphics/tileset/settlements_4/tiles_a.4bpp.mtf"
	.global Tileset_Settlements4TilesB
Tileset_Settlements4TilesB:
	.incbin "games/tla/src/graphics/tileset/settlements_4/tiles_b.4bpp.mtf"
	.byte 0x04, 0x7a	@ PACKER: left in the buffer by Tileset_Settlements4TilesA+0x199a
	.global Tileset_Settlements4TilesC
Tileset_Settlements4TilesC:
	.incbin "games/tla/src/graphics/tileset/settlements_4/tiles_c.4bpp.mtf"
	.global Tileset_Settlements4TilesD
Tileset_Settlements4TilesD:
	.incbin "games/tla/src/graphics/tileset/settlements_4/tiles_d.4bpp.mtf"
	.byte 0x0d	@ PACKER: left in the buffer by Tileset_Settlements4TilesC+0xb83
