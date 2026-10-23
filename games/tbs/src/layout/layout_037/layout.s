	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout037
Field_Layout037:
	layout_header 0, 0, 58, 60, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout037, .LField_Layout037_METATILES
	layout_offset Field_Layout037, .LField_Layout037_SHAPES
	layout_offset Field_Layout037, .LField_Layout037_GRID
	layout_offset Field_Layout037, .LField_Layout037_ANIMATION
	layout_offset Field_Layout037, .LField_Layout037_BLEND
	layout_offset Field_Layout037, .LField_Layout037_MARKERS
.LField_Layout037_METATILES:
	.incbin "games/tbs/src/layout/layout_037/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout037_SHAPES:
	.incbin "games/tbs/src/layout/layout_037/shapes.table.lz"
	.balign 4, 0
.LField_Layout037_GRID:
	.incbin "games/tbs/src/layout/layout_037/grid.parts.lz"
	.balign 4, 0
.LField_Layout037_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/animation.script.lz"
	.balign 4, 0
.LField_Layout037_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout037_MARKERS:
	.incbin "games/tbs/src/layout/layout_037/markers.table"
	.balign 4, 0
	.global Tileset_Set29Palette
Tileset_Set29Palette:
	.incbin "games/tbs/src/graphics/tileset/set_29/palette.gbapal.lz"
	.byte 0xc0, 0x23	@ PACKER: left in the buffer by Field_Layout037+0x182
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
