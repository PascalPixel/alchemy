	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map014
Field_Map014:
	map_header 0, 0, 112, 80, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 112, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map014, .LField_Map014_METATILES
	map_block Field_Map014, .LField_Map014_SHAPES
	map_block Field_Map014, .LField_Map014_GRID
	map_block Field_Map014, .LField_Map014_ANIMATION
	map_block Field_Map014, .LField_Map014_BLEND
	map_block Field_Map014, .LField_Map014_MARKERS
.LField_Map014_METATILES:
	.incbin "map/map_014/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map014_SHAPES:
	.incbin "map/map_014/shapes.table.lz"
	.balign 4, 0
.LField_Map014_GRID:
	.incbin "map/map_014/grid.parts.lz"
	.balign 4, 0
.LField_Map014_ANIMATION:
	.incbin "map/map_014/animation.script.lz"
	.balign 4, 0
.LField_Map014_BLEND:
	.incbin "map/map_014/blend.script.lz"
	.balign 4, 0
.LField_Map014_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set12Palette
Tileset_Set12Palette:
	.incbin "graphics/tileset/set_12/palette.gbapal.lz"
	.byte 0xb1	@ PACKER: left in the buffer by Field_Map014+0x127
	.global Tileset_Set12TilesA
Tileset_Set12TilesA:
	.incbin "graphics/tileset/set_12/tiles_a.4bpp.mtf"
	.byte 0x7a	@ PACKER: left in the buffer by Field_Map014+0x1887
	.global Tileset_Set12TilesB
Tileset_Set12TilesB:
	.incbin "graphics/tileset/set_12/tiles_b.4bpp.mtf"
	.global Tileset_Set12TilesC
Tileset_Set12TilesC:
	.incbin "graphics/tileset/set_12/tiles_c.4bpp.mtf"
	.byte 0x8a, 0x18, 0x32	@ PACKER: left in the buffer by Tileset_Set12TilesB+0xf3d
	.global Tileset_Set12TilesD
Tileset_Set12TilesD:
	.incbin "graphics/tileset/set_12/tiles_d.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Tileset_Set12TilesC+0x13f
