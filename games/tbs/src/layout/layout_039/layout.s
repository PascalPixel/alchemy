	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout039
Field_Layout039:
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout039, .LField_Layout039_METATILES
	layout_offset Field_Layout039, .LField_Layout039_SHAPES
	layout_offset Field_Layout039, .LField_Layout039_GRID
	layout_offset Field_Layout039, .LField_Layout039_ANIMATION
	layout_offset Field_Layout039, .LField_Layout039_BLEND
	layout_offset Field_Layout039, .LField_Layout039_MARKERS
.LField_Layout039_METATILES:
	.incbin "games/tbs/src/layout/layout_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout039_SHAPES:
	.incbin "games/tbs/src/layout/layout_039/shapes.table.lz"
	.balign 4, 0
.LField_Layout039_GRID:
	.incbin "games/tbs/src/layout/layout_039/map_es.grid.lz"
	.balign 4, 0
.LField_Layout039_ANIMATION:
	.incbin "games/tbs/src/layout/layout_039/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout039_BLEND:
	.incbin "games/tbs/src/layout/layout_039/blend.script.lz"
	.balign 4, 0
.LField_Layout039_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout039, .LField_Layout039_METATILES
	layout_offset Field_Layout039, .LField_Layout039_SHAPES
	layout_offset Field_Layout039, .LField_Layout039_GRID
	layout_offset Field_Layout039, .LField_Layout039_ANIMATION
	layout_offset Field_Layout039, .LField_Layout039_BLEND
	layout_offset Field_Layout039, .LField_Layout039_MARKERS
.LField_Layout039_METATILES:
	.incbin "games/tbs/src/layout/layout_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout039_SHAPES:
	.incbin "games/tbs/src/layout/layout_039/shapes.table.lz"
	.balign 4, 0
.LField_Layout039_GRID:
	.incbin "games/tbs/src/layout/layout_039/map_es.grid.lz"
	.balign 4, 0
.LField_Layout039_ANIMATION:
	.incbin "games/tbs/src/layout/layout_039/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout039_BLEND:
	.incbin "games/tbs/src/layout/layout_039/blend.script.lz"
	.balign 4, 0
.LField_Layout039_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout039, .LField_Layout039_METATILES
	layout_offset Field_Layout039, .LField_Layout039_SHAPES
	layout_offset Field_Layout039, .LField_Layout039_GRID
	layout_offset Field_Layout039, .LField_Layout039_ANIMATION
	layout_offset Field_Layout039, .LField_Layout039_BLEND
	layout_offset Field_Layout039, .LField_Layout039_MARKERS
.LField_Layout039_METATILES:
	.incbin "games/tbs/src/layout/layout_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout039_SHAPES:
	.incbin "games/tbs/src/layout/layout_039/shapes.table.lz"
	.balign 4, 0
.LField_Layout039_GRID:
	.incbin "games/tbs/src/layout/layout_039/map_es.grid.lz"
	.balign 4, 0
.LField_Layout039_ANIMATION:
	.incbin "games/tbs/src/layout/layout_039/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout039_BLEND:
	.incbin "games/tbs/src/layout/layout_039/blend.script.lz"
	.balign 4, 0
.LField_Layout039_MARKERS:
	treasure_spots_end
	.else
	layout_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout039, .LField_Layout039_METATILES
	layout_offset Field_Layout039, .LField_Layout039_SHAPES
	layout_offset Field_Layout039, .LField_Layout039_GRID
	layout_offset Field_Layout039, .LField_Layout039_ANIMATION
	layout_offset Field_Layout039, .LField_Layout039_BLEND
	layout_offset Field_Layout039, .LField_Layout039_MARKERS
.LField_Layout039_METATILES:
	.incbin "games/tbs/src/layout/layout_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout039_SHAPES:
	.incbin "games/tbs/src/layout/layout_039/shapes.table.lz"
	.balign 4, 0
.LField_Layout039_GRID:
	.incbin "games/tbs/src/layout/layout_039/map.grid.lz"
	.balign 4, 0
.LField_Layout039_ANIMATION:
	.incbin "games/tbs/src/layout/layout_039/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout039_BLEND:
	.incbin "games/tbs/src/layout/layout_039/blend.script.lz"
	.balign 4, 0
.LField_Layout039_MARKERS:
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Set31Palette
Tileset_Set31Palette:
	.incbin "games/tbs/src/graphics/tileset/set_31/palette.gbapal.lz"
	.byte 0x9e, 0x99, 0xb7	@ PACKER: left in the buffer by Field_Layout039+0x17d
	.global Tileset_Set31TilesA
Tileset_Set31TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_31/tiles_a.4bpp.mtf"
	.global Tileset_Set31TilesB
Tileset_Set31TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_31/tiles_b.4bpp.mtf"
	.byte 0x4d, 0x0c, 0x83	@ PACKER: left in the buffer by Tileset_Set31TilesA+0x1d6d
	.global Tileset_Set31TilesC
Tileset_Set31TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_31/tiles_c.4bpp.mtf"
	.byte 0xfa, 0x80	@ PACKER: left in the buffer by Tileset_Set31TilesB+0x1042
	.global Tileset_Set31TilesD
Tileset_Set31TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_31/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_Set31TilesC+0xe22
