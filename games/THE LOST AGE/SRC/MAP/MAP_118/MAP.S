@ Field_Map118: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map118
Field_Map118:
	map_header 0, 0, 90, 100, 3, 3, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map118, .LField_Map118_METATILES
	map_block Field_Map118, .LField_Map118_SHAPES
	map_block Field_Map118, .LField_Map118_GRID
	map_block Field_Map118, .LField_Map118_SHAPE_GRID
	map_block Field_Map118, .LField_Map118_ANIMATION
	map_block Field_Map118, .LField_Map118_BLEND
	map_block Field_Map118, .LField_Map118_SCENE
.LField_Map118_METATILES:
	.incbin "MAP/MAP_118/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map118_SHAPES:
	.incbin "MAP/MAP_118/SHAPES.table.lz"
	.balign 4, 0
.LField_Map118_GRID:
	.incbin "MAP/MAP_118/GRID.parts.lz"
	.balign 4, 0
.LField_Map118_SHAPE_GRID:
	.incbin "MAP/MAP_118/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map118_ANIMATION:
	.incbin "MAP/MAP_021/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map118_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map118_SCENE:
	.incbin "MAP/MAP_118/SCENE.parts.lz"
