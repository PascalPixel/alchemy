@ Field_Map122: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map122
Field_Map122:
	map_header 0, 0, 110, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 110, 16, 16, 0, 0, 0xff, 0xff
	map_layer 110, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map122, .LField_Map122_METATILES
	map_block Field_Map122, .LField_Map122_SHAPES
	map_block Field_Map122, .LField_Map122_GRID
	map_block Field_Map122, .LField_Map122_SHAPE_GRID
	map_block Field_Map122, .LField_Map122_ANIMATION
	map_block Field_Map122, .LField_Map122_BLEND
	map_block Field_Map122, .LField_Map122_SCENE
.LField_Map122_METATILES:
	.incbin "MAP/MAP_122/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map122_SHAPES:
	.incbin "MAP/MAP_122/SHAPES.table.lz"
	.balign 4, 0
.LField_Map122_GRID:
	.incbin "MAP/MAP_122/GRID.parts.lz"
	.balign 4, 0
.LField_Map122_SHAPE_GRID:
	.incbin "MAP/MAP_122/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map122_ANIMATION:
	.incbin "MAP/MAP_122/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map122_BLEND:
	.incbin "MAP/MAP_122/BLEND.script.lz"
	.balign 4, 0
.LField_Map122_SCENE:
	.incbin "MAP/MAP_122/SCENE.parts.lz"
