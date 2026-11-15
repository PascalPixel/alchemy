	.global Field_MadraCatacombs2
Field_MadraCatacombs2:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombs2, .LField_MadraCatacombs2_CELL_PICTURES
	map_block Field_MadraCatacombs2, .LField_MadraCatacombs2_SHAPES
	map_block Field_MadraCatacombs2, .LField_MadraCatacombs2_GRID
	map_block Field_MadraCatacombs2, .LField_MadraCatacombs2_SHAPE_GRID
	map_block Field_MadraCatacombs2, .LField_MadraCatacombs2_ANIMATION
	map_block Field_MadraCatacombs2, .LField_MadraCatacombs2_BLEND
	map_block Field_MadraCatacombs2, .LField_MadraCatacombs2_SCENE
.LField_MadraCatacombs2_CELL_PICTURES:
	.incbin "games/tla/src/map/madra_catacombs_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MadraCatacombs2_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_2/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombs2_GRID:
	.incbin "games/tla/src/map/madra_catacombs_2/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombs2_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombs2_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombs2_BLEND:
	.incbin "games/tla/src/map/madra_catacombs_2/blend.script.lz"
	.balign 4, 0
.LField_MadraCatacombs2_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_2/scene.scene.lz"
	.global Tileset_MadraCatacombs2Palette
Tileset_MadraCatacombs2Palette:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_2/palette.gbapal.lz"
	.global Tileset_MadraCatacombs2TilesA
Tileset_MadraCatacombs2TilesA:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_2/tiles_a.4bpp.mtf"
	.byte 0xdf	@ PACKER: left in the buffer by Tileset_MadraCatacombsEntranceTilesA+0x1f5b
	.global Tileset_MadraCatacombs2TilesB
Tileset_MadraCatacombs2TilesB:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_2/tiles_b.4bpp.mtf"
	.byte 0xfa, 0x5c	@ PACKER: left in the buffer by Tileset_MadraCatacombs2TilesA+0x185e
	.global Tileset_MadraCatacombs2TilesC
Tileset_MadraCatacombs2TilesC:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_2/tiles_c.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_MadraCatacombs2TilesB+0x60f
	.global Tileset_MadraCatacombs2TilesD
Tileset_MadraCatacombs2TilesD:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_2/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_MadraCatacombs2TilesC+0x13f
