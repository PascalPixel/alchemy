	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map052
Field_Map052:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map052, .LField_Map052_CELL_PICTURES
	map_offset Field_Map052, .LField_Map052_SHAPES
	map_offset Field_Map052, .LField_Map052_GRID
	map_offset Field_Map052, .LField_Map052_ANIMATION
	map_offset Field_Map052, .LField_Map052_BLEND
	map_offset Field_Map052, .LField_Map052_MARKERS
.LField_Map052_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_052/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map052_SHAPES:
	.incbin "games/tbs/src/map/map_052/shapes.table.lz"
	.balign 4, 0
.LField_Map052_GRID:
	.incbin "games/tbs/src/map/map_052/map.grid.lz"
	.balign 4, 0
.LField_Map052_ANIMATION:
	.incbin "games/tbs/src/map/map_052/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map052_BLEND:
	.incbin "games/tbs/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map052_MARKERS:
	treasure_spot 7, 5, 200
	treasure_spot 8, 5, 200
	treasure_spot 7, 6, 200
	treasure_spot 8, 6, 200
	treasure_spot 4, 7, 100
	treasure_spot 21, 34, 101
	treasure_spot 15, 46, 102
	treasure_spot 16, 46, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set42Palette
Tileset_Set42Palette:
	.incbin "games/tbs/src/graphics/tileset/set_42/palette.gbapal.lz"
	.byte 0x1f	@ PACKER: left in the buffer by Field_Map052+0x1c7
	.global Tileset_Set42TilesA
Tileset_Set42TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_42/tiles_a.4bpp.mtf"
	.byte 0x47, 0x90, 0x0e	@ PACKER: left in the buffer by Tileset_Set40TilesA+0x2901
	.global Tileset_Set42TilesB
Tileset_Set42TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_42/tiles_b.4bpp.mtf"
	.global Tileset_Set42TilesC
Tileset_Set42TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_42/tiles_c.4bpp.mtf"
	.byte 0xc3, 0x73, 0x89	@ PACKER: left in the buffer by Tileset_Set42TilesB+0x1c59
	.global Tileset_Set42TilesD
Tileset_Set42TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_42/tiles_d.4bpp.mtf"
	.byte 0xc4, 0x83	@ PACKER: left in the buffer by Tileset_Set42TilesC+0x156e
