	.global Field_AnemosInnerSanctumElevator
Field_AnemosInnerSanctumElevator:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnemosInnerSanctumElevator, .LField_AnemosInnerSanctumElevator_CELL_TILES
	map_block Field_AnemosInnerSanctumElevator, .LField_AnemosInnerSanctumElevator_SHAPES
	map_block Field_AnemosInnerSanctumElevator, .LField_AnemosInnerSanctumElevator_GRID
	map_block Field_AnemosInnerSanctumElevator, .LField_AnemosInnerSanctumElevator_SHAPE_GRID
	map_block Field_AnemosInnerSanctumElevator, .LField_AnemosInnerSanctumElevator_ANIMATION
	map_block Field_AnemosInnerSanctumElevator
	map_block Field_AnemosInnerSanctumElevator, .LField_AnemosInnerSanctumElevator_SCENE
.LField_AnemosInnerSanctumElevator_CELL_TILES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_elevator/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumElevator_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_elevator/shapes.table.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumElevator_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_elevator/map.grid.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumElevator_SHAPE_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_elevator/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumElevator_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumElevator_SCENE:
	.incbin "games/tla/src/map/anemos_inner_sanctum_elevator/scene.scene.lz"
	.balign 4, 0
