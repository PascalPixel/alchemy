	.global Field_NaribweRooms
Field_NaribweRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_NaribweRooms, .LField_NaribweRooms_CELL_TILES
	map_block Field_NaribweRooms, .LField_NaribweRooms_SHAPES
	map_block Field_NaribweRooms, .LField_NaribweRooms_GRID
	map_block Field_NaribweRooms, .LField_NaribweRooms_SHAPE_GRID
	map_block Field_NaribweRooms, .LField_NaribweRooms_ANIMATION
	map_block Field_NaribweRooms, .LField_NaribweRooms_BLEND
	map_block Field_NaribweRooms, .LField_NaribweRooms_SCENE
.LField_NaribweRooms_CELL_TILES:
	.incbin "games/tla/src/map/naribwe_rooms/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_NaribweRooms_SHAPES:
	.incbin "games/tla/src/map/naribwe_rooms/shapes.table.lz"
	.balign 4, 0
.LField_NaribweRooms_GRID:
	.incbin "games/tla/src/map/naribwe_rooms/map.grid.lz"
	.balign 4, 0
.LField_NaribweRooms_SHAPE_GRID:
	.incbin "games/tla/src/map/naribwe_rooms/shape_grid.plane.lz"
	.balign 4, 0
.LField_NaribweRooms_ANIMATION:
	.incbin "games/tla/src/map/naribwe_rooms/animation.script.lz"
	.balign 4, 0
.LField_NaribweRooms_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_NaribweRooms_SCENE:
	.incbin "games/tla/src/map/naribwe_rooms/scene.scene.lz"
	.balign 4, 0
	.global Tileset_NaribweRoomsPalette
Tileset_NaribweRoomsPalette:
	.incbin "games/tla/src/graphics/tileset/naribwe_rooms/palette.gbapal.lz"
	.global Tileset_NaribweRoomsTilesA
Tileset_NaribweRoomsTilesA:
	.incbin "games/tla/src/graphics/tileset/naribwe_rooms/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_NaribweRoomsTilesB
Tileset_NaribweRoomsTilesB:
	.incbin "games/tla/src/graphics/tileset/naribwe_rooms/tiles_b.4bpp.mtf"
	.byte 0x71, 0x76, 0xf0	@ PACKER: left in the buffer by Tileset_NaribweRoomsTilesA+0x1969
	.global Tileset_NaribweRoomsTilesC
Tileset_NaribweRoomsTilesC:
	.incbin "games/tla/src/graphics/tileset/naribwe_rooms/tiles_c.4bpp.mtf"
	.global Tileset_NaribweRoomsTilesD
Tileset_NaribweRoomsTilesD:
	.incbin "games/tla/src/graphics/tileset/naribwe_rooms/tiles_d.4bpp.mtf"
	.byte 0x3e, 0x8d	@ PACKER: left in the buffer by Tileset_NaribweRoomsTilesC+0xf1e
