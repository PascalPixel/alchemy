	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout031
Field_Layout031:
	layout_header 0, 0, 56, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout031, .LField_Layout031_METATILES
	layout_offset Field_Layout031, .LField_Layout031_SHAPES
	layout_offset Field_Layout031, .LField_Layout031_GRID
	layout_offset Field_Layout031, .LField_Layout031_ANIMATION
	layout_offset Field_Layout031, .LField_Layout031_BLEND
	layout_offset Field_Layout031, .LField_Layout031_MARKERS
.LField_Layout031_METATILES:
	.incbin "games/tbs/src/layout/layout_031/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout031_SHAPES:
	.incbin "games/tbs/src/layout/layout_031/shapes.table.lz"
	.balign 4, 0
.LField_Layout031_GRID:
	.incbin "games/tbs/src/layout/layout_031/map.grid.lz"
	.balign 4, 0
.LField_Layout031_ANIMATION:
	.incbin "games/tbs/src/layout/layout_031/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout031_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout031_MARKERS:
	.incbin "games/tbs/src/layout/layout_031/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set23Palette
Tileset_Set23Palette:
	.incbin "games/tbs/src/graphics/tileset/set_23/palette.gbapal.lz"
	.balign 4, 0
	.global Tileset_Set23TilesA
Tileset_Set23TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_23/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set23TilesB
Tileset_Set23TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_23/tiles_b.4bpp.mtf"
	.byte 0xa6, 0x09, 0x9b	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x28c5
	.global Tileset_Set23TilesC
Tileset_Set23TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_23/tiles_c.4bpp.mtf"
	.global Tileset_Set23TilesD
Tileset_Set23TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_23/tiles_d.4bpp.mtf"
