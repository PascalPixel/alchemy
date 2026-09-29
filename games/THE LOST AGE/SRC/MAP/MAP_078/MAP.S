@ Field_Map078: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map078
Field_Map078:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map078, .LField_Map078_METATILES
	map_block Field_Map078, .LField_Map078_SHAPES
	map_block Field_Map078, .LField_Map078_GRID
	map_block Field_Map078, .LField_Map078_SHAPE_GRID
	map_block Field_Map078, .LField_Map078_ANIMATION
	map_block Field_Map078, .LField_Map078_BLEND
	map_block Field_Map078, .LField_Map078_SCENE
.LField_Map078_METATILES:
	.incbin "MAP/MAP_078/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map078_SHAPES:
	.incbin "MAP/MAP_078/SHAPES.table.lz"
	.balign 4, 0
.LField_Map078_GRID:
	.incbin "MAP/MAP_078/GRID.parts.lz"
	.balign 4, 0
.LField_Map078_SHAPE_GRID:
	.incbin "MAP/MAP_078/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map078_ANIMATION:
	.incbin "MAP/MAP_078/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map078_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map078_SCENE:
	.incbin "MAP/MAP_078/SCENE.parts.lz"
