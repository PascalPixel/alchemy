	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout068
Field_Layout068:
	layout_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout068, .LField_Layout068_METATILES
	layout_offset Field_Layout068, .LField_Layout068_SHAPES
	layout_offset Field_Layout068, .LField_Layout068_GRID
	layout_offset Field_Layout068, .LField_Layout068_ANIMATION
	layout_offset Field_Layout068
	layout_offset Field_Layout068, .LField_Layout068_MARKERS
.LField_Layout068_METATILES:
	.incbin "games/tbs/src/layout/layout_068/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout068_SHAPES:
	.incbin "games/tbs/src/layout/layout_068/shapes.table.lz"
	.balign 4, 0
.LField_Layout068_GRID:
	.incbin "games/tbs/src/layout/layout_068/grid.parts.lz"
	.balign 4, 0
.LField_Layout068_ANIMATION:
	.incbin "games/tbs/src/layout/layout_068/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout068_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set46Palette
Tileset_Set46Palette:
	.incbin "games/tbs/src/graphics/tileset/set_46/palette.gbapal.lz"
	.byte 0xcc	@ PACKER: left in the buffer by Field_Layout068+0x16b
	.global Tileset_Set46TilesA
Tileset_Set46TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_46/tiles_a.4bpp.mtf"
	.byte 0x63	@ PACKER: left in the buffer by Field_Layout068+0x2d9b
	.global Tileset_Set46TilesB
Tileset_Set46TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_46/tiles_b.4bpp.mtf"
	.byte 0x10, 0xa1	@ PACKER: left in the buffer by Tileset_Set46TilesA+0x218e
	.global Tileset_Set46TilesC
Tileset_Set46TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_46/tiles_c.4bpp.mtf"
	.byte 0xe5, 0xa2	@ PACKER: left in the buffer by Tileset_Set46TilesB+0x706
	.global Tileset_Set46TilesD
Tileset_Set46TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_46/tiles_d.4bpp.mtf"
	.byte 0x0a, 0x07	@ PACKER: left in the buffer by Tileset_Set46TilesB+0xc12
