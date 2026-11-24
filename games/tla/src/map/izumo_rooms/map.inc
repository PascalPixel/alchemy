	.global Field_IzumoRooms
Field_IzumoRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_IzumoRooms, .LField_IzumoRooms_CELL_TILES
	map_block Field_IzumoRooms, .LField_IzumoRooms_SHAPES
	map_block Field_IzumoRooms, .LField_IzumoRooms_GRID
	map_block Field_IzumoRooms, .LField_IzumoRooms_SHAPE_GRID
	map_block Field_IzumoRooms, .LField_IzumoRooms_ANIMATION
	map_block Field_IzumoRooms, .LField_IzumoRooms_BLEND
	map_block Field_IzumoRooms, .LField_IzumoRooms_SCENE
.LField_IzumoRooms_CELL_TILES:
	.incbin "games/tla/src/map/izumo_rooms/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_IzumoRooms_SHAPES:
	.incbin "games/tla/src/map/izumo_rooms/shapes.table.lz"
	.balign 4, 0
.LField_IzumoRooms_GRID:
	.incbin "games/tla/src/map/izumo_rooms/map.grid.lz"
	.balign 4, 0
.LField_IzumoRooms_SHAPE_GRID:
	.incbin "games/tla/src/map/izumo_rooms/shape_grid.plane.lz"
	.balign 4, 0
.LField_IzumoRooms_ANIMATION:
	.incbin "games/tla/src/map/izumo_rooms/animation.script.lz"
	.balign 4, 0
.LField_IzumoRooms_BLEND:
	.incbin "games/tla/src/map/izumo_rooms/blend.script.lz"
	.balign 4, 0
.LField_IzumoRooms_SCENE:
	.incbin "games/tla/src/map/izumo_rooms/scene.scene.lz"
	.balign 4, 0
	.global Tileset_IzumoRoomsPalette
Tileset_IzumoRoomsPalette:
	.incbin "games/tla/src/graphics/tileset/izumo_rooms/palette.gbapal.lz"
	.byte 0xcc, 0x0c, 0x26	@ PACKER: left in the buffer by Field_IzumoRooms+0x1b1
	.global Tileset_IzumoRoomsTilesA
Tileset_IzumoRoomsTilesA:
	.incbin "games/tla/src/graphics/tileset/izumo_rooms/tiles_a.4bpp.mtf"
	.byte 0x05, 0x80, 0x00	@ PACKER: left in the buffer by Field_IzumoRooms+0x1701
	.global Tileset_IzumoRoomsTilesB
Tileset_IzumoRoomsTilesB:
	.incbin "games/tla/src/graphics/tileset/izumo_rooms/tiles_b.4bpp.mtf"
	.global Tileset_IzumoRoomsTilesC
Tileset_IzumoRoomsTilesC:
	.incbin "games/tla/src/graphics/tileset/izumo_rooms/tiles_c.4bpp.mtf"
	.byte 0x7a	@ PACKER: left in the buffer by Tileset_IzumoRoomsTilesB+0xc8f
	.global Tileset_IzumoRoomsTilesD
Tileset_IzumoRoomsTilesD:
	.incbin "games/tla/src/graphics/tileset/izumo_rooms/tiles_d.4bpp.mtf"
	.byte 0xb1, 0x74, 0x4f	@ PACKER: left in the buffer by Tileset_IzumoRoomsTilesB+0x1349
