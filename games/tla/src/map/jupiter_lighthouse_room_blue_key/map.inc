	.global Field_JupiterLighthouseRoomBlueKey
Field_JupiterLighthouseRoomBlueKey:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoomBlueKey, .LField_JupiterLighthouseRoomBlueKey_CELL_TILES
	map_block Field_JupiterLighthouseRoomBlueKey, .LField_JupiterLighthouseRoomBlueKey_SHAPES
	map_block Field_JupiterLighthouseRoomBlueKey, .LField_JupiterLighthouseRoomBlueKey_GRID
	map_block Field_JupiterLighthouseRoomBlueKey, .LField_JupiterLighthouseRoomBlueKey_SHAPE_GRID
	map_block Field_JupiterLighthouseRoomBlueKey, .LField_JupiterLighthouseRoomBlueKey_ANIMATION
	map_block Field_JupiterLighthouseRoomBlueKey
	map_block Field_JupiterLighthouseRoomBlueKey, .LField_JupiterLighthouseRoomBlueKey_SCENE
.LField_JupiterLighthouseRoomBlueKey_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_blue_key/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomBlueKey_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_blue_key/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomBlueKey_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_blue_key/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomBlueKey_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_blue_key/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomBlueKey_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomBlueKey_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_blue_key/scene.scene.lz"
	.balign 4, 0
