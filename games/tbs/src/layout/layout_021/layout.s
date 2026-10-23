	.include "games/tbs/src/layout/layout.inc"
	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.global Field_Map021
Field_Map021:
	map_header 0, 0, 64, 57, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map021, .LField_Map021_METATILES
	map_block Field_Map021, .LField_Map021_SHAPES
	map_block Field_Map021, .LField_Map021_GRID
	map_block Field_Map021, .LField_Map021_ANIMATION
	map_block Field_Map021, .LField_Map021_BLEND
	map_block Field_Map021, .LField_Map021_MARKERS
.LField_Map021_METATILES:
	.incbin "games/tbs/src/layout/layout_021/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map021_SHAPES:
	.incbin "games/tbs/src/layout/layout_021/shapes.table.lz"
	.balign 4, 0
.LField_Map021_GRID:
	.incbin "games/tbs/src/layout/layout_021/grid.parts.lz"
	.balign 4, 0
.LField_Map021_ANIMATION:
	.incbin "games/tbs/src/layout/layout_021/animation.script.lz"
	.balign 4, 0
.LField_Map021_BLEND:
	.incbin "games/tbs/src/layout/layout_021/blend.script.lz"
	.balign 4, 0
.LField_Map021_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set18Palette
Tileset_Set18Palette:
	.incbin "games/tbs/src/graphics/tileset/set_18/palette.gbapal.lz"
	.byte 0x71	@ PACKER: left in the buffer by Field_Map021+0xcb
	.global Tileset_Set18TilesA
Tileset_Set18TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_18/tiles_a.4bpp.mtf"
	.global Tileset_Set18TilesB
Tileset_Set18TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_18/tiles_b.4bpp.mtf"
	.global Tileset_Set18TilesC
Tileset_Set18TilesC:
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/tileset/set_18/tiles_c_es.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/tileset/set_18/tiles_c_es.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/tileset/set_18/tiles_c_es.4bpp.mtf"
	.else
	.incbin "games/tbs/src/graphics/tileset/set_18/tiles_c.4bpp.mtf"
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x60, 0x82, 0xc0	@ PACKER: left in the buffer by Tileset_Set18TilesB+0xec1
	.else
	.ifdef TBS_EDITION_FR
	.byte 0x60, 0x82, 0xc0	@ PACKER: left in the buffer by Tileset_Set18TilesB+0xec1
	.else
	.ifdef TBS_EDITION_IT
	.byte 0x60, 0x82, 0xc0	@ PACKER: left in the buffer by Tileset_Set18TilesB+0xec1
	.else
	.byte 0x00, 0x90	@ PACKER: left in the buffer by Tileset_Set18TilesB+0xfaa
	.endif
	.endif
	.endif
	.global Tileset_Set18TilesD
Tileset_Set18TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_18/tiles_d.4bpp.mtf"
	.byte 0x04	@ PACKER: left in the buffer by Tileset_Set18TilesC+0x13f
