	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map143
Field_Map143:
	map_header 0, 0, 72, 72, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map143, .LField_Map143_METATILES
	map_block Field_Map143, .LField_Map143_SHAPES
	map_block Field_Map143, .LField_Map143_GRID
	map_block Field_Map143, .LField_Map143_ANIMATION
	map_block Field_Map143, .LField_Map143_BLEND
	map_block Field_Map143, .LField_Map143_MARKERS
.LField_Map143_METATILES:
	.incbin "games/tbs/src/map/map_143/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map143_SHAPES:
	.incbin "games/tbs/src/map/map_143/shapes.table.lz"
	.balign 4, 0
.LField_Map143_GRID:
	.incbin "games/tbs/src/map/map_143/grid.parts.lz"
	.balign 4, 0
.LField_Map143_ANIMATION:
	.incbin "games/tbs/src/map/map_028/animation.script.lz"
	.balign 4, 0
.LField_Map143_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map143_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette37
Tileset_Palette37:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_37.gbapal.lz"
