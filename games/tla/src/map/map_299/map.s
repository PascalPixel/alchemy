	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map299
Field_Map299:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map299, .LField_Map299_METATILES
	map_block Field_Map299, .LField_Map299_SHAPES
	map_block Field_Map299, .LField_Map299_GRID
	map_block Field_Map299, .LField_Map299_SHAPE_GRID
	map_block Field_Map299
	map_block Field_Map299
	map_block Field_Map299, .LField_Map299_SCENE
.LField_Map299_METATILES:
	.incbin "common/map/map_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map299_SHAPES:
	.incbin "map/map_008/shapes.table.lz"
	.balign 4, 0
.LField_Map299_GRID:
	.incbin "map/map_299/grid.parts.lz"
	.balign 4, 0
.LField_Map299_SHAPE_GRID:
	.incbin "map/map_008/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map299_SCENE:
	.incbin "map/map_000/scene.parts.lz"
