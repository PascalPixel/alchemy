	.global Field_Map289
Field_Map289:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map289, .LField_Map289_CELL_PICTURES
	map_block Field_Map289, .LField_Map289_SHAPES
	map_block Field_Map289, .LField_Map289_GRID
	map_block Field_Map289, .LField_Map289_SHAPE_GRID
	map_block Field_Map289, .LField_Map289_ANIMATION
	map_block Field_Map289, .LField_Map289_BLEND
	map_block Field_Map289, .LField_Map289_SCENE
.LField_Map289_CELL_PICTURES:
	.incbin "games/tla/src/map/map_289/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map289_SHAPES:
	.incbin "games/tla/src/map/map_179/shapes.table.lz"
	.balign 4, 0
.LField_Map289_GRID:
	.incbin "games/tla/src/map/map_289/map.grid.lz"
	.balign 4, 0
.LField_Map289_SHAPE_GRID:
	.incbin "games/tla/src/map/map_289/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map289_ANIMATION:
	.incbin "games/tla/src/map/map_289/animation.script.lz"
	.balign 4, 0
.LField_Map289_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map289_SCENE:
	.incbin "games/tla/src/map/map_236/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set118Palette
Tileset_Set118Palette:
	.incbin "games/tla/src/graphics/tileset/set_118/palette.gbapal.lz"
	.global Tileset_Set118TilesA
Tileset_Set118TilesA:
	.incbin "games/tla/src/graphics/tileset/set_118/tiles_a.4bpp.mtf"
	.byte 0x0e, 0x59, 0x80	@ PACKER: left in the buffer by Tileset_Set117TilesD+0x167d
	.global Tileset_Set118TilesB
Tileset_Set118TilesB:
	.incbin "games/tla/src/graphics/tileset/set_118/tiles_b.4bpp.mtf"
	.byte 0x0c, 0xea, 0x4c	@ PACKER: left in the buffer by Tileset_Set118TilesA+0x115d
	.global Tileset_Set118TilesC
Tileset_Set118TilesC:
	.incbin "games/tla/src/graphics/tileset/set_118/tiles_c.4bpp.mtf"
	.byte 0x56	@ PACKER: left in the buffer by Tileset_Set118TilesB+0x111b
	.global Tileset_Set118TilesD
Tileset_Set118TilesD:
	.incbin "games/tla/src/graphics/tileset/set_118/tiles_d.4bpp.mtf"
	.byte 0x3e, 0x7f	@ PACKER: left in the buffer by Tileset_Set118TilesC+0x206
