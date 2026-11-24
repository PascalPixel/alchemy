	.global Field_YampiDesertCoinsPuddle
Field_YampiDesertCoinsPuddle:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertCoinsPuddle, .LField_YampiDesertCoinsPuddle_CELL_TILES
	map_block Field_YampiDesertCoinsPuddle, .LField_YampiDesertCoinsPuddle_SHAPES
	map_block Field_YampiDesertCoinsPuddle, .LField_YampiDesertCoinsPuddle_GRID
	map_block Field_YampiDesertCoinsPuddle, .LField_YampiDesertCoinsPuddle_SHAPE_GRID
	map_block Field_YampiDesertCoinsPuddle, .LField_YampiDesertCoinsPuddle_ANIMATION
	map_block Field_YampiDesertCoinsPuddle, .LField_YampiDesertCoinsPuddle_BLEND
	map_block Field_YampiDesertCoinsPuddle, .LField_YampiDesertCoinsPuddle_SCENE
.LField_YampiDesertCoinsPuddle_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_coins_puddle/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertCoinsPuddle_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_footprints/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertCoinsPuddle_GRID:
	.incbin "games/tla/src/map/yampi_desert_coins_puddle/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertCoinsPuddle_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_coins_puddle/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertCoinsPuddle_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertCoinsPuddle_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertCoinsPuddle_SCENE:
	.incbin "games/tla/src/map/yampi_desert_coins_puddle/scene.scene.lz"
	.balign 4, 0
