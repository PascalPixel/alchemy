	.global Field_Map146
Field_Map146:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map146, .LField_Map146_CELL_PICTURES
	map_block Field_Map146, .LField_Map146_SHAPES
	map_block Field_Map146, .LField_Map146_GRID
	map_block Field_Map146, .LField_Map146_SHAPE_GRID
	map_block Field_Map146, .LField_Map146_ANIMATION
	map_block Field_Map146, .LField_Map146_BLEND
	map_block Field_Map146, .LField_Map146_SCENE
.LField_Map146_CELL_PICTURES:
	.incbin "games/tla/src/map/map_146/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map146_SHAPES:
	.incbin "games/tla/src/map/map_146/shapes.table.lz"
	.balign 4, 0
.LField_Map146_GRID:
	.incbin "games/tla/src/map/map_146/map.grid.lz"
	.balign 4, 0
.LField_Map146_SHAPE_GRID:
	.incbin "games/tla/src/map/map_146/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map146_ANIMATION:
	.incbin "games/tla/src/map/map_131/animation.script.lz"
	.balign 4, 0
.LField_Map146_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map146_SCENE:
	.incbin "games/tla/src/map/map_146/scene.scene.lz"
	.global Tileset_Set69Palette
Tileset_Set69Palette:
	.incbin "games/tla/src/graphics/tileset/set_69/palette.gbapal.lz"
	.byte 0x17, 0x03, 0xcc	@ PACKER: left in the buffer by Field_Map146+0x1c5
	.global Tileset_Set69TilesA
Tileset_Set69TilesA:
	.incbin "games/tla/src/graphics/tileset/set_69/tiles_a.4bpp.mtf"
	.byte 0x3e, 0xe0, 0xde	@ PACKER: left in the buffer by Field_Map146+0x1989
	.global Tileset_Set69TilesB
Tileset_Set69TilesB:
	.incbin "games/tla/src/graphics/tileset/set_69/tiles_b.4bpp.mtf"
	.byte 0xa8, 0x53	@ PACKER: left in the buffer by Tileset_Set69TilesA+0x112a
	.global Tileset_Set69TilesC
Tileset_Set69TilesC:
	.incbin "games/tla/src/graphics/tileset/set_69/tiles_c.4bpp.mtf"
	.byte 0x72, 0x1c	@ PACKER: left in the buffer by Tileset_Set69TilesB+0xbae
	.global Tileset_Set69TilesD
Tileset_Set69TilesD:
	.incbin "games/tla/src/graphics/tileset/set_69/tiles_d.4bpp.mtf"
