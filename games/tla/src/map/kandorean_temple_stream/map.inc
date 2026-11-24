	.global Field_KandoreanTempleStream
Field_KandoreanTempleStream:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTempleStream, .LField_KandoreanTempleStream_CELL_TILES
	map_block Field_KandoreanTempleStream, .LField_KandoreanTempleStream_SHAPES
	map_block Field_KandoreanTempleStream, .LField_KandoreanTempleStream_GRID
	map_block Field_KandoreanTempleStream, .LField_KandoreanTempleStream_SHAPE_GRID
	map_block Field_KandoreanTempleStream, .LField_KandoreanTempleStream_ANIMATION
	map_block Field_KandoreanTempleStream, .LField_KandoreanTempleStream_BLEND
	map_block Field_KandoreanTempleStream, .LField_KandoreanTempleStream_SCENE
.LField_KandoreanTempleStream_CELL_TILES:
	.incbin "games/tla/src/map/kandorean_temple_stream/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_KandoreanTempleStream_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_stream/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTempleStream_GRID:
	.incbin "games/tla/src/map/kandorean_temple_stream/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTempleStream_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_stream/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTempleStream_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_stream/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTempleStream_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KandoreanTempleStream_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_stream/scene.scene.lz"
	.balign 4, 0
