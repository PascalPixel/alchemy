@ Field_Map184: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map184
Field_Map184:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map184, .LField_Map184_METATILES
	map_block Field_Map184, .LField_Map184_SHAPES
	map_block Field_Map184, .LField_Map184_GRID
	map_block Field_Map184, .LField_Map184_SHAPE_GRID
	map_block Field_Map184, .LField_Map184_ANIMATION
	map_block Field_Map184, .LField_Map184_BLEND
	map_block Field_Map184, .LField_Map184_SCENE
.LField_Map184_METATILES:
	.incbin "MAP/MAP_184/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map184_SHAPES:
	.incbin "MAP/MAP_184/SHAPES.table.lz"
	.balign 4, 0
.LField_Map184_GRID:
	.incbin "MAP/MAP_184/GRID.parts.lz"
	.balign 4, 0
.LField_Map184_SHAPE_GRID:
	.incbin "MAP/MAP_184/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map184_ANIMATION:
	.incbin "MAP/MAP_182/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map184_BLEND:
	.incbin "MAP/MAP_030/BLEND.script.lz"
	.balign 4, 0
.LField_Map184_SCENE:
	.incbin "MAP/MAP_184/SCENE.parts.lz"
