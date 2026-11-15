	.global Field_MadraCatacombs1
Field_MadraCatacombs1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombs1, .LField_MadraCatacombs1_CELL_PICTURES
	map_block Field_MadraCatacombs1, .LField_MadraCatacombs1_SHAPES
	map_block Field_MadraCatacombs1, .LField_MadraCatacombs1_GRID
	map_block Field_MadraCatacombs1, .LField_MadraCatacombs1_SHAPE_GRID
	map_block Field_MadraCatacombs1, .LField_MadraCatacombs1_ANIMATION
	map_block Field_MadraCatacombs1, .LField_MadraCatacombs1_BLEND
	map_block Field_MadraCatacombs1, .LField_MadraCatacombs1_SCENE
.LField_MadraCatacombs1_CELL_PICTURES:
	.incbin "games/tla/src/map/madra_catacombs_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MadraCatacombs1_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_1/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombs1_GRID:
	.incbin "games/tla/src/map/madra_catacombs_1/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombs1_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombs1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombs1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MadraCatacombs1_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MadraCatacombs1Palette
Tileset_MadraCatacombs1Palette:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_1/palette.gbapal.lz"
	.byte 0x08	@ PACKER: left in the buffer by Field_MadraCatacombs1+0x10b
	.global Tileset_MadraCatacombs1TilesA
Tileset_MadraCatacombs1TilesA:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_1/tiles_a.4bpp.mtf"
	.byte 0xce	@ PACKER: left in the buffer by Field_MadraInteriors2+0x212f
	.global Tileset_MadraCatacombs1TilesB
Tileset_MadraCatacombs1TilesB:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_1/tiles_b.4bpp.mtf"
	.byte 0x22, 0x0e, 0xf8	@ PACKER: left in the buffer by Tileset_MadraCatacombs1TilesA+0x1bd5
	.global Tileset_MadraCatacombs1TilesC
Tileset_MadraCatacombs1TilesC:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_1/tiles_c.4bpp.mtf"
	.byte 0x19	@ PACKER: left in the buffer by Tileset_MadraCatacombs1TilesB+0x60f
	.global Tileset_MadraCatacombs1TilesD
Tileset_MadraCatacombs1TilesD:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_1/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_MadraCatacombs1TilesC+0x13f
