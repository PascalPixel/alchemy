@ Field_Map289: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map289
Field_Map289:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map289, .LField_Map289_METATILES
	map_block Field_Map289, .LField_Map289_SHAPES
	map_block Field_Map289, .LField_Map289_GRID
	map_block Field_Map289, .LField_Map289_SHAPE_GRID
	map_block Field_Map289, .LField_Map289_ANIMATION
	map_block Field_Map289, .LField_Map289_BLEND
	map_block Field_Map289, .LField_Map289_SCENE
.LField_Map289_METATILES:
	.incbin "MAP/MAP_289/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map289_SHAPES:
	.incbin "MAP/MAP_179/SHAPES.table.lz"
	.balign 4, 0
.LField_Map289_GRID:
	.incbin "MAP/MAP_289/GRID.parts.lz"
	.balign 4, 0
.LField_Map289_SHAPE_GRID:
	.incbin "MAP/MAP_289/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map289_ANIMATION:
	.incbin "MAP/MAP_289/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map289_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map289_SCENE:
	.incbin "MAP/MAP_236/SCENE.parts.lz"
