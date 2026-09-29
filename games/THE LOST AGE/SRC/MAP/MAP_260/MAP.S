@ Field_Map260: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map260
Field_Map260:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map260, .LField_Map260_METATILES
	map_block Field_Map260, .LField_Map260_SHAPES
	map_block Field_Map260, .LField_Map260_GRID
	map_block Field_Map260, .LField_Map260_SHAPE_GRID
	map_block Field_Map260, .LField_Map260_ANIMATION
	map_block Field_Map260, .LField_Map260_BLEND
	map_block Field_Map260, .LField_Map260_SCENE
.LField_Map260_METATILES:
	.incbin "MAP/MAP_260/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map260_SHAPES:
	.incbin "MAP/MAP_260/SHAPES.table.lz"
	.balign 4, 0
.LField_Map260_GRID:
	.incbin "MAP/MAP_260/GRID.parts.lz"
	.balign 4, 0
.LField_Map260_SHAPE_GRID:
	.incbin "MAP/MAP_260/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map260_ANIMATION:
	.incbin "MAP/MAP_259/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map260_BLEND:
	.incbin "MAP/MAP_260/BLEND.script.lz"
	.balign 4, 0
.LField_Map260_SCENE:
	.incbin "MAP/MAP_260/SCENE.parts.lz"
