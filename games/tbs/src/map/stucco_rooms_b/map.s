	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_StuccoRoomsB
Field_StuccoRoomsB:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_StuccoRoomsB, .LField_StuccoRoomsB_CELL_PICTURES
	map_offset Field_StuccoRoomsB, .LField_StuccoRoomsB_SHAPES
	map_offset Field_StuccoRoomsB, .LField_StuccoRoomsB_GRID
	map_offset Field_StuccoRoomsB, .LField_StuccoRoomsB_ANIMATION
	map_offset Field_StuccoRoomsB, .LField_StuccoRoomsB_BLEND
	map_offset Field_StuccoRoomsB, .LField_StuccoRoomsB_MARKERS
.LField_StuccoRoomsB_CELL_PICTURES:
	.incbin "games/tbs/src/map/stucco_rooms_b/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_StuccoRoomsB_SHAPES:
	.incbin "games/tbs/src/map/stucco_rooms_b/shapes.table.lz"
	.balign 4, 0
.LField_StuccoRoomsB_GRID:
	.incbin "games/tbs/src/map/stucco_rooms_b/map.grid.lz"
	.balign 4, 0
.LField_StuccoRoomsB_ANIMATION:
	.incbin "games/tbs/src/map/stucco_rooms_a/tileset_anims.script.lz"
	.balign 4, 0
.LField_StuccoRoomsB_BLEND:
	.incbin "games/tbs/src/map/vault_log_rooms/blend.script.lz"
	.balign 4, 0
.LField_StuccoRoomsB_MARKERS:
	treasure_spot 20, 5, 200
	treasure_spot 21, 5, 200
	treasure_spot 19, 7, 100
	treasure_spot 6, 20, 101
	treasure_spot 36, 20, 101
	treasure_spot 37, 20, 201
	treasure_spot 38, 20, 201
	treasure_spot 14, 35, 202
	treasure_spot 15, 35, 202
	treasure_spot 35, 52, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteStuccoRooms
Tileset_PaletteStuccoRooms:
	.incbin "games/tbs/src/graphics/tileset/palettes/stucco_rooms.gbapal.lz"
	.byte 0x9c, 0x00	@ PACKER: left in the buffer by Field_StuccoRoomsB+0x1de
