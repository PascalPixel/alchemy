@ Field_Map114: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map114
Field_Map114:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map114, .LField_Map114_METATILES
	map_block Field_Map114, .LField_Map114_SHAPES
	map_block Field_Map114, .LField_Map114_GRID
	map_block Field_Map114, .LField_Map114_SHAPE_GRID
	map_block Field_Map114, .LField_Map114_ANIMATION
	map_block Field_Map114, .LField_Map114_BLEND
	map_block Field_Map114, .LField_Map114_SCENE
.LField_Map114_METATILES:
	.incbin "MAP/MAP_114/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map114_SHAPES:
	.incbin "MAP/MAP_114/SHAPES.table.lz"
	.balign 4, 0
.LField_Map114_GRID:
	.incbin "MAP/MAP_114/GRID.parts.lz"
	.balign 4, 0
.LField_Map114_SHAPE_GRID:
	.incbin "MAP/MAP_114/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map114_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map114_BLEND:
	.incbin "MAP/MAP_114/BLEND.script.lz"
	.balign 4, 0
.LField_Map114_SCENE:
	.incbin "MAP/MAP_114/SCENE.parts.lz"
