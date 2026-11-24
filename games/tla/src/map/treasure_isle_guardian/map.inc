	.global Field_TreasureIsleGuardian
Field_TreasureIsleGuardian:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TreasureIsleGuardian, .LField_TreasureIsleGuardian_CELL_TILES
	map_block Field_TreasureIsleGuardian, .LField_TreasureIsleGuardian_SHAPES
	map_block Field_TreasureIsleGuardian, .LField_TreasureIsleGuardian_GRID
	map_block Field_TreasureIsleGuardian, .LField_TreasureIsleGuardian_SHAPE_GRID
	map_block Field_TreasureIsleGuardian, .LField_TreasureIsleGuardian_ANIMATION
	map_block Field_TreasureIsleGuardian, .LField_TreasureIsleGuardian_BLEND
	map_block Field_TreasureIsleGuardian, .LField_TreasureIsleGuardian_SCENE
.LField_TreasureIsleGuardian_CELL_TILES:
	.incbin "games/tla/src/map/treasure_isle_guardian/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TreasureIsleGuardian_SHAPES:
	.incbin "games/tla/src/map/treasure_isle_guardian/shapes.table.lz"
	.balign 4, 0
.LField_TreasureIsleGuardian_GRID:
	.incbin "games/tla/src/map/treasure_isle_guardian/map.grid.lz"
	.balign 4, 0
.LField_TreasureIsleGuardian_SHAPE_GRID:
	.incbin "games/tla/src/map/treasure_isle_guardian/shape_grid.plane.lz"
	.balign 4, 0
.LField_TreasureIsleGuardian_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_TreasureIsleGuardian_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TreasureIsleGuardian_SCENE:
	.incbin "games/tla/src/map/treasure_isle_guardian/scene.scene.lz"
	.balign 4, 0
