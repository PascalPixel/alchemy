	.global Field_AnemosInnerSanctumEntrance
Field_AnemosInnerSanctumEntrance:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnemosInnerSanctumEntrance, .LField_AnemosInnerSanctumEntrance_CELL_PICTURES
	map_block Field_AnemosInnerSanctumEntrance, .LField_AnemosInnerSanctumEntrance_SHAPES
	map_block Field_AnemosInnerSanctumEntrance, .LField_AnemosInnerSanctumEntrance_GRID
	map_block Field_AnemosInnerSanctumEntrance, .LField_AnemosInnerSanctumEntrance_SHAPE_GRID
	map_block Field_AnemosInnerSanctumEntrance, .LField_AnemosInnerSanctumEntrance_ANIMATION
	map_block Field_AnemosInnerSanctumEntrance, .LField_AnemosInnerSanctumEntrance_BLEND
	map_block Field_AnemosInnerSanctumEntrance, .LField_AnemosInnerSanctumEntrance_SCENE
.LField_AnemosInnerSanctumEntrance_CELL_PICTURES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_entrance/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumEntrance_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_entrance/shapes.table.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumEntrance_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_entrance/map.grid.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumEntrance_SHAPE_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_entrance/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumEntrance_ANIMATION:
	.incbin "games/tla/src/map/map_182/animation.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumEntrance_BLEND:
	.incbin "games/tla/src/map/anemos_inner_sanctum_entrance/blend.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumEntrance_SCENE:
	.incbin "games/tla/src/map/anemos_inner_sanctum_entrance/scene.scene.lz"
