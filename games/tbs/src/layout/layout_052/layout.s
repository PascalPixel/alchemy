	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout052
Field_Layout052:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout052, .LField_Layout052_METATILES
	layout_offset Field_Layout052, .LField_Layout052_SHAPES
	layout_offset Field_Layout052, .LField_Layout052_GRID
	layout_offset Field_Layout052, .LField_Layout052_ANIMATION
	layout_offset Field_Layout052, .LField_Layout052_BLEND
	layout_offset Field_Layout052, .LField_Layout052_MARKERS
.LField_Layout052_METATILES:
	.incbin "games/tbs/src/layout/layout_052/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout052_SHAPES:
	.incbin "games/tbs/src/layout/layout_052/shapes.table.lz"
	.balign 4, 0
.LField_Layout052_GRID:
	.incbin "games/tbs/src/layout/layout_052/grid.parts.lz"
	.balign 4, 0
.LField_Layout052_ANIMATION:
	.incbin "games/tbs/src/layout/layout_052/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout052_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout052_MARKERS:
	.incbin "games/tbs/src/layout/layout_052/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set42Palette
Tileset_Set42Palette:
	.incbin "games/tbs/src/graphics/tileset/set_42/palette.gbapal.lz"
	.byte 0x1f	@ PACKER: left in the buffer by Field_Layout052+0x1c7
	.global Tileset_Set42TilesA
Tileset_Set42TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_42/tiles_a.4bpp.mtf"
	.byte 0x47, 0x90, 0x0e	@ PACKER: left in the buffer by Tileset_Set40TilesA+0x2901
	.global Tileset_Set42TilesB
Tileset_Set42TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_42/tiles_b.4bpp.mtf"
	.global Tileset_Set42TilesC
Tileset_Set42TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_42/tiles_c.4bpp.mtf"
	.byte 0xc3, 0x73, 0x89	@ PACKER: left in the buffer by Tileset_Set42TilesB+0x1c59
	.global Tileset_Set42TilesD
Tileset_Set42TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_42/tiles_d.4bpp.mtf"
	.byte 0xc4, 0x83	@ PACKER: left in the buffer by Tileset_Set42TilesC+0x156e
