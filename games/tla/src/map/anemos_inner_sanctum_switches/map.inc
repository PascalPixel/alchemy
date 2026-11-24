	.global Field_AnemosInnerSanctumSwitches
Field_AnemosInnerSanctumSwitches:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnemosInnerSanctumSwitches, .LField_AnemosInnerSanctumSwitches_CELL_TILES
	map_block Field_AnemosInnerSanctumSwitches, .LField_AnemosInnerSanctumSwitches_SHAPES
	map_block Field_AnemosInnerSanctumSwitches, .LField_AnemosInnerSanctumSwitches_GRID
	map_block Field_AnemosInnerSanctumSwitches, .LField_AnemosInnerSanctumSwitches_SHAPE_GRID
	map_block Field_AnemosInnerSanctumSwitches, .LField_AnemosInnerSanctumSwitches_ANIMATION
	map_block Field_AnemosInnerSanctumSwitches
	map_block Field_AnemosInnerSanctumSwitches, .LField_AnemosInnerSanctumSwitches_SCENE
.LField_AnemosInnerSanctumSwitches_CELL_TILES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_switches/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumSwitches_SHAPES:
	.incbin "games/tla/src/map/anemos_inner_sanctum_switches/shapes.table.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumSwitches_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_switches/map.grid.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumSwitches_SHAPE_GRID:
	.incbin "games/tla/src/map/anemos_inner_sanctum_switches/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumSwitches_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AnemosInnerSanctumSwitches_SCENE:
	.incbin "games/tla/src/map/anemos_inner_sanctum_switches/scene.scene.lz"
	.balign 4, 0
