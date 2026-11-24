	.global Field_GabombaCatacombsStream
Field_GabombaCatacombsStream:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaCatacombsStream, .LField_GabombaCatacombsStream_CELL_TILES
	map_block Field_GabombaCatacombsStream, .LField_GabombaCatacombsStream_SHAPES
	map_block Field_GabombaCatacombsStream, .LField_GabombaCatacombsStream_GRID
	map_block Field_GabombaCatacombsStream, .LField_GabombaCatacombsStream_SHAPE_GRID
	map_block Field_GabombaCatacombsStream, .LField_GabombaCatacombsStream_ANIMATION
	map_block Field_GabombaCatacombsStream, .LField_GabombaCatacombsStream_BLEND
	map_block Field_GabombaCatacombsStream, .LField_GabombaCatacombsStream_SCENE
.LField_GabombaCatacombsStream_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_catacombs_stream/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GabombaCatacombsStream_SHAPES:
	.incbin "games/tla/src/map/gabomba_catacombs_stream/shapes.table.lz"
	.balign 4, 0
.LField_GabombaCatacombsStream_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_stream/map.grid.lz"
	.balign 4, 0
.LField_GabombaCatacombsStream_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_stream/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaCatacombsStream_ANIMATION:
	.incbin "games/tla/src/map/gabomba_catacombs_stream/animation.script.lz"
	.balign 4, 0
.LField_GabombaCatacombsStream_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaCatacombsStream_SCENE:
	.incbin "games/tla/src/map/gabomba_catacombs_stream/scene.scene.lz"
