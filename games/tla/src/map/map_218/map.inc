	.global Field_Map218
Field_Map218:
	map_header 0, 0, 104, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 104, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map218, .LField_Map218_CELL_PICTURES
	map_block Field_Map218, .LField_Map218_SHAPES
	map_block Field_Map218, .LField_Map218_GRID
	map_block Field_Map218, .LField_Map218_SHAPE_GRID
	map_block Field_Map218, .LField_Map218_ANIMATION
	map_block Field_Map218, .LField_Map218_BLEND
	map_block Field_Map218, .LField_Map218_SCENE
.LField_Map218_CELL_PICTURES:
	.incbin "games/tla/src/map/map_218/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map218_SHAPES:
	.incbin "games/tla/src/map/map_218/shapes.table.lz"
	.balign 4, 0
.LField_Map218_GRID:
	.incbin "games/tla/src/map/map_218/map.grid.lz"
	.balign 4, 0
.LField_Map218_SHAPE_GRID:
	.incbin "games/tla/src/map/map_218/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map218_ANIMATION:
	.incbin "games/tla/src/map/map_218/animation.script.lz"
	.balign 4, 0
.LField_Map218_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map218_SCENE:
	.incbin "games/tla/src/map/map_218/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set92Palette
Tileset_Set92Palette:
	.incbin "games/tla/src/graphics/tileset/set_92/palette.gbapal.lz"
	.byte 0xa8, 0xad, 0x01	@ PACKER: left in the buffer by Field_Map218+0x141
	.global Tileset_Set92TilesA
Tileset_Set92TilesA:
	.incbin "games/tla/src/graphics/tileset/set_92/tiles_a.4bpp.mtf"
	.byte 0x40, 0x00	@ PACKER: left in the buffer by Field_Map218+0x18ba
