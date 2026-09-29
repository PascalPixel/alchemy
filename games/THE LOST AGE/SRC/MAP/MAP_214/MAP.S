@ Field_Map214: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map214
Field_Map214:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map214, .LField_Map214_METATILES
	map_block Field_Map214, .LField_Map214_SHAPES
	map_block Field_Map214, .LField_Map214_GRID
	map_block Field_Map214, .LField_Map214_SHAPE_GRID
	map_block Field_Map214, .LField_Map214_ANIMATION
	map_block Field_Map214, .LField_Map214_BLEND
	map_block Field_Map214, .LField_Map214_SCENE
.LField_Map214_METATILES:
	.incbin "MAP/MAP_214/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map214_SHAPES:
	.incbin "MAP/MAP_214/SHAPES.table.lz"
	.balign 4, 0
.LField_Map214_GRID:
	.incbin "MAP/MAP_214/GRID.parts.lz"
	.balign 4, 0
.LField_Map214_SHAPE_GRID:
	.incbin "MAP/MAP_214/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map214_ANIMATION:
	.incbin "MAP/MAP_211/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map214_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map214_SCENE:
	.incbin "MAP/MAP_214/SCENE.parts.lz"
