	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_GondowanPassage
Field_GondowanPassage:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GondowanPassage, .LField_GondowanPassage_METATILES
	map_block Field_GondowanPassage, .LField_GondowanPassage_SHAPES
	map_block Field_GondowanPassage, .LField_GondowanPassage_GRID
	map_block Field_GondowanPassage, .LField_GondowanPassage_ANIMATION
	map_block Field_GondowanPassage, .LField_GondowanPassage_BLEND
	map_block Field_GondowanPassage, .LField_GondowanPassage_MARKERS
.LField_GondowanPassage_METATILES:
	.incbin "games/tbs/src/layout/gondowan_passage/metatiles.delta1.lz"
	.balign 4, 0
.LField_GondowanPassage_SHAPES:
	.incbin "games/tbs/src/layout/gondowan_passage/shapes.table.lz"
	.balign 4, 0
.LField_GondowanPassage_GRID:
	.incbin "games/tbs/src/layout/gondowan_passage/grid.parts.lz"
	.balign 4, 0
.LField_GondowanPassage_ANIMATION:
	.incbin "games/tbs/src/layout/gondowan_passage/animation.script.lz"
	.balign 4, 0
.LField_GondowanPassage_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_GondowanPassage_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette33
Tileset_Palette33:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_33.gbapal.lz"
