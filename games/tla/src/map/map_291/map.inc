	.global Field_Map291
Field_Map291:
	map_header 0, 0, 100, 64, 3, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map291, .LField_Map291_CELL_PICTURES
	map_block Field_Map291, .LField_Map291_SHAPES
	map_block Field_Map291, .LField_Map291_GRID
	map_block Field_Map291, .LField_Map291_SHAPE_GRID
	map_block Field_Map291, .LField_Map291_ANIMATION
	map_block Field_Map291, .LField_Map291_BLEND
	map_block Field_Map291, .LField_Map291_SCENE
.LField_Map291_CELL_PICTURES:
	.incbin "games/tla/src/map/map_291/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map291_SHAPES:
	.incbin "games/tla/src/map/map_291/shapes.table.lz"
	.balign 4, 0
.LField_Map291_GRID:
	.incbin "games/tla/src/map/map_291/map.grid.lz"
	.balign 4, 0
.LField_Map291_SHAPE_GRID:
	.incbin "games/tla/src/map/map_291/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map291_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map291_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map291_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set121Palette
Tileset_Set121Palette:
	.incbin "games/tla/src/graphics/tileset/set_121/palette.gbapal.lz"
	.global Tileset_Set121TilesA
Tileset_Set121TilesA:
	.incbin "games/tla/src/graphics/tileset/set_121/tiles_a.4bpp.mtf"
	.byte 0x26, 0x00, 0x00	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.global Tileset_Set121TilesB
Tileset_Set121TilesB:
	.incbin "games/tla/src/graphics/tileset/set_121/tiles_b.4bpp.mtf"
	.byte 0x0f, 0xd8	@ PACKER: left in the buffer by Tileset_Set121TilesA+0x1772
	.global Tileset_Set121TilesC
Tileset_Set121TilesC:
	.incbin "games/tla/src/graphics/tileset/set_121/tiles_c.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Tileset_Set121TilesB+0x12bb
	.global Tileset_Set121TilesD
Tileset_Set121TilesD:
	.incbin "games/tla/src/graphics/tileset/set_119/tiles_d.4bpp.mtf"
	.byte 0xe5, 0x3d	@ PACKER: left in the buffer by Tileset_Set121TilesC+0x842
