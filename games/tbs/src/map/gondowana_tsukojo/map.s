	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_GondowanaTsukojo
Field_GondowanaTsukojo:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_METATILES
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_SHAPES
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_GRID
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_ANIMATION
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_BLEND
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_MARKERS
.LField_GondowanaTsukojo_METATILES:
	.incbin "games/tbs/src/map/gondowana_tsukojo/metatiles.delta1.lz"
	.balign 4, 0
.LField_GondowanaTsukojo_SHAPES:
	.incbin "games/tbs/src/map/gondowana_tsukojo/shapes.table.lz"
	.balign 4, 0
.LField_GondowanaTsukojo_GRID:
	.incbin "games/tbs/src/map/gondowana_tsukojo/grid.parts.lz"
	.balign 4, 0
.LField_GondowanaTsukojo_ANIMATION:
	.incbin "games/tbs/src/map/gondowana_tsukojo/animation.script.lz"
	.balign 4, 0
.LField_GondowanaTsukojo_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_GondowanaTsukojo_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette33
Tileset_Palette33:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_33.gbapal.lz"
