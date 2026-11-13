	.global Field_MadraInteriors
Field_MadraInteriors:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraInteriors, .LField_MadraInteriors_CELL_PICTURES
	map_block Field_MadraInteriors, .LField_MadraInteriors_SHAPES
	map_block Field_MadraInteriors, .LField_MadraInteriors_GRID
	map_block Field_MadraInteriors, .LField_MadraInteriors_SHAPE_GRID
	map_block Field_MadraInteriors, .LField_MadraInteriors_ANIMATION
	map_block Field_MadraInteriors, .LField_MadraInteriors_BLEND
	map_block Field_MadraInteriors, .LField_MadraInteriors_SCENE
.LField_MadraInteriors_CELL_PICTURES:
	.incbin "games/tla/src/map/madra_interiors/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MadraInteriors_SHAPES:
	.incbin "games/tla/src/map/madra_interiors/shapes.table.lz"
	.balign 4, 0
.LField_MadraInteriors_GRID:
	.incbin "games/tla/src/map/madra_interiors/map.grid.lz"
	.balign 4, 0
.LField_MadraInteriors_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_interiors/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraInteriors_ANIMATION:
	.incbin "games/tla/src/map/madra_interiors/animation.script.lz"
	.balign 4, 0
.LField_MadraInteriors_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_MadraInteriors_SCENE:
	.incbin "games/tla/src/map/madra_interiors/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set23Palette
Tileset_Set23Palette:
	.incbin "games/tla/src/graphics/tileset/set_23/palette.gbapal.lz"
	.global Tileset_Set23TilesA
Tileset_Set23TilesA:
	.incbin "games/tla/src/graphics/tileset/set_23/tiles_a.4bpp.mtf"
	.byte 0x9e	@ PACKER: left in the buffer by Field_MadraInteriors+0x1bdf
	.global Tileset_Set23TilesB
Tileset_Set23TilesB:
	.incbin "games/tla/src/graphics/tileset/set_23/tiles_b.4bpp.mtf"
	.byte 0x62, 0xf8, 0x54	@ PACKER: left in the buffer by Field_MadraInteriors+0x1e1d
	.global Tileset_Set23TilesC
Tileset_Set23TilesC:
	.incbin "games/tla/src/graphics/tileset/set_23/tiles_c.4bpp.mtf"
	.byte 0x69, 0xd7, 0x36	@ PACKER: left in the buffer by Tileset_Set23TilesB+0x1ad9
	.global Tileset_Set23TilesD
Tileset_Set23TilesD:
	.incbin "games/tla/src/graphics/tileset/set_23/tiles_d.4bpp.mtf"
	.byte 0x0c, 0xbc	@ PACKER: left in the buffer by Tileset_Set23TilesC+0x15e6
