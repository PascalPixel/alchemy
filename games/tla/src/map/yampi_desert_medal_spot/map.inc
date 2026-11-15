	.global Field_YampiDesertMedalSpot
Field_YampiDesertMedalSpot:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertMedalSpot, .LField_YampiDesertMedalSpot_CELL_PICTURES
	map_block Field_YampiDesertMedalSpot, .LField_YampiDesertMedalSpot_SHAPES
	map_block Field_YampiDesertMedalSpot, .LField_YampiDesertMedalSpot_GRID
	map_block Field_YampiDesertMedalSpot, .LField_YampiDesertMedalSpot_SHAPE_GRID
	map_block Field_YampiDesertMedalSpot, .LField_YampiDesertMedalSpot_ANIMATION
	map_block Field_YampiDesertMedalSpot, .LField_YampiDesertMedalSpot_BLEND
	map_block Field_YampiDesertMedalSpot, .LField_YampiDesertMedalSpot_SCENE
.LField_YampiDesertMedalSpot_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_medal_spot/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesertMedalSpot_SHAPES:
	.incbin "games/tla/src/map/gondowan_cliffs_hideout/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertMedalSpot_GRID:
	.incbin "games/tla/src/map/yampi_desert_medal_spot/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertMedalSpot_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_medal_spot/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertMedalSpot_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertMedalSpot_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertMedalSpot_SCENE:
	.incbin "games/tla/src/map/yampi_desert_medal_spot/scene.scene.lz"
	.balign 4, 0
