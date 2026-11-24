	.global Field_YampiDesertScorpionPit
Field_YampiDesertScorpionPit:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertScorpionPit, .LField_YampiDesertScorpionPit_CELL_TILES
	map_block Field_YampiDesertScorpionPit, .LField_YampiDesertScorpionPit_SHAPES
	map_block Field_YampiDesertScorpionPit, .LField_YampiDesertScorpionPit_GRID
	map_block Field_YampiDesertScorpionPit, .LField_YampiDesertScorpionPit_SHAPE_GRID
	map_block Field_YampiDesertScorpionPit, .LField_YampiDesertScorpionPit_ANIMATION
	map_block Field_YampiDesertScorpionPit, .LField_YampiDesertScorpionPit_BLEND
	map_block Field_YampiDesertScorpionPit, .LField_YampiDesertScorpionPit_SCENE
.LField_YampiDesertScorpionPit_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_scorpion_pit/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertScorpionPit_SHAPES:
	.incbin "games/tla/src/map/gondowan_cliffs_hideout/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertScorpionPit_GRID:
	.incbin "games/tla/src/map/yampi_desert_scorpion_pit/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertScorpionPit_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_scorpion_pit/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertScorpionPit_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertScorpionPit_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertScorpionPit_SCENE:
	.incbin "games/tla/src/map/yampi_desert_scorpion_pit/scene.scene.lz"
	.balign 4, 0
