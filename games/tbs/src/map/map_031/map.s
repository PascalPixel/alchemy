	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map031
Field_Map031:
	map_header 0, 0, 56, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map031, .LField_Map031_METATILES
	map_block Field_Map031, .LField_Map031_SHAPES
	map_block Field_Map031, .LField_Map031_GRID
	map_block Field_Map031, .LField_Map031_ANIMATION
	map_block Field_Map031, .LField_Map031_BLEND
	map_block Field_Map031, .LField_Map031_MARKERS
.LField_Map031_METATILES:
	.incbin "map/map_031/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map031_SHAPES:
	.incbin "map/map_031/shapes.table.lz"
	.balign 4, 0
.LField_Map031_GRID:
	.incbin "map/map_031/grid.parts.lz"
	.balign 4, 0
.LField_Map031_ANIMATION:
	.incbin "common/map/map_031/animation.script.lz"
	.balign 4, 0
.LField_Map031_BLEND:
	.incbin "map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map031_MARKERS:
	.incbin "map/map_031/markers.table"
	.balign 4, 0
	.global Tileset_Set23Palette
Tileset_Set23Palette:
	.incbin "graphics/tileset/set_23/palette.gbapal.lz"
	.balign 4, 0
	.global Tileset_Set23TilesA
Tileset_Set23TilesA:
	.incbin "graphics/tileset/set_23/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set23TilesB
Tileset_Set23TilesB:
	.incbin "graphics/tileset/set_23/tiles_b.4bpp.mtf"
	.byte 0xa6, 0x09, 0x9b	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x28c5
	.global Tileset_Set23TilesC
Tileset_Set23TilesC:
	.incbin "graphics/tileset/set_23/tiles_c.4bpp.mtf"
	.global Tileset_Set23TilesD
Tileset_Set23TilesD:
	.incbin "graphics/tileset/set_23/tiles_d.4bpp.mtf"
