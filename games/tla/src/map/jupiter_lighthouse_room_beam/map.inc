	.global Field_JupiterLighthouseRoomBeam
Field_JupiterLighthouseRoomBeam:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoomBeam, .LField_JupiterLighthouseRoomBeam_CELL_TILES
	map_block Field_JupiterLighthouseRoomBeam, .LField_JupiterLighthouseRoomBeam_SHAPES
	map_block Field_JupiterLighthouseRoomBeam, .LField_JupiterLighthouseRoomBeam_GRID
	map_block Field_JupiterLighthouseRoomBeam, .LField_JupiterLighthouseRoomBeam_SHAPE_GRID
	map_block Field_JupiterLighthouseRoomBeam, .LField_JupiterLighthouseRoomBeam_ANIMATION
	map_block Field_JupiterLighthouseRoomBeam
	map_block Field_JupiterLighthouseRoomBeam, .LField_JupiterLighthouseRoomBeam_SCENE
.LField_JupiterLighthouseRoomBeam_CELL_TILES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_beam/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomBeam_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_beam/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomBeam_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_beam/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomBeam_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_beam/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomBeam_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomBeam_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_beam/scene.scene.lz"
	.balign 4, 0
