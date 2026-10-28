	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map047
Field_Map047:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map047, .LField_Map047_CELL_PICTURES
	map_offset Field_Map047, .LField_Map047_SHAPES
	map_offset Field_Map047, .LField_Map047_GRID
	map_offset Field_Map047, .LField_Map047_ANIMATION
	map_offset Field_Map047, .LField_Map047_BLEND
	map_offset Field_Map047, .LField_Map047_MARKERS
.LField_Map047_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_047/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map047_SHAPES:
	.incbin "games/tbs/src/map/map_047/shapes.table.lz"
	.balign 4, 0
.LField_Map047_GRID:
	.incbin "games/tbs/src/map/map_047/map.grid.lz"
	.balign 4, 0
.LField_Map047_ANIMATION:
	.incbin "games/tbs/src/map/map_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map047_BLEND:
	.incbin "games/tbs/src/map/map_047/blend.script.lz"
	.balign 4, 0
.LField_Map047_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set37Palette
Tileset_Set37Palette:
	.incbin "games/tbs/src/graphics/tileset/set_37/palette.gbapal.lz"
	.byte 0x9b, 0x46	@ PACKER: left in the buffer by Field_Map047+0x16a
	.global Tileset_Set37TilesA
Tileset_Set37TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_37/tiles_a.4bpp.mtf"
	.byte 0xe0, 0x91, 0x0f	@ PACKER: left in the buffer by Tileset_Set36TilesB+0x1d59
	.global Tileset_Set37TilesB
Tileset_Set37TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_37/tiles_b.4bpp.mtf"
	.byte 0x2f	@ PACKER: left in the buffer by Tileset_Set37TilesA+0x13f
	.global Tileset_Set37TilesC
Tileset_Set37TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_37/tiles_c.4bpp.mtf"
	.byte 0x23, 0xe7	@ PACKER: left in the buffer by Tileset_Set36TilesB+0x27fe
	.global Tileset_Set37TilesD
Tileset_Set37TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_37/tiles_d.4bpp.mtf"
	.byte 0x09	@ PACKER: left in the buffer by Tileset_Set37TilesC+0x13f
