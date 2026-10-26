	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map046
Field_Map046:
	map_header 0, 0, 128, 68, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map046, .LField_Map046_METATILES
	map_offset Field_Map046, .LField_Map046_SHAPES
	map_offset Field_Map046, .LField_Map046_GRID
	map_offset Field_Map046, .LField_Map046_ANIMATION
	map_offset Field_Map046, .LField_Map046_BLEND
	map_offset Field_Map046, .LField_Map046_MARKERS
.LField_Map046_METATILES:
	.incbin "games/tbs/src/map/map_046/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map046_SHAPES:
	.incbin "games/tbs/src/map/map_046/shapes.table.lz"
	.balign 4, 0
.LField_Map046_GRID:
	.incbin "games/tbs/src/map/map_046/map.grid.lz"
	.balign 4, 0
.LField_Map046_ANIMATION:
	.incbin "games/tbs/src/map/map_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map046_BLEND:
	.incbin "games/tbs/src/map/map_046/blend.script.lz"
	.balign 4, 0
.LField_Map046_MARKERS:
	treasure_spot 9, 21, 102
	treasure_spot 10, 24, 100
	treasure_spot 54, 25, 101
	treasure_spots_end
	.global Tileset_Set36Palette
Tileset_Set36Palette:
	.incbin "games/tbs/src/graphics/tileset/set_36/palette.gbapal.lz"
	.byte 0x34	@ PACKER: left in the buffer by Field_Map046+0x18f
	.global Tileset_Set36TilesA
Tileset_Set36TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_36/tiles_a.4bpp.mtf"
	.balign 4, 0
	.global Tileset_Set36TilesB
Tileset_Set36TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_36/tiles_b.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set36TilesC
Tileset_Set36TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_36/tiles_c.4bpp.mtf"
	.byte 0xe8, 0x95, 0x5a	@ PACKER: left in the buffer by Tileset_Set36TilesB+0xc81
	.global Tileset_Set36TilesD
Tileset_Set36TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_36/tiles_d.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_Set36TilesC+0x13f
