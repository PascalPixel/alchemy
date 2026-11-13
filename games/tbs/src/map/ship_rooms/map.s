	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_ShipRooms
Field_ShipRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ShipRooms, .LField_ShipRooms_CELL_PICTURES
	map_offset Field_ShipRooms, .LField_ShipRooms_SHAPES
	map_offset Field_ShipRooms, .LField_ShipRooms_GRID
	map_offset Field_ShipRooms, .LField_ShipRooms_ANIMATION
	map_offset Field_ShipRooms, .LField_ShipRooms_BLEND
	map_offset Field_ShipRooms, .LField_ShipRooms_MARKERS
.LField_ShipRooms_CELL_PICTURES:
	.incbin "games/tbs/src/map/ship_rooms/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ShipRooms_SHAPES:
	.incbin "games/tbs/src/map/ship_rooms/shapes.table.lz"
	.balign 4, 0
.LField_ShipRooms_GRID:
	.incbin "games/tbs/src/map/ship_rooms/map.grid.lz"
	.balign 4, 0
.LField_ShipRooms_ANIMATION:
	.incbin "games/tbs/src/map/ship_rooms/tileset_anims.script.lz"
	.balign 4, 0
.LField_ShipRooms_BLEND:
	.incbin "games/tbs/src/map/ship_rooms/blend.script.lz"
	.balign 4, 0
.LField_ShipRooms_MARKERS:
	treasure_spot 10, 19, 100
	treasure_spot 9, 35, 101
	treasure_spot 26, 40, 102
	treasure_spots_end
	.global Tileset_ShipRoomsPalette
Tileset_ShipRoomsPalette:
	.incbin "games/tbs/src/graphics/tileset/ship_rooms/palette.gbapal.lz"
	.global Tileset_ShipRoomsTilesA
Tileset_ShipRoomsTilesA:
	.incbin "games/tbs/src/graphics/tileset/ship_rooms/tiles_a.4bpp.mtf"
	.byte 0xc0, 0xbe, 0xc0	@ PACKER: left in the buffer by Tileset_Set56TilesC+0x13f1
	.global Tileset_ShipRoomsTilesB
Tileset_ShipRoomsTilesB:
	.incbin "games/tbs/src/graphics/tileset/ship_rooms/tiles_b.4bpp.mtf"
	.byte 0x21, 0xcf, 0x49	@ PACKER: left in the buffer by Tileset_Set56TilesC+0x1795
	.global Tileset_ShipRoomsTilesC
Tileset_ShipRoomsTilesC:
	.incbin "games/tbs/src/graphics/tileset/ship_rooms/tiles_c.4bpp.mtf"
	.byte 0xd6, 0x83, 0x39	@ PACKER: left in the buffer by Tileset_ShipRoomsTilesB+0x1271
	.global Tileset_ShipRoomsTilesD
Tileset_ShipRoomsTilesD:
	.incbin "games/tbs/src/graphics/tileset/ship_rooms/tiles_d.4bpp.mtf"
