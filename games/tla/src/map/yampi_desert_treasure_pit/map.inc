	.global Field_YampiDesertTreasurePit
Field_YampiDesertTreasurePit:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertTreasurePit, .LField_YampiDesertTreasurePit_CELL_PICTURES
	map_block Field_YampiDesertTreasurePit, .LField_YampiDesertTreasurePit_SHAPES
	map_block Field_YampiDesertTreasurePit, .LField_YampiDesertTreasurePit_GRID
	map_block Field_YampiDesertTreasurePit, .LField_YampiDesertTreasurePit_SHAPE_GRID
	map_block Field_YampiDesertTreasurePit, .LField_YampiDesertTreasurePit_ANIMATION
	map_block Field_YampiDesertTreasurePit, .LField_YampiDesertTreasurePit_BLEND
	map_block Field_YampiDesertTreasurePit, .LField_YampiDesertTreasurePit_SCENE
.LField_YampiDesertTreasurePit_CELL_PICTURES:
	.incbin "games/tla/src/map/yampi_desert_treasure_pit/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YampiDesertTreasurePit_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_4/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertTreasurePit_GRID:
	.incbin "games/tla/src/map/yampi_desert_treasure_pit/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertTreasurePit_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_treasure_pit/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertTreasurePit_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertTreasurePit_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertTreasurePit_SCENE:
	.incbin "games/tla/src/map/yampi_desert_treasure_pit/scene.scene.lz"
	.balign 4, 0
