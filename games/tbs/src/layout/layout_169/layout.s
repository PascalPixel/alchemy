	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout169
Field_Layout169:
	layout_header 0, 0, 128, 53, 2, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0x3f, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout169, .LField_Layout169_METATILES
	layout_offset Field_Layout169, .LField_Layout169_SHAPES
	layout_offset Field_Layout169, .LField_Layout169_GRID
	layout_offset Field_Layout169, .LField_Layout169_ANIMATION
	layout_offset Field_Layout169, .LField_Layout169_BLEND
	layout_offset Field_Layout169, .LField_Layout169_MARKERS
.LField_Layout169_METATILES:
	.incbin "games/tbs/src/layout/layout_169/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout169_SHAPES:
	.incbin "games/tbs/src/layout/layout_169/shapes.table.lz"
	.balign 4, 0
.LField_Layout169_GRID:
	.incbin "games/tbs/src/layout/layout_169/map.grid.lz"
	.balign 4, 0
.LField_Layout169_ANIMATION:
	.incbin "games/tbs/src/layout/layout_169/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout169_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout169_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set66Palette
Tileset_Set66Palette:
	.incbin "games/tbs/src/graphics/tileset/set_66/palette.gbapal.lz"
	.byte 0x43, 0x11, 0xf8	@ PACKER: left in the buffer by Field_Layout169+0xe5
	.global Tileset_Set66TilesA
Tileset_Set66TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_66/tiles_a.4bpp.mtf"
	.byte 0xfd, 0x17, 0x05	@ PACKER: left in the buffer by Field_Layout169+0x299
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
