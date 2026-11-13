	.global Field_Map145
Field_Map145:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map145, .LField_Map145_CELL_PICTURES
	map_block Field_Map145, .LField_Map145_SHAPES
	map_block Field_Map145, .LField_Map145_GRID
	map_block Field_Map145, .LField_Map145_SHAPE_GRID
	map_block Field_Map145, .LField_Map145_ANIMATION
	map_block Field_Map145, .LField_Map145_BLEND
	map_block Field_Map145, .LField_Map145_SCENE
.LField_Map145_CELL_PICTURES:
	.incbin "games/tla/src/map/map_145/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map145_SHAPES:
	.incbin "games/tla/src/map/map_145/shapes.table.lz"
	.balign 4, 0
.LField_Map145_GRID:
	.incbin "games/tla/src/map/map_145/map.grid.lz"
	.balign 4, 0
.LField_Map145_SHAPE_GRID:
	.incbin "games/tla/src/map/map_145/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map145_ANIMATION:
	.incbin "games/tla/src/map/map_145/animation.script.lz"
	.balign 4, 0
.LField_Map145_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Map145_SCENE:
	.incbin "games/tla/src/map/map_145/scene.scene.lz"
	.global Tileset_Set68Palette
Tileset_Set68Palette:
	.incbin "games/tla/src/graphics/tileset/set_68/palette.gbapal.lz"
	.global Tileset_Set68TilesA
Tileset_Set68TilesA:
	.incbin "games/tla/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0x0d, 0x09	@ PACKER: left in the buffer by Field_Map144+0x1e3a
	.global Tileset_Set68TilesB
Tileset_Set68TilesB:
	.incbin "games/tla/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.global Tileset_Set68TilesC
Tileset_Set68TilesC:
	.incbin "games/tla/src/graphics/tileset/set_68/tiles_c.4bpp.mtf"
	.global Tileset_Set68TilesD
Tileset_Set68TilesD:
	.incbin "games/tla/src/graphics/tileset/set_68/tiles_d.4bpp.mtf"
	.byte 0x11	@ PACKER: left in the buffer by Tileset_Set68TilesC+0x195f
