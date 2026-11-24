	.global Field_MarsLighthouseRoomMInscriptions
Field_MarsLighthouseRoomMInscriptions:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseRoomMInscriptions, .LField_MarsLighthouseRoomMInscriptions_CELL_TILES
	map_block Field_MarsLighthouseRoomMInscriptions, .LField_MarsLighthouseRoomMInscriptions_SHAPES
	map_block Field_MarsLighthouseRoomMInscriptions, .LField_MarsLighthouseRoomMInscriptions_GRID
	map_block Field_MarsLighthouseRoomMInscriptions, .LField_MarsLighthouseRoomMInscriptions_SHAPE_GRID
	map_block Field_MarsLighthouseRoomMInscriptions, .LField_MarsLighthouseRoomMInscriptions_ANIMATION
	map_block Field_MarsLighthouseRoomMInscriptions, .LField_MarsLighthouseRoomMInscriptions_BLEND
	map_block Field_MarsLighthouseRoomMInscriptions, .LField_MarsLighthouseRoomMInscriptions_SCENE
.LField_MarsLighthouseRoomMInscriptions_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_inscriptions/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMInscriptions_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_inscriptions/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMInscriptions_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_inscriptions/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMInscriptions_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_inscriptions/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMInscriptions_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_mars_tower/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMInscriptions_BLEND:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_tower/blend.script.lz"
	.balign 4, 0
.LField_MarsLighthouseRoomMInscriptions_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_room_m_inscriptions/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MarsLighthouseRoomMInscriptionsPalette
Tileset_MarsLighthouseRoomMInscriptionsPalette:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_m_inscriptions/palette.gbapal.lz"
	.byte 0x05, 0x18	@ PACKER: left in the buffer by Field_MarsLighthouseRoomMInscriptions+0x1b2
	.global Tileset_MarsLighthouseRoomMInscriptionsTilesA
Tileset_MarsLighthouseRoomMInscriptionsTilesA:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_m_inscriptions/tiles_a.4bpp.mtf"
	.byte 0x23, 0x44, 0x28	@ PACKER: left in the buffer by Tileset_MarsLighthouseAerieTilesA+0x223d
	.global Tileset_MarsLighthouseRoomMInscriptionsTilesB
Tileset_MarsLighthouseRoomMInscriptionsTilesB:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_room_m_inscriptions/tiles_b.4bpp.mtf"
	.byte 0x8e	@ PACKER: left in the buffer by Tileset_MarsLighthouseRoomMInscriptionsTilesA+0xb23
