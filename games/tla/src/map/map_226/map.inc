	.global Field_Map226
Field_Map226:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map226, .LField_Map226_CELL_PICTURES
	map_block Field_Map226, .LField_Map226_SHAPES
	map_block Field_Map226, .LField_Map226_GRID
	map_block Field_Map226, .LField_Map226_SHAPE_GRID
	map_block Field_Map226, .LField_Map226_ANIMATION
	map_block Field_Map226, .LField_Map226_BLEND
	map_block Field_Map226, .LField_Map226_SCENE
.LField_Map226_CELL_PICTURES:
	.incbin "games/tla/src/map/map_226/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map226_SHAPES:
	.incbin "games/tla/src/map/map_226/shapes.table.lz"
	.balign 4, 0
.LField_Map226_GRID:
	.incbin "games/tla/src/map/map_226/map.grid.lz"
	.balign 4, 0
.LField_Map226_SHAPE_GRID:
	.incbin "games/tla/src/map/map_226/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map226_ANIMATION:
	.incbin "games/tla/src/map/map_226/animation.script.lz"
	.balign 4, 0
.LField_Map226_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map226_SCENE:
	.incbin "games/tla/src/map/map_226/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set98Palette
Tileset_Set98Palette:
	.incbin "games/tla/src/graphics/tileset/set_98/palette.gbapal.lz"
	.byte 0xf7, 0x99, 0x1a	@ PACKER: left in the buffer by Field_Map226+0x1d9
	.global Tileset_Set98TilesA
Tileset_Set98TilesA:
	.incbin "games/tla/src/graphics/tileset/set_98/tiles_a.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Field_Map226+0x1b33
	.global Tileset_Set98TilesB
Tileset_Set98TilesB:
	.incbin "games/tla/src/graphics/tileset/set_98/tiles_b.4bpp.mtf"
	.global Tileset_Set98TilesC
Tileset_Set98TilesC:
	.incbin "games/tla/src/graphics/tileset/set_98/tiles_c.4bpp.mtf"
	.byte 0x3a, 0x6c, 0xb7	@ PACKER: left in the buffer by Tileset_Set98TilesA+0x1a01
	.global Tileset_Set98TilesD
Tileset_Set98TilesD:
	.incbin "games/tla/src/graphics/tileset/set_98/tiles_d.4bpp.mtf"
	.byte 0x84, 0xb0, 0xa3	@ PACKER: left in the buffer by Tileset_Set98TilesC+0x11b9
	.global Tileset_Palette52
Tileset_Palette52:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_52.gbapal.lz"
	.byte 0xce, 0xe0, 0x07	@ PACKER: left in the buffer by Tileset_Set98TilesD+0x1e5
	.global Tileset_Palette53
Tileset_Palette53:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_53.gbapal.lz"
	.byte 0xf2	@ PACKER: left in the buffer by Tileset_Palette52+0x18f
