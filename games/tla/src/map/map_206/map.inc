	.global Field_Map206
Field_Map206:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map206, .LField_Map206_CELL_PICTURES
	map_block Field_Map206, .LField_Map206_SHAPES
	map_block Field_Map206, .LField_Map206_GRID
	map_block Field_Map206, .LField_Map206_SHAPE_GRID
	map_block Field_Map206, .LField_Map206_ANIMATION
	map_block Field_Map206, .LField_Map206_BLEND
	map_block Field_Map206, .LField_Map206_SCENE
.LField_Map206_CELL_PICTURES:
	.incbin "games/tla/src/map/map_206/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map206_SHAPES:
	.incbin "games/tla/src/map/map_206/shapes.table.lz"
	.balign 4, 0
.LField_Map206_GRID:
	.incbin "games/tla/src/map/map_206/map.grid.lz"
	.balign 4, 0
.LField_Map206_SHAPE_GRID:
	.incbin "games/tla/src/map/map_206/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map206_ANIMATION:
	.incbin "games/tla/src/map/map_206/animation.script.lz"
	.balign 4, 0
.LField_Map206_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map206_SCENE:
	.incbin "games/tla/src/map/map_206/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set91Palette
Tileset_Set91Palette:
	.incbin "games/tla/src/graphics/tileset/set_91/palette.gbapal.lz"
	.global Tileset_Set91TilesA
Tileset_Set91TilesA:
	.incbin "games/tla/src/graphics/tileset/set_91/tiles_a.4bpp.mtf"
	.byte 0xb8	@ PACKER: left in the buffer by Tileset_Set90TilesC+0x2677
	.global Tileset_Set91TilesB
Tileset_Set91TilesB:
	.incbin "games/tla/src/graphics/tileset/set_91/tiles_b.4bpp.mtf"
	.byte 0xf5, 0x13	@ PACKER: left in the buffer by Tileset_Set91TilesA+0x161e
	.global Tileset_Set91TilesC
Tileset_Set91TilesC:
	.incbin "games/tla/src/graphics/tileset/set_91/tiles_c.4bpp.mtf"
	.byte 0xde	@ PACKER: left in the buffer by Tileset_Set91TilesA+0x195f
	.global Tileset_Set91TilesD
Tileset_Set91TilesD:
	.incbin "games/tla/src/graphics/tileset/set_91/tiles_d.4bpp.mtf"
