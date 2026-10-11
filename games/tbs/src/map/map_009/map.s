	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map009
Field_Map009:
	map_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map009, .LField_Map009_METATILES
	map_block Field_Map009, .LField_Map009_SHAPES
	map_block Field_Map009, .LField_Map009_GRID
	map_block Field_Map009, .LField_Map009_ANIMATION
	map_block Field_Map009, .LField_Map009_BLEND
	map_block Field_Map009, .LField_Map009_MARKERS
.LField_Map009_METATILES:
	.incbin "map/map_009/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map009_SHAPES:
	.incbin "map/map_009/shapes.table.lz"
	.balign 4, 0
.LField_Map009_GRID:
	.incbin "map/map_009/grid.parts.lz"
	.balign 4, 0
.LField_Map009_ANIMATION:
	.incbin "map/map_009/animation.script.lz"
	.balign 4, 0
.LField_Map009_BLEND:
	.incbin "map/map_009/blend.script.lz"
	.balign 4, 0
.LField_Map009_MARKERS:
	.incbin "map/map_009/markers.table"
	.balign 4, 0
	.global Tileset_Set08Palette
Tileset_Set08Palette:
	.incbin "graphics/tileset/set_08/palette.gbapal.lz"
	.byte 0xcf, 0x83, 0x07	@ PACKER: left in the buffer by Field_Map009+0x159
	.global Tileset_Set08TilesA
Tileset_Set08TilesA:
	.incbin "graphics/tileset/set_08/tiles_a.4bpp.mtf"
	.global Tileset_Set08TilesB
Tileset_Set08TilesB:
	.incbin "graphics/tileset/set_08/tiles_b.4bpp.mtf"
	.byte 0x18	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x1a4b
	.global Tileset_Set08TilesC
Tileset_Set08TilesC:
	.incbin "graphics/tileset/set_08/tiles_c.4bpp.mtf"
	.global Tileset_Set08TilesD
Tileset_Set08TilesD:
	.incbin "common/graphics/tileset/set_08/tiles_d.4bpp.mtf"
	.byte 0xe1, 0x71	@ PACKER: left in the buffer by Tileset_Set08TilesC+0x842
