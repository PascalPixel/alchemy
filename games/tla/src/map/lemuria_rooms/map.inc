	.global Field_LemuriaRooms
Field_LemuriaRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemuriaRooms, .LField_LemuriaRooms_CELL_TILES
	map_block Field_LemuriaRooms, .LField_LemuriaRooms_SHAPES
	map_block Field_LemuriaRooms, .LField_LemuriaRooms_GRID
	map_block Field_LemuriaRooms, .LField_LemuriaRooms_SHAPE_GRID
	map_block Field_LemuriaRooms, .LField_LemuriaRooms_ANIMATION
	map_block Field_LemuriaRooms
	map_block Field_LemuriaRooms, .LField_LemuriaRooms_SCENE
.LField_LemuriaRooms_CELL_TILES:
	.incbin "games/tla/src/map/lemuria_rooms/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_LemuriaRooms_SHAPES:
	.incbin "games/tla/src/map/lemuria_rooms/shapes.table.lz"
	.balign 4, 0
.LField_LemuriaRooms_GRID:
	.incbin "games/tla/src/map/lemuria_rooms/map.grid.lz"
	.balign 4, 0
.LField_LemuriaRooms_SHAPE_GRID:
	.incbin "games/tla/src/map/lemuria_rooms/shape_grid.plane.lz"
	.balign 4, 0
.LField_LemuriaRooms_ANIMATION:
	.incbin "games/tla/src/map/lemuria_rooms/animation.script.lz"
	.balign 4, 0
.LField_LemuriaRooms_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/lemuria_rooms/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/lemuria_rooms/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_LemuriaRoomsPalette
Tileset_LemuriaRoomsPalette:
	.incbin "games/tla/src/graphics/tileset/lemuria_rooms/palette.gbapal.lz"
	.byte 0x9d, 0xf8	@ PACKER: left in the buffer by Field_LemuriaRooms+0x1d2
	.global Tileset_LemuriaRoomsTilesA
Tileset_LemuriaRoomsTilesA:
	.incbin "games/tla/src/graphics/tileset/lemuria_rooms/tiles_a.4bpp.mtf"
	.byte 0x0a, 0x16	@ PACKER: left in the buffer by Field_LemuriaRooms+0x203e
	.global Tileset_LemuriaRoomsTilesB
Tileset_LemuriaRoomsTilesB:
	.incbin "games/tla/src/graphics/tileset/lemuria_rooms/tiles_b.4bpp.mtf"
	.byte 0x40, 0x60, 0xb9	@ PACKER: left in the buffer by Tileset_LemuriaRoomsTilesA+0x13f5
	.global Tileset_LemuriaRoomsTilesC
Tileset_LemuriaRoomsTilesC:
	.incbin "games/tla/src/graphics/tileset/lemuria_rooms/tiles_c.4bpp.mtf"
	.byte 0x44, 0xd3	@ PACKER: left in the buffer by Tileset_LemuriaRoomsTilesB+0xb3a
	.global Tileset_LemuriaRoomsTilesD
Tileset_LemuriaRoomsTilesD:
	.incbin "games/tla/src/graphics/tileset/lemuria_rooms/tiles_d.4bpp.mtf"
	.byte 0xbf, 0x45, 0xca	@ PACKER: left in the buffer by Tileset_LemuriaRoomsTilesB+0x1169
