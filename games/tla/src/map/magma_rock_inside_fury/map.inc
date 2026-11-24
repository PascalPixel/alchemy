	.global Field_MagmaRockInsideFury
Field_MagmaRockInsideFury:
	map_header 0, 0, 130, 112, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockInsideFury, .LField_MagmaRockInsideFury_CELL_TILES
	map_block Field_MagmaRockInsideFury, .LField_MagmaRockInsideFury_SHAPES
	map_block Field_MagmaRockInsideFury, .LField_MagmaRockInsideFury_GRID
	map_block Field_MagmaRockInsideFury, .LField_MagmaRockInsideFury_SHAPE_GRID
	map_block Field_MagmaRockInsideFury, .LField_MagmaRockInsideFury_ANIMATION
	map_block Field_MagmaRockInsideFury, .LField_MagmaRockInsideFury_BLEND
	map_block Field_MagmaRockInsideFury, .LField_MagmaRockInsideFury_SCENE
.LField_MagmaRockInsideFury_CELL_TILES:
	.incbin "games/tla/src/map/magma_rock_inside_fury/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MagmaRockInsideFury_SHAPES:
	.incbin "games/tla/src/map/magma_rock_inside_fury/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockInsideFury_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_fury/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockInsideFury_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_fury/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockInsideFury_ANIMATION:
	.incbin "games/tla/src/map/magma_rock_inside_entrance/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideFury_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideFury_SCENE:
	.incbin "games/tla/src/map/magma_rock_inside_fury/scene.scene.lz"
