	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map170
Field_Map170:
	map_header 0, 0, 30, 22, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 132, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map170, .LField_Map170_METATILES
	map_block Field_Map170, .LField_Map170_SHAPES
	map_block Field_Map170, .LField_Map170_GRID
	map_block Field_Map170, .LField_Map170_ANIMATION
	map_block Field_Map170, .LField_Map170_BLEND
	map_block Field_Map170, .LField_Map170_MARKERS
.LField_Map170_METATILES:
	.incbin "games/tbs/src/layout/layout_170/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map170_SHAPES:
	.incbin "games/tbs/src/layout/layout_170/shapes.table.lz"
	.balign 4, 0
.LField_Map170_GRID:
	.incbin "games/tbs/src/layout/layout_170/grid.parts.lz"
	.balign 4, 0
.LField_Map170_ANIMATION:
	.incbin "games/tbs/src/layout/layout_170/animation.script.lz"
	.balign 4, 0
.LField_Map170_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Map170_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set67Palette
Tileset_Set67Palette:
	.incbin "games/tbs/src/graphics/tileset/set_67/palette.gbapal.lz"
	.global Tileset_Set67TilesA
Tileset_Set67TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_67/tiles_a.4bpp.mtf"
	.byte 0x4e, 0x14	@ PACKER: left in the buffer by Tileset_Set66TilesC+0x1856
	.global Tileset_Set67TilesB
Tileset_Set67TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_67/tiles_b.4bpp.mtf"
	.byte 0x58, 0xfe, 0x8a	@ PACKER: left in the buffer by Tileset_Set67TilesA+0xc6d
	.global Tileset_Set67TilesC
Tileset_Set67TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_67/tiles_c.4bpp.mtf"
	.byte 0x0f, 0x7c, 0xfe	@ PACKER: left in the buffer by Tileset_Set67TilesB+0x2a5
	.global Tileset_Set67TilesD
Tileset_Set67TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_67/tiles_d.4bpp.mtf"
	.byte 0xc4, 0x02	@ PACKER: left in the buffer by Tileset_Set67TilesA+0xe82
