	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout025
Field_Layout025:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout025, .LField_Layout025_METATILES
	layout_offset Field_Layout025, .LField_Layout025_SHAPES
	layout_offset Field_Layout025, .LField_Layout025_GRID
	layout_offset Field_Layout025, .LField_Layout025_ANIMATION
	layout_offset Field_Layout025, .LField_Layout025_BLEND
	layout_offset Field_Layout025, .LField_Layout025_MARKERS
.LField_Layout025_METATILES:
	.incbin "games/tbs/src/layout/layout_025/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout025_SHAPES:
	.incbin "games/tbs/src/layout/layout_025/shapes.table.lz"
	.balign 4, 0
.LField_Layout025_GRID:
	.incbin "games/tbs/src/layout/layout_025/map.grid.lz"
	.balign 4, 0
.LField_Layout025_ANIMATION:
	.incbin "games/tbs/src/layout/layout_025/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout025_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout025_MARKERS:
	treasure_spot 16, 47, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set20Palette
Tileset_Set20Palette:
	.incbin "games/tbs/src/graphics/tileset/set_20/palette.gbapal.lz"
	.byte 0xe2, 0x17	@ PACKER: left in the buffer by Field_Layout025+0x19a
	.global Tileset_Set20TilesA
Tileset_Set20TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_20/tiles_a.4bpp.mtf"
	.balign 4, 0
	.global Tileset_Set20TilesB
Tileset_Set20TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_20/tiles_b.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set20TilesC
Tileset_Set20TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_20/tiles_c.4bpp.mtf"
	.global Tileset_Set20TilesD
Tileset_Set20TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_20/tiles_d.4bpp.mtf"
	.byte 0xc6, 0x39	@ PACKER: left in the buffer by Tileset_Set20TilesB+0x1a36
