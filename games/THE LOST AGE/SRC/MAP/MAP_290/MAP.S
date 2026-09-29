@ Field_Map290: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map290
Field_Map290:
	map_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map290, .LField_Map290_METATILES
	map_block Field_Map290, .LField_Map290_SHAPES
	map_block Field_Map290, .LField_Map290_GRID
	map_block Field_Map290, .LField_Map290_SHAPE_GRID
	map_block Field_Map290, .LField_Map290_ANIMATION
	map_block Field_Map290, .LField_Map290_BLEND
	map_block Field_Map290, .LField_Map290_SCENE
.LField_Map290_METATILES:
	.incbin "MAP/MAP_290/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map290_SHAPES:
	.incbin "MAP/MAP_290/SHAPES.table.lz"
	.balign 4, 0
.LField_Map290_GRID:
	.incbin "MAP/MAP_290/GRID.parts.lz"
	.balign 4, 0
.LField_Map290_SHAPE_GRID:
	.incbin "MAP/MAP_290/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map290_ANIMATION:
	.incbin "MAP/MAP_290/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map290_BLEND:
	.incbin "MAP/MAP_290/BLEND.script.lz"
	.balign 4, 0
.LField_Map290_SCENE:
	.incbin "MAP/MAP_290/SCENE.parts.lz"
