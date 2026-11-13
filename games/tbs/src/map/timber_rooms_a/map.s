	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_TimberRoomsA
Field_TimberRoomsA:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_TimberRoomsA, .LField_TimberRoomsA_CELL_PICTURES
	map_offset Field_TimberRoomsA, .LField_TimberRoomsA_SHAPES
	map_offset Field_TimberRoomsA, .LField_TimberRoomsA_GRID
	map_offset Field_TimberRoomsA, .LField_TimberRoomsA_ANIMATION
	map_offset Field_TimberRoomsA, .LField_TimberRoomsA_BLEND
	map_offset Field_TimberRoomsA, .LField_TimberRoomsA_MARKERS
.LField_TimberRoomsA_CELL_PICTURES:
	.incbin "games/tbs/src/map/timber_rooms_a/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TimberRoomsA_SHAPES:
	.incbin "games/tbs/src/map/timber_rooms_a/shapes.table.lz"
	.balign 4, 0
.LField_TimberRoomsA_GRID:
	.incbin "games/tbs/src/map/timber_rooms_a/map.grid.lz"
	.balign 4, 0
.LField_TimberRoomsA_ANIMATION:
	.incbin "games/tbs/src/map/timber_rooms_a/tileset_anims.script.lz"
	.balign 4, 0
.LField_TimberRoomsA_BLEND:
	.incbin "games/tbs/src/map/vault_log_rooms/blend.script.lz"
	.balign 4, 0
.LField_TimberRoomsA_MARKERS:
	treasure_spot 9, 3, 100
	treasure_spot 10, 3, 100
	treasure_spot 20, 3, 101
	treasure_spot 21, 3, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_TimberRoomsPalette
Tileset_TimberRoomsPalette:
	.incbin "games/tbs/src/graphics/tileset/timber_rooms/palette.gbapal.lz"
	.global Tileset_TimberRoomsTilesA
Tileset_TimberRoomsTilesA:
	.incbin "games/tbs/src/graphics/tileset/timber_rooms/tiles_a.4bpp.mtf"
	.byte 0x94	@ PACKER: left in the buffer by Field_TimberRoomsA+0x1e7f
	.balign 4, 0
	.global Tileset_TimberRoomsTilesB
Tileset_TimberRoomsTilesB:
	.incbin "games/tbs/src/graphics/tileset/timber_rooms/tiles_b.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_TimberRoomsTilesC
Tileset_TimberRoomsTilesC:
	.incbin "games/tbs/src/graphics/tileset/timber_rooms/tiles_c.4bpp.mtf"
	.global Tileset_TimberRoomsTilesD
Tileset_TimberRoomsTilesD:
	.incbin "games/tbs/src/graphics/tileset/timber_rooms/tiles_d.4bpp.mtf"
	.byte 0x69, 0x4e, 0x50	@ PACKER: left in the buffer by Tileset_TimberRoomsTilesC+0x160d
