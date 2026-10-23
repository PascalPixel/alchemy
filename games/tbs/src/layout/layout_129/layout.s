	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map129
Field_Map129:
	map_header 0, 0, 206, 52, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 52, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 104, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map129, .LField_Map129_METATILES
	map_block Field_Map129, .LField_Map129_SHAPES
	map_block Field_Map129, .LField_Map129_GRID
	map_block Field_Map129, .LField_Map129_ANIMATION
	map_block Field_Map129, .LField_Map129_BLEND
	map_block Field_Map129, .LField_Map129_MARKERS
.LField_Map129_METATILES:
	.incbin "games/tbs/src/layout/layout_129/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map129_SHAPES:
	.incbin "games/tbs/src/layout/layout_129/shapes.table.lz"
	.balign 4, 0
.LField_Map129_GRID:
	.incbin "games/tbs/src/layout/layout_129/grid.parts.lz"
	.balign 4, 0
.LField_Map129_ANIMATION:
	.incbin "games/tbs/src/layout/layout_129/animation.script.lz"
	.balign 4, 0
.LField_Map129_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Map129_MARKERS:
	.incbin "games/tbs/src/layout/layout_129/markers.table"
	.global Tileset_Set62Palette
Tileset_Set62Palette:
	.incbin "games/tbs/src/graphics/tileset/set_62/palette.gbapal.lz"
	.byte 0x22, 0xb6	@ PACKER: left in the buffer by Field_Map129+0x13a
	.global Tileset_Set62TilesA
Tileset_Set62TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_62/tiles_a.4bpp.mtf"
	.byte 0x81, 0xdf	@ PACKER: left in the buffer by Tileset_Set61TilesA+0x2732
	.global Tileset_Set62TilesB
Tileset_Set62TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_62/tiles_b.4bpp.mtf"
	.byte 0xba	@ PACKER: left in the buffer by Tileset_Set62TilesA+0x1c43
	.global Tileset_Set62TilesC
Tileset_Set62TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_62/tiles_c.4bpp.mtf"
	.byte 0x3c, 0xe0, 0x13	@ PACKER: left in the buffer by Tileset_Set62TilesB+0x1445
	.global Tileset_Set62TilesD
Tileset_Set62TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_62/tiles_d.4bpp.mtf"
	.byte 0xa3	@ PACKER: left in the buffer by Tileset_Set62TilesB+0x1b7b
