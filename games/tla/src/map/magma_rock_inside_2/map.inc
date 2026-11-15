	.global Field_MagmaRockInside2
Field_MagmaRockInside2:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockInside2, .LField_MagmaRockInside2_CELL_PICTURES
	map_block Field_MagmaRockInside2, .LField_MagmaRockInside2_SHAPES
	map_block Field_MagmaRockInside2, .LField_MagmaRockInside2_GRID
	map_block Field_MagmaRockInside2, .LField_MagmaRockInside2_SHAPE_GRID
	map_block Field_MagmaRockInside2, .LField_MagmaRockInside2_ANIMATION
	map_block Field_MagmaRockInside2, .LField_MagmaRockInside2_BLEND
	map_block Field_MagmaRockInside2, .LField_MagmaRockInside2_SCENE
.LField_MagmaRockInside2_CELL_PICTURES:
	.incbin "games/tla/src/map/magma_rock_inside_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MagmaRockInside2_SHAPES:
	.incbin "games/tla/src/map/magma_rock_inside_2/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockInside2_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_2/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockInside2_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockInside2_ANIMATION:
	.incbin "games/tla/src/map/magma_rock_inside_2/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockInside2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRockInside2_SCENE:
	.incbin "games/tla/src/map/magma_rock_inside_2/scene.scene.lz"
	.balign 4, 0
