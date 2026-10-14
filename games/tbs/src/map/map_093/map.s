	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map093
Field_Map093:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map093, .LField_Map093_METATILES
	map_block Field_Map093, .LField_Map093_SHAPES
	map_block Field_Map093, .LField_Map093_GRID
	map_block Field_Map093, .LField_Map093_ANIMATION
	map_block Field_Map093, .LField_Map093_BLEND
	map_block Field_Map093, .LField_Map093_MARKERS
.LField_Map093_METATILES:
	.incbin "games/tbs/src/map/map_093/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map093_SHAPES:
	.incbin "games/tbs/src/map/map_093/shapes.table.lz"
	.balign 4, 0
.LField_Map093_GRID:
	.incbin "games/tbs/src/map/map_093/grid.parts.lz"
	.balign 4, 0
.LField_Map093_ANIMATION:
	.incbin "games/tbs/src/map/map_093/animation.script.lz"
	.balign 4, 0
.LField_Map093_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map093_MARKERS:
	.incbin "games/tbs/src/map/map_093/markers.table"
	.balign 4, 0
	.global Tileset_Set51Palette
Tileset_Set51Palette:
	.incbin "games/tbs/src/graphics/tileset/set_51/palette.gbapal.lz"
	.byte 0x94	@ PACKER: left in the buffer by Field_Map093+0x17b
	.balign 4, 0
	.global Tileset_Set51TilesA
Tileset_Set51TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_51/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set51TilesB
Tileset_Set51TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_51/tiles_b.4bpp.mtf"
	.byte 0xd1, 0xbc	@ PACKER: left in the buffer by Field_Map092+0x2776
	.global Tileset_Set51TilesC
Tileset_Set51TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_51/tiles_c.4bpp.mtf"
	.byte 0x24, 0xda, 0xbb	@ PACKER: left in the buffer by Tileset_Set51TilesB+0x1865
	.global Tileset_Set51TilesD
Tileset_Set51TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_51/tiles_d.4bpp.mtf"
