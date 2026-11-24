	.global Field_GabombaCatacombsMud
Field_GabombaCatacombsMud:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaCatacombsMud, .LField_GabombaCatacombsMud_CELL_TILES
	map_block Field_GabombaCatacombsMud, .LField_GabombaCatacombsMud_SHAPES
	map_block Field_GabombaCatacombsMud, .LField_GabombaCatacombsMud_GRID
	map_block Field_GabombaCatacombsMud, .LField_GabombaCatacombsMud_SHAPE_GRID
	map_block Field_GabombaCatacombsMud, .LField_GabombaCatacombsMud_ANIMATION
	map_block Field_GabombaCatacombsMud, .LField_GabombaCatacombsMud_BLEND
	map_block Field_GabombaCatacombsMud, .LField_GabombaCatacombsMud_SCENE
.LField_GabombaCatacombsMud_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_catacombs_mud/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GabombaCatacombsMud_SHAPES:
	.incbin "games/tla/src/map/gabomba_catacombs_mud/shapes.table.lz"
	.balign 4, 0
.LField_GabombaCatacombsMud_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_mud/map.grid.lz"
	.balign 4, 0
.LField_GabombaCatacombsMud_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_mud/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaCatacombsMud_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/animation.script.lz"
	.balign 4, 0
.LField_GabombaCatacombsMud_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/blend.script.lz"
	.balign 4, 0
.LField_GabombaCatacombsMud_SCENE:
	.incbin "games/tla/src/map/gabomba_catacombs_mud/scene.scene.lz"
	.balign 4, 0
