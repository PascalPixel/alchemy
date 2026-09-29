@ Field_Map272: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map272
Field_Map272:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map272, .LField_Map272_METATILES
	map_block Field_Map272, .LField_Map272_SHAPES
	map_block Field_Map272, .LField_Map272_GRID
	map_block Field_Map272, .LField_Map272_SHAPE_GRID
	map_block Field_Map272, .LField_Map272_ANIMATION
	map_block Field_Map272, .LField_Map272_BLEND
	map_block Field_Map272, .LField_Map272_SCENE
.LField_Map272_METATILES:
	.incbin "MAP/MAP_272/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map272_SHAPES:
	.incbin "MAP/MAP_272/SHAPES.table.lz"
	.balign 4, 0
.LField_Map272_GRID:
	.incbin "MAP/MAP_272/GRID.parts.lz"
	.balign 4, 0
.LField_Map272_SHAPE_GRID:
	.incbin "MAP/MAP_272/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map272_ANIMATION:
	.incbin "MAP/MAP_272/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map272_BLEND:
	.incbin "MAP/MAP_272/BLEND.script.lz"
	.balign 4, 0
.LField_Map272_SCENE:
	.incbin "MAP/MAP_272/SCENE.parts.lz"
