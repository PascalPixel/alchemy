	.global Field_MagmaRockInsideEmberStone
Field_MagmaRockInsideEmberStone:
	map_header 0, 0, 130, 112, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockInsideEmberStone, .LField_MagmaRockInsideEmberStone_CELL_TILES
	map_block Field_MagmaRockInsideEmberStone, .LField_MagmaRockInsideEmberStone_SHAPES
	map_block Field_MagmaRockInsideEmberStone, .LField_MagmaRockInsideEmberStone_GRID
	map_block Field_MagmaRockInsideEmberStone, .LField_MagmaRockInsideEmberStone_SHAPE_GRID
	map_block Field_MagmaRockInsideEmberStone, .LField_MagmaRockInsideEmberStone_ANIMATION
	map_block Field_MagmaRockInsideEmberStone, .LField_MagmaRockInsideEmberStone_BLEND
	map_block Field_MagmaRockInsideEmberStone, .LField_MagmaRockInsideEmberStone_SCENE
.LField_MagmaRockInsideEmberStone_CELL_TILES:
	.incbin "games/tla/src/map/magma_rock_inside_ember_stone/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MagmaRockInsideEmberStone_SHAPES:
	.incbin "games/tla/src/map/magma_rock_inside_ember_stone/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockInsideEmberStone_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_ember_stone/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockInsideEmberStone_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_ember_stone/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockInsideEmberStone_ANIMATION:
	.incbin "games/tla/src/map/magma_rock_inside_ember_stone/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideEmberStone_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideEmberStone_SCENE:
	.incbin "games/tla/src/map/magma_rock_inside_ember_stone/scene.scene.lz"
	.balign 4, 0
