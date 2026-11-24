	.global Field_MagmaRockInsideCrackedPillars
Field_MagmaRockInsideCrackedPillars:
	map_header 0, 0, 130, 130, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockInsideCrackedPillars, .LField_MagmaRockInsideCrackedPillars_CELL_TILES
	map_block Field_MagmaRockInsideCrackedPillars, .LField_MagmaRockInsideCrackedPillars_SHAPES
	map_block Field_MagmaRockInsideCrackedPillars, .LField_MagmaRockInsideCrackedPillars_GRID
	map_block Field_MagmaRockInsideCrackedPillars, .LField_MagmaRockInsideCrackedPillars_SHAPE_GRID
	map_block Field_MagmaRockInsideCrackedPillars, .LField_MagmaRockInsideCrackedPillars_ANIMATION
	map_block Field_MagmaRockInsideCrackedPillars, .LField_MagmaRockInsideCrackedPillars_BLEND
	map_block Field_MagmaRockInsideCrackedPillars, .LField_MagmaRockInsideCrackedPillars_SCENE
.LField_MagmaRockInsideCrackedPillars_CELL_TILES:
	.incbin "games/tla/src/map/magma_rock_inside_cracked_pillars/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MagmaRockInsideCrackedPillars_SHAPES:
	.incbin "games/tla/src/map/magma_rock_inside_cracked_pillars/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockInsideCrackedPillars_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_cracked_pillars/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockInsideCrackedPillars_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_cracked_pillars/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockInsideCrackedPillars_ANIMATION:
	.incbin "games/tla/src/map/magma_rock_inside_entrance/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideCrackedPillars_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideCrackedPillars_SCENE:
	.incbin "games/tla/src/map/magma_rock_inside_cracked_pillars/scene.scene.lz"
	.balign 4, 0
