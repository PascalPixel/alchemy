	.global Field_MadraCatacombs4
Field_MadraCatacombs4:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombs4, .LField_MadraCatacombs4_CELL_PICTURES
	map_block Field_MadraCatacombs4, .LField_MadraCatacombs4_SHAPES
	map_block Field_MadraCatacombs4, .LField_MadraCatacombs4_GRID
	map_block Field_MadraCatacombs4, .LField_MadraCatacombs4_SHAPE_GRID
	map_block Field_MadraCatacombs4, .LField_MadraCatacombs4_ANIMATION
	map_block Field_MadraCatacombs4, .LField_MadraCatacombs4_BLEND
	map_block Field_MadraCatacombs4, .LField_MadraCatacombs4_SCENE
.LField_MadraCatacombs4_CELL_PICTURES:
	.incbin "games/tla/src/map/madra_catacombs_4/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MadraCatacombs4_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_4/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombs4_GRID:
	.incbin "games/tla/src/map/madra_catacombs_4/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombs4_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_4/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombs4_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombs4_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MadraCatacombs4_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_4/scene.scene.lz"
	.global Tileset_PaletteMadraCatacombs4
Tileset_PaletteMadraCatacombs4:
	.incbin "games/tla/src/graphics/tileset/palettes/madra_catacombs_4.gbapal.lz"
	.byte 0xe3, 0x56	@ PACKER: left in the buffer by Field_MadraCatacombs4+0x176
