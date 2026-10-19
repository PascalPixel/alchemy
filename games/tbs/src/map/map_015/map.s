	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map015
Field_Map015:
	map_header 0, 0, 112, 80, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 112, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map015, .LField_Map015_METATILES
	map_block Field_Map015, .LField_Map015_SHAPES
	map_block Field_Map015, .LField_Map015_GRID
	map_block Field_Map015, .LField_Map015_ANIMATION
	map_block Field_Map015, .LField_Map015_BLEND
	map_block Field_Map015, .LField_Map015_MARKERS
.LField_Map015_METATILES:
	.incbin "games/tbs/src/map/map_015/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map015_SHAPES:
	.incbin "games/tbs/src/map/map_015/shapes.table.lz"
	.balign 4, 0
.LField_Map015_GRID:
	.incbin "games/tbs/src/map/map_015/grid.parts.lz"
	.balign 4, 0
.LField_Map015_ANIMATION:
	.incbin "games/tbs/src/map/map_015/animation.script.lz"
	.balign 4, 0
.LField_Map015_BLEND:
	.incbin "games/tbs/src/map/map_014/blend.script.lz"
	.balign 4, 0
.LField_Map015_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set13Palette
Tileset_Set13Palette:
	.incbin "games/tbs/src/graphics/tileset/set_13/palette.gbapal.lz"
	.byte 0x27, 0x50	@ PACKER: left in the buffer by Field_Map015+0x10a
	.global Tileset_Set13TilesA
Tileset_Set13TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_13/tiles_a.4bpp.mtf"
	.byte 0xfe, 0x4c	@ PACKER: left in the buffer by Field_Map015+0x1fe2
	.global Tileset_Set13TilesB
Tileset_Set13TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_13/tiles_b.4bpp.mtf"
	.byte 0xa5	@ PACKER: left in the buffer by Tileset_Set13TilesA+0x1723
	.global Tileset_Set13TilesC
Tileset_Set13TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_13/tiles_c.4bpp.mtf"
	.global Tileset_Set13TilesD
Tileset_Set13TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_13/tiles_d.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Tileset_Set13TilesC+0x13f
