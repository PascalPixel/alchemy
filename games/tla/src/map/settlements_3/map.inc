	.global Field_Settlements3
Field_Settlements3:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Settlements3, .LField_Settlements3_CELL_PICTURES
	map_block Field_Settlements3, .LField_Settlements3_SHAPES
	map_block Field_Settlements3, .LField_Settlements3_GRID
	map_block Field_Settlements3, .LField_Settlements3_SHAPE_GRID
	map_block Field_Settlements3, .LField_Settlements3_ANIMATION
	map_block Field_Settlements3, .LField_Settlements3_BLEND
	map_block Field_Settlements3, .LField_Settlements3_SCENE
.LField_Settlements3_CELL_PICTURES:
	.incbin "games/tla/src/map/settlements_3/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Settlements3_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_cave_1/shapes.table.lz"
	.balign 4, 0
.LField_Settlements3_GRID:
	.incbin "games/tla/src/map/settlements_3/map.grid.lz"
	.balign 4, 0
.LField_Settlements3_SHAPE_GRID:
	.incbin "games/tla/src/map/settlements_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_Settlements3_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_Settlements3_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Settlements3_SCENE:
	.incbin "games/tla/src/map/settlements_3/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Settlements3Palette
Tileset_Settlements3Palette:
	.incbin "games/tla/src/graphics/tileset/settlements_3/palette.gbapal.lz"
	.byte 0x09, 0x12, 0xe0	@ PACKER: left in the buffer by Field_Settlements3+0x14d
	.global Tileset_Settlements3TilesA
Tileset_Settlements3TilesA:
	.incbin "games/tla/src/graphics/tileset/settlements_3/tiles_a.4bpp.mtf"
	.global Tileset_Settlements3TilesB
Tileset_Settlements3TilesB:
	.incbin "games/tla/src/graphics/tileset/settlements_3/tiles_b.4bpp.mtf"
	.byte 0x2b	@ PACKER: left in the buffer by Tileset_Settlements3TilesA+0x1bcf
	.global Tileset_Settlements3TilesC
Tileset_Settlements3TilesC:
	.incbin "games/tla/src/graphics/tileset/settlements_3/tiles_c.4bpp.mtf"
	.global Tileset_Settlements3TilesD
Tileset_Settlements3TilesD:
	.incbin "games/tla/src/graphics/tileset/settlements_3/tiles_d.4bpp.mtf"
	.byte 0x38, 0x5a, 0x61	@ PACKER: left in the buffer by Tileset_Settlements3TilesC+0xb29
