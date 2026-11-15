	.global Field_TaopoSwamp11
Field_TaopoSwamp11:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_TaopoSwamp11, .LField_TaopoSwamp11_CELL_PICTURES
	map_block Field_TaopoSwamp11, .LField_TaopoSwamp11_SHAPES
	map_block Field_TaopoSwamp11, .LField_TaopoSwamp11_GRID
	map_block Field_TaopoSwamp11, .LField_TaopoSwamp11_SHAPE_GRID
	map_block Field_TaopoSwamp11, .LField_TaopoSwamp11_ANIMATION
	map_block Field_TaopoSwamp11, .LField_TaopoSwamp11_BLEND
	map_block Field_TaopoSwamp11, .LField_TaopoSwamp11_SCENE
.LField_TaopoSwamp11_CELL_PICTURES:
	.incbin "games/tla/src/map/taopo_swamp_11/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TaopoSwamp11_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_11/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwamp11_GRID:
	.incbin "games/tla/src/map/taopo_swamp_11/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwamp11_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_11/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwamp11_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_10/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwamp11_BLEND:
	.incbin "games/tla/src/map/taopo_swamp_11/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwamp11_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_11/scene.scene.lz"
	.balign 4, 0
