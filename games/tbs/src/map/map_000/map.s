	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map000
Field_Map000:
	map_header 0, 0, 60, 188, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map000, .LField_Map000_CELL_PICTURES
	map_offset Field_Map000, .LField_Map000_SHAPES
	map_offset Field_Map000, .LField_Map000_GRID
	map_offset Field_Map000, .LField_Map000_ANIMATION
	map_offset Field_Map000
	map_offset Field_Map000, .LField_Map000_MARKERS
.LField_Map000_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_000/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map000_SHAPES:
	.incbin "games/tbs/src/map/map_000/shapes.table.lz"
	.balign 4, 0
.LField_Map000_GRID:
	.incbin "games/tbs/src/map/map_000/map.grid.lz"
	.balign 4, 0
.LField_Map000_ANIMATION:
	.incbin "games/tbs/src/map/map_000/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map000_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set00Palette
Tileset_Set00Palette:
	.incbin "games/tbs/src/graphics/tileset/set_00/palette.gbapal.lz"
	.byte 0x3f, 0x55, 0x78	@ PACKER: left in the buffer by Field_Map000+0x18d
	.global Tileset_Set00TilesA
Tileset_Set00TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_00/tiles_a.4bpp.mtf"
	.global Tileset_Set00TilesB
Tileset_Set00TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_00/tiles_b.4bpp.mtf"
	.byte 0x5d, 0x04, 0x53	@ PACKER: left in the buffer by Tileset_Set00TilesA+0x2529
	.global Tileset_Set00TilesC
Tileset_Set00TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_00/tiles_c.4bpp.mtf"
	.global Tileset_Set00TilesD
Tileset_Set00TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_00/tiles_d.4bpp.mtf"
	.byte 0x14, 0x85, 0xc2	@ PACKER: left in the buffer by Tileset_Set00TilesC+0x1e8d
