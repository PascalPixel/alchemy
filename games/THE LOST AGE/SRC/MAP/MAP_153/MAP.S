@ Field_Map153: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map153
Field_Map153:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map153, .LField_Map153_METATILES
	map_block Field_Map153, .LField_Map153_SHAPES
	map_block Field_Map153, .LField_Map153_GRID
	map_block Field_Map153, .LField_Map153_SHAPE_GRID
	map_block Field_Map153, .LField_Map153_ANIMATION
	map_block Field_Map153, .LField_Map153_BLEND
	map_block Field_Map153, .LField_Map153_SCENE
.LField_Map153_METATILES:
	.incbin "MAP/MAP_153/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map153_SHAPES:
	.incbin "MAP/MAP_153/SHAPES.table.lz"
	.balign 4, 0
.LField_Map153_GRID:
	.incbin "MAP/MAP_153/GRID.parts.lz"
	.balign 4, 0
.LField_Map153_SHAPE_GRID:
	.incbin "MAP/MAP_153/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map153_ANIMATION:
	.incbin "MAP/MAP_153/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map153_BLEND:
	.incbin "MAP/MAP_153/BLEND.script.lz"
	.balign 4, 0
.LField_Map153_SCENE:
	.incbin "MAP/MAP_153/SCENE.parts.lz"
