	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map098
Field_Map098:
	map_header 0, 0, 56, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map098, .LField_Map098_METATILES
	map_offset Field_Map098, .LField_Map098_SHAPES
	map_offset Field_Map098, .LField_Map098_GRID
	map_offset Field_Map098, .LField_Map098_ANIMATION
	map_offset Field_Map098, .LField_Map098_BLEND
	map_offset Field_Map098, .LField_Map098_MARKERS
.LField_Map098_METATILES:
	.incbin "games/tbs/src/map/map_098/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map098_SHAPES:
	.incbin "games/tbs/src/map/map_098/shapes.table.lz"
	.balign 4, 0
.LField_Map098_GRID:
	.incbin "games/tbs/src/map/map_098/map.grid.lz"
	.balign 4, 0
.LField_Map098_ANIMATION:
	.incbin "games/tbs/src/map/map_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map098_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map098_MARKERS:
	treasure_spot 1, 11, 100
	treasure_spot 24, 12, 101
	treasure_spot 7, 16, 102
	treasure_spot 28, 31, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set53Palette
Tileset_Set53Palette:
	.incbin "games/tbs/src/graphics/tileset/set_53/palette.gbapal.lz"
	.byte 0x14, 0x41, 0xb9	@ PACKER: left in the buffer by Field_Map098+0x189
	.global Tileset_Set53TilesA
Tileset_Set53TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_53/tiles_a.4bpp.mtf"
	.byte 0x09, 0x02	@ PACKER: left in the buffer by Field_Map098+0x1a82
	.global Tileset_Set53TilesB
Tileset_Set53TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_53/tiles_b.4bpp.mtf"
