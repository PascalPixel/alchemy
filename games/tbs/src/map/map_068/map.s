	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map068
Field_Map068:
	map_header 0, 0, 106, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 108, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map068, .LField_Map068_METATILES
	map_block Field_Map068, .LField_Map068_SHAPES
	map_block Field_Map068, .LField_Map068_GRID
	map_block Field_Map068, .LField_Map068_ANIMATION
	map_block Field_Map068
	map_block Field_Map068, .LField_Map068_MARKERS
.LField_Map068_METATILES:
	.incbin "map/map_068/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map068_SHAPES:
	.incbin "map/map_068/shapes.table.lz"
	.balign 4, 0
.LField_Map068_GRID:
	.incbin "map/map_068/grid.parts.lz"
	.balign 4, 0
.LField_Map068_ANIMATION:
	.incbin "map/map_068/animation.script.lz"
	.balign 4, 0
.LField_Map068_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set46Palette
Tileset_Set46Palette:
	.incbin "graphics/tileset/set_46/palette.gbapal.lz"
	.byte 0xcc	@ PACKER: left in the buffer by Field_Map068+0x16b
	.global Tileset_Set46TilesA
Tileset_Set46TilesA:
	.incbin "graphics/tileset/set_46/tiles_a.4bpp.mtf"
	.byte 0x63	@ PACKER: left in the buffer by Field_Map068+0x2d9b
	.global Tileset_Set46TilesB
Tileset_Set46TilesB:
	.incbin "graphics/tileset/set_46/tiles_b.4bpp.mtf"
	.byte 0x10, 0xa1	@ PACKER: left in the buffer by Tileset_Set46TilesA+0x218e
	.global Tileset_Set46TilesC
Tileset_Set46TilesC:
	.incbin "graphics/tileset/set_46/tiles_c.4bpp.mtf"
	.byte 0xe5, 0xa2	@ PACKER: left in the buffer by Tileset_Set46TilesB+0x706
	.global Tileset_Set46TilesD
Tileset_Set46TilesD:
	.incbin "graphics/tileset/set_46/tiles_d.4bpp.mtf"
	.byte 0x0a, 0x07	@ PACKER: left in the buffer by Tileset_Set46TilesB+0xc12
