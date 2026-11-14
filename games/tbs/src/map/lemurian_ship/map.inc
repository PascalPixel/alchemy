	.global Field_LemurianShip
Field_LemurianShip:
	map_header 0, 0, 128, 53, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x3f, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LemurianShip, .LField_LemurianShip_CELL_PICTURES
	map_offset Field_LemurianShip, .LField_LemurianShip_SHAPES
	map_offset Field_LemurianShip, .LField_LemurianShip_GRID
	map_offset Field_LemurianShip, .LField_LemurianShip_ANIMATION
	map_offset Field_LemurianShip, .LField_LemurianShip_BLEND
	map_offset Field_LemurianShip, .LField_LemurianShip_MARKERS
.LField_LemurianShip_CELL_PICTURES:
	.incbin "games/tbs/src/map/lemurian_ship/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LemurianShip_SHAPES:
	.incbin "games/tbs/src/map/lemurian_ship/shapes.table.lz"
	.balign 4, 0
.LField_LemurianShip_GRID:
	.incbin "games/tbs/src/map/lemurian_ship/map.grid.lz"
	.balign 4, 0
.LField_LemurianShip_ANIMATION:
	.incbin "games/tbs/src/map/lemurian_ship/tileset_anims.script.lz"
	.balign 4, 0
.LField_LemurianShip_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_LemurianShip_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_LemurianShipPalette
Tileset_LemurianShipPalette:
	.incbin "games/tbs/src/graphics/tileset/lemurian_ship/palette.gbapal.lz"
	.byte 0x43, 0x11, 0xf8	@ PACKER: left in the buffer by Field_LemurianShip+0xe5
	.global Tileset_LemurianShipTilesA
Tileset_LemurianShipTilesA:
	.incbin "games/tbs/src/graphics/tileset/lemurian_ship/tiles_a.4bpp.mtf"
	.byte 0xfd, 0x17, 0x05	@ PACKER: left in the buffer by Field_LemurianShip+0x299
	.global Tileset_LemurianShipTilesB
Tileset_LemurianShipTilesB:
	.incbin "games/tbs/src/graphics/tileset/lemurian_ship/tiles_b.4bpp.mtf"
	.global Tileset_LemurianShipTilesC
Tileset_LemurianShipTilesC:
	.incbin "games/tbs/src/graphics/tileset/lemurian_ship/tiles_c.4bpp.mtf"
	.byte 0xd2, 0x14	@ PACKER: left in the buffer by Tileset_LemurianShipTilesB+0x1ef2
	.global Tileset_LemurianShipTilesD
Tileset_LemurianShipTilesD:
	.incbin "games/tbs/src/graphics/tileset/lemurian_ship/tiles_d.4bpp.mtf"
	.byte 0x04, 0x5c	@ PACKER: left in the buffer by Tileset_LemurianShipTilesC+0x5b2
	.balign 4, 0
