	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout041
Field_Layout041:
	layout_header 0, 0, 64, 84, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout041, .LField_Layout041_METATILES
	layout_offset Field_Layout041, .LField_Layout041_SHAPES
	layout_offset Field_Layout041, .LField_Layout041_GRID
	layout_offset Field_Layout041, .LField_Layout041_ANIMATION
	layout_offset Field_Layout041
	layout_offset Field_Layout041, .LField_Layout041_MARKERS
.LField_Layout041_METATILES:
	.incbin "games/tbs/src/layout/layout_041/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout041_SHAPES:
	.incbin "games/tbs/src/layout/layout_041/shapes.table.lz"
	.balign 4, 0
.LField_Layout041_GRID:
	.incbin "games/tbs/src/layout/layout_041/grid.parts.lz"
	.balign 4, 0
.LField_Layout041_ANIMATION:
	.incbin "games/tbs/src/layout/layout_041/animation.script.lz"
	.balign 4, 0
.LField_Layout041_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set32Palette
Tileset_Set32Palette:
	.incbin "games/tbs/src/graphics/tileset/set_32/palette.gbapal.lz"
	.byte 0xb1, 0xc1	@ PACKER: left in the buffer by Field_Layout041+0x176
	.global Tileset_Set32TilesA
Tileset_Set32TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_32/tiles_a.4bpp.mtf"
	.global Tileset_Set32TilesB
Tileset_Set32TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_32/tiles_b.4bpp.mtf"
	.byte 0x4d, 0x0c, 0x83	@ PACKER: left in the buffer by Tileset_Set32TilesA+0x1d6d
	.global Tileset_Set32TilesC
Tileset_Set32TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_32/tiles_c.4bpp.mtf"
	.byte 0xb8, 0x92, 0x64	@ PACKER: left in the buffer by Tileset_Set32TilesB+0xa65
	.global Tileset_Set32TilesD
Tileset_Set32TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_32/tiles_d.4bpp.mtf"
