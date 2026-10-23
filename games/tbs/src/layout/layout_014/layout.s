	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout014
Field_Layout014:
	layout_header 0, 0, 112, 80, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 112, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout014, .LField_Layout014_METATILES
	layout_offset Field_Layout014, .LField_Layout014_SHAPES
	layout_offset Field_Layout014, .LField_Layout014_GRID
	layout_offset Field_Layout014, .LField_Layout014_ANIMATION
	layout_offset Field_Layout014, .LField_Layout014_BLEND
	layout_offset Field_Layout014, .LField_Layout014_MARKERS
.LField_Layout014_METATILES:
	.incbin "games/tbs/src/layout/layout_014/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout014_SHAPES:
	.incbin "games/tbs/src/layout/layout_014/shapes.table.lz"
	.balign 4, 0
.LField_Layout014_GRID:
	.incbin "games/tbs/src/layout/layout_014/grid.parts.lz"
	.balign 4, 0
.LField_Layout014_ANIMATION:
	.incbin "games/tbs/src/layout/layout_014/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout014_BLEND:
	.incbin "games/tbs/src/layout/layout_014/blend.script.lz"
	.balign 4, 0
.LField_Layout014_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set12Palette
Tileset_Set12Palette:
	.incbin "games/tbs/src/graphics/tileset/set_12/palette.gbapal.lz"
	.byte 0xb1	@ PACKER: left in the buffer by Field_Layout014+0x127
	.global Tileset_Set12TilesA
Tileset_Set12TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_12/tiles_a.4bpp.mtf"
	.byte 0x7a	@ PACKER: left in the buffer by Field_Layout014+0x1887
	.global Tileset_Set12TilesB
Tileset_Set12TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_12/tiles_b.4bpp.mtf"
	.global Tileset_Set12TilesC
Tileset_Set12TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_12/tiles_c.4bpp.mtf"
	.byte 0x8a, 0x18, 0x32	@ PACKER: left in the buffer by Tileset_Set12TilesB+0xf3d
	.global Tileset_Set12TilesD
Tileset_Set12TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_12/tiles_d.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Tileset_Set12TilesC+0x13f
