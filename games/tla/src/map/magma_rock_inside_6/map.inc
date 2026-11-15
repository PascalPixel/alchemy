	.global Field_MagmaRockInside6
Field_MagmaRockInside6:
	map_header 0, 0, 130, 112, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockInside6, .LField_MagmaRockInside6_CELL_PICTURES
	map_block Field_MagmaRockInside6, .LField_MagmaRockInside6_SHAPES
	map_block Field_MagmaRockInside6, .LField_MagmaRockInside6_GRID
	map_block Field_MagmaRockInside6, .LField_MagmaRockInside6_SHAPE_GRID
	map_block Field_MagmaRockInside6, .LField_MagmaRockInside6_ANIMATION
	map_block Field_MagmaRockInside6, .LField_MagmaRockInside6_BLEND
	map_block Field_MagmaRockInside6, .LField_MagmaRockInside6_SCENE
.LField_MagmaRockInside6_CELL_PICTURES:
	.incbin "games/tla/src/map/magma_rock_inside_6/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MagmaRockInside6_SHAPES:
	.incbin "games/tla/src/map/magma_rock_inside_6/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockInside6_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_6/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockInside6_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockInside6_ANIMATION:
	.incbin "games/tla/src/map/magma_rock_inside_2/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockInside6_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRockInside6_SCENE:
	.incbin "games/tla/src/map/magma_rock_inside_6/scene.scene.lz"
	.balign 4, 0
