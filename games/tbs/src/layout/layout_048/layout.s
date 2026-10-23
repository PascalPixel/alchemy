	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map048
Field_Map048:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map048, .LField_Map048_METATILES
	map_block Field_Map048, .LField_Map048_SHAPES
	map_block Field_Map048, .LField_Map048_GRID
	map_block Field_Map048, .LField_Map048_ANIMATION
	map_block Field_Map048, .LField_Map048_BLEND
	map_block Field_Map048, .LField_Map048_MARKERS
.LField_Map048_METATILES:
	.incbin "games/tbs/src/layout/layout_048/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map048_SHAPES:
	.incbin "games/tbs/src/layout/layout_048/shapes.table.lz"
	.balign 4, 0
.LField_Map048_GRID:
	.incbin "games/tbs/src/layout/layout_048/grid.parts.lz"
	.balign 4, 0
.LField_Map048_ANIMATION:
	.incbin "games/tbs/src/layout/layout_048/animation.script.lz"
	.balign 4, 0
.LField_Map048_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Map048_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set38Palette
Tileset_Set38Palette:
	.incbin "games/tbs/src/graphics/tileset/set_38/palette.gbapal.lz"
	.global Tileset_Set38TilesA
Tileset_Set38TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_38/tiles_a.4bpp.mtf"
	.byte 0x80, 0x03	@ PACKER: left in the buffer by Field_Map048+0x13da
