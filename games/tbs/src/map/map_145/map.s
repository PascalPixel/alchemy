	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map145
Field_Map145:
	map_header 0, 0, 100, 100, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map145, .LField_Map145_METATILES
	map_block Field_Map145, .LField_Map145_SHAPES
	map_block Field_Map145, .LField_Map145_GRID
	map_block Field_Map145, .LField_Map145_ANIMATION
	map_block Field_Map145, .LField_Map145_BLEND
	map_block Field_Map145, .LField_Map145_MARKERS
.LField_Map145_METATILES:
	.incbin "games/tbs/src/map/map_145/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map145_SHAPES:
	.incbin "games/tbs/src/map/map_015/shapes.table.lz"
	.balign 4, 0
.LField_Map145_GRID:
	.incbin "games/tbs/src/map/map_145/grid.parts.lz"
	.balign 4, 0
.LField_Map145_ANIMATION:
	.incbin "games/tbs/src/map/map_145/animation.script.lz"
	.balign 4, 0
.LField_Map145_BLEND:
	.incbin "games/tbs/src/map/map_009/blend.script.lz"
	.balign 4, 0
.LField_Map145_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set63Palette
Tileset_Set63Palette:
	.incbin "games/tbs/src/graphics/tileset/set_63/palette.gbapal.lz"
	.byte 0x4a, 0xf0, 0x09	@ PACKER: left in the buffer by Field_Map145+0x161
	.global Tileset_Set63TilesA
Tileset_Set63TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_63/tiles_a.4bpp.mtf"
	.byte 0x02, 0x04, 0x30	@ PACKER: left in the buffer by Field_Map143+0x29c9
