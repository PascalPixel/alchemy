	.global Field_JupiterLighthouseRoomLockedDoors
Field_JupiterLighthouseRoomLockedDoors:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoomLockedDoors, .LField_JupiterLighthouseRoomLockedDoors_CELL_TILES
	map_block Field_JupiterLighthouseRoomLockedDoors, .LField_JupiterLighthouseRoomLockedDoors_SHAPES
	map_block Field_JupiterLighthouseRoomLockedDoors, .LField_JupiterLighthouseRoomLockedDoors_GRID
	map_block Field_JupiterLighthouseRoomLockedDoors, .LField_JupiterLighthouseRoomLockedDoors_SHAPE_GRID
	map_block Field_JupiterLighthouseRoomLockedDoors, .LField_JupiterLighthouseRoomLockedDoors_ANIMATION
	map_block Field_JupiterLighthouseRoomLockedDoors
	map_block Field_JupiterLighthouseRoomLockedDoors, .LField_JupiterLighthouseRoomLockedDoors_SCENE
.LField_JupiterLighthouseRoomLockedDoors_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_locked_doors/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomLockedDoors_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_locked_doors/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomLockedDoors_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_locked_doors/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomLockedDoors_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_locked_doors/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomLockedDoors_ANIMATION:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_locked_doors/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomLockedDoors_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_locked_doors/scene.scene.lz"
	.balign 4, 0
