@ Field_Map218: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map218
Field_Map218:
	map_header 0, 0, 104, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 104, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map218, .LField_Map218_METATILES
	map_block Field_Map218, .LField_Map218_SHAPES
	map_block Field_Map218, .LField_Map218_GRID
	map_block Field_Map218, .LField_Map218_SHAPE_GRID
	map_block Field_Map218, .LField_Map218_ANIMATION
	map_block Field_Map218, .LField_Map218_BLEND
	map_block Field_Map218, .LField_Map218_SCENE
.LField_Map218_METATILES:
	.incbin "MAP/MAP_218/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map218_SHAPES:
	.incbin "MAP/MAP_218/SHAPES.table.lz"
	.balign 4, 0
.LField_Map218_GRID:
	.incbin "MAP/MAP_218/GRID.parts.lz"
	.balign 4, 0
.LField_Map218_SHAPE_GRID:
	.incbin "MAP/MAP_218/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map218_ANIMATION:
	.incbin "MAP/MAP_218/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map218_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map218_SCENE:
	.incbin "MAP/MAP_218/SCENE.parts.lz"
