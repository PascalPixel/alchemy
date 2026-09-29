@ Field_Map181: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map181
Field_Map181:
	map_header 0, 0, 96, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map181, .LField_Map181_METATILES
	map_block Field_Map181, .LField_Map181_SHAPES
	map_block Field_Map181, .LField_Map181_GRID
	map_block Field_Map181, .LField_Map181_SHAPE_GRID
	map_block Field_Map181, .LField_Map181_ANIMATION
	map_block Field_Map181, .LField_Map181_BLEND
	map_block Field_Map181, .LField_Map181_SCENE
.LField_Map181_METATILES:
	.incbin "MAP/MAP_181/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map181_SHAPES:
	.incbin "MAP/MAP_181/SHAPES.table.lz"
	.balign 4, 0
.LField_Map181_GRID:
	.incbin "MAP/MAP_181/GRID.parts.lz"
	.balign 4, 0
.LField_Map181_SHAPE_GRID:
	.incbin "MAP/MAP_181/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map181_ANIMATION:
	.incbin "MAP/MAP_181/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map181_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map181_SCENE:
	.incbin "MAP/MAP_181/SCENE.parts.lz"
