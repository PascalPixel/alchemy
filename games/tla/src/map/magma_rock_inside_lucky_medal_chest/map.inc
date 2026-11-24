	.global Field_MagmaRockInsideLuckyMedalChest
Field_MagmaRockInsideLuckyMedalChest:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockInsideLuckyMedalChest, .LField_MagmaRockInsideLuckyMedalChest_CELL_TILES
	map_block Field_MagmaRockInsideLuckyMedalChest, .LField_MagmaRockInsideLuckyMedalChest_SHAPES
	map_block Field_MagmaRockInsideLuckyMedalChest, .LField_MagmaRockInsideLuckyMedalChest_GRID
	map_block Field_MagmaRockInsideLuckyMedalChest, .LField_MagmaRockInsideLuckyMedalChest_SHAPE_GRID
	map_block Field_MagmaRockInsideLuckyMedalChest, .LField_MagmaRockInsideLuckyMedalChest_ANIMATION
	map_block Field_MagmaRockInsideLuckyMedalChest, .LField_MagmaRockInsideLuckyMedalChest_BLEND
	map_block Field_MagmaRockInsideLuckyMedalChest, .LField_MagmaRockInsideLuckyMedalChest_SCENE
.LField_MagmaRockInsideLuckyMedalChest_CELL_TILES:
	.incbin "games/tla/src/map/magma_rock_inside_lucky_medal_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MagmaRockInsideLuckyMedalChest_SHAPES:
	.incbin "games/tla/src/map/magma_rock_inside_lucky_medal_chest/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockInsideLuckyMedalChest_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_lucky_medal_chest/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockInsideLuckyMedalChest_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_lucky_medal_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockInsideLuckyMedalChest_ANIMATION:
	.incbin "games/tla/src/map/magma_rock_inside_lucky_medal_chest/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideLuckyMedalChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideLuckyMedalChest_SCENE:
	.incbin "games/tla/src/map/magma_rock_inside_lucky_medal_chest/scene.scene.lz"
	.global Tileset_PaletteMagmaRockInsideLuckyMedalChest
Tileset_PaletteMagmaRockInsideLuckyMedalChest:
	.incbin "games/tla/src/graphics/tileset/palettes/magma_rock_inside_lucky_medal_chest.gbapal.lz"
