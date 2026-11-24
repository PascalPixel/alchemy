	.global Field_MadraRooms
Field_MadraRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraRooms, .LField_MadraRooms_CELL_TILES
	map_block Field_MadraRooms, .LField_MadraRooms_SHAPES
	map_block Field_MadraRooms, .LField_MadraRooms_GRID
	map_block Field_MadraRooms, .LField_MadraRooms_SHAPE_GRID
	map_block Field_MadraRooms, .LField_MadraRooms_ANIMATION
	map_block Field_MadraRooms, .LField_MadraRooms_BLEND
	map_block Field_MadraRooms, .LField_MadraRooms_SCENE
.LField_MadraRooms_CELL_TILES:
	.incbin "games/tla/src/map/madra_rooms/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MadraRooms_SHAPES:
	.incbin "games/tla/src/map/madra_rooms/shapes.table.lz"
	.balign 4, 0
.LField_MadraRooms_GRID:
	.incbin "games/tla/src/map/madra_rooms/map.grid.lz"
	.balign 4, 0
.LField_MadraRooms_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_rooms/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraRooms_ANIMATION:
	.incbin "games/tla/src/map/madra_rooms/animation.script.lz"
	.balign 4, 0
.LField_MadraRooms_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_MadraRooms_SCENE:
	.incbin "games/tla/src/map/madra_rooms/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MadraRoomsPalette
Tileset_MadraRoomsPalette:
	.incbin "games/tla/src/graphics/tileset/madra_rooms/palette.gbapal.lz"
	.global Tileset_MadraRoomsTilesA
Tileset_MadraRoomsTilesA:
	.incbin "games/tla/src/graphics/tileset/madra_rooms/tiles_a.4bpp.mtf"
	.byte 0x9e	@ PACKER: left in the buffer by Field_MadraRooms+0x1bdf
	.global Tileset_MadraRoomsTilesB
Tileset_MadraRoomsTilesB:
	.incbin "games/tla/src/graphics/tileset/madra_rooms/tiles_b.4bpp.mtf"
	.byte 0x62, 0xf8, 0x54	@ PACKER: left in the buffer by Field_MadraRooms+0x1e1d
	.global Tileset_MadraRoomsTilesC
Tileset_MadraRoomsTilesC:
	.incbin "games/tla/src/graphics/tileset/madra_rooms/tiles_c.4bpp.mtf"
	.byte 0x69, 0xd7, 0x36	@ PACKER: left in the buffer by Tileset_MadraRoomsTilesB+0x1ad9
	.global Tileset_MadraRoomsTilesD
Tileset_MadraRoomsTilesD:
	.incbin "games/tla/src/graphics/tileset/madra_rooms/tiles_d.4bpp.mtf"
	.byte 0x0c, 0xbc	@ PACKER: left in the buffer by Tileset_MadraRoomsTilesC+0x15e6
