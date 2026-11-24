	.global Field_KandoreanTempleCurrent
Field_KandoreanTempleCurrent:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTempleCurrent, .LField_KandoreanTempleCurrent_CELL_TILES
	map_block Field_KandoreanTempleCurrent, .LField_KandoreanTempleCurrent_SHAPES
	map_block Field_KandoreanTempleCurrent, .LField_KandoreanTempleCurrent_GRID
	map_block Field_KandoreanTempleCurrent, .LField_KandoreanTempleCurrent_SHAPE_GRID
	map_block Field_KandoreanTempleCurrent, .LField_KandoreanTempleCurrent_ANIMATION
	map_block Field_KandoreanTempleCurrent, .LField_KandoreanTempleCurrent_BLEND
	map_block Field_KandoreanTempleCurrent, .LField_KandoreanTempleCurrent_SCENE
.LField_KandoreanTempleCurrent_CELL_TILES:
	.incbin "games/tla/src/map/kandorean_temple_current/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_KandoreanTempleCurrent_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_current/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTempleCurrent_GRID:
	.incbin "games/tla/src/map/kandorean_temple_current/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTempleCurrent_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_current/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTempleCurrent_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_current/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTempleCurrent_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KandoreanTempleCurrent_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_current/scene.scene.lz"
	.balign 4, 0
