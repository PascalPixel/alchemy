	.global Field_JupiterLighthouseRoomCrackedFloor
Field_JupiterLighthouseRoomCrackedFloor:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoomCrackedFloor, .LField_JupiterLighthouseRoomCrackedFloor_CELL_TILES
	map_block Field_JupiterLighthouseRoomCrackedFloor, .LField_JupiterLighthouseRoomCrackedFloor_SHAPES
	map_block Field_JupiterLighthouseRoomCrackedFloor, .LField_JupiterLighthouseRoomCrackedFloor_GRID
	map_block Field_JupiterLighthouseRoomCrackedFloor, .LField_JupiterLighthouseRoomCrackedFloor_SHAPE_GRID
	map_block Field_JupiterLighthouseRoomCrackedFloor, .LField_JupiterLighthouseRoomCrackedFloor_ANIMATION
	map_block Field_JupiterLighthouseRoomCrackedFloor
	map_block Field_JupiterLighthouseRoomCrackedFloor, .LField_JupiterLighthouseRoomCrackedFloor_SCENE
.LField_JupiterLighthouseRoomCrackedFloor_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_cracked_floor/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomCrackedFloor_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_cracked_floor/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomCrackedFloor_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_cracked_floor/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomCrackedFloor_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_cracked_floor/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomCrackedFloor_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomCrackedFloor_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_cracked_floor/scene.scene.lz"
	.balign 4, 0
