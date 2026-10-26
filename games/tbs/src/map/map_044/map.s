	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map044
Field_Map044:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map044, .LField_Map044_METATILES
	map_offset Field_Map044, .LField_Map044_SHAPES
	map_offset Field_Map044, .LField_Map044_GRID
	map_offset Field_Map044, .LField_Map044_ANIMATION
	map_offset Field_Map044
	map_offset Field_Map044, .LField_Map044_MARKERS
.LField_Map044_METATILES:
	.incbin "games/tbs/src/map/map_044/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map044_SHAPES:
	.incbin "games/tbs/src/map/map_044/shapes.table.lz"
	.balign 4, 0
.LField_Map044_GRID:
	.incbin "games/tbs/src/map/map_044/map.grid.lz"
	.balign 4, 0
.LField_Map044_ANIMATION:
	.incbin "games/tbs/src/map/map_044/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map044_MARKERS:
	treasure_spot 25, 7, 100
	treasure_spot 57, 7, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set34Palette
Tileset_Set34Palette:
	.incbin "games/tbs/src/graphics/tileset/set_34/palette.gbapal.lz"
	.byte 0x6a, 0x7b	@ PACKER: left in the buffer by Field_Map044+0x132
	.balign 4, 0
	.global Tileset_Set34TilesA
Tileset_Set34TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_34/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set34TilesB
Tileset_Set34TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_34/tiles_b.4bpp.mtf"
	.global Tileset_Set34TilesC
Tileset_Set34TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_34/tiles_c.4bpp.mtf"
	.byte 0xd6, 0xb6, 0x08	@ PACKER: left in the buffer by Tileset_Set34TilesB+0x17c1
	.global Tileset_Set34TilesD
Tileset_Set34TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_34/tiles_d.4bpp.mtf"
