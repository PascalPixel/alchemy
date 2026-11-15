	.global Field_DailaInteriors1
Field_DailaInteriors1:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DailaInteriors1, .LField_DailaInteriors1_CELL_PICTURES
	map_block Field_DailaInteriors1, .LField_DailaInteriors1_SHAPES
	map_block Field_DailaInteriors1, .LField_DailaInteriors1_GRID
	map_block Field_DailaInteriors1, .LField_DailaInteriors1_SHAPE_GRID
	map_block Field_DailaInteriors1, .LField_DailaInteriors1_ANIMATION
	map_block Field_DailaInteriors1, .LField_DailaInteriors1_BLEND
	map_block Field_DailaInteriors1, .LField_DailaInteriors1_SCENE
.LField_DailaInteriors1_CELL_PICTURES:
	.incbin "games/tla/src/map/daila_interiors_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_DailaInteriors1_SHAPES:
	.incbin "games/tla/src/map/daila_interiors_1/shapes.table.lz"
	.balign 4, 0
.LField_DailaInteriors1_GRID:
	.incbin "games/tla/src/map/daila_interiors_1/map.grid.lz"
	.balign 4, 0
.LField_DailaInteriors1_SHAPE_GRID:
	.incbin "games/tla/src/map/daila_interiors_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_DailaInteriors1_ANIMATION:
	.incbin "games/tla/src/map/daila_interiors_1/animation.script.lz"
	.balign 4, 0
.LField_DailaInteriors1_BLEND:
	.incbin "games/tla/src/map/daila_interiors_1/blend.script.lz"
	.balign 4, 0
.LField_DailaInteriors1_SCENE:
	.incbin "games/tla/src/map/daila_interiors_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DailaInteriors1Palette
Tileset_DailaInteriors1Palette:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_1/palette.gbapal.lz"
	.global Tileset_DailaInteriors1TilesA
Tileset_DailaInteriors1TilesA:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_1/tiles_a.4bpp.mtf"
	.global Tileset_DailaInteriors1TilesB
Tileset_DailaInteriors1TilesB:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_1/tiles_b.4bpp.mtf"
	.byte 0x58	@ PACKER: left in the buffer by Tileset_DailaInteriors1TilesA+0x12b3
	.global Tileset_DailaInteriors1TilesC
Tileset_DailaInteriors1TilesC:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_1/tiles_c.4bpp.mtf"
	.global Tileset_DailaInteriors1TilesD
Tileset_DailaInteriors1TilesD:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_1/tiles_d.4bpp.mtf"
