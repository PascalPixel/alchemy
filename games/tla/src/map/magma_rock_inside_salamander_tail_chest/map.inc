	.global Field_MagmaRockInsideSalamanderTailChest
Field_MagmaRockInsideSalamanderTailChest:
	map_header 0, 0, 130, 112, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockInsideSalamanderTailChest, .LField_MagmaRockInsideSalamanderTailChest_CELL_TILES
	map_block Field_MagmaRockInsideSalamanderTailChest, .LField_MagmaRockInsideSalamanderTailChest_SHAPES
	map_block Field_MagmaRockInsideSalamanderTailChest, .LField_MagmaRockInsideSalamanderTailChest_GRID
	map_block Field_MagmaRockInsideSalamanderTailChest, .LField_MagmaRockInsideSalamanderTailChest_SHAPE_GRID
	map_block Field_MagmaRockInsideSalamanderTailChest, .LField_MagmaRockInsideSalamanderTailChest_ANIMATION
	map_block Field_MagmaRockInsideSalamanderTailChest, .LField_MagmaRockInsideSalamanderTailChest_BLEND
	map_block Field_MagmaRockInsideSalamanderTailChest, .LField_MagmaRockInsideSalamanderTailChest_SCENE
.LField_MagmaRockInsideSalamanderTailChest_CELL_TILES:
	.incbin "games/tla/src/map/magma_rock_inside_salamander_tail_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MagmaRockInsideSalamanderTailChest_SHAPES:
	.incbin "games/tla/src/map/magma_rock_inside_salamander_tail_chest/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockInsideSalamanderTailChest_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_salamander_tail_chest/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockInsideSalamanderTailChest_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_salamander_tail_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockInsideSalamanderTailChest_ANIMATION:
	.incbin "games/tla/src/map/magma_rock_inside_entrance/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideSalamanderTailChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideSalamanderTailChest_SCENE:
	.incbin "games/tla/src/map/magma_rock_inside_salamander_tail_chest/scene.scene.lz"
	.balign 4, 0
