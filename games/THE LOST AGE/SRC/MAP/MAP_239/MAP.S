@ Field_Map239: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map239
Field_Map239:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map239, .LField_Map239_METATILES
	map_block Field_Map239, .LField_Map239_SHAPES
	map_block Field_Map239, .LField_Map239_GRID
	map_block Field_Map239, .LField_Map239_SHAPE_GRID
	map_block Field_Map239, .LField_Map239_ANIMATION
	map_block Field_Map239, .LField_Map239_BLEND
	map_block Field_Map239, .LField_Map239_SCENE
.LField_Map239_METATILES:
	.incbin "MAP/MAP_239/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map239_SHAPES:
	.incbin "MAP/MAP_239/SHAPES.table.lz"
	.balign 4, 0
.LField_Map239_GRID:
	.incbin "MAP/MAP_239/GRID.parts.lz"
	.balign 4, 0
.LField_Map239_SHAPE_GRID:
	.incbin "MAP/MAP_239/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map239_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map239_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map239_SCENE:
	.incbin "MAP/MAP_239/SCENE.parts.lz"
