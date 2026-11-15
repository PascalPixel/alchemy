	.global Field_MagmaRock3
Field_MagmaRock3:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRock3, .LField_MagmaRock3_CELL_PICTURES
	map_block Field_MagmaRock3, .LField_MagmaRock3_SHAPES
	map_block Field_MagmaRock3, .LField_MagmaRock3_GRID
	map_block Field_MagmaRock3, .LField_MagmaRock3_SHAPE_GRID
	map_block Field_MagmaRock3, .LField_MagmaRock3_ANIMATION
	map_block Field_MagmaRock3, .LField_MagmaRock3_BLEND
	map_block Field_MagmaRock3, .LField_MagmaRock3_SCENE
.LField_MagmaRock3_CELL_PICTURES:
	.incbin "games/tla/src/map/magma_rock_3/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MagmaRock3_SHAPES:
	.incbin "games/tla/src/map/magma_rock_3/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRock3_GRID:
	.incbin "games/tla/src/map/magma_rock_3/map.grid.lz"
	.balign 4, 0
.LField_MagmaRock3_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRock3_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_MagmaRock3_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRock3_SCENE:
	.incbin "games/tla/src/map/magma_rock_3/scene.scene.lz"
	.balign 4, 0
