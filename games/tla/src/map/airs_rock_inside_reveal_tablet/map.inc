	.global Field_AirsRockInsideRevealTablet
Field_AirsRockInsideRevealTablet:
	map_header 0, 0, 128, 128, 2, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 5, 5, 0x1f, 0x1f
	map_block Field_AirsRockInsideRevealTablet, .LField_AirsRockInsideRevealTablet_CELL_TILES
	map_block Field_AirsRockInsideRevealTablet, .LField_AirsRockInsideRevealTablet_SHAPES
	map_block Field_AirsRockInsideRevealTablet, .LField_AirsRockInsideRevealTablet_GRID
	map_block Field_AirsRockInsideRevealTablet, .LField_AirsRockInsideRevealTablet_SHAPE_GRID
	map_block Field_AirsRockInsideRevealTablet, .LField_AirsRockInsideRevealTablet_ANIMATION
	map_block Field_AirsRockInsideRevealTablet, .LField_AirsRockInsideRevealTablet_BLEND
	map_block Field_AirsRockInsideRevealTablet, .LField_AirsRockInsideRevealTablet_SCENE
.LField_AirsRockInsideRevealTablet_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_inside_reveal_tablet/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AirsRockInsideRevealTablet_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_reveal_tablet/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInsideRevealTablet_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_reveal_tablet/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInsideRevealTablet_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_reveal_tablet/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInsideRevealTablet_ANIMATION:
	.incbin "games/tla/src/map/airs_rock_inside_reveal_tablet/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInsideRevealTablet_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInsideRevealTablet_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_reveal_tablet/scene.scene.lz"
