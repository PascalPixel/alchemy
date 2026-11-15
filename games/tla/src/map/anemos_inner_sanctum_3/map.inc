	.global Field_AnemosInnerSanctum3
Field_AnemosInnerSanctum3:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnemosInnerSanctum3, .LField_AnemosInnerSanctum3_CELL_PICTURES
	map_block Field_AnemosInnerSanctum3, .LField_AnemosInnerSanctum3_SHAPES
	map_block Field_AnemosInnerSanctum3, .LField_AnemosInnerSanctum3_GRID
	map_block Field_AnemosInnerSanctum3, .LField_AnemosInnerSanctum3_SHAPE_GRID
	map_block Field_AnemosInnerSanctum3, .LField_AnemosInnerSanctum3_ANIMATION
	map_block Field_AnemosInnerSanctum3
	map_block Field_AnemosInnerSanctum3, .LField_AnemosInnerSanctum3_SCENE
.LField_AnemosInnerSanctum3_CELL_PICTURES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_3/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum3_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_3/shapes.table.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum3_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_3/map.grid.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum3_SHAPE_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum3_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctum3_SCENE:
	.incbin "games/tla/src/map/anemos_inner_sanctum_3/scene.scene.lz"
	.balign 4, 0
