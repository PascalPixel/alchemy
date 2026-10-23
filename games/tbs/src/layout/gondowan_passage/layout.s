	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_GondowanPassage
Field_GondowanPassage:
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_GondowanPassage, .LField_GondowanPassage_METATILES
	layout_offset Field_GondowanPassage, .LField_GondowanPassage_SHAPES
	layout_offset Field_GondowanPassage, .LField_GondowanPassage_GRID
	layout_offset Field_GondowanPassage, .LField_GondowanPassage_ANIMATION
	layout_offset Field_GondowanPassage, .LField_GondowanPassage_BLEND
	layout_offset Field_GondowanPassage, .LField_GondowanPassage_MARKERS
.LField_GondowanPassage_METATILES:
	.incbin "games/tbs/src/layout/gondowan_passage/metatiles.delta1.lz"
	.balign 4, 0
.LField_GondowanPassage_SHAPES:
	.incbin "games/tbs/src/layout/gondowan_passage/shapes.table.lz"
	.balign 4, 0
.LField_GondowanPassage_GRID:
	.incbin "games/tbs/src/layout/gondowan_passage/map.grid.lz"
	.balign 4, 0
.LField_GondowanPassage_ANIMATION:
	.incbin "games/tbs/src/layout/gondowan_passage/tileset_anims.script.lz"
	.balign 4, 0
.LField_GondowanPassage_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_GondowanPassage_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette33
Tileset_Palette33:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_33.gbapal.lz"
