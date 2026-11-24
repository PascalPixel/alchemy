	.global Field_AnemosInnerSanctumGuardian
Field_AnemosInnerSanctumGuardian:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnemosInnerSanctumGuardian, .LField_AnemosInnerSanctumGuardian_CELL_TILES
	map_block Field_AnemosInnerSanctumGuardian, .LField_AnemosInnerSanctumGuardian_SHAPES
	map_block Field_AnemosInnerSanctumGuardian, .LField_AnemosInnerSanctumGuardian_GRID
	map_block Field_AnemosInnerSanctumGuardian, .LField_AnemosInnerSanctumGuardian_SHAPE_GRID
	map_block Field_AnemosInnerSanctumGuardian, .LField_AnemosInnerSanctumGuardian_ANIMATION
	map_block Field_AnemosInnerSanctumGuardian
	map_block Field_AnemosInnerSanctumGuardian, .LField_AnemosInnerSanctumGuardian_SCENE
.LField_AnemosInnerSanctumGuardian_CELL_TILES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_guardian/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumGuardian_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_guardian/shapes.table.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumGuardian_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_guardian/map.grid.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumGuardian_SHAPE_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_guardian/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumGuardian_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumGuardian_SCENE:
	.incbin "games/tla/src/map/anemos_inner_sanctum_guardian/scene.scene.lz"
	.balign 4, 0
