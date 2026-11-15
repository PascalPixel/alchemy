	.global Field_YallamInteriors2
Field_YallamInteriors2:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_YallamInteriors2, .LField_YallamInteriors2_CELL_PICTURES
	map_block Field_YallamInteriors2, .LField_YallamInteriors2_SHAPES
	map_block Field_YallamInteriors2, .LField_YallamInteriors2_GRID
	map_block Field_YallamInteriors2, .LField_YallamInteriors2_SHAPE_GRID
	map_block Field_YallamInteriors2, .LField_YallamInteriors2_ANIMATION
	map_block Field_YallamInteriors2, .LField_YallamInteriors2_BLEND
	map_block Field_YallamInteriors2, .LField_YallamInteriors2_SCENE
.LField_YallamInteriors2_CELL_PICTURES:
	.incbin "games/tla/src/map/yallam_interiors_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_YallamInteriors2_SHAPES:
	.incbin "games/tla/src/map/yallam_interiors_2/shapes.table.lz"
	.balign 4, 0
.LField_YallamInteriors2_GRID:
	.incbin "games/tla/src/map/yallam_interiors_2/map.grid.lz"
	.balign 4, 0
.LField_YallamInteriors2_SHAPE_GRID:
	.incbin "games/tla/src/map/yallam_interiors_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_YallamInteriors2_ANIMATION:
	.incbin "games/tla/src/map/madra_interiors_2/animation.script.lz"
	.balign 4, 0
.LField_YallamInteriors2_BLEND:
	.incbin "games/tla/src/map/daila_interiors_1/blend.script.lz"
	.balign 4, 0
.LField_YallamInteriors2_SCENE:
	.incbin "games/tla/src/map/yallam_interiors_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteYallamInteriors2A
Tileset_PaletteYallamInteriors2A:
	.incbin "games/tla/src/graphics/tileset/palettes/yallam_interiors_2_a.gbapal.lz"
	.byte 0xf6, 0x1c, 0xf7	@ PACKER: left in the buffer by Field_YallamInteriors2+0x199
	.global Tileset_PaletteYallamInteriors2B
Tileset_PaletteYallamInteriors2B:
	.incbin "games/tla/src/graphics/tileset/palettes/yallam_interiors_2_b.gbapal.lz"
	.byte 0x44	@ PACKER: left in the buffer by Tileset_PaletteYallamInteriors2A+0x15b
