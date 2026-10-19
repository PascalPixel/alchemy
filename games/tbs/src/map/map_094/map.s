	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map094
Field_Map094:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map094, .LField_Map094_METATILES
	map_block Field_Map094, .LField_Map094_SHAPES
	map_block Field_Map094, .LField_Map094_GRID
	map_block Field_Map094, .LField_Map094_ANIMATION
	map_block Field_Map094, .LField_Map094_BLEND
	map_block Field_Map094, .LField_Map094_MARKERS
.LField_Map094_METATILES:
	.incbin "games/tbs/src/map/map_094/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map094_SHAPES:
	.incbin "games/tbs/src/map/map_094/shapes.table.lz"
	.balign 4, 0
.LField_Map094_GRID:
	.incbin "games/tbs/src/map/map_094/grid.parts.lz"
	.balign 4, 0
.LField_Map094_ANIMATION:
	.incbin "games/tbs/src/map/map_094/animation.script.lz"
	.balign 4, 0
.LField_Map094_BLEND:
	.incbin "games/tbs/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map094_MARKERS:
	.incbin "games/tbs/src/map/map_094/markers.table"
	.balign 4, 0
	.global Tileset_Set52Palette
Tileset_Set52Palette:
	.incbin "games/tbs/src/graphics/tileset/set_52/palette.gbapal.lz"
	.byte 0xc0	@ PACKER: left in the buffer by Field_Map094+0x1c7
	.global Tileset_Set52TilesA
Tileset_Set52TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_52/tiles_a.4bpp.mtf"
	.global Tileset_Set52TilesB
Tileset_Set52TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_52/tiles_b.4bpp.mtf"
	.global Tileset_Set52TilesC
Tileset_Set52TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_52/tiles_c.4bpp.mtf"
	.global Tileset_Set52TilesD
Tileset_Set52TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_52/tiles_d.4bpp.mtf"
