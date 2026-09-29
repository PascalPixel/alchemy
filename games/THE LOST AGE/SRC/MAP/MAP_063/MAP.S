@ Field_Map063: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map063
Field_Map063:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map063, .LField_Map063_METATILES
	map_block Field_Map063, .LField_Map063_SHAPES
	map_block Field_Map063, .LField_Map063_GRID
	map_block Field_Map063, .LField_Map063_SHAPE_GRID
	map_block Field_Map063, .LField_Map063_ANIMATION
	map_block Field_Map063, .LField_Map063_BLEND
	map_block Field_Map063, .LField_Map063_SCENE
.LField_Map063_METATILES:
	.incbin "MAP/MAP_063/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map063_SHAPES:
	.incbin "MAP/MAP_063/SHAPES.table.lz"
	.balign 4, 0
.LField_Map063_GRID:
	.incbin "MAP/MAP_063/GRID.parts.lz"
	.balign 4, 0
.LField_Map063_SHAPE_GRID:
	.incbin "MAP/MAP_063/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map063_ANIMATION:
	.incbin "MAP/MAP_063/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map063_BLEND:
	.incbin "MAP/MAP_063/BLEND.script.lz"
	.balign 4, 0
.LField_Map063_SCENE:
	.incbin "MAP/MAP_063/SCENE.parts.lz"
