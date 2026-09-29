@ Field_Map268: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map268
Field_Map268:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map268, .LField_Map268_METATILES
	map_block Field_Map268, .LField_Map268_SHAPES
	map_block Field_Map268, .LField_Map268_GRID
	map_block Field_Map268, .LField_Map268_SHAPE_GRID
	map_block Field_Map268, .LField_Map268_ANIMATION
	map_block Field_Map268, .LField_Map268_BLEND
	map_block Field_Map268, .LField_Map268_SCENE
.LField_Map268_METATILES:
	.incbin "MAP/MAP_268/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map268_SHAPES:
	.incbin "MAP/MAP_268/SHAPES.table.lz"
	.balign 4, 0
.LField_Map268_GRID:
	.incbin "MAP/MAP_268/GRID.parts.lz"
	.balign 4, 0
.LField_Map268_SHAPE_GRID:
	.incbin "MAP/MAP_268/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map268_ANIMATION:
	.incbin "MAP/MAP_268/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map268_BLEND:
	.incbin "MAP/MAP_268/BLEND.script.lz"
	.balign 4, 0
.LField_Map268_SCENE:
	.incbin "MAP/MAP_268/SCENE.parts.lz"
