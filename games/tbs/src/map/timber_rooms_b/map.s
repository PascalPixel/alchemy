	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_TimberRoomsB
Field_TimberRoomsB:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_TimberRoomsB, .LField_TimberRoomsB_CELL_PICTURES
	map_offset Field_TimberRoomsB, .LField_TimberRoomsB_SHAPES
	map_offset Field_TimberRoomsB, .LField_TimberRoomsB_GRID
	map_offset Field_TimberRoomsB, .LField_TimberRoomsB_ANIMATION
	map_offset Field_TimberRoomsB, .LField_TimberRoomsB_BLEND
	map_offset Field_TimberRoomsB, .LField_TimberRoomsB_MARKERS
.LField_TimberRoomsB_CELL_PICTURES:
	.incbin "games/tbs/src/map/timber_rooms_b/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TimberRoomsB_SHAPES:
	.incbin "games/tbs/src/map/timber_rooms_b/shapes.table.lz"
	.balign 4, 0
.LField_TimberRoomsB_GRID:
	.incbin "games/tbs/src/map/timber_rooms_b/map.grid.lz"
	.balign 4, 0
.LField_TimberRoomsB_ANIMATION:
	.incbin "games/tbs/src/map/timber_rooms_a/tileset_anims.script.lz"
	.balign 4, 0
.LField_TimberRoomsB_BLEND:
	.incbin "games/tbs/src/map/vault_log_rooms/blend.script.lz"
	.balign 4, 0
.LField_TimberRoomsB_MARKERS:
	treasure_spot 3, 3, 200
	treasure_spot 4, 3, 200
	treasure_spot 25, 3, 201
	treasure_spot 26, 3, 201
	treasure_spot 27, 6, 100
	treasure_spot 18, 16, 202
	treasure_spot 19, 16, 202
	treasure_spot 10, 19, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette48
Tileset_Palette48:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_48.gbapal.lz"
	.byte 0x38, 0x16	@ PACKER: left in the buffer by Field_TimberRoomsB+0x1e6
