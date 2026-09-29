@ Field_Map278: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map278
Field_Map278:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map278, .LField_Map278_METATILES
	map_block Field_Map278, .LField_Map278_SHAPES
	map_block Field_Map278, .LField_Map278_GRID
	map_block Field_Map278, .LField_Map278_SHAPE_GRID
	map_block Field_Map278, .LField_Map278_ANIMATION
	map_block Field_Map278, .LField_Map278_BLEND
	map_block Field_Map278, .LField_Map278_SCENE
.LField_Map278_METATILES:
	.incbin "MAP/MAP_278/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map278_SHAPES:
	.incbin "MAP/MAP_278/SHAPES.table.lz"
	.balign 4, 0
.LField_Map278_GRID:
	.incbin "MAP/MAP_278/GRID.parts.lz"
	.balign 4, 0
.LField_Map278_SHAPE_GRID:
	.incbin "MAP/MAP_278/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map278_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map278_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map278_SCENE:
	.incbin "MAP/MAP_278/SCENE.parts.lz"
