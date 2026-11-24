	.global Field_AirsRockFujinShieldChest
Field_AirsRockFujinShieldChest:
	map_header 0, 0, 128, 128, 2, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockFujinShieldChest, .LField_AirsRockFujinShieldChest_CELL_TILES
	map_block Field_AirsRockFujinShieldChest, .LField_AirsRockFujinShieldChest_SHAPES
	map_block Field_AirsRockFujinShieldChest, .LField_AirsRockFujinShieldChest_GRID
	map_block Field_AirsRockFujinShieldChest, .LField_AirsRockFujinShieldChest_SHAPE_GRID
	map_block Field_AirsRockFujinShieldChest, .LField_AirsRockFujinShieldChest_ANIMATION
	map_block Field_AirsRockFujinShieldChest, .LField_AirsRockFujinShieldChest_BLEND
	map_block Field_AirsRockFujinShieldChest, .LField_AirsRockFujinShieldChest_SCENE
.LField_AirsRockFujinShieldChest_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_fujin_shield_chest/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AirsRockFujinShieldChest_SHAPES:
	.incbin "games/tla/src/map/airs_rock_fujin_shield_chest/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockFujinShieldChest_GRID:
	.incbin "games/tla/src/map/airs_rock_fujin_shield_chest/map.grid.lz"
	.balign 4, 0
.LField_AirsRockFujinShieldChest_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_fujin_shield_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockFujinShieldChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AirsRockFujinShieldChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockFujinShieldChest_SCENE:
	.incbin "games/tla/src/map/airs_rock_fujin_shield_chest/scene.scene.lz"
	.balign 4, 0
