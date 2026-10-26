	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map151
Field_Map151:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map151, .LField_Map151_METATILES
	map_offset Field_Map151, .LField_Map151_SHAPES
	map_offset Field_Map151, .LField_Map151_GRID
	map_offset Field_Map151, .LField_Map151_ANIMATION
	map_offset Field_Map151, .LField_Map151_BLEND
	map_offset Field_Map151, .LField_Map151_MARKERS
.LField_Map151_METATILES:
	.incbin "games/tbs/src/map/map_151/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map151_SHAPES:
	.incbin "games/tbs/src/map/map_151/shapes.table.lz"
	.balign 4, 0
.LField_Map151_GRID:
	.incbin "games/tbs/src/map/map_151/map.grid.lz"
	.balign 4, 0
.LField_Map151_ANIMATION:
	.incbin "games/tbs/src/map/map_151/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map151_BLEND:
	.incbin "games/tbs/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map151_MARKERS:
	treasure_spot 9, 3, 100
	treasure_spot 10, 3, 100
	treasure_spot 20, 3, 101
	treasure_spot 21, 3, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set65Palette
Tileset_Set65Palette:
	.incbin "games/tbs/src/graphics/tileset/set_65/palette.gbapal.lz"
	.global Tileset_Set65TilesA
Tileset_Set65TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_65/tiles_a.4bpp.mtf"
	.byte 0x94	@ PACKER: left in the buffer by Field_Map151+0x1e7f
	.balign 4, 0
	.global Tileset_Set65TilesB
Tileset_Set65TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_65/tiles_b.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set65TilesC
Tileset_Set65TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_65/tiles_c.4bpp.mtf"
	.global Tileset_Set65TilesD
Tileset_Set65TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_65/tiles_d.4bpp.mtf"
	.byte 0x69, 0x4e, 0x50	@ PACKER: left in the buffer by Tileset_Set65TilesC+0x160d
