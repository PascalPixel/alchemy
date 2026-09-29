@ Field_Map205: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map205
Field_Map205:
	map_header 0, 0, 52, 54, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 52, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 54, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map205, .LField_Map205_METATILES
	map_block Field_Map205, .LField_Map205_SHAPES
	map_block Field_Map205, .LField_Map205_GRID
	map_block Field_Map205, .LField_Map205_SHAPE_GRID
	map_block Field_Map205, .LField_Map205_ANIMATION
	map_block Field_Map205, .LField_Map205_BLEND
	map_block Field_Map205, .LField_Map205_SCENE
.LField_Map205_METATILES:
	.incbin "MAP/MAP_205/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map205_SHAPES:
	.incbin "MAP/MAP_205/SHAPES.table.lz"
	.balign 4, 0
.LField_Map205_GRID:
	.incbin "MAP/MAP_205/GRID.parts.lz"
	.balign 4, 0
.LField_Map205_SHAPE_GRID:
	.incbin "MAP/MAP_205/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map205_ANIMATION:
	.incbin "MAP/MAP_205/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map205_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map205_SCENE:
	.incbin "MAP/MAP_205/SCENE.parts.lz"
