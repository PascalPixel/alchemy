@ Field_Map049: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map049
Field_Map049:
	map_header 0, 0, 74, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 74, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map049, .LField_Map049_METATILES
	map_block Field_Map049, .LField_Map049_SHAPES
	map_block Field_Map049, .LField_Map049_GRID
	map_block Field_Map049, .LField_Map049_SHAPE_GRID
	map_block Field_Map049, .LField_Map049_ANIMATION
	map_block Field_Map049, .LField_Map049_BLEND
	map_block Field_Map049, .LField_Map049_SCENE
.LField_Map049_METATILES:
	.incbin "MAP/MAP_049/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map049_SHAPES:
	.incbin "MAP/MAP_049/SHAPES.table.lz"
	.balign 4, 0
.LField_Map049_GRID:
	.incbin "MAP/MAP_049/GRID.parts.lz"
	.balign 4, 0
.LField_Map049_SHAPE_GRID:
	.incbin "MAP/MAP_049/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map049_ANIMATION:
	.incbin "MAP/MAP_049/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map049_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map049_SCENE:
	.incbin "MAP/MAP_049/SCENE.parts.lz"
