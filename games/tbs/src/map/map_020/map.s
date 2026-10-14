	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map020
Field_Map020:
	map_header 0, 0, 62, 58, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 62, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 124, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map020, .LField_Map020_METATILES
	map_block Field_Map020, .LField_Map020_SHAPES
	map_block Field_Map020, .LField_Map020_GRID
	map_block Field_Map020, .LField_Map020_ANIMATION
	map_block Field_Map020, .LField_Map020_BLEND
	map_block Field_Map020, .LField_Map020_MARKERS
.LField_Map020_METATILES:
	.incbin "games/tbs/src/map/map_020/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map020_SHAPES:
	.incbin "games/tbs/src/map/map_020/shapes.table.lz"
	.balign 4, 0
.LField_Map020_GRID:
	.incbin "games/tbs/src/map/map_020/grid.parts.lz"
	.balign 4, 0
.LField_Map020_ANIMATION:
	.incbin "games/tbs/src/map/map_020/animation.script.lz"
	.balign 4, 0
.LField_Map020_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map020_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set17Palette
Tileset_Set17Palette:
	.incbin "games/tbs/src/graphics/tileset/set_17/palette.gbapal.lz"
	.global Tileset_Set17TilesA
Tileset_Set17TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_a.4bpp.mtf"
	.byte 0xe1, 0xe8, 0xf4	@ PACKER: left in the buffer by Field_Map006+0x2d51
	.global Tileset_Set17TilesB
Tileset_Set17TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_b.4bpp.mtf"
	.global Tileset_Set17TilesC
Tileset_Set17TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_c.4bpp.mtf"
	.global Tileset_Set17TilesD
Tileset_Set17TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_d.4bpp.mtf"
