	.global Field_MikasallaRooms
Field_MikasallaRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MikasallaRooms, .LField_MikasallaRooms_CELL_TILES
	map_block Field_MikasallaRooms, .LField_MikasallaRooms_SHAPES
	map_block Field_MikasallaRooms, .LField_MikasallaRooms_GRID
	map_block Field_MikasallaRooms, .LField_MikasallaRooms_SHAPE_GRID
	map_block Field_MikasallaRooms, .LField_MikasallaRooms_ANIMATION
	map_block Field_MikasallaRooms, .LField_MikasallaRooms_BLEND
	map_block Field_MikasallaRooms, .LField_MikasallaRooms_SCENE
.LField_MikasallaRooms_CELL_TILES:
	.incbin "games/tla/src/map/mikasalla_rooms/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MikasallaRooms_SHAPES:
	.incbin "games/tla/src/map/mikasalla_rooms/shapes.table.lz"
	.balign 4, 0
.LField_MikasallaRooms_GRID:
	.incbin "games/tla/src/map/mikasalla_rooms/map.grid.lz"
	.balign 4, 0
.LField_MikasallaRooms_SHAPE_GRID:
	.incbin "games/tla/src/map/mikasalla_rooms/shape_grid.plane.lz"
	.balign 4, 0
.LField_MikasallaRooms_ANIMATION:
	.incbin "games/tla/src/map/mikasalla_rooms/animation.script.lz"
	.balign 4, 0
.LField_MikasallaRooms_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_MikasallaRooms_SCENE:
	.incbin "games/tla/src/map/mikasalla_rooms/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MikasallaRoomsPalette
Tileset_MikasallaRoomsPalette:
	.incbin "games/tla/src/graphics/tileset/mikasalla_rooms/palette.gbapal.lz"
	.global Tileset_MikasallaRoomsTilesA
Tileset_MikasallaRoomsTilesA:
	.incbin "games/tla/src/graphics/tileset/mikasalla_rooms/tiles_a.4bpp.mtf"
	.byte 0x40, 0x4e, 0x01	@ PACKER: left in the buffer by Field_MikasallaRooms+0x20ed
	.global Tileset_MikasallaRoomsTilesB
Tileset_MikasallaRoomsTilesB:
	.incbin "games/tla/src/graphics/tileset/mikasalla_rooms/tiles_b.4bpp.mtf"
	.byte 0x43, 0x23	@ PACKER: left in the buffer by Tileset_MikasallaRoomsTilesA+0x1a1a
	.global Tileset_MikasallaRoomsTilesC
Tileset_MikasallaRoomsTilesC:
	.incbin "games/tla/src/graphics/tileset/mikasalla_rooms/tiles_c.4bpp.mtf"
	.byte 0x6e	@ PACKER: left in the buffer by Tileset_MikasallaRoomsTilesB+0x110b
	.global Tileset_MikasallaRoomsTilesD
Tileset_MikasallaRoomsTilesD:
	.incbin "games/tla/src/graphics/tileset/mikasalla_rooms/tiles_d.4bpp.mtf"
