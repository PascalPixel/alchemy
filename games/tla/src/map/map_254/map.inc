	.global Field_Map254
Field_Map254:
	map_header 0, 0, 38, 50, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 38, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 50, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map254, .LField_Map254_CELL_PICTURES
	map_block Field_Map254, .LField_Map254_SHAPES
	map_block Field_Map254, .LField_Map254_GRID
	map_block Field_Map254, .LField_Map254_SHAPE_GRID
	map_block Field_Map254, .LField_Map254_ANIMATION
	map_block Field_Map254, .LField_Map254_BLEND
	map_block Field_Map254, .LField_Map254_SCENE
.LField_Map254_CELL_PICTURES:
	.incbin "games/tla/src/map/map_254/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map254_SHAPES:
	.incbin "games/tla/src/map/map_254/shapes.table.lz"
	.balign 4, 0
.LField_Map254_GRID:
	.incbin "games/tla/src/map/map_254/map.grid.lz"
	.balign 4, 0
.LField_Map254_SHAPE_GRID:
	.incbin "games/tla/src/map/map_254/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map254_ANIMATION:
	.incbin "games/tla/src/map/map_254/animation.script.lz"
	.balign 4, 0
.LField_Map254_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Map254_SCENE:
	.incbin "games/tla/src/map/map_254/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set107Palette
Tileset_Set107Palette:
	.incbin "games/tla/src/graphics/tileset/set_107/palette.gbapal.lz"
	.byte 0x18	@ PACKER: left in the buffer by Field_Map254+0x15f
	.global Tileset_Set107TilesA
Tileset_Set107TilesA:
	.incbin "games/tla/src/graphics/tileset/set_107/tiles_a.4bpp.mtf"
	.global Tileset_Set107TilesB
Tileset_Set107TilesB:
	.incbin "games/tla/src/graphics/tileset/set_107/tiles_b.4bpp.mtf"
	.byte 0xf6	@ PACKER: left in the buffer by Tileset_Set107TilesA+0x2173
	.global Tileset_Set107TilesC
Tileset_Set107TilesC:
	.incbin "games/tla/src/graphics/tileset/set_107/tiles_c.4bpp.mtf"
	.byte 0x65, 0xab, 0x31	@ PACKER: left in the buffer by Tileset_Set107TilesA+0x26bd
	.global Tileset_Set107TilesD
Tileset_Set107TilesD:
	.incbin "games/tla/src/graphics/tileset/set_107/tiles_d.4bpp.mtf"
	.byte 0xe1	@ PACKER: left in the buffer by Tileset_Set107TilesC+0x5db
