@ Field_Map015: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map015
Field_Map015:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map015, .LField_Map015_METATILES
	map_block Field_Map015, .LField_Map015_SHAPES
	map_block Field_Map015, .LField_Map015_GRID
	map_block Field_Map015, .LField_Map015_SHAPE_GRID
	map_block Field_Map015, .LField_Map015_ANIMATION
	map_block Field_Map015, .LField_Map015_BLEND
	map_block Field_Map015, .LField_Map015_SCENE
.LField_Map015_METATILES:
	.incbin "MAP/MAP_015/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map015_SHAPES:
	.incbin "MAP/MAP_015/SHAPES.table.lz"
	.balign 4, 0
.LField_Map015_GRID:
	.incbin "MAP/MAP_015/GRID.parts.lz"
	.balign 4, 0
.LField_Map015_SHAPE_GRID:
	.incbin "MAP/MAP_015/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map015_ANIMATION:
	.incbin "MAP/MAP_015/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map015_BLEND:
	.incbin "MAP/MAP_015/BLEND.script.lz"
	.balign 4, 0
.LField_Map015_SCENE:
	.incbin "MAP/MAP_015/SCENE.parts.lz"
