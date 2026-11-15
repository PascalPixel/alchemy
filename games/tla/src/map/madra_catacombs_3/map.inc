	.global Field_MadraCatacombs3
Field_MadraCatacombs3:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombs3, .LField_MadraCatacombs3_CELL_PICTURES
	map_block Field_MadraCatacombs3, .LField_MadraCatacombs3_SHAPES
	map_block Field_MadraCatacombs3, .LField_MadraCatacombs3_GRID
	map_block Field_MadraCatacombs3, .LField_MadraCatacombs3_SHAPE_GRID
	map_block Field_MadraCatacombs3, .LField_MadraCatacombs3_ANIMATION
	map_block Field_MadraCatacombs3
	map_block Field_MadraCatacombs3, .LField_MadraCatacombs3_SCENE
.LField_MadraCatacombs3_CELL_PICTURES:
	.incbin "games/tla/src/map/madra_catacombs_3/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MadraCatacombs3_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_3/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombs3_GRID:
	.incbin "games/tla/src/map/madra_catacombs_3/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombs3_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombs3_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombs3_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_3/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MadraCatacombs3Palette
Tileset_MadraCatacombs3Palette:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_3/palette.gbapal.lz"
	.byte 0x60, 0x3f, 0xbe	@ PACKER: left in the buffer by Field_MadraCatacombs3+0x115
	.global Tileset_MadraCatacombs3TilesA
Tileset_MadraCatacombs3TilesA:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_3/tiles_a.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Field_MadraInteriors2+0x22d3
	.global Tileset_MadraCatacombs3TilesB
Tileset_MadraCatacombs3TilesB:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_3/tiles_b.4bpp.mtf"
	.byte 0x11, 0xa7	@ PACKER: left in the buffer by Tileset_MadraCatacombs3TilesA+0x1c52
	.global Tileset_MadraCatacombs3TilesC
Tileset_MadraCatacombs3TilesC:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_3/tiles_c.4bpp.mtf"
	.byte 0x43	@ PACKER: left in the buffer by Tileset_MadraCatacombs3TilesB+0x643
	.global Tileset_MadraCatacombs3TilesD
Tileset_MadraCatacombs3TilesD:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_3/tiles_d.4bpp.mtf"
	.byte 0x50	@ PACKER: left in the buffer by Tileset_MadraCatacombs3TilesC+0x13f
