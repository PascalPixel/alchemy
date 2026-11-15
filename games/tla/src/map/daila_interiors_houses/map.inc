	.global Field_DailaInteriorsHouses
Field_DailaInteriorsHouses:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DailaInteriorsHouses, .LField_DailaInteriorsHouses_CELL_PICTURES
	map_block Field_DailaInteriorsHouses, .LField_DailaInteriorsHouses_SHAPES
	map_block Field_DailaInteriorsHouses, .LField_DailaInteriorsHouses_GRID
	map_block Field_DailaInteriorsHouses, .LField_DailaInteriorsHouses_SHAPE_GRID
	map_block Field_DailaInteriorsHouses, .LField_DailaInteriorsHouses_ANIMATION
	map_block Field_DailaInteriorsHouses, .LField_DailaInteriorsHouses_BLEND
	map_block Field_DailaInteriorsHouses, .LField_DailaInteriorsHouses_SCENE
.LField_DailaInteriorsHouses_CELL_PICTURES:
	.incbin "games/tla/src/map/daila_interiors_houses/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_DailaInteriorsHouses_SHAPES:
	.incbin "games/tla/src/map/daila_interiors_houses/shapes.table.lz"
	.balign 4, 0
.LField_DailaInteriorsHouses_GRID:
	.incbin "games/tla/src/map/daila_interiors_houses/map.grid.lz"
	.balign 4, 0
.LField_DailaInteriorsHouses_SHAPE_GRID:
	.incbin "games/tla/src/map/daila_interiors_houses/shape_grid.plane.lz"
	.balign 4, 0
.LField_DailaInteriorsHouses_ANIMATION:
	.incbin "games/tla/src/map/daila_interiors_houses/animation.script.lz"
	.balign 4, 0
.LField_DailaInteriorsHouses_BLEND:
	.incbin "games/tla/src/map/daila_interiors_houses/blend.script.lz"
	.balign 4, 0
.LField_DailaInteriorsHouses_SCENE:
	.incbin "games/tla/src/map/daila_interiors_houses/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DailaInteriorsHousesPalette
Tileset_DailaInteriorsHousesPalette:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_houses/palette.gbapal.lz"
	.global Tileset_DailaInteriorsHousesTilesA
Tileset_DailaInteriorsHousesTilesA:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_houses/tiles_a.4bpp.mtf"
	.global Tileset_DailaInteriorsHousesTilesB
Tileset_DailaInteriorsHousesTilesB:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_houses/tiles_b.4bpp.mtf"
	.byte 0x58	@ PACKER: left in the buffer by Tileset_DailaInteriorsHousesTilesA+0x12b3
	.global Tileset_DailaInteriorsHousesTilesC
Tileset_DailaInteriorsHousesTilesC:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_houses/tiles_c.4bpp.mtf"
	.global Tileset_DailaInteriorsHousesTilesD
Tileset_DailaInteriorsHousesTilesD:
	.incbin "games/tla/src/graphics/tileset/daila_interiors_houses/tiles_d.4bpp.mtf"
