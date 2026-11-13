	.global Field_Map043
Field_Map043:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map043, .LField_Map043_CELL_PICTURES
	map_block Field_Map043, .LField_Map043_SHAPES
	map_block Field_Map043, .LField_Map043_GRID
	map_block Field_Map043, .LField_Map043_SHAPE_GRID
	map_block Field_Map043, .LField_Map043_ANIMATION
	map_block Field_Map043, .LField_Map043_BLEND
	map_block Field_Map043, .LField_Map043_SCENE
.LField_Map043_CELL_PICTURES:
	.incbin "games/tla/src/map/map_043/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map043_SHAPES:
	.incbin "games/tla/src/map/map_043/shapes.table.lz"
	.balign 4, 0
.LField_Map043_GRID:
	.incbin "games/tla/src/map/map_043/map.grid.lz"
	.balign 4, 0
.LField_Map043_SHAPE_GRID:
	.incbin "games/tla/src/map/map_043/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map043_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map043_BLEND:
	.incbin "games/tla/src/map/map_043/blend.script.lz"
	.balign 4, 0
.LField_Map043_SCENE:
	.incbin "games/tla/src/map/map_043/scene.scene.lz"
	.global Tileset_Set26Palette
Tileset_Set26Palette:
	.incbin "games/tla/src/graphics/tileset/set_26/palette.gbapal.lz"
	.global Tileset_Set26TilesA
Tileset_Set26TilesA:
	.incbin "games/tla/src/graphics/tileset/set_26/tiles_a.4bpp.mtf"
	.byte 0xdf	@ PACKER: left in the buffer by Tileset_Set25TilesA+0x1f5b
	.global Tileset_Set26TilesB
Tileset_Set26TilesB:
	.incbin "games/tla/src/graphics/tileset/set_26/tiles_b.4bpp.mtf"
	.byte 0xfa, 0x5c	@ PACKER: left in the buffer by Tileset_Set26TilesA+0x185e
	.global Tileset_Set26TilesC
Tileset_Set26TilesC:
	.incbin "games/tla/src/graphics/tileset/set_26/tiles_c.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_Set26TilesB+0x60f
	.global Tileset_Set26TilesD
Tileset_Set26TilesD:
	.incbin "games/tla/src/graphics/tileset/set_26/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_Set26TilesC+0x13f
