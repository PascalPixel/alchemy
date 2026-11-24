	.global Field_YampiDesertBlitz
Field_YampiDesertBlitz:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertBlitz, .LField_YampiDesertBlitz_CELL_TILES
	map_block Field_YampiDesertBlitz, .LField_YampiDesertBlitz_SHAPES
	map_block Field_YampiDesertBlitz, .LField_YampiDesertBlitz_GRID
	map_block Field_YampiDesertBlitz, .LField_YampiDesertBlitz_SHAPE_GRID
	map_block Field_YampiDesertBlitz, .LField_YampiDesertBlitz_ANIMATION
	map_block Field_YampiDesertBlitz, .LField_YampiDesertBlitz_BLEND
	map_block Field_YampiDesertBlitz, .LField_YampiDesertBlitz_SCENE
.LField_YampiDesertBlitz_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_blitz/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertBlitz_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_blitz/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertBlitz_GRID:
	.incbin "games/tla/src/map/yampi_desert_blitz/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertBlitz_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_blitz/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertBlitz_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertBlitz_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertBlitz_SCENE:
	.incbin "games/tla/src/map/yampi_desert_blitz/scene.scene.lz"
	.balign 4, 0
