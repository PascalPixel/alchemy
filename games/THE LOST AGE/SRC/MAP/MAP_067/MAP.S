@ Field_Map067: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map067
Field_Map067:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map067, .LField_Map067_METATILES
	map_block Field_Map067, .LField_Map067_SHAPES
	map_block Field_Map067, .LField_Map067_GRID
	map_block Field_Map067, .LField_Map067_SHAPE_GRID
	map_block Field_Map067, .LField_Map067_ANIMATION
	map_block Field_Map067, .LField_Map067_BLEND
	map_block Field_Map067, .LField_Map067_SCENE
.LField_Map067_METATILES:
	.incbin "MAP/MAP_067/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map067_SHAPES:
	.incbin "MAP/MAP_067/SHAPES.table.lz"
	.balign 4, 0
.LField_Map067_GRID:
	.incbin "MAP/MAP_067/GRID.parts.lz"
	.balign 4, 0
.LField_Map067_SHAPE_GRID:
	.incbin "MAP/MAP_067/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map067_ANIMATION:
	.incbin "MAP/MAP_067/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map067_BLEND:
	.incbin "MAP/MAP_060/BLEND.script.lz"
	.balign 4, 0
.LField_Map067_SCENE:
	.incbin "MAP/MAP_067/SCENE.parts.lz"
