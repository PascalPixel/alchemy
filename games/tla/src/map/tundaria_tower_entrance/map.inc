	.global Field_TundariaTowerEntrance
Field_TundariaTowerEntrance:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TundariaTowerEntrance, .LField_TundariaTowerEntrance_CELL_TILES
	map_block Field_TundariaTowerEntrance, .LField_TundariaTowerEntrance_SHAPES
	map_block Field_TundariaTowerEntrance, .LField_TundariaTowerEntrance_GRID
	map_block Field_TundariaTowerEntrance, .LField_TundariaTowerEntrance_SHAPE_GRID
	map_block Field_TundariaTowerEntrance, .LField_TundariaTowerEntrance_ANIMATION
	map_block Field_TundariaTowerEntrance
	map_block Field_TundariaTowerEntrance, .LField_TundariaTowerEntrance_SCENE
.LField_TundariaTowerEntrance_CELL_TILES:
	.incbin "games/tla/src/map/tundaria_tower_entrance/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_TundariaTowerEntrance_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_entrance/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTowerEntrance_GRID:
	.incbin "games/tla/src/map/tundaria_tower_entrance/map.grid.lz"
	.balign 4, 0
.LField_TundariaTowerEntrance_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_entrance/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTowerEntrance_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_TundariaTowerEntrance_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_entrance/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TundariaTowerEntrancePalette
Tileset_TundariaTowerEntrancePalette:
	.incbin "games/tla/src/graphics/tileset/tundaria_tower_entrance/palette.gbapal.lz"
	.byte 0xe6	@ PACKER: left in the buffer by Field_TundariaTowerEntrance+0x16b
	.global Tileset_TundariaTowerEntranceTilesA
Tileset_TundariaTowerEntranceTilesA:
	.incbin "games/tla/src/graphics/tileset/tundaria_tower_entrance/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x61	@ PACKER: left in the buffer by Field_TundariaTowerPillarPuzzle+0xfbe
