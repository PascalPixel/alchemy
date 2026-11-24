	.global Field_MadraDrawbridge
Field_MadraDrawbridge:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_CELL_TILES
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_SHAPES
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_GRID
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_SHAPE_GRID
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_ANIMATION
	map_block Field_MadraDrawbridge
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_SCENE
.LField_MadraDrawbridge_CELL_TILES:
	.incbin "games/tla/src/map/madra_drawbridge/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MadraDrawbridge_SHAPES:
	.incbin "games/tla/src/map/madra_drawbridge/shapes.table.lz"
	.balign 4, 0
.LField_MadraDrawbridge_GRID:
	.incbin "games/tla/src/map/madra_drawbridge/map.grid.lz"
	.balign 4, 0
.LField_MadraDrawbridge_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_drawbridge/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraDrawbridge_ANIMATION:
	.incbin "games/tla/src/map/madra_drawbridge/animation.script.lz"
	.balign 4, 0
.LField_MadraDrawbridge_SCENE:
	.incbin "games/tla/src/map/madra_drawbridge/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MadraDrawbridgePalette
Tileset_MadraDrawbridgePalette:
	.incbin "games/tla/src/graphics/tileset/madra_drawbridge/palette.gbapal.lz"
	.byte 0x70, 0x28	@ PACKER: left in the buffer by Field_MadraDrawbridge+0x16a
	.global Tileset_MadraDrawbridgeTilesA
Tileset_MadraDrawbridgeTilesA:
	.incbin "games/tla/src/graphics/tileset/madra_drawbridge/tiles_a.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Field_MadraRooms+0x2e4f
	.global Tileset_MadraDrawbridgeTilesB
Tileset_MadraDrawbridgeTilesB:
	.incbin "games/tla/src/graphics/tileset/madra_drawbridge/tiles_b.4bpp.mtf"
	.byte 0xcf, 0xde	@ PACKER: left in the buffer by Tileset_MadraDrawbridgeTilesA+0x27be
	.global Tileset_MadraDrawbridgeTilesC
Tileset_MadraDrawbridgeTilesC:
	.incbin "games/tla/src/graphics/tileset/madra_drawbridge/tiles_c.4bpp.mtf"
	.global Tileset_MadraDrawbridgeTilesD
Tileset_MadraDrawbridgeTilesD:
	.incbin "games/tla/src/graphics/tileset/madra_drawbridge/tiles_d.4bpp.mtf"
