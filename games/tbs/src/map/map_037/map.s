	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map037
Field_Map037:
	map_header 0, 0, 58, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map037, .LField_Map037_CELL_PICTURES
	map_offset Field_Map037, .LField_Map037_SHAPES
	map_offset Field_Map037, .LField_Map037_GRID
	map_offset Field_Map037, .LField_Map037_ANIMATION
	map_offset Field_Map037, .LField_Map037_BLEND
	map_offset Field_Map037, .LField_Map037_MARKERS
.LField_Map037_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_037/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map037_SHAPES:
	.incbin "games/tbs/src/map/map_037/shapes.table.lz"
	.balign 4, 0
.LField_Map037_GRID:
	.incbin "games/tbs/src/map/map_037/map.grid.lz"
	.balign 4, 0
.LField_Map037_ANIMATION:
	.incbin "games/tbs/src/map/map_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map037_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map037_MARKERS:
	treasure_spot 23, 8, 100
	treasure_spot 13, 23, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set29Palette
Tileset_Set29Palette:
	.incbin "games/tbs/src/graphics/tileset/set_29/palette.gbapal.lz"
	.byte 0xc0, 0x23	@ PACKER: left in the buffer by Field_Map037+0x182
	.global Tileset_Set29TilesA
Tileset_Set29TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_29/tiles_a.4bpp.mtf"
	.global Tileset_Set29TilesB
Tileset_Set29TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_29/tiles_b.4bpp.mtf"
	.global Tileset_Set29TilesC
Tileset_Set29TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_29/tiles_c.4bpp.mtf"
	.byte 0xba	@ PACKER: left in the buffer by Tileset_Set29TilesA+0x2847
	.global Tileset_Set29TilesD
Tileset_Set29TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_29/tiles_d.4bpp.mtf"
	.byte 0x68, 0x18, 0x2a	@ PACKER: left in the buffer by Tileset_Set29TilesC+0x509
