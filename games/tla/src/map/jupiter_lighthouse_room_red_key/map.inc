	.global Field_JupiterLighthouseRoomRedKey
Field_JupiterLighthouseRoomRedKey:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoomRedKey, .LField_JupiterLighthouseRoomRedKey_CELL_TILES
	map_block Field_JupiterLighthouseRoomRedKey, .LField_JupiterLighthouseRoomRedKey_SHAPES
	map_block Field_JupiterLighthouseRoomRedKey, .LField_JupiterLighthouseRoomRedKey_GRID
	map_block Field_JupiterLighthouseRoomRedKey, .LField_JupiterLighthouseRoomRedKey_SHAPE_GRID
	map_block Field_JupiterLighthouseRoomRedKey, .LField_JupiterLighthouseRoomRedKey_ANIMATION
	map_block Field_JupiterLighthouseRoomRedKey, .LField_JupiterLighthouseRoomRedKey_BLEND
	map_block Field_JupiterLighthouseRoomRedKey, .LField_JupiterLighthouseRoomRedKey_SCENE
.LField_JupiterLighthouseRoomRedKey_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_red_key/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomRedKey_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_red_key/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomRedKey_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_red_key/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomRedKey_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_red_key/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomRedKey_ANIMATION:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_red_key/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomRedKey_BLEND:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_red_key/blend.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomRedKey_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_red_key/scene.scene.lz"
	.balign 4, 0
