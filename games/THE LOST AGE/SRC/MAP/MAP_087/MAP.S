@ Field_Map087: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map087
Field_Map087:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map087, .LField_Map087_METATILES
	map_block Field_Map087, .LField_Map087_SHAPES
	map_block Field_Map087, .LField_Map087_GRID
	map_block Field_Map087, .LField_Map087_SHAPE_GRID
	map_block Field_Map087, .LField_Map087_ANIMATION
	map_block Field_Map087, .LField_Map087_BLEND
	map_block Field_Map087, .LField_Map087_SCENE
.LField_Map087_METATILES:
	.incbin "MAP/MAP_087/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map087_SHAPES:
	.incbin "MAP/MAP_087/SHAPES.table.lz"
	.balign 4, 0
.LField_Map087_GRID:
	.incbin "MAP/MAP_087/GRID.parts.lz"
	.balign 4, 0
.LField_Map087_SHAPE_GRID:
	.incbin "MAP/MAP_087/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map087_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map087_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map087_SCENE:
	.incbin "MAP/MAP_087/SCENE.parts.lz"
