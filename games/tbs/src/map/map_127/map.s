	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map127
Field_Map127:
	map_header 28, 0, 180, 56, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 56, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 112, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map127, .LField_Map127_METATILES
	map_offset Field_Map127, .LField_Map127_SHAPES
	map_offset Field_Map127, .LField_Map127_GRID
	map_offset Field_Map127, .LField_Map127_ANIMATION
	map_offset Field_Map127, .LField_Map127_BLEND
	map_offset Field_Map127, .LField_Map127_MARKERS
.LField_Map127_METATILES:
	.incbin "games/tbs/src/map/map_127/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map127_SHAPES:
	.incbin "games/tbs/src/map/map_127/shapes.table.lz"
	.balign 4, 0
.LField_Map127_GRID:
	.incbin "games/tbs/src/map/map_127/map.grid.lz"
	.balign 4, 0
.LField_Map127_ANIMATION:
	.incbin "games/tbs/src/map/map_127/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map127_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map127_MARKERS:
	treasure_spot 28, 5, 100
	treasure_spot 48, 6, 101
	treasure_spot 61, 13, 103
	treasure_spots_end
	.global Tileset_Set60Palette
Tileset_Set60Palette:
	.incbin "games/tbs/src/graphics/tileset/set_60/palette.gbapal.lz"
	.byte 0x31, 0x9d, 0x0f	@ PACKER: left in the buffer by Field_Map127+0x14d
	.global Tileset_Set60TilesA
Tileset_Set60TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_60/tiles_a.4bpp.mtf"
	.byte 0x63	@ PACKER: left in the buffer by Tileset_Set59TilesA+0x29df
	.global Tileset_Set60TilesB
Tileset_Set60TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_60/tiles_b.4bpp.mtf"
	.global Tileset_Set60TilesC
Tileset_Set60TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_60/tiles_c.4bpp.mtf"
	.byte 0x25, 0x96, 0x85	@ PACKER: left in the buffer by Tileset_Set60TilesB+0x1895
	.global Tileset_Set60TilesD
Tileset_Set60TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_60/tiles_d.4bpp.mtf"
	.byte 0x69	@ PACKER: left in the buffer by Tileset_Set60TilesB+0x1907
