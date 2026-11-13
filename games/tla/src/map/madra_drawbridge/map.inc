	.global Field_MadraDrawbridge
Field_MadraDrawbridge:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_CELL_PICTURES
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_SHAPES
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_GRID
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_SHAPE_GRID
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_ANIMATION
	map_block Field_MadraDrawbridge
	map_block Field_MadraDrawbridge, .LField_MadraDrawbridge_SCENE
.LField_MadraDrawbridge_CELL_PICTURES:
	.incbin "games/tla/src/map/madra_drawbridge/cell_pictures.delta1.lz"
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
	.global Tileset_Set29Palette
Tileset_Set29Palette:
	.incbin "games/tla/src/graphics/tileset/set_29/palette.gbapal.lz"
	.byte 0x70, 0x28	@ PACKER: left in the buffer by Field_MadraDrawbridge+0x16a
	.global Tileset_Set29TilesA
Tileset_Set29TilesA:
	.incbin "games/tla/src/graphics/tileset/set_29/tiles_a.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Field_MadraInteriors+0x2e4f
	.global Tileset_Set29TilesB
Tileset_Set29TilesB:
	.incbin "games/tla/src/graphics/tileset/set_29/tiles_b.4bpp.mtf"
	.byte 0xcf, 0xde	@ PACKER: left in the buffer by Tileset_Set29TilesA+0x27be
	.global Tileset_Set29TilesC
Tileset_Set29TilesC:
	.incbin "games/tla/src/graphics/tileset/set_29/tiles_c.4bpp.mtf"
	.global Tileset_Set29TilesD
Tileset_Set29TilesD:
	.incbin "games/tla/src/graphics/tileset/set_29/tiles_d.4bpp.mtf"
