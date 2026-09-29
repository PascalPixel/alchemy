@ Field_Map110: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map110
Field_Map110:
	map_header 0, 0, 100, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map110, .LField_Map110_METATILES
	map_block Field_Map110, .LField_Map110_SHAPES
	map_block Field_Map110, .LField_Map110_GRID
	map_block Field_Map110, .LField_Map110_SHAPE_GRID
	map_block Field_Map110, .LField_Map110_ANIMATION
	map_block Field_Map110, .LField_Map110_BLEND
	map_block Field_Map110, .LField_Map110_SCENE
.LField_Map110_METATILES:
	.incbin "MAP/MAP_110/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map110_SHAPES:
	.incbin "MAP/MAP_107/SHAPES.table.lz"
	.balign 4, 0
.LField_Map110_GRID:
	.incbin "MAP/MAP_110/GRID.parts.lz"
	.balign 4, 0
.LField_Map110_SHAPE_GRID:
	.incbin "MAP/MAP_110/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map110_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map110_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map110_SCENE:
	.incbin "MAP/MAP_110/SCENE.parts.lz"
