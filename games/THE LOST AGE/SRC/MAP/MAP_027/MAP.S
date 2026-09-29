@ Field_Map027: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map027
Field_Map027:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map027, .LField_Map027_METATILES
	map_block Field_Map027, .LField_Map027_SHAPES
	map_block Field_Map027, .LField_Map027_GRID
	map_block Field_Map027, .LField_Map027_SHAPE_GRID
	map_block Field_Map027, .LField_Map027_ANIMATION
	map_block Field_Map027, .LField_Map027_BLEND
	map_block Field_Map027, .LField_Map027_SCENE
.LField_Map027_METATILES:
	.incbin "MAP/MAP_027/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map027_SHAPES:
	.incbin "MAP/MAP_027/SHAPES.table.lz"
	.balign 4, 0
.LField_Map027_GRID:
	.incbin "MAP/MAP_027/GRID.parts.lz"
	.balign 4, 0
.LField_Map027_SHAPE_GRID:
	.incbin "MAP/MAP_027/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map027_ANIMATION:
	.incbin "MAP/MAP_027/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map027_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map027_SCENE:
	.incbin "MAP/MAP_027/SCENE.parts.lz"
