	.global Field_Yallam
Field_Yallam:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Yallam, .LField_Yallam_CELL_PICTURES
	map_block Field_Yallam, .LField_Yallam_SHAPES
	map_block Field_Yallam, .LField_Yallam_GRID
	map_block Field_Yallam, .LField_Yallam_SHAPE_GRID
	map_block Field_Yallam, .LField_Yallam_ANIMATION
	map_block Field_Yallam
	map_block Field_Yallam, .LField_Yallam_SCENE
.LField_Yallam_CELL_PICTURES:
	.incbin "games/tla/src/map/yallam/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Yallam_SHAPES:
	.incbin "games/tla/src/map/yallam/shapes.table.lz"
	.balign 4, 0
.LField_Yallam_GRID:
	.incbin "games/tla/src/map/yallam/map.grid.lz"
	.balign 4, 0
.LField_Yallam_SHAPE_GRID:
	.incbin "games/tla/src/map/yallam/shape_grid.plane.lz"
	.balign 4, 0
.LField_Yallam_ANIMATION:
	.incbin "games/tla/src/map/yallam/animation.script.lz"
	.balign 4, 0
.LField_Yallam_SCENE:
	.incbin "games/tla/src/map/yallam/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteYallam
Tileset_PaletteYallam:
	.incbin "games/tla/src/graphics/tileset/palettes/yallam.gbapal.lz"
