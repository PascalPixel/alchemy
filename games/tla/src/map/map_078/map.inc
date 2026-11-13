	.global Field_Map078
Field_Map078:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map078, .LField_Map078_CELL_PICTURES
	map_block Field_Map078, .LField_Map078_SHAPES
	map_block Field_Map078, .LField_Map078_GRID
	map_block Field_Map078, .LField_Map078_SHAPE_GRID
	map_block Field_Map078, .LField_Map078_ANIMATION
	map_block Field_Map078, .LField_Map078_BLEND
	map_block Field_Map078, .LField_Map078_SCENE
.LField_Map078_CELL_PICTURES:
	.incbin "games/tla/src/map/map_078/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map078_SHAPES:
	.incbin "games/tla/src/map/map_078/shapes.table.lz"
	.balign 4, 0
.LField_Map078_GRID:
	.incbin "games/tla/src/map/map_078/map.grid.lz"
	.balign 4, 0
.LField_Map078_SHAPE_GRID:
	.incbin "games/tla/src/map/map_078/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map078_ANIMATION:
	.incbin "games/tla/src/map/map_078/animation.script.lz"
	.balign 4, 0
.LField_Map078_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map078_SCENE:
	.incbin "games/tla/src/map/map_078/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set40Palette
Tileset_Set40Palette:
	.incbin "games/tla/src/graphics/tileset/set_40/palette.gbapal.lz"
	.byte 0x94, 0x75	@ PACKER: left in the buffer by Field_Map078+0x14a
	.global Tileset_Set40TilesA
Tileset_Set40TilesA:
	.incbin "games/tla/src/graphics/tileset/set_40/tiles_a.4bpp.mtf"
	.byte 0x09	@ PACKER: left in the buffer by Field_Map078+0x1e83
	.global Tileset_Set40TilesB
Tileset_Set40TilesB:
	.incbin "games/tla/src/graphics/tileset/set_40/tiles_b.4bpp.mtf"
	.byte 0x7f, 0x04	@ PACKER: left in the buffer by Tileset_Set40TilesA+0x15a6
	.global Tileset_Set40TilesC
Tileset_Set40TilesC:
	.incbin "games/tla/src/graphics/tileset/set_40/tiles_c.4bpp.mtf"
	.byte 0x04, 0x40, 0xd8	@ PACKER: left in the buffer by Field_Map078+0x1fa9
	.global Tileset_Set40TilesD
Tileset_Set40TilesD:
	.incbin "games/tla/src/graphics/tileset/set_40/tiles_d.4bpp.mtf"
	.byte 0xe3, 0xf4	@ PACKER: left in the buffer by Tileset_Set40TilesC+0xf6e
