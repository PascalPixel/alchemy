@ Field_Map251: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map251
Field_Map251:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map251, .LField_Map251_METATILES
	map_block Field_Map251, .LField_Map251_SHAPES
	map_block Field_Map251, .LField_Map251_GRID
	map_block Field_Map251, .LField_Map251_SHAPE_GRID
	map_block Field_Map251, .LField_Map251_ANIMATION
	map_block Field_Map251, .LField_Map251_BLEND
	map_block Field_Map251, .LField_Map251_SCENE
.LField_Map251_METATILES:
	.incbin "MAP/MAP_251/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map251_SHAPES:
	.incbin "MAP/MAP_251/SHAPES.table.lz"
	.balign 4, 0
.LField_Map251_GRID:
	.incbin "MAP/MAP_251/GRID.parts.lz"
	.balign 4, 0
.LField_Map251_SHAPE_GRID:
	.incbin "MAP/MAP_251/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map251_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map251_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map251_SCENE:
	.incbin "MAP/MAP_251/SCENE.parts.lz"
