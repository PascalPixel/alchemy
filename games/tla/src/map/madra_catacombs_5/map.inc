	.global Field_MadraCatacombs5
Field_MadraCatacombs5:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombs5, .LField_MadraCatacombs5_CELL_PICTURES
	map_block Field_MadraCatacombs5, .LField_MadraCatacombs5_SHAPES
	map_block Field_MadraCatacombs5, .LField_MadraCatacombs5_GRID
	map_block Field_MadraCatacombs5, .LField_MadraCatacombs5_SHAPE_GRID
	map_block Field_MadraCatacombs5, .LField_MadraCatacombs5_ANIMATION
	map_block Field_MadraCatacombs5, .LField_MadraCatacombs5_BLEND
	map_block Field_MadraCatacombs5, .LField_MadraCatacombs5_SCENE
.LField_MadraCatacombs5_CELL_PICTURES:
	.incbin "games/tla/src/map/madra_catacombs_5/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MadraCatacombs5_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_5/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombs5_GRID:
	.incbin "games/tla/src/map/madra_catacombs_5/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombs5_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_5/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombs5_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombs5_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MadraCatacombs5_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_5/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteMadraCatacombs5
Tileset_PaletteMadraCatacombs5:
	.incbin "games/tla/src/graphics/tileset/palettes/madra_catacombs_5.gbapal.lz"
	.byte 0xf0, 0x21, 0x38	@ PACKER: left in the buffer by Field_MadraCatacombs5+0x165
