	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map101
Field_Map101:
	map_header 5, 124, 51, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map101, .LField_Map101_METATILES
	map_block Field_Map101, .LField_Map101_SHAPES
	map_block Field_Map101, .LField_Map101_GRID
	map_block Field_Map101, .LField_Map101_ANIMATION
	map_block Field_Map101
	map_block Field_Map101, .LField_Map101_MARKERS
.LField_Map101_METATILES:
	.incbin "map/map_101/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map101_SHAPES:
	.incbin "map/map_101/shapes.table.lz"
	.balign 4, 0
.LField_Map101_GRID:
	.incbin "map/map_101/grid.parts.lz"
	.balign 4, 0
.LField_Map101_ANIMATION:
	.incbin "map/map_101/animation.script.lz"
	.balign 4, 0
.LField_Map101_MARKERS:
	.incbin "map/map_101/markers.table"
	.balign 4, 0
	.global Tileset_Set54Palette
Tileset_Set54Palette:
	.incbin "graphics/tileset/set_54/palette.gbapal.lz"
	.byte 0x2a, 0x91, 0x1b	@ PACKER: left in the buffer by Field_Map101+0x1a5
	.global Tileset_Set54TilesA
Tileset_Set54TilesA:
	.incbin "graphics/tileset/set_54/tiles_a.4bpp.mtf"
	.global Tileset_Set54TilesB
Tileset_Set54TilesB:
	.incbin "graphics/tileset/set_54/tiles_b.4bpp.mtf"
	.byte 0x83	@ PACKER: left in the buffer by Tileset_Set54TilesA+0x23e3
	.global Tileset_Set54TilesC
Tileset_Set54TilesC:
	.incbin "graphics/tileset/set_54/tiles_c.4bpp.mtf"
