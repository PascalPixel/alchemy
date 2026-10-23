	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout103
Field_Layout103:
	layout_header 0, 0, 45, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, -25, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout103, .LField_Layout103_METATILES
	layout_offset Field_Layout103, .LField_Layout103_SHAPES
	layout_offset Field_Layout103, .LField_Layout103_GRID
	layout_offset Field_Layout103, .LField_Layout103_ANIMATION
	layout_offset Field_Layout103
	layout_offset Field_Layout103, .LField_Layout103_MARKERS
.LField_Layout103_METATILES:
	.incbin "games/tbs/src/layout/layout_103/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout103_SHAPES:
	.incbin "games/tbs/src/layout/layout_103/shapes.table.lz"
	.balign 4, 0
.LField_Layout103_GRID:
	.incbin "games/tbs/src/layout/layout_103/grid.parts.lz"
	.balign 4, 0
.LField_Layout103_ANIMATION:
	.incbin "games/tbs/src/layout/layout_103/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout103_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set56Palette
Tileset_Set56Palette:
	.incbin "games/tbs/src/graphics/tileset/set_56/palette.gbapal.lz"
	.byte 0xc5, 0x3e, 0x03	@ PACKER: left in the buffer by Field_Layout103+0x1d1
	.global Tileset_Set56TilesA
Tileset_Set56TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_56/tiles_a.4bpp.mtf"
	.byte 0xd1, 0x43	@ PACKER: left in the buffer by Tileset_Set54TilesB+0x208e
	.global Tileset_Set56TilesB
Tileset_Set56TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_56/tiles_b.4bpp.mtf"
	.byte 0x79, 0x3a	@ PACKER: left in the buffer by Tileset_Set54TilesB+0x20fe
	.global Tileset_Set56TilesC
Tileset_Set56TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_56/tiles_c.4bpp.mtf"
	.global Tileset_Set56TilesD
Tileset_Set56TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_56/tiles_d.4bpp.mtf"
	.byte 0x08, 0x7e	@ PACKER: left in the buffer by Tileset_Set56TilesC+0x99a
