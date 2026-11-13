	.global Field_Lobby
Field_Lobby:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Lobby, .LField_Lobby_CELL_PICTURES
	map_block Field_Lobby, .LField_Lobby_SHAPES
	map_block Field_Lobby, .LField_Lobby_GRID
	map_block Field_Lobby, .LField_Lobby_SHAPE_GRID
	map_block Field_Lobby, .LField_Lobby_ANIMATION
	map_block Field_Lobby, .LField_Lobby_BLEND
	map_block Field_Lobby, .LField_Lobby_SCENE
.LField_Lobby_CELL_PICTURES:
	.incbin "games/tla/src/map/lobby/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Lobby_SHAPES:
	.incbin "games/tla/src/map/lobby/shapes.table.lz"
	.balign 4, 0
.LField_Lobby_GRID:
	.incbin "games/tla/src/map/lobby/map.grid.lz"
	.balign 4, 0
.LField_Lobby_SHAPE_GRID:
	.incbin "games/tla/src/map/lobby/shape_grid.plane.lz"
	.balign 4, 0
.LField_Lobby_ANIMATION:
	.incbin "games/tla/src/map/lobby/animation.script.lz"
	.balign 4, 0
.LField_Lobby_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Lobby_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Tileset_LobbyPalette
Tileset_LobbyPalette:
	.incbin "games/tla/src/graphics/tileset/lobby/palette.gbapal.lz"
	.byte 0x85, 0x50	@ PACKER: left in the buffer by Field_Lobby+0x1ce
	.global Tileset_LobbyTilesA
Tileset_LobbyTilesA:
	.incbin "games/tla/src/graphics/tileset/lobby/tiles_a.4bpp.mtf"
	.byte 0x92	@ PACKER: left in the buffer by Monster_RedWarriorSprites+0x209f
	.global Tileset_LobbyTilesB
Tileset_LobbyTilesB:
	.incbin "games/tla/src/graphics/tileset/lobby/tiles_b.4bpp.mtf"
	.global Tileset_LobbyTilesC
Tileset_LobbyTilesC:
	.incbin "games/tla/src/graphics/tileset/lobby/tiles_c.4bpp.mtf"
	.byte 0x4c, 0x9e, 0x10	@ PACKER: left in the buffer by Monster_RedWarriorSprites+0x22d5
	.global Tileset_LobbyTilesD
Tileset_LobbyTilesD:
	.incbin "games/tla/src/graphics/tileset/lobby/tiles_d.4bpp.mtf"
