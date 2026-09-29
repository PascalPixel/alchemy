@ Field_Map033: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map033
Field_Map033:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map033, .LField_Map033_METATILES
	map_block Field_Map033, .LField_Map033_SHAPES
	map_block Field_Map033, .LField_Map033_GRID
	map_block Field_Map033, .LField_Map033_SHAPE_GRID
	map_block Field_Map033, .LField_Map033_ANIMATION
	map_block Field_Map033, .LField_Map033_BLEND
	map_block Field_Map033, .LField_Map033_SCENE
.LField_Map033_METATILES:
	.incbin "MAP/MAP_033/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map033_SHAPES:
	.incbin "MAP/MAP_033/SHAPES.table.lz"
	.balign 4, 0
.LField_Map033_GRID:
	.incbin "MAP/MAP_033/GRID.parts.lz"
	.balign 4, 0
.LField_Map033_SHAPE_GRID:
	.incbin "MAP/MAP_033/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map033_ANIMATION:
	.incbin "MAP/MAP_033/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map033_BLEND:
	.incbin "MAP/MAP_033/BLEND.script.lz"
	.balign 4, 0
.LField_Map033_SCENE:
	.incbin "MAP/MAP_033/SCENE.parts.lz"
