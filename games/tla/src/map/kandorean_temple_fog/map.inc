	.global Field_KandoreanTempleFog
Field_KandoreanTempleFog:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTempleFog, .LField_KandoreanTempleFog_CELL_TILES
	map_block Field_KandoreanTempleFog, .LField_KandoreanTempleFog_SHAPES
	map_block Field_KandoreanTempleFog, .LField_KandoreanTempleFog_GRID
	map_block Field_KandoreanTempleFog, .LField_KandoreanTempleFog_SHAPE_GRID
	map_block Field_KandoreanTempleFog, .LField_KandoreanTempleFog_ANIMATION
	map_block Field_KandoreanTempleFog, .LField_KandoreanTempleFog_BLEND
	map_block Field_KandoreanTempleFog, .LField_KandoreanTempleFog_SCENE
.LField_KandoreanTempleFog_CELL_TILES:
	.incbin "games/tla/src/map/kandorean_temple_fog/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_KandoreanTempleFog_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_fog/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTempleFog_GRID:
	.incbin "games/tla/src/map/kandorean_temple_fog/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTempleFog_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_fog/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTempleFog_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTempleFog_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KandoreanTempleFog_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_fog/scene.scene.lz"
	.balign 4, 0
