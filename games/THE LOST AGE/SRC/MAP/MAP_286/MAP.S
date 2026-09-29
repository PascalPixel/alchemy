@ Field_Map286: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map286
Field_Map286:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map286, .LField_Map286_METATILES
	map_block Field_Map286, .LField_Map286_SHAPES
	map_block Field_Map286, .LField_Map286_GRID
	map_block Field_Map286, .LField_Map286_SHAPE_GRID
	map_block Field_Map286, .LField_Map286_ANIMATION
	map_block Field_Map286, .LField_Map286_BLEND
	map_block Field_Map286, .LField_Map286_SCENE
.LField_Map286_METATILES:
	.incbin "MAP/MAP_286/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map286_SHAPES:
	.incbin "MAP/MAP_286/SHAPES.table.lz"
	.balign 4, 0
.LField_Map286_GRID:
	.incbin "MAP/MAP_286/GRID.parts.lz"
	.balign 4, 0
.LField_Map286_SHAPE_GRID:
	.incbin "MAP/MAP_286/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map286_ANIMATION:
	.incbin "MAP/MAP_035/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map286_BLEND:
	.incbin "MAP/MAP_035/BLEND.script.lz"
	.balign 4, 0
.LField_Map286_SCENE:
	.incbin "MAP/MAP_286/SCENE.parts.lz"
