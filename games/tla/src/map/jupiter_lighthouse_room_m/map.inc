	.global Field_JupiterLighthouseRoomM
Field_JupiterLighthouseRoomM:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoomM, .LField_JupiterLighthouseRoomM_CELL_PICTURES
	map_block Field_JupiterLighthouseRoomM, .LField_JupiterLighthouseRoomM_SHAPES
	map_block Field_JupiterLighthouseRoomM, .LField_JupiterLighthouseRoomM_GRID
	map_block Field_JupiterLighthouseRoomM, .LField_JupiterLighthouseRoomM_SHAPE_GRID
	map_block Field_JupiterLighthouseRoomM, .LField_JupiterLighthouseRoomM_ANIMATION
	map_block Field_JupiterLighthouseRoomM
	map_block Field_JupiterLighthouseRoomM, .LField_JupiterLighthouseRoomM_SCENE
.LField_JupiterLighthouseRoomM_CELL_PICTURES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_m/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomM_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_m/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomM_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_m/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomM_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_m/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomM_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoomM_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_m/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Palette55
Tileset_Palette55:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_55.gbapal.lz"
	.byte 0x34, 0x61, 0x91	@ PACKER: left in the buffer by Field_JupiterLighthouseRoomM+0x13d
