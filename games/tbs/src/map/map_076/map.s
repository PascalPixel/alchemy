	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map076
Field_Map076:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map076, .LField_Map076_METATILES
	map_block Field_Map076, .LField_Map076_SHAPES
	map_block Field_Map076, .LField_Map076_GRID
	map_block Field_Map076, .LField_Map076_ANIMATION
	map_block Field_Map076, .LField_Map076_BLEND
	map_block Field_Map076, .LField_Map076_MARKERS
.LField_Map076_METATILES:
	.incbin "games/tbs/src/map/map_076/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map076_SHAPES:
	.incbin "games/tbs/src/map/map_076/shapes.table.lz"
	.balign 4, 0
.LField_Map076_GRID:
	.incbin "games/tbs/src/map/map_076/grid.parts.lz"
	.balign 4, 0
.LField_Map076_ANIMATION:
	.incbin "games/tbs/src/map/map_032/animation.script.lz"
	.balign 4, 0
.LField_Map076_BLEND:
	.incbin "games/tbs/src/map/map_005/blend.script.lz"
	.balign 4, 0
.LField_Map076_MARKERS:
	.incbin "games/tbs/src/map/map_076/markers.table"
	.global Tileset_Set49Palette
Tileset_Set49Palette:
	.incbin "games/tbs/src/graphics/tileset/set_49/palette.gbapal.lz"
	.byte 0xb3, 0xec, 0xe0	@ PACKER: left in the buffer by Field_Map076+0x1c9
	.global Tileset_Set49TilesA
Tileset_Set49TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_49/tiles_a.4bpp.mtf"
	.byte 0x00, 0xc1, 0x01	@ PACKER: left in the buffer by Field_Map076+0x2345
