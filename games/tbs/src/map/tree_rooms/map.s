	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_TreeRooms
Field_TreeRooms:
	map_header 0, 0, 128, 68, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_TreeRooms, .LField_TreeRooms_CELL_PICTURES
	map_offset Field_TreeRooms, .LField_TreeRooms_SHAPES
	map_offset Field_TreeRooms, .LField_TreeRooms_GRID
	map_offset Field_TreeRooms, .LField_TreeRooms_ANIMATION
	map_offset Field_TreeRooms, .LField_TreeRooms_BLEND
	map_offset Field_TreeRooms, .LField_TreeRooms_MARKERS
.LField_TreeRooms_CELL_PICTURES:
	.incbin "games/tbs/src/map/tree_rooms/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TreeRooms_SHAPES:
	.incbin "games/tbs/src/map/tree_rooms/shapes.table.lz"
	.balign 4, 0
.LField_TreeRooms_GRID:
	.incbin "games/tbs/src/map/tree_rooms/map.grid.lz"
	.balign 4, 0
.LField_TreeRooms_ANIMATION:
	.incbin "games/tbs/src/map/kolima/tileset_anims.script.lz"
	.balign 4, 0
.LField_TreeRooms_BLEND:
	.incbin "games/tbs/src/map/tree_rooms/blend.script.lz"
	.balign 4, 0
.LField_TreeRooms_MARKERS:
	treasure_spot 9, 21, 102
	treasure_spot 10, 24, 100
	treasure_spot 54, 25, 101
	treasure_spots_end
	.global Tileset_TreeRoomsPalette
Tileset_TreeRoomsPalette:
	.incbin "games/tbs/src/graphics/tileset/tree_rooms/palette.gbapal.lz"
	.byte 0x34	@ PACKER: left in the buffer by Field_TreeRooms+0x18f
	.global Tileset_TreeRoomsTilesA
Tileset_TreeRoomsTilesA:
	.incbin "games/tbs/src/graphics/tileset/tree_rooms/tiles_a.4bpp.mtf"
	.balign 4, 0
	.global Tileset_TreeRoomsTilesB
Tileset_TreeRoomsTilesB:
	.incbin "games/tbs/src/graphics/tileset/tree_rooms/tiles_b.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_TreeRoomsTilesC
Tileset_TreeRoomsTilesC:
	.incbin "games/tbs/src/graphics/tileset/tree_rooms/tiles_c.4bpp.mtf"
	.byte 0xe8, 0x95, 0x5a	@ PACKER: left in the buffer by Tileset_TreeRoomsTilesB+0xc81
	.global Tileset_TreeRoomsTilesD
Tileset_TreeRoomsTilesD:
	.incbin "games/tbs/src/graphics/tileset/tree_rooms/tiles_d.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_TreeRoomsTilesC+0x13f
