@ Field_Map094: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map094
Field_Map094:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map094, .LField_Map094_METATILES
	map_block Field_Map094, .LField_Map094_SHAPES
	map_block Field_Map094, .LField_Map094_GRID
	map_block Field_Map094, .LField_Map094_SHAPE_GRID
	map_block Field_Map094, .LField_Map094_ANIMATION
	map_block Field_Map094
	map_block Field_Map094, .LField_Map094_SCENE
.LField_Map094_METATILES:
	.incbin "MAP/MAP_094/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map094_SHAPES:
	.incbin "MAP/MAP_094/SHAPES.table.lz"
	.balign 4, 0
.LField_Map094_GRID:
	.incbin "MAP/MAP_094/GRID.parts.lz"
	.balign 4, 0
.LField_Map094_SHAPE_GRID:
	.incbin "MAP/MAP_094/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map094_ANIMATION:
	.incbin "MAP/MAP_021/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map094_SCENE:
	.incbin "MAP/MAP_094/SCENE.parts.lz"
