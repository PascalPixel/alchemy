	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout054
Field_Layout054:
	layout_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout054, .LField_Layout054_METATILES
	layout_offset Field_Layout054, .LField_Layout054_SHAPES
	layout_offset Field_Layout054, .LField_Layout054_GRID
	layout_offset Field_Layout054, .LField_Layout054_ANIMATION
	layout_offset Field_Layout054, .LField_Layout054_BLEND
	layout_offset Field_Layout054, .LField_Layout054_MARKERS
.LField_Layout054_METATILES:
	.incbin "games/tbs/src/layout/layout_054/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout054_SHAPES:
	.incbin "games/tbs/src/layout/layout_054/shapes.table.lz"
	.balign 4, 0
.LField_Layout054_GRID:
	.incbin "games/tbs/src/layout/layout_054/grid.parts.lz"
	.balign 4, 0
.LField_Layout054_ANIMATION:
	.incbin "games/tbs/src/layout/layout_054/animation.script.lz"
	.balign 4, 0
.LField_Layout054_BLEND:
	.incbin "games/tbs/src/layout/layout_054/blend.script.lz"
	.balign 4, 0
.LField_Layout054_MARKERS:
	.incbin "games/tbs/src/layout/layout_054/markers.table"
	.balign 4, 0
	.global Tileset_Set43Palette
Tileset_Set43Palette:
	.incbin "games/tbs/src/graphics/tileset/set_43/palette.gbapal.lz"
	.byte 0xc0, 0xcf, 0x02	@ PACKER: left in the buffer by Field_Layout054+0x16d
	.global Tileset_Set43TilesA
Tileset_Set43TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_43/tiles_a.4bpp.mtf"
	.byte 0x2b	@ PACKER: left in the buffer by Tileset_Set42TilesA+0x28bf
	.global Tileset_Set43TilesB
Tileset_Set43TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_43/tiles_b.4bpp.mtf"
	.byte 0xe5	@ PACKER: left in the buffer by Tileset_Set43TilesA+0x27c7
	.global Tileset_Set43TilesC
Tileset_Set43TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_43/tiles_c.4bpp.mtf"
	.byte 0x7a, 0x40, 0xe2	@ PACKER: left in the buffer by Tileset_Set43TilesB+0x12f9
	.global Tileset_Set43TilesD
Tileset_Set43TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_43/tiles_d.4bpp.mtf"
	.byte 0x0c	@ PACKER: left in the buffer by Tileset_Set43TilesB+0x1943
