	.global Field_CottageRoomsE
Field_CottageRoomsE:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CottageRoomsE, .LField_CottageRoomsE_CELL_PICTURES
	map_offset Field_CottageRoomsE, .LField_CottageRoomsE_SHAPES
	map_offset Field_CottageRoomsE, .LField_CottageRoomsE_GRID
	map_offset Field_CottageRoomsE, .LField_CottageRoomsE_ANIMATION
	map_offset Field_CottageRoomsE, .LField_CottageRoomsE_BLEND
	map_offset Field_CottageRoomsE, .LField_CottageRoomsE_MARKERS
.LField_CottageRoomsE_CELL_PICTURES:
	.incbin "games/tbs/src/map/cottage_rooms_e/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_CottageRoomsE_SHAPES:
	.incbin "games/tbs/src/map/cottage_rooms_e/shapes.table.lz"
	.balign 4, 0
.LField_CottageRoomsE_GRID:
	.incbin "games/tbs/src/map/cottage_rooms_e/map.grid.lz"
	.balign 4, 0
.LField_CottageRoomsE_ANIMATION:
	.incbin "games/tbs/src/map/vault_log_rooms/tileset_anims.script.lz"
	.balign 4, 0
.LField_CottageRoomsE_BLEND:
	.incbin "games/tbs/src/map/vault_log_rooms/blend.script.lz"
	.balign 4, 0
.LField_CottageRoomsE_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_CottageRoomsEPalette
Tileset_CottageRoomsEPalette:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_e/palette.gbapal.lz"
	.byte 0x05	@ PACKER: left in the buffer by Field_CottageRoomsE+0x1bf
	.global Tileset_CottageRoomsETilesA
Tileset_CottageRoomsETilesA:
	.incbin "games/tbs/src/graphics/tileset/cottage_rooms_e/tiles_a.4bpp.mtf"
	.balign 4, 0
