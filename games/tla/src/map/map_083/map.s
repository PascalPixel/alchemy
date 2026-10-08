	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map083
Field_Map083:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map083, .LField_Map083_METATILES
	map_block Field_Map083, .LField_Map083_SHAPES
	map_block Field_Map083, .LField_Map083_GRID
	map_block Field_Map083, .LField_Map083_SHAPE_GRID
	map_block Field_Map083, .LField_Map083_ANIMATION
	map_block Field_Map083, .LField_Map083_BLEND
	map_block Field_Map083, .LField_Map083_SCENE
.LField_Map083_METATILES:
	.incbin "map/map_083/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map083_SHAPES:
	.incbin "map/map_083/shapes.table.lz"
	.balign 4, 0
.LField_Map083_GRID:
	.incbin "map/map_083/grid.parts.lz"
	.balign 4, 0
.LField_Map083_SHAPE_GRID:
	.incbin "map/map_083/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map083_ANIMATION:
	.incbin "map/map_078/animation.script.lz"
	.balign 4, 0
.LField_Map083_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map083_SCENE:
	.incbin "map/map_083/scene.parts.lz"
