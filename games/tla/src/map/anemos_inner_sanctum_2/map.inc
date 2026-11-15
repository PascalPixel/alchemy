	.global Field_AnemosInnerSanctum2
Field_AnemosInnerSanctum2:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnemosInnerSanctum2, .LField_AnemosInnerSanctum2_CELL_PICTURES
	map_block Field_AnemosInnerSanctum2, .LField_AnemosInnerSanctum2_SHAPES
	map_block Field_AnemosInnerSanctum2, .LField_AnemosInnerSanctum2_GRID
	map_block Field_AnemosInnerSanctum2, .LField_AnemosInnerSanctum2_SHAPE_GRID
	map_block Field_AnemosInnerSanctum2, .LField_AnemosInnerSanctum2_ANIMATION
	map_block Field_AnemosInnerSanctum2
	map_block Field_AnemosInnerSanctum2, .LField_AnemosInnerSanctum2_SCENE
.LField_AnemosInnerSanctum2_CELL_PICTURES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum2_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_2/shapes.table.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum2_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_2/map.grid.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum2_SHAPE_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum2_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum2_SCENE:
	.incbin "games/tla/src/map/anemos_inner_sanctum_2/scene.scene.lz"
	.balign 4, 0
