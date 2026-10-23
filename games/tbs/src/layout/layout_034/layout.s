	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map034
Field_Map034:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_METATILES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034
	map_block Field_Map034, .LField_Map034_MARKERS
.LField_Map034_METATILES:
	.incbin "games/tbs/src/layout/layout_034/metatiles_es.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "games/tbs/src/layout/layout_034/shapes.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "games/tbs/src/layout/layout_034/grid_es.parts.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "games/tbs/src/layout/layout_034/animation.script.lz"
	.balign 4, 0
.LField_Map034_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_METATILES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034
	map_block Field_Map034, .LField_Map034_MARKERS
.LField_Map034_METATILES:
	.incbin "games/tbs/src/layout/layout_034/metatiles_es.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "games/tbs/src/layout/layout_034/shapes.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "games/tbs/src/layout/layout_034/grid_es.parts.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "games/tbs/src/layout/layout_034/animation.script.lz"
	.balign 4, 0
.LField_Map034_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_METATILES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034
	map_block Field_Map034, .LField_Map034_MARKERS
.LField_Map034_METATILES:
	.incbin "games/tbs/src/layout/layout_034/metatiles_es.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "games/tbs/src/layout/layout_034/shapes.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "games/tbs/src/layout/layout_034/grid_es.parts.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "games/tbs/src/layout/layout_034/animation.script.lz"
	.balign 4, 0
.LField_Map034_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map034, .LField_Map034_METATILES
	map_block Field_Map034, .LField_Map034_SHAPES
	map_block Field_Map034, .LField_Map034_GRID
	map_block Field_Map034, .LField_Map034_ANIMATION
	map_block Field_Map034
	map_block Field_Map034, .LField_Map034_MARKERS
.LField_Map034_METATILES:
	.incbin "games/tbs/src/layout/layout_034/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map034_SHAPES:
	.incbin "games/tbs/src/layout/layout_034/shapes.table.lz"
	.balign 4, 0
.LField_Map034_GRID:
	.incbin "games/tbs/src/layout/layout_034/grid.parts.lz"
	.balign 4, 0
.LField_Map034_ANIMATION:
	.incbin "games/tbs/src/layout/layout_034/animation.script.lz"
	.balign 4, 0
.LField_Map034_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Set26Palette
Tileset_Set26Palette:
	.incbin "games/tbs/src/graphics/tileset/set_26/palette.gbapal.lz"
	.byte 0x08	@ PACKER: left in the buffer by Field_Map034+0x183
	.global Tileset_Set26TilesA
Tileset_Set26TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_26/tiles_a.4bpp.mtf"
	.byte 0x9e	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x2b3b
	.global Tileset_Set26TilesB
Tileset_Set26TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_26/tiles_b.4bpp.mtf"
	.byte 0xf1, 0xb3, 0x21	@ PACKER: left in the buffer by Tileset_Set26TilesA+0x2779
	.global Tileset_Set26TilesC
Tileset_Set26TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_26/tiles_c.4bpp.mtf"
	.byte 0xc9, 0xe1	@ PACKER: left in the buffer by Tileset_Set26TilesB+0xfea
