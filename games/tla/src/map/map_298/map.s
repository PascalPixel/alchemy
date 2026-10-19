	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map298
Field_Map298:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map298, .LField_Map298_METATILES
	map_block Field_Map298, .LField_Map298_SHAPES
	map_block Field_Map298, .LField_Map298_GRID
	map_block Field_Map298, .LField_Map298_SHAPE_GRID
	map_block Field_Map298
	map_block Field_Map298
	map_block Field_Map298, .LField_Map298_SCENE
.LField_Map298_METATILES:
	.incbin "games/tla/src/map/map_000/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map298_SHAPES:
	.incbin "games/tla/src/map/map_231/shapes.table.lz"
	.balign 4, 0
.LField_Map298_GRID:
	.incbin "games/tla/src/map/map_298/grid.parts.lz"
	.balign 4, 0
.LField_Map298_SHAPE_GRID:
	.incbin "games/tla/src/map/map_231/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map298_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
