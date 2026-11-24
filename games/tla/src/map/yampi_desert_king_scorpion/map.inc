	.global Field_YampiDesertKingScorpion
Field_YampiDesertKingScorpion:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertKingScorpion, .LField_YampiDesertKingScorpion_CELL_TILES
	map_block Field_YampiDesertKingScorpion, .LField_YampiDesertKingScorpion_SHAPES
	map_block Field_YampiDesertKingScorpion, .LField_YampiDesertKingScorpion_GRID
	map_block Field_YampiDesertKingScorpion, .LField_YampiDesertKingScorpion_SHAPE_GRID
	map_block Field_YampiDesertKingScorpion, .LField_YampiDesertKingScorpion_ANIMATION
	map_block Field_YampiDesertKingScorpion, .LField_YampiDesertKingScorpion_BLEND
	map_block Field_YampiDesertKingScorpion, .LField_YampiDesertKingScorpion_SCENE
.LField_YampiDesertKingScorpion_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_king_scorpion/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertKingScorpion_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_king_scorpion/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertKingScorpion_GRID:
	.incbin "games/tla/src/map/yampi_desert_king_scorpion/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertKingScorpion_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_king_scorpion/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertKingScorpion_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertKingScorpion_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertKingScorpion_SCENE:
	.incbin "games/tla/src/map/yampi_desert_king_scorpion/scene.scene.lz"
	.balign 4, 0
