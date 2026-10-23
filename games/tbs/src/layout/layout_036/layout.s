	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout036
Field_Layout036:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout036, .LField_Layout036_METATILES
	layout_offset Field_Layout036, .LField_Layout036_SHAPES
	layout_offset Field_Layout036, .LField_Layout036_GRID
	layout_offset Field_Layout036, .LField_Layout036_ANIMATION
	layout_offset Field_Layout036, .LField_Layout036_BLEND
	layout_offset Field_Layout036, .LField_Layout036_MARKERS
.LField_Layout036_METATILES:
	.incbin "games/tbs/src/layout/layout_036/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout036_SHAPES:
	.incbin "games/tbs/src/layout/layout_036/shapes.table.lz"
	.balign 4, 0
.LField_Layout036_GRID:
	.incbin "games/tbs/src/layout/layout_036/grid.parts.lz"
	.balign 4, 0
.LField_Layout036_ANIMATION:
	.incbin "games/tbs/src/layout/layout_036/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout036_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout036_MARKERS:
	.incbin "games/tbs/src/layout/layout_036/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set28Palette
Tileset_Set28Palette:
	.incbin "games/tbs/src/graphics/tileset/set_28/palette.gbapal.lz"
	.byte 0x40, 0x15, 0x08	@ PACKER: left in the buffer by Field_Layout036+0x1a1
	.global Tileset_Set28TilesA
Tileset_Set28TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_28/tiles_a.4bpp.mtf"
	.byte 0xfe	@ PACKER: left in the buffer by Tileset_Set27TilesB+0x209f
	.global Tileset_Set28TilesB
Tileset_Set28TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_28/tiles_b.4bpp.mtf"
	.global Tileset_Set28TilesC
Tileset_Set28TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_28/tiles_c.4bpp.mtf"
	.byte 0x10, 0x84, 0x55	@ PACKER: left in the buffer by Tileset_Set27TilesB+0x22d5
	.global Tileset_Set28TilesD
Tileset_Set28TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_28/tiles_d.4bpp.mtf"
