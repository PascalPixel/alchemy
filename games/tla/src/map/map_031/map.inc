	.global Field_Map031
Field_Map031:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 12, 12, 0, 0, 0xff, 0xff
	map_block Field_Map031, .LField_Map031_CELL_PICTURES
	map_block Field_Map031, .LField_Map031_SHAPES
	map_block Field_Map031, .LField_Map031_GRID
	map_block Field_Map031, .LField_Map031_SHAPE_GRID
	map_block Field_Map031, .LField_Map031_ANIMATION
	map_block Field_Map031
	map_block Field_Map031, .LField_Map031_SCENE
.LField_Map031_CELL_PICTURES:
	.incbin "games/tla/src/map/map_031/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map031_SHAPES:
	.incbin "games/tla/src/map/map_031/shapes.table.lz"
	.balign 4, 0
.LField_Map031_GRID:
	.incbin "games/tla/src/map/map_031/map.grid.lz"
	.balign 4, 0
.LField_Map031_SHAPE_GRID:
	.incbin "games/tla/src/map/map_031/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map031_ANIMATION:
	.incbin "games/tla/src/map/map_031/animation.script.lz"
	.balign 4, 0
.LField_Map031_SCENE:
	.incbin "games/tla/src/map/map_031/scene.scene.lz"
	.global Tileset_Set14Palette
Tileset_Set14Palette:
	.incbin "games/tla/src/graphics/tileset/set_14/palette.gbapal.lz"
	.byte 0x27, 0x1f	@ PACKER: left in the buffer by Field_Map031+0x16e
	.global Tileset_Set14TilesA
Tileset_Set14TilesA:
	.incbin "games/tla/src/graphics/tileset/set_14/tiles_a.4bpp.mtf"
	.byte 0x81, 0x45	@ PACKER: left in the buffer by Tileset_Set12TilesA+0x259a
	.global Tileset_Set14TilesB
Tileset_Set14TilesB:
	.incbin "games/tla/src/graphics/tileset/set_14/tiles_b.4bpp.mtf"
	.global Tileset_Set14TilesC
Tileset_Set14TilesC:
	.incbin "games/tla/src/graphics/tileset/set_14/tiles_c.4bpp.mtf"
	.byte 0xd8, 0x18	@ PACKER: left in the buffer by Tileset_Set14TilesB+0x1e86
	.global Tileset_Set14TilesD
Tileset_Set14TilesD:
	.incbin "games/tla/src/graphics/tileset/set_14/tiles_d.4bpp.mtf"
	.byte 0x08	@ PACKER: left in the buffer by Tileset_Set14TilesC+0x6c3
