	.global Field_TaopoSwampPillarGap
Field_TaopoSwampPillarGap:
	map_header 0, 0, 90, 100, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwampPillarGap, .LField_TaopoSwampPillarGap_CELL_TILES
	map_block Field_TaopoSwampPillarGap, .LField_TaopoSwampPillarGap_SHAPES
	map_block Field_TaopoSwampPillarGap, .LField_TaopoSwampPillarGap_GRID
	map_block Field_TaopoSwampPillarGap, .LField_TaopoSwampPillarGap_SHAPE_GRID
	map_block Field_TaopoSwampPillarGap, .LField_TaopoSwampPillarGap_ANIMATION
	map_block Field_TaopoSwampPillarGap, .LField_TaopoSwampPillarGap_BLEND
	map_block Field_TaopoSwampPillarGap, .LField_TaopoSwampPillarGap_SCENE
.LField_TaopoSwampPillarGap_CELL_TILES:
	.incbin "games/tla/src/map/taopo_swamp_pillar_gap/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TaopoSwampPillarGap_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_pillar_gap/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwampPillarGap_GRID:
	.incbin "games/tla/src/map/taopo_swamp_pillar_gap/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwampPillarGap_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_pillar_gap/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwampPillarGap_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_pillar_gap/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwampPillarGap_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwampPillarGap_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_pillar_gap/scene.scene.lz"
