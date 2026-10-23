	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout150
Field_Layout150:
	layout_header 0, 0, 48, 48, 2, 1, 3, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout150, .LField_Layout150_METATILES
	layout_offset Field_Layout150, .LField_Layout150_SHAPES
	layout_offset Field_Layout150, .LField_Layout150_GRID
	layout_offset Field_Layout150, .LField_Layout150_ANIMATION
	layout_offset Field_Layout150
	layout_offset Field_Layout150, .LField_Layout150_MARKERS
.LField_Layout150_METATILES:
	.incbin "games/tbs/src/layout/layout_150/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout150_SHAPES:
	.incbin "games/tbs/src/layout/layout_150/shapes.table.lz"
	.balign 4, 0
.LField_Layout150_GRID:
	.incbin "games/tbs/src/layout/layout_150/grid.parts.lz"
	.balign 4, 0
.LField_Layout150_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/animation.script.lz"
	.balign 4, 0
.LField_Layout150_MARKERS:
	.incbin "games/tbs/src/layout/layout_150/markers.table"
	.global Tileset_Set64Palette
Tileset_Set64Palette:
	.incbin "games/tbs/src/graphics/tileset/set_64/palette.gbapal.lz"
	.global Tileset_Set64TilesA
Tileset_Set64TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_a.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xd8	@ PACKER: left in the buffer by Field_Layout148+0x2617
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xd8	@ PACKER: left in the buffer by Field_Layout148+0x2617
	.else
	.byte 0x80	@ PACKER: left in the buffer by Field_Layout148+0x2617
	.endif
	.endif
	.global Tileset_Set64TilesB
Tileset_Set64TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_b.4bpp.mtf"
	.global Tileset_Set64TilesC
Tileset_Set64TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_c.4bpp.mtf"
	.byte 0x00, 0x14	@ PACKER: left in the buffer by Tileset_Set64TilesB+0x106a
	.global Tileset_Set64TilesD
Tileset_Set64TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_d.4bpp.mtf"
	.byte 0x81, 0x79	@ PACKER: left in the buffer by Tileset_Set64TilesB+0x1a26
