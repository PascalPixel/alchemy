	.global Field_YallamRooms
Field_YallamRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_YallamRooms, .LField_YallamRooms_CELL_TILES
	map_block Field_YallamRooms, .LField_YallamRooms_SHAPES
	map_block Field_YallamRooms, .LField_YallamRooms_GRID
	map_block Field_YallamRooms, .LField_YallamRooms_SHAPE_GRID
	map_block Field_YallamRooms, .LField_YallamRooms_ANIMATION
	map_block Field_YallamRooms, .LField_YallamRooms_BLEND
	map_block Field_YallamRooms, .LField_YallamRooms_SCENE
.LField_YallamRooms_CELL_TILES:
	.incbin "games/tla/src/map/yallam_rooms/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_YallamRooms_SHAPES:
	.incbin "games/tla/src/map/yallam_rooms/shapes.table.lz"
	.balign 4, 0
.LField_YallamRooms_GRID:
	.incbin "games/tla/src/map/yallam_rooms/map.grid.lz"
	.balign 4, 0
.LField_YallamRooms_SHAPE_GRID:
	.incbin "games/tla/src/map/yallam_rooms/shape_grid.plane.lz"
	.balign 4, 0
.LField_YallamRooms_ANIMATION:
	.incbin "games/tla/src/map/yallam_rooms/animation.script.lz"
	.balign 4, 0
.LField_YallamRooms_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_YallamRooms_SCENE:
	.incbin "games/tla/src/map/yallam_rooms/scene.scene.lz"
	.balign 4, 0
	.global Tileset_YallamRoomsPalette
Tileset_YallamRoomsPalette:
	.incbin "games/tla/src/graphics/tileset/yallam_rooms/palette.gbapal.lz"
	.byte 0x04	@ PACKER: left in the buffer by Field_YallamRooms+0x1db
	.global Tileset_YallamRoomsTilesA
Tileset_YallamRoomsTilesA:
	.incbin "games/tla/src/graphics/tileset/yallam_rooms/tiles_a.4bpp.mtf"
	.global Tileset_YallamRoomsTilesB
Tileset_YallamRoomsTilesB:
	.incbin "games/tla/src/graphics/tileset/yallam_rooms/tiles_b.4bpp.mtf"
	.byte 0x0c	@ PACKER: left in the buffer by Field_YallamRooms+0x1aef
	.global Tileset_YallamRoomsTilesC
Tileset_YallamRoomsTilesC:
	.incbin "games/tla/src/graphics/tileset/yallam_rooms/tiles_c.4bpp.mtf"
	.byte 0xbb, 0x1f	@ PACKER: left in the buffer by Tileset_YallamRoomsTilesB+0x1482
	.global Tileset_YallamRoomsTilesD
Tileset_YallamRoomsTilesD:
	.incbin "games/tla/src/graphics/tileset/yallam_rooms/tiles_d.4bpp.mtf"
	.byte 0x7b, 0x9c, 0xde	@ PACKER: left in the buffer by Tileset_YallamRoomsTilesC+0x104d
