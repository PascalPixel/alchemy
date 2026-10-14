	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map028
Field_Map028:
	map_header 0, 0, 42, 54, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map028, .LField_Map028_METATILES
	map_block Field_Map028, .LField_Map028_SHAPES
	map_block Field_Map028, .LField_Map028_GRID
	map_block Field_Map028, .LField_Map028_ANIMATION
	map_block Field_Map028, .LField_Map028_BLEND
	map_block Field_Map028, .LField_Map028_MARKERS
.LField_Map028_METATILES:
	.incbin "games/tbs/src/map/map_028/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map028_SHAPES:
	.incbin "games/tbs/src/map/map_028/shapes.table.lz"
	.balign 4, 0
.LField_Map028_GRID:
	.incbin "games/tbs/src/map/map_028/grid.parts.lz"
	.balign 4, 0
.LField_Map028_ANIMATION:
	.incbin "games/tbs/src/map/map_028/animation.script.lz"
	.balign 4, 0
.LField_Map028_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map028_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set21Palette
Tileset_Set21Palette:
	.incbin "games/tbs/src/graphics/tileset/set_21/palette.gbapal.lz"
	.byte 0x98, 0x49	@ PACKER: left in the buffer by Field_Map028+0x19a
	.global Tileset_Set21TilesA
Tileset_Set21TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_21/tiles_a.4bpp.mtf"
	.byte 0xdc, 0x06, 0xaa	@ PACKER: left in the buffer by Field_Map027+0x2bad
	.global Tileset_Set21TilesB
Tileset_Set21TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_21/tiles_b.4bpp.mtf"
	.byte 0xf1, 0xe8, 0x05	@ PACKER: left in the buffer by Tileset_Set21TilesA+0x20d1
	.global Tileset_Set21TilesC
Tileset_Set21TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_21/tiles_c.4bpp.mtf"
	.byte 0x83, 0x38	@ PACKER: left in the buffer by Tileset_Set21TilesB+0x2056
	.global Tileset_Set21TilesD
Tileset_Set21TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_21/tiles_d.4bpp.mtf"
	.byte 0x7c, 0x2f	@ PACKER: left in the buffer by Tileset_Set21TilesC+0x1cea
