	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map073
Field_Map073:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map073, .LField_Map073_METATILES
	map_block Field_Map073, .LField_Map073_SHAPES
	map_block Field_Map073, .LField_Map073_GRID
	map_block Field_Map073, .LField_Map073_ANIMATION
	map_block Field_Map073, .LField_Map073_BLEND
	map_block Field_Map073, .LField_Map073_MARKERS
.LField_Map073_METATILES:
	.incbin "games/tbs/src/map/map_073/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map073_SHAPES:
	.incbin "games/tbs/src/map/map_073/shapes.table.lz"
	.balign 4, 0
.LField_Map073_GRID:
	.incbin "games/tbs/src/map/map_073/grid.parts.lz"
	.balign 4, 0
.LField_Map073_ANIMATION:
	.incbin "games/tbs/src/map/map_073/animation.script.lz"
	.balign 4, 0
.LField_Map073_BLEND:
	.incbin "games/tbs/src/map/map_073/blend.script.lz"
	.balign 4, 0
.LField_Map073_MARKERS:
	.incbin "games/tbs/src/map/map_073/markers.table"
	.balign 4, 0
	.global Tileset_Set47Palette
Tileset_Set47Palette:
	.incbin "games/tbs/src/graphics/tileset/set_47/palette.gbapal.lz"
	.byte 0x98, 0xc9	@ PACKER: left in the buffer by Field_Map073+0x192
	.global Tileset_Set47TilesA
Tileset_Set47TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_47/tiles_a.4bpp.mtf"
	.byte 0x01, 0x6d	@ PACKER: left in the buffer by Field_Map073+0x150e
	.global Tileset_Set47TilesB
Tileset_Set47TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_47/tiles_b.4bpp.mtf"
	.byte 0xec	@ PACKER: left in the buffer by Field_Map073+0x1de3
	.global Tileset_Set47TilesC
Tileset_Set47TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_47/tiles_c.4bpp.mtf"
	.byte 0x2e, 0x1f	@ PACKER: left in the buffer by Tileset_Set47TilesB+0x1092
	.global Tileset_Set47TilesD
Tileset_Set47TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_47/tiles_d.4bpp.mtf"
