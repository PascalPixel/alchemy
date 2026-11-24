	.global Field_GabombaCatacombsHiddenWall
Field_GabombaCatacombsHiddenWall:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaCatacombsHiddenWall, .LField_GabombaCatacombsHiddenWall_CELL_TILES
	map_block Field_GabombaCatacombsHiddenWall, .LField_GabombaCatacombsHiddenWall_SHAPES
	map_block Field_GabombaCatacombsHiddenWall, .LField_GabombaCatacombsHiddenWall_GRID
	map_block Field_GabombaCatacombsHiddenWall, .LField_GabombaCatacombsHiddenWall_SHAPE_GRID
	map_block Field_GabombaCatacombsHiddenWall, .LField_GabombaCatacombsHiddenWall_ANIMATION
	map_block Field_GabombaCatacombsHiddenWall, .LField_GabombaCatacombsHiddenWall_BLEND
	map_block Field_GabombaCatacombsHiddenWall, .LField_GabombaCatacombsHiddenWall_SCENE
.LField_GabombaCatacombsHiddenWall_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_catacombs_hidden_wall/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GabombaCatacombsHiddenWall_SHAPES:
	.incbin "games/tla/src/map/gabomba_catacombs_hidden_wall/shapes.table.lz"
	.balign 4, 0
.LField_GabombaCatacombsHiddenWall_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_hidden_wall/map.grid.lz"
	.balign 4, 0
.LField_GabombaCatacombsHiddenWall_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_hidden_wall/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaCatacombsHiddenWall_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/animation.script.lz"
	.balign 4, 0
.LField_GabombaCatacombsHiddenWall_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/blend.script.lz"
	.balign 4, 0
.LField_GabombaCatacombsHiddenWall_SCENE:
	.incbin "games/tla/src/map/gabomba_catacombs_hidden_wall/scene.scene.lz"
	.balign 4, 0
