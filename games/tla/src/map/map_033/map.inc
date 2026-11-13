	.global Field_Map033
Field_Map033:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map033, .LField_Map033_CELL_PICTURES
	map_block Field_Map033, .LField_Map033_SHAPES
	map_block Field_Map033, .LField_Map033_GRID
	map_block Field_Map033, .LField_Map033_SHAPE_GRID
	map_block Field_Map033, .LField_Map033_ANIMATION
	map_block Field_Map033, .LField_Map033_BLEND
	map_block Field_Map033, .LField_Map033_SCENE
.LField_Map033_CELL_PICTURES:
	.incbin "games/tla/src/map/map_033/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map033_SHAPES:
	.incbin "games/tla/src/map/map_033/shapes.table.lz"
	.balign 4, 0
.LField_Map033_GRID:
	.incbin "games/tla/src/map/map_033/map.grid.lz"
	.balign 4, 0
.LField_Map033_SHAPE_GRID:
	.incbin "games/tla/src/map/map_033/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map033_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map033_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map033_SCENE:
	.incbin "games/tla/src/map/map_033/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set16Palette
Tileset_Set16Palette:
	.incbin "games/tla/src/graphics/tileset/set_16/palette.gbapal.lz"
	.byte 0x98	@ PACKER: left in the buffer by Field_Map033+0x14f
	.global Tileset_Set16TilesA
Tileset_Set16TilesA:
	.incbin "games/tla/src/graphics/tileset/set_16/tiles_a.4bpp.mtf"
	.global Tileset_Set16TilesB
Tileset_Set16TilesB:
	.incbin "games/tla/src/graphics/tileset/set_16/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_Set16TilesA+0xfda
	.global Tileset_Set16TilesC
Tileset_Set16TilesC:
	.incbin "games/tla/src/graphics/tileset/set_16/tiles_c.4bpp.mtf"
	.byte 0x01	@ PACKER: left in the buffer by Field_Map033+0x217f
	.global Tileset_Set16TilesD
Tileset_Set16TilesD:
	.incbin "games/tla/src/graphics/tileset/set_16/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_Set16TilesC+0x13f
