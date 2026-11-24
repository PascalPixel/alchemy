	.global Field_GabombaCatacombsTomegathericon
Field_GabombaCatacombsTomegathericon:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaCatacombsTomegathericon, .LField_GabombaCatacombsTomegathericon_CELL_TILES
	map_block Field_GabombaCatacombsTomegathericon, .LField_GabombaCatacombsTomegathericon_SHAPES
	map_block Field_GabombaCatacombsTomegathericon, .LField_GabombaCatacombsTomegathericon_GRID
	map_block Field_GabombaCatacombsTomegathericon, .LField_GabombaCatacombsTomegathericon_SHAPE_GRID
	map_block Field_GabombaCatacombsTomegathericon, .LField_GabombaCatacombsTomegathericon_ANIMATION
	map_block Field_GabombaCatacombsTomegathericon, .LField_GabombaCatacombsTomegathericon_BLEND
	map_block Field_GabombaCatacombsTomegathericon, .LField_GabombaCatacombsTomegathericon_SCENE
.LField_GabombaCatacombsTomegathericon_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_catacombs_tomegathericon/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GabombaCatacombsTomegathericon_SHAPES:
	.incbin "games/tla/src/map/gabomba_catacombs_tomegathericon/shapes.table.lz"
	.balign 4, 0
.LField_GabombaCatacombsTomegathericon_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_tomegathericon/map.grid.lz"
	.balign 4, 0
.LField_GabombaCatacombsTomegathericon_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_tomegathericon/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaCatacombsTomegathericon_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/animation.script.lz"
	.balign 4, 0
.LField_GabombaCatacombsTomegathericon_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/blend.script.lz"
	.balign 4, 0
.LField_GabombaCatacombsTomegathericon_SCENE:
	.incbin "games/tla/src/map/gabomba_catacombs_tomegathericon/scene.scene.lz"
	.balign 4, 0
