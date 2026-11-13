	.global Field_AlhafraManor
Field_AlhafraManor:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AlhafraManor, .LField_AlhafraManor_CELL_PICTURES
	map_block Field_AlhafraManor, .LField_AlhafraManor_SHAPES
	map_block Field_AlhafraManor, .LField_AlhafraManor_GRID
	map_block Field_AlhafraManor, .LField_AlhafraManor_SHAPE_GRID
	map_block Field_AlhafraManor, .LField_AlhafraManor_ANIMATION
	map_block Field_AlhafraManor, .LField_AlhafraManor_BLEND
	map_block Field_AlhafraManor, .LField_AlhafraManor_SCENE
.LField_AlhafraManor_CELL_PICTURES:
	.incbin "games/tla/src/map/alhafra_manor/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AlhafraManor_SHAPES:
	.incbin "games/tla/src/map/alhafra_manor/shapes.table.lz"
	.balign 4, 0
.LField_AlhafraManor_GRID:
	.incbin "games/tla/src/map/alhafra_manor/map.grid.lz"
	.balign 4, 0
.LField_AlhafraManor_SHAPE_GRID:
	.incbin "games/tla/src/map/alhafra_manor/shape_grid.plane.lz"
	.balign 4, 0
.LField_AlhafraManor_ANIMATION:
	.incbin "games/tla/src/map/alhafra_manor/animation.script.lz"
	.balign 4, 0
.LField_AlhafraManor_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_AlhafraManor_SCENE:
	.incbin "games/tla/src/map/alhafra_manor/scene.scene.lz"
