	.global Field_LohoRooms
Field_LohoRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LohoRooms, .LField_LohoRooms_CELL_TILES
	map_block Field_LohoRooms, .LField_LohoRooms_SHAPES
	map_block Field_LohoRooms, .LField_LohoRooms_GRID
	map_block Field_LohoRooms, .LField_LohoRooms_SHAPE_GRID
	map_block Field_LohoRooms, .LField_LohoRooms_ANIMATION
	map_block Field_LohoRooms, .LField_LohoRooms_BLEND
	map_block Field_LohoRooms, .LField_LohoRooms_SCENE
.LField_LohoRooms_CELL_TILES:
	.incbin "games/tla/src/map/loho_rooms/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_LohoRooms_SHAPES:
	.incbin "games/tla/src/map/loho_rooms/shapes.table.lz"
	.balign 4, 0
.LField_LohoRooms_GRID:
	.incbin "games/tla/src/map/loho_rooms/map.grid.lz"
	.balign 4, 0
.LField_LohoRooms_SHAPE_GRID:
	.incbin "games/tla/src/map/loho_rooms/shape_grid.plane.lz"
	.balign 4, 0
.LField_LohoRooms_ANIMATION:
	.incbin "games/tla/src/map/loho_rooms/animation.script.lz"
	.balign 4, 0
.LField_LohoRooms_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_LohoRooms_SCENE:
	.incbin "games/tla/src/map/loho_rooms/scene.scene.lz"
	.balign 4, 0
	.global Tileset_LohoRoomsPalette
Tileset_LohoRoomsPalette:
	.incbin "games/tla/src/graphics/tileset/loho_rooms/palette.gbapal.lz"
	.global Tileset_LohoRoomsTilesA
Tileset_LohoRoomsTilesA:
	.incbin "games/tla/src/graphics/tileset/loho_rooms/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_LohoRooms+0x14fb
	.global Tileset_LohoRoomsTilesB
Tileset_LohoRoomsTilesB:
	.incbin "games/tla/src/graphics/tileset/loho_rooms/tiles_b.4bpp.mtf"
	.byte 0xb5	@ PACKER: left in the buffer by Field_LohoRooms+0x1bc7
	.global Tileset_LohoRoomsTilesC
Tileset_LohoRoomsTilesC:
	.incbin "games/tla/src/graphics/tileset/loho_rooms/tiles_c.4bpp.mtf"
	.byte 0x95, 0x3b	@ PACKER: left in the buffer by Tileset_LohoRoomsTilesB+0x1b7a
	.global Tileset_LohoRoomsTilesD
Tileset_LohoRoomsTilesD:
	.incbin "games/tla/src/graphics/tileset/loho_rooms/tiles_d.4bpp.mtf"
	.byte 0x82, 0x94, 0x06	@ PACKER: left in the buffer by Tileset_LohoRoomsTilesC+0x449
