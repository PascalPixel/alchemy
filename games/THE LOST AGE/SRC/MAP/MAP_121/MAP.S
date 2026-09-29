@ Field_Map121: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map121
Field_Map121:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map121, .LField_Map121_METATILES
	map_block Field_Map121, .LField_Map121_SHAPES
	map_block Field_Map121, .LField_Map121_GRID
	map_block Field_Map121, .LField_Map121_SHAPE_GRID
	map_block Field_Map121, .LField_Map121_ANIMATION
	map_block Field_Map121, .LField_Map121_BLEND
	map_block Field_Map121, .LField_Map121_SCENE
.LField_Map121_METATILES:
	.incbin "MAP/MAP_121/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map121_SHAPES:
	.incbin "MAP/MAP_121/SHAPES.table.lz"
	.balign 4, 0
.LField_Map121_GRID:
	.incbin "MAP/MAP_121/GRID.parts.lz"
	.balign 4, 0
.LField_Map121_SHAPE_GRID:
	.incbin "MAP/MAP_121/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map121_ANIMATION:
	.incbin "MAP/MAP_021/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map121_BLEND:
	.incbin "MAP/MAP_021/BLEND.script.lz"
	.balign 4, 0
.LField_Map121_SCENE:
	.incbin "MAP/MAP_121/SCENE.parts.lz"
