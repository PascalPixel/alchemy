	.global Field_TaopoSwampStarDust
Field_TaopoSwampStarDust:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_TaopoSwampStarDust, .LField_TaopoSwampStarDust_CELL_TILES
	map_block Field_TaopoSwampStarDust, .LField_TaopoSwampStarDust_SHAPES
	map_block Field_TaopoSwampStarDust, .LField_TaopoSwampStarDust_GRID
	map_block Field_TaopoSwampStarDust, .LField_TaopoSwampStarDust_SHAPE_GRID
	map_block Field_TaopoSwampStarDust, .LField_TaopoSwampStarDust_ANIMATION
	map_block Field_TaopoSwampStarDust, .LField_TaopoSwampStarDust_BLEND
	map_block Field_TaopoSwampStarDust, .LField_TaopoSwampStarDust_SCENE
.LField_TaopoSwampStarDust_CELL_TILES:
	.incbin "games/tla/src/map/taopo_swamp_star_dust/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TaopoSwampStarDust_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_star_dust/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwampStarDust_GRID:
	.incbin "games/tla/src/map/taopo_swamp_star_dust/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwampStarDust_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_star_dust/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwampStarDust_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_tremor/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwampStarDust_BLEND:
	.incbin "games/tla/src/map/taopo_swamp_star_dust/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwampStarDust_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_star_dust/scene.scene.lz"
	.balign 4, 0
