@ Field_Map211: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map211
Field_Map211:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map211, .LField_Map211_METATILES
	map_block Field_Map211, .LField_Map211_SHAPES
	map_block Field_Map211, .LField_Map211_GRID
	map_block Field_Map211, .LField_Map211_SHAPE_GRID
	map_block Field_Map211, .LField_Map211_ANIMATION
	map_block Field_Map211, .LField_Map211_BLEND
	map_block Field_Map211, .LField_Map211_SCENE
.LField_Map211_METATILES:
	.incbin "MAP/MAP_211/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map211_SHAPES:
	.incbin "MAP/MAP_211/SHAPES.table.lz"
	.balign 4, 0
.LField_Map211_GRID:
	.incbin "MAP/MAP_211/GRID.parts.lz"
	.balign 4, 0
.LField_Map211_SHAPE_GRID:
	.incbin "MAP/MAP_211/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map211_ANIMATION:
	.incbin "MAP/MAP_211/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map211_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map211_SCENE:
	.incbin "MAP/MAP_211/SCENE.parts.lz"
