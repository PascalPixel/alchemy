	.global Field_YampiDesertTrainersWhipChest
Field_YampiDesertTrainersWhipChest:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertTrainersWhipChest, .LField_YampiDesertTrainersWhipChest_CELL_TILES
	map_block Field_YampiDesertTrainersWhipChest, .LField_YampiDesertTrainersWhipChest_SHAPES
	map_block Field_YampiDesertTrainersWhipChest, .LField_YampiDesertTrainersWhipChest_GRID
	map_block Field_YampiDesertTrainersWhipChest, .LField_YampiDesertTrainersWhipChest_SHAPE_GRID
	map_block Field_YampiDesertTrainersWhipChest, .LField_YampiDesertTrainersWhipChest_ANIMATION
	map_block Field_YampiDesertTrainersWhipChest, .LField_YampiDesertTrainersWhipChest_BLEND
	map_block Field_YampiDesertTrainersWhipChest, .LField_YampiDesertTrainersWhipChest_SCENE
.LField_YampiDesertTrainersWhipChest_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_trainers_whip_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertTrainersWhipChest_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_trainers_whip_chest/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertTrainersWhipChest_GRID:
	.incbin "games/tla/src/map/yampi_desert_trainers_whip_chest/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertTrainersWhipChest_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_trainers_whip_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertTrainersWhipChest_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_trainers_whip_chest/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertTrainersWhipChest_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertTrainersWhipChest_SCENE:
	.incbin "games/tla/src/map/yampi_desert_trainers_whip_chest/scene.scene.lz"
