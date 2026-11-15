	.global Field_TaopoSwamp10
Field_TaopoSwamp10:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_TaopoSwamp10, .LField_TaopoSwamp10_CELL_PICTURES
	map_block Field_TaopoSwamp10, .LField_TaopoSwamp10_SHAPES
	map_block Field_TaopoSwamp10, .LField_TaopoSwamp10_GRID
	map_block Field_TaopoSwamp10, .LField_TaopoSwamp10_SHAPE_GRID
	map_block Field_TaopoSwamp10, .LField_TaopoSwamp10_ANIMATION
	map_block Field_TaopoSwamp10, .LField_TaopoSwamp10_BLEND
	map_block Field_TaopoSwamp10, .LField_TaopoSwamp10_SCENE
.LField_TaopoSwamp10_CELL_PICTURES:
	.incbin "games/tla/src/map/taopo_swamp_10/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TaopoSwamp10_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_10/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwamp10_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_10/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_10/map.grid.lz"
	.endif
	.balign 4, 0
.LField_TaopoSwamp10_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_10/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_10/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_TaopoSwamp10_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_10/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwamp10_BLEND:
	.incbin "games/tla/src/map/yampi_desert_1/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwamp10_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/taopo_swamp_10/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/taopo_swamp_10/scene.scene.lz"
	.endif
