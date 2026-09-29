@ Field_Map050: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map050
Field_Map050:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map050, .LField_Map050_METATILES
	map_block Field_Map050, .LField_Map050_SHAPES
	map_block Field_Map050, .LField_Map050_GRID
	map_block Field_Map050, .LField_Map050_SHAPE_GRID
	map_block Field_Map050, .LField_Map050_ANIMATION
	map_block Field_Map050, .LField_Map050_BLEND
	map_block Field_Map050, .LField_Map050_SCENE
.LField_Map050_METATILES:
	.incbin "MAP/MAP_050/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map050_SHAPES:
	.incbin "MAP/MAP_050/SHAPES.table.lz"
	.balign 4, 0
.LField_Map050_GRID:
	.incbin "MAP/MAP_050/GRID.parts.lz"
	.balign 4, 0
.LField_Map050_SHAPE_GRID:
	.incbin "MAP/MAP_050/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map050_ANIMATION:
	.incbin "MAP/MAP_050/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map050_BLEND:
	.incbin "MAP/MAP_050/BLEND.script.lz"
	.balign 4, 0
.LField_Map050_SCENE:
	.incbin "MAP/MAP_050/SCENE.parts.lz"
