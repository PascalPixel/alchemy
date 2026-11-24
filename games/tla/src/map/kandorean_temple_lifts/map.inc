	.global Field_KandoreanTempleLifts
Field_KandoreanTempleLifts:
	map_header 0, 0, 110, 110, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTempleLifts, .LField_KandoreanTempleLifts_CELL_TILES
	map_block Field_KandoreanTempleLifts, .LField_KandoreanTempleLifts_SHAPES
	map_block Field_KandoreanTempleLifts, .LField_KandoreanTempleLifts_GRID
	map_block Field_KandoreanTempleLifts, .LField_KandoreanTempleLifts_SHAPE_GRID
	map_block Field_KandoreanTempleLifts, .LField_KandoreanTempleLifts_ANIMATION
	map_block Field_KandoreanTempleLifts, .LField_KandoreanTempleLifts_BLEND
	map_block Field_KandoreanTempleLifts, .LField_KandoreanTempleLifts_SCENE
.LField_KandoreanTempleLifts_CELL_TILES:
	.incbin "games/tla/src/map/kandorean_temple_lifts/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_KandoreanTempleLifts_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_lifts/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTempleLifts_GRID:
	.incbin "games/tla/src/map/kandorean_temple_lifts/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTempleLifts_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_lifts/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTempleLifts_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTempleLifts_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KandoreanTempleLifts_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_lifts/scene.scene.lz"
	.balign 4, 0
