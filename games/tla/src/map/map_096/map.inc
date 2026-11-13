	.global Field_Map096
Field_Map096:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map096, .LField_Map096_CELL_PICTURES
	map_block Field_Map096, .LField_Map096_SHAPES
	map_block Field_Map096, .LField_Map096_GRID
	map_block Field_Map096, .LField_Map096_SHAPE_GRID
	map_block Field_Map096, .LField_Map096_ANIMATION
	map_block Field_Map096, .LField_Map096_BLEND
	map_block Field_Map096, .LField_Map096_SCENE
.LField_Map096_CELL_PICTURES:
	.incbin "games/tla/src/map/map_096/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map096_SHAPES:
	.incbin "games/tla/src/map/map_096/shapes.table.lz"
	.balign 4, 0
.LField_Map096_GRID:
	.incbin "games/tla/src/map/map_096/map.grid.lz"
	.balign 4, 0
.LField_Map096_SHAPE_GRID:
	.incbin "games/tla/src/map/map_096/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map096_ANIMATION:
	.incbin "games/tla/src/map/map_096/animation.script.lz"
	.balign 4, 0
.LField_Map096_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map096_SCENE:
	.incbin "games/tla/src/map/map_096/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set47Palette
Tileset_Set47Palette:
	.incbin "games/tla/src/graphics/tileset/set_47/palette.gbapal.lz"
	.global Tileset_Set47TilesA
Tileset_Set47TilesA:
	.incbin "games/tla/src/graphics/tileset/set_47/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set47TilesB
Tileset_Set47TilesB:
	.incbin "games/tla/src/graphics/tileset/set_47/tiles_b.4bpp.mtf"
	.byte 0x71, 0x76, 0xf0	@ PACKER: left in the buffer by Tileset_Set47TilesA+0x1969
	.global Tileset_Set47TilesC
Tileset_Set47TilesC:
	.incbin "games/tla/src/graphics/tileset/set_47/tiles_c.4bpp.mtf"
	.global Tileset_Set47TilesD
Tileset_Set47TilesD:
	.incbin "games/tla/src/graphics/tileset/set_47/tiles_d.4bpp.mtf"
	.byte 0x3e, 0x8d	@ PACKER: left in the buffer by Tileset_Set47TilesC+0xf1e
