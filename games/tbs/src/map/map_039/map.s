	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map039
Field_Map039:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map039, .LField_Map039_METATILES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_MARKERS
.LField_Map039_METATILES:
	.incbin "map/map_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "map/map_039/shapes.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "map/map_039/grid_es.parts.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map039_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map039, .LField_Map039_METATILES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_MARKERS
.LField_Map039_METATILES:
	.incbin "map/map_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "map/map_039/shapes.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "map/map_039/grid_es.parts.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map039_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map039, .LField_Map039_METATILES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_MARKERS
.LField_Map039_METATILES:
	.incbin "map/map_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "map/map_039/shapes.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "map/map_039/grid_es.parts.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map039_MARKERS:
	.incbin "map/map_000/markers.table"
	.else
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map039, .LField_Map039_METATILES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_MARKERS
.LField_Map039_METATILES:
	.incbin "map/map_039/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "map/map_039/shapes.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "map/map_039/grid.parts.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map039_MARKERS:
	.incbin "map/map_000/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Set31Palette
Tileset_Set31Palette:
	.incbin "graphics/tileset/set_31/palette.gbapal.lz"
	.byte 0x9e, 0x99, 0xb7	@ PACKER: left in the buffer by Field_Map039+0x17d
	.global Tileset_Set31TilesA
Tileset_Set31TilesA:
	.incbin "graphics/tileset/set_31/tiles_a.4bpp.mtf"
	.global Tileset_Set31TilesB
Tileset_Set31TilesB:
	.incbin "graphics/tileset/set_31/tiles_b.4bpp.mtf"
	.byte 0x4d, 0x0c, 0x83	@ PACKER: left in the buffer by Tileset_Set31TilesA+0x1d6d
	.global Tileset_Set31TilesC
Tileset_Set31TilesC:
	.incbin "graphics/tileset/set_31/tiles_c.4bpp.mtf"
	.byte 0xfa, 0x80	@ PACKER: left in the buffer by Tileset_Set31TilesB+0x1042
	.global Tileset_Set31TilesD
Tileset_Set31TilesD:
	.incbin "graphics/tileset/set_31/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_Set31TilesC+0xe22
