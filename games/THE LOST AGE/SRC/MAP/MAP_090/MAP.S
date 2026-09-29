@ Field_Map090: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map090
Field_Map090:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map090, .LField_Map090_METATILES
	map_block Field_Map090, .LField_Map090_SHAPES
	map_block Field_Map090, .LField_Map090_GRID
	map_block Field_Map090, .LField_Map090_SHAPE_GRID
	map_block Field_Map090, .LField_Map090_ANIMATION
	map_block Field_Map090, .LField_Map090_BLEND
	map_block Field_Map090, .LField_Map090_SCENE
.LField_Map090_METATILES:
	.incbin "MAP/MAP_090/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map090_SHAPES:
	.incbin "MAP/MAP_090/SHAPES.table.lz"
	.balign 4, 0
.LField_Map090_GRID:
	.incbin "MAP/MAP_090/GRID.parts.lz"
	.balign 4, 0
.LField_Map090_SHAPE_GRID:
	.incbin "MAP/MAP_090/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map090_ANIMATION:
	.incbin "MAP/MAP_090/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map090_BLEND:
	.incbin "MAP/MAP_010/BLEND.script.lz"
	.balign 4, 0
.LField_Map090_SCENE:
	.incbin "MAP/MAP_090/SCENE.parts.lz"
