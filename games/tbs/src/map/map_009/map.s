	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map009
Field_Map009:
	map_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map009, .LField_Map009_CELL_PICTURES
	map_offset Field_Map009, .LField_Map009_SHAPES
	map_offset Field_Map009, .LField_Map009_GRID
	map_offset Field_Map009, .LField_Map009_ANIMATION
	map_offset Field_Map009, .LField_Map009_BLEND
	map_offset Field_Map009, .LField_Map009_MARKERS
.LField_Map009_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_009/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map009_SHAPES:
	.incbin "games/tbs/src/map/map_009/shapes.table.lz"
	.balign 4, 0
.LField_Map009_GRID:
	.incbin "games/tbs/src/map/map_009/map.grid.lz"
	.balign 4, 0
.LField_Map009_ANIMATION:
	.incbin "games/tbs/src/map/map_009/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map009_BLEND:
	.incbin "games/tbs/src/map/map_009/blend.script.lz"
	.balign 4, 0
.LField_Map009_MARKERS:
	treasure_spot 3, 15, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set08Palette
Tileset_Set08Palette:
	.incbin "games/tbs/src/graphics/tileset/set_08/palette.gbapal.lz"
	.byte 0xcf, 0x83, 0x07	@ PACKER: left in the buffer by Field_Map009+0x159
	.global Tileset_Set08TilesA
Tileset_Set08TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_a.4bpp.mtf"
	.global Tileset_Set08TilesB
Tileset_Set08TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_b.4bpp.mtf"
	.byte 0x18	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x1a4b
	.global Tileset_Set08TilesC
Tileset_Set08TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_c.4bpp.mtf"
	.global Tileset_Set08TilesD
Tileset_Set08TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_08/tiles_d.4bpp.mtf"
	.byte 0xe1, 0x71	@ PACKER: left in the buffer by Tileset_Set08TilesC+0x842
