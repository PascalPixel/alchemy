	.global Field_Map259
Field_Map259:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map259, .LField_Map259_CELL_PICTURES
	map_block Field_Map259, .LField_Map259_SHAPES
	map_block Field_Map259, .LField_Map259_GRID
	map_block Field_Map259, .LField_Map259_SHAPE_GRID
	map_block Field_Map259, .LField_Map259_ANIMATION
	map_block Field_Map259, .LField_Map259_BLEND
	map_block Field_Map259, .LField_Map259_SCENE
.LField_Map259_CELL_PICTURES:
	.incbin "games/tla/src/map/map_259/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map259_SHAPES:
	.incbin "games/tla/src/map/map_259/shapes.table.lz"
	.balign 4, 0
.LField_Map259_GRID:
	.incbin "games/tla/src/map/map_259/map.grid.lz"
	.balign 4, 0
.LField_Map259_SHAPE_GRID:
	.incbin "games/tla/src/map/map_259/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map259_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map259_BLEND:
	.incbin "games/tla/src/map/venus_lighthouse/blend.script.lz"
	.balign 4, 0
.LField_Map259_SCENE:
	.incbin "games/tla/src/map/map_259/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set109Palette
Tileset_Set109Palette:
	.incbin "games/tla/src/graphics/tileset/set_109/palette.gbapal.lz"
	.byte 0x14, 0x8f	@ PACKER: left in the buffer by Field_Map259+0x19a
	.global Tileset_Set109TilesA
Tileset_Set109TilesA:
	.incbin "games/tla/src/graphics/tileset/set_109/tiles_a.4bpp.mtf"
	.byte 0x10, 0x00	@ PACKER: left in the buffer by Field_Map257+0x22aa
	.global Tileset_Set109TilesB
Tileset_Set109TilesB:
	.incbin "games/tla/src/graphics/tileset/set_109/tiles_b.4bpp.mtf"
	.byte 0xa6, 0x68, 0x02	@ PACKER: left in the buffer by Tileset_Set109TilesA+0x21c5
	.global Tileset_Set109TilesC
Tileset_Set109TilesC:
	.incbin "games/tla/src/graphics/tileset/set_109/tiles_c.4bpp.mtf"
	.byte 0x10	@ PACKER: left in the buffer by Tileset_Set109TilesB+0x16fb
	.global Tileset_Set109TilesD
Tileset_Set109TilesD:
	.incbin "games/tla/src/graphics/tileset/set_109/tiles_d.4bpp.mtf"
	.byte 0x9e	@ PACKER: left in the buffer by Tileset_Set109TilesC+0xb23
