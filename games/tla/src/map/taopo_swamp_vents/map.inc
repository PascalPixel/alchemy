	.global Field_TaopoSwampVents
Field_TaopoSwampVents:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwampVents, .LField_TaopoSwampVents_CELL_TILES
	map_block Field_TaopoSwampVents, .LField_TaopoSwampVents_SHAPES
	map_block Field_TaopoSwampVents, .LField_TaopoSwampVents_GRID
	map_block Field_TaopoSwampVents, .LField_TaopoSwampVents_SHAPE_GRID
	map_block Field_TaopoSwampVents, .LField_TaopoSwampVents_ANIMATION
	map_block Field_TaopoSwampVents, .LField_TaopoSwampVents_BLEND
	map_block Field_TaopoSwampVents, .LField_TaopoSwampVents_SCENE
.LField_TaopoSwampVents_CELL_TILES:
	.incbin "games/tla/src/map/taopo_swamp_vents/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_TaopoSwampVents_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_vents/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwampVents_GRID:
	.incbin "games/tla/src/map/taopo_swamp_vents/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwampVents_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_vents/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwampVents_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwampVents_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwampVents_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_vents/scene.scene.lz"
	.balign 4, 0
