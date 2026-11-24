	.global Field_MagmaRockStatue
Field_MagmaRockStatue:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockStatue, .LField_MagmaRockStatue_CELL_TILES
	map_block Field_MagmaRockStatue, .LField_MagmaRockStatue_SHAPES
	map_block Field_MagmaRockStatue, .LField_MagmaRockStatue_GRID
	map_block Field_MagmaRockStatue, .LField_MagmaRockStatue_SHAPE_GRID
	map_block Field_MagmaRockStatue, .LField_MagmaRockStatue_ANIMATION
	map_block Field_MagmaRockStatue, .LField_MagmaRockStatue_BLEND
	map_block Field_MagmaRockStatue, .LField_MagmaRockStatue_SCENE
.LField_MagmaRockStatue_CELL_TILES:
	.incbin "games/tla/src/map/magma_rock_statue/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MagmaRockStatue_SHAPES:
	.incbin "games/tla/src/map/magma_rock_statue/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockStatue_GRID:
	.incbin "games/tla/src/map/magma_rock_statue/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockStatue_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_statue/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockStatue_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockStatue_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRockStatue_SCENE:
	.incbin "games/tla/src/map/magma_rock_statue/scene.scene.lz"
	.balign 4, 0
