	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map018
Field_Map018:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map018, .LField_Map018_METATILES
	map_block Field_Map018, .LField_Map018_SHAPES
	map_block Field_Map018, .LField_Map018_GRID
	map_block Field_Map018, .LField_Map018_ANIMATION
	map_block Field_Map018, .LField_Map018_BLEND
	map_block Field_Map018, .LField_Map018_MARKERS
.LField_Map018_METATILES:
	.incbin "games/tbs/src/map/map_018/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map018_SHAPES:
	.incbin "games/tbs/src/map/map_018/shapes.table.lz"
	.balign 4, 0
.LField_Map018_GRID:
	.incbin "games/tbs/src/map/map_018/grid.parts.lz"
	.balign 4, 0
.LField_Map018_ANIMATION:
	.incbin "games/tbs/src/map/map_018/animation.script.lz"
	.balign 4, 0
.LField_Map018_BLEND:
	.incbin "games/tbs/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map018_MARKERS:
	.incbin "games/tbs/src/map/map_018/markers.table"
	.balign 4, 0
	.global Tileset_Set15Palette
Tileset_Set15Palette:
	.incbin "games/tbs/src/graphics/tileset/set_15/palette.gbapal.lz"
	.global Tileset_Set15TilesA
Tileset_Set15TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_15/tiles_a.4bpp.mtf"
	.byte 0x80, 0x36, 0x05	@ PACKER: left in the buffer by Field_Map018+0x1c29
	.global Tileset_Set15TilesB
Tileset_Set15TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_15/tiles_b.4bpp.mtf"
	.byte 0x3b	@ PACKER: left in the buffer by Field_Map018+0x2547
	.global Tileset_Set15TilesC
Tileset_Set15TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_15/tiles_c.4bpp.mtf"
	.byte 0x00, 0xa5	@ PACKER: left in the buffer by Tileset_Set15TilesB+0x12ca
	.global Tileset_Set15TilesD
Tileset_Set15TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_15/tiles_d.4bpp.mtf"
	.byte 0x6f	@ PACKER: left in the buffer by Tileset_Set15TilesC+0xcaf
