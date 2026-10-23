	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map006
Field_Map006:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map006, .LField_Map006_METATILES
	map_block Field_Map006, .LField_Map006_SHAPES
	map_block Field_Map006, .LField_Map006_GRID
	map_block Field_Map006, .LField_Map006_ANIMATION
	map_block Field_Map006, .LField_Map006_BLEND
	map_block Field_Map006, .LField_Map006_MARKERS
.LField_Map006_METATILES:
	.incbin "games/tbs/src/layout/layout_006/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map006_SHAPES:
	.incbin "games/tbs/src/layout/layout_006/shapes.table.lz"
	.balign 4, 0
.LField_Map006_GRID:
	.incbin "games/tbs/src/layout/layout_006/grid.parts.lz"
	.balign 4, 0
.LField_Map006_ANIMATION:
	.incbin "games/tbs/src/layout/layout_006/animation.script.lz"
	.balign 4, 0
.LField_Map006_BLEND:
	.incbin "games/tbs/src/layout/layout_005/blend.script.lz"
	.balign 4, 0
.LField_Map006_MARKERS:
	.incbin "games/tbs/src/layout/layout_006/markers.table"
	.balign 4, 0
	.global Tileset_Set05Palette
Tileset_Set05Palette:
	.incbin "games/tbs/src/graphics/tileset/set_05/palette.gbapal.lz"
	.byte 0x64, 0x36, 0x80	@ PACKER: left in the buffer by Field_Map006+0x18d
	.global Tileset_Set05TilesA
Tileset_Set05TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_05/tiles_a.4bpp.mtf"
	.byte 0x86, 0x47	@ PACKER: left in the buffer by Field_Map006+0x1dae
	.global Tileset_Set05TilesB
Tileset_Set05TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_05/tiles_b.4bpp.mtf"
	.global Tileset_Set05TilesC
Tileset_Set05TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_05/tiles_c.4bpp.mtf"
	.byte 0x38, 0xf0, 0x3d	@ PACKER: left in the buffer by Tileset_Set05TilesB+0x1609
	.global Tileset_Set05TilesD
Tileset_Set05TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_05/tiles_d.4bpp.mtf"
	.byte 0x7e	@ PACKER: left in the buffer by Tileset_Set05TilesC+0x150b
