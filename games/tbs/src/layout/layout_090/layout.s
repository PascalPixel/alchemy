	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map090
Field_Map090:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map090, .LField_Map090_METATILES
	map_block Field_Map090, .LField_Map090_SHAPES
	map_block Field_Map090, .LField_Map090_GRID
	map_block Field_Map090, .LField_Map090_ANIMATION
	map_block Field_Map090
	map_block Field_Map090, .LField_Map090_MARKERS
.LField_Map090_METATILES:
	.incbin "games/tbs/src/layout/layout_090/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map090_SHAPES:
	.incbin "games/tbs/src/layout/layout_090/shapes.table.lz"
	.balign 4, 0
.LField_Map090_GRID:
	.incbin "games/tbs/src/layout/layout_090/grid.parts.lz"
	.balign 4, 0
.LField_Map090_ANIMATION:
	.incbin "games/tbs/src/layout/layout_090/animation.script.lz"
	.balign 4, 0
.LField_Map090_MARKERS:
	.incbin "games/tbs/src/layout/layout_090/markers.table"
	.balign 4, 0
	.global Tileset_Set50Palette
Tileset_Set50Palette:
	.incbin "games/tbs/src/graphics/tileset/set_50/palette.gbapal.lz"
	.global Tileset_Set50TilesA
Tileset_Set50TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_50/tiles_a.4bpp.mtf"
	.byte 0x54	@ PACKER: left in the buffer by Field_Map089+0x28ef
	.global Tileset_Set50TilesB
Tileset_Set50TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_50/tiles_b.4bpp.mtf"
	.byte 0x9c, 0x52	@ PACKER: left in the buffer by Tileset_Set50TilesA+0x220a
	.global Tileset_Set50TilesC
Tileset_Set50TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_50/tiles_c.4bpp.mtf"
	.byte 0x96	@ PACKER: left in the buffer by Tileset_Set50TilesB+0x13f
	.global Tileset_Set50TilesD
Tileset_Set50TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_50/tiles_d.4bpp.mtf"
	.byte 0xc0, 0x84	@ PACKER: left in the buffer by Tileset_Set50TilesB+0x1a16
