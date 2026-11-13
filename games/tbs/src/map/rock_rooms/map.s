	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_RockRooms
Field_RockRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_RockRooms, .LField_RockRooms_CELL_PICTURES
	map_offset Field_RockRooms, .LField_RockRooms_SHAPES
	map_offset Field_RockRooms, .LField_RockRooms_GRID
	map_offset Field_RockRooms, .LField_RockRooms_ANIMATION
	map_offset Field_RockRooms, .LField_RockRooms_BLEND
	map_offset Field_RockRooms, .LField_RockRooms_MARKERS
.LField_RockRooms_CELL_PICTURES:
	.incbin "games/tbs/src/map/rock_rooms/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_RockRooms_SHAPES:
	.incbin "games/tbs/src/map/rock_rooms/shapes.table.lz"
	.balign 4, 0
.LField_RockRooms_GRID:
	.incbin "games/tbs/src/map/rock_rooms/map.grid.lz"
	.balign 4, 0
.LField_RockRooms_ANIMATION:
	.incbin "games/tbs/src/map/ice_rooms/tileset_anims.script.lz"
	.balign 4, 0
.LField_RockRooms_BLEND:
	.incbin "games/tbs/src/map/vault_log_rooms/blend.script.lz"
	.balign 4, 0
.LField_RockRooms_MARKERS:
	treasure_spot 6, 4, 200
	treasure_spot 7, 4, 200
	treasure_spot 23, 10, 100
	treasure_spot 8, 29, 201
	treasure_spot 9, 29, 201
	treasure_spot 30, 53, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette09
Tileset_Palette09:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_09.gbapal.lz"
	.byte 0xcc	@ PACKER: left in the buffer by Field_RockRooms+0x1c3
