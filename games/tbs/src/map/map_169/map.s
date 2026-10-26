	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map169
Field_Map169:
	map_header 0, 0, 128, 53, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x3f, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map169, .LField_Map169_METATILES
	map_offset Field_Map169, .LField_Map169_SHAPES
	map_offset Field_Map169, .LField_Map169_GRID
	map_offset Field_Map169, .LField_Map169_ANIMATION
	map_offset Field_Map169, .LField_Map169_BLEND
	map_offset Field_Map169, .LField_Map169_MARKERS
.LField_Map169_METATILES:
	.incbin "games/tbs/src/map/map_169/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map169_SHAPES:
	.incbin "games/tbs/src/map/map_169/shapes.table.lz"
	.balign 4, 0
.LField_Map169_GRID:
	.incbin "games/tbs/src/map/map_169/map.grid.lz"
	.balign 4, 0
.LField_Map169_ANIMATION:
	.incbin "games/tbs/src/map/map_169/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map169_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map169_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set66Palette
Tileset_Set66Palette:
	.incbin "games/tbs/src/graphics/tileset/set_66/palette.gbapal.lz"
	.byte 0x43, 0x11, 0xf8	@ PACKER: left in the buffer by Field_Map169+0xe5
	.global Tileset_Set66TilesA
Tileset_Set66TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_66/tiles_a.4bpp.mtf"
	.byte 0xfd, 0x17, 0x05	@ PACKER: left in the buffer by Field_Map169+0x299
	.global Tileset_Set66TilesB
Tileset_Set66TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_66/tiles_b.4bpp.mtf"
	.global Tileset_Set66TilesC
Tileset_Set66TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_66/tiles_c.4bpp.mtf"
	.byte 0xd2, 0x14	@ PACKER: left in the buffer by Tileset_Set66TilesB+0x1ef2
	.global Tileset_Set66TilesD
Tileset_Set66TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_66/tiles_d.4bpp.mtf"
	.byte 0x04, 0x5c	@ PACKER: left in the buffer by Tileset_Set66TilesC+0x5b2
