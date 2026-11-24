	.global Field_AnemosInnerSanctumOrihalconChest
Field_AnemosInnerSanctumOrihalconChest:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnemosInnerSanctumOrihalconChest, .LField_AnemosInnerSanctumOrihalconChest_CELL_TILES
	map_block Field_AnemosInnerSanctumOrihalconChest, .LField_AnemosInnerSanctumOrihalconChest_SHAPES
	map_block Field_AnemosInnerSanctumOrihalconChest, .LField_AnemosInnerSanctumOrihalconChest_GRID
	map_block Field_AnemosInnerSanctumOrihalconChest, .LField_AnemosInnerSanctumOrihalconChest_SHAPE_GRID
	map_block Field_AnemosInnerSanctumOrihalconChest, .LField_AnemosInnerSanctumOrihalconChest_ANIMATION
	map_block Field_AnemosInnerSanctumOrihalconChest
	map_block Field_AnemosInnerSanctumOrihalconChest, .LField_AnemosInnerSanctumOrihalconChest_SCENE
.LField_AnemosInnerSanctumOrihalconChest_CELL_TILES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_orihalcon_chest/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumOrihalconChest_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_orihalcon_chest/shapes.table.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumOrihalconChest_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_orihalcon_chest/map.grid.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumOrihalconChest_SHAPE_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_orihalcon_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumOrihalconChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumOrihalconChest_SCENE:
	.incbin "games/tla/src/map/anemos_inner_sanctum_orihalcon_chest/scene.scene.lz"
