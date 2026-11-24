	.global Field_TaopoSwampDrips
Field_TaopoSwampDrips:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwampDrips, .LField_TaopoSwampDrips_CELL_TILES
	map_block Field_TaopoSwampDrips, .LField_TaopoSwampDrips_SHAPES
	map_block Field_TaopoSwampDrips, .LField_TaopoSwampDrips_GRID
	map_block Field_TaopoSwampDrips, .LField_TaopoSwampDrips_SHAPE_GRID
	map_block Field_TaopoSwampDrips, .LField_TaopoSwampDrips_ANIMATION
	map_block Field_TaopoSwampDrips, .LField_TaopoSwampDrips_BLEND
	map_block Field_TaopoSwampDrips, .LField_TaopoSwampDrips_SCENE
.LField_TaopoSwampDrips_CELL_TILES:
	.incbin "games/tla/src/map/taopo_swamp_drips/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TaopoSwampDrips_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_drips/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwampDrips_GRID:
	.incbin "games/tla/src/map/taopo_swamp_drips/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwampDrips_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_drips/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwampDrips_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_pillar_gap/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwampDrips_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwampDrips_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_drips/scene.scene.lz"
	.balign 4, 0
