	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Test0
Field_Test0:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Test0, .LField_Test0_CELL_PICTURES
	map_offset Field_Test0, .LField_Test0_SHAPES
	map_offset Field_Test0, .LField_Test0_GRID
	map_offset Field_Test0
	map_offset Field_Test0
	map_offset Field_Test0, .LField_Test0_MARKERS
.LField_Test0_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_001/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Test0_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Test0_GRID:
	.incbin "games/tbs/src/map/map_001/map.grid.lz"
	.balign 4, 0
.LField_Test0_MARKERS:
	treasure_spots_end
	.balign 4, 0
@ Tilesets 68 to 75 belong to the test scenes (Field_Test0 to Field_Test7) and
@ are one blank set: fourteen black palettes, two tiles in colour 1 and three
@ empty banks. Their sheets are drawn with that black palette, the only one
@ the game pairs them with, as 1-bit images of its first two colours; the
@ bank with two tiles is drawn two tiles wide, as the layout finder reads it.
	.global Tileset_Set68Palette
Tileset_Set68Palette:
	.incbin "games/tbs/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Test0+0xb
	.global Tileset_Set68TilesA
Tileset_Set68TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Test0+0x14e
	.global Tileset_Set68TilesB
Tileset_Set68TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set68TilesA+0x13f
	.global Tileset_Set68TilesC
Tileset_Set68TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set68TilesA+0x13f
	.global Tileset_Set68TilesD
Tileset_Set68TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set68TilesA+0x13f
