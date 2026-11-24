	.global Field_Izumo
Field_Izumo:
	map_header 0, 0, 68, 88, 3, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Izumo, .LField_Izumo_CELL_TILES
	map_block Field_Izumo, .LField_Izumo_SHAPES
	map_block Field_Izumo, .LField_Izumo_GRID
	map_block Field_Izumo, .LField_Izumo_SHAPE_GRID
	map_block Field_Izumo, .LField_Izumo_ANIMATION
	map_block Field_Izumo, .LField_Izumo_BLEND
	map_block Field_Izumo, .LField_Izumo_SCENE
.LField_Izumo_CELL_TILES:
	.incbin "games/tla/src/map/izumo/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_Izumo_SHAPES:
	.incbin "games/tla/src/map/izumo/shapes.table.lz"
	.balign 4, 0
.LField_Izumo_GRID:
	.incbin "games/tla/src/map/izumo/map.grid.lz"
	.balign 4, 0
.LField_Izumo_SHAPE_GRID:
	.incbin "games/tla/src/map/izumo/shape_grid.plane.lz"
	.balign 4, 0
.LField_Izumo_ANIMATION:
	.incbin "games/tla/src/map/islets_dog/animation.script.lz"
	.balign 4, 0
.LField_Izumo_BLEND:
	.incbin "games/tla/src/map/izumo/blend.script.lz"
	.balign 4, 0
.LField_Izumo_SCENE:
	.incbin "games/tla/src/map/izumo/scene.scene.lz"
	.balign 4, 0
	.global Tileset_IzumoPalette
Tileset_IzumoPalette:
	.incbin "games/tla/src/graphics/tileset/izumo/palette.gbapal.lz"
	.byte 0xf3	@ PACKER: left in the buffer by Field_Izumo+0x18f
	.global Tileset_IzumoTilesA
Tileset_IzumoTilesA:
	.incbin "games/tla/src/graphics/tileset/izumo/tiles_a.4bpp.mtf"
	.global Tileset_IzumoTilesB
Tileset_IzumoTilesB:
	.incbin "games/tla/src/graphics/tileset/izumo/tiles_b.4bpp.mtf"
	.global Tileset_IzumoTilesC
Tileset_IzumoTilesC:
	.incbin "games/tla/src/graphics/tileset/izumo/tiles_c.4bpp.mtf"
	.byte 0x91, 0x81, 0x07	@ PACKER: left in the buffer by Tileset_IzumoTilesB+0x21a5
	.global Tileset_IzumoTilesD
Tileset_IzumoTilesD:
	.incbin "games/tla/src/graphics/tileset/izumo/tiles_d.4bpp.mtf"
	.byte 0x10	@ PACKER: left in the buffer by Tileset_IzumoTilesB+0x27d3
