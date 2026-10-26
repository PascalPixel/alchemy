	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map075
Field_Map075:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map075, .LField_Map075_METATILES
	map_offset Field_Map075, .LField_Map075_SHAPES
	map_offset Field_Map075, .LField_Map075_GRID
	map_offset Field_Map075, .LField_Map075_ANIMATION
	map_offset Field_Map075, .LField_Map075_BLEND
	map_offset Field_Map075, .LField_Map075_MARKERS
.LField_Map075_METATILES:
	.incbin "games/tbs/src/map/map_075/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map075_SHAPES:
	.incbin "games/tbs/src/map/map_075/shapes.table.lz"
	.balign 4, 0
.LField_Map075_GRID:
	.incbin "games/tbs/src/map/map_075/map.grid.lz"
	.balign 4, 0
.LField_Map075_ANIMATION:
	.incbin "games/tbs/src/map/map_075/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map075_BLEND:
	.incbin "games/tbs/src/map/map_075/blend.script.lz"
	.balign 4, 0
.LField_Map075_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set48Palette
Tileset_Set48Palette:
	.incbin "games/tbs/src/graphics/tileset/set_48/palette.gbapal.lz"
	.global Tileset_Set48TilesA
Tileset_Set48TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_48/tiles_a.4bpp.mtf"
	.global Tileset_Set48TilesB
Tileset_Set48TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_48/tiles_b.4bpp.mtf"
	.byte 0xd6, 0x54	@ PACKER: left in the buffer by Tileset_Set48TilesA+0x2062
	.global Tileset_Set48TilesC
Tileset_Set48TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_48/tiles_c.4bpp.mtf"
	.global Tileset_Set48TilesD
Tileset_Set48TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_48/tiles_d.4bpp.mtf"
	.byte 0xd0	@ PACKER: left in the buffer by Tileset_Set48TilesC+0x130b
