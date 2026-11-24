	.global Field_ProxMold
Field_ProxMold:
	map_header 0, 0, 90, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 66, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ProxMold, .LField_ProxMold_CELL_TILES
	map_block Field_ProxMold, .LField_ProxMold_SHAPES
	map_block Field_ProxMold, .LField_ProxMold_GRID
	map_block Field_ProxMold, .LField_ProxMold_SHAPE_GRID
	map_block Field_ProxMold, .LField_ProxMold_ANIMATION
	map_block Field_ProxMold, .LField_ProxMold_BLEND
	map_block Field_ProxMold, .LField_ProxMold_SCENE
.LField_ProxMold_CELL_TILES:
	.incbin "games/tla/src/map/prox_mold/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ProxMold_SHAPES:
	.incbin "games/tla/src/map/prox_mold/shapes.table.lz"
	.balign 4, 0
.LField_ProxMold_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/prox_mold/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/prox_mold/map.grid.lz"
	.endif
	.balign 4, 0
.LField_ProxMold_SHAPE_GRID:
	.incbin "games/tla/src/map/prox_mold/shape_grid.plane.lz"
	.balign 4, 0
.LField_ProxMold_ANIMATION:
	.incbin "games/tla/src/map/prox_mold/animation.script.lz"
	.balign 4, 0
.LField_ProxMold_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ProxMold_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/prox_mold/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/prox_mold/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_PaletteProxMold
Tileset_PaletteProxMold:
	.incbin "games/tla/src/graphics/tileset/palettes/prox_mold.gbapal.lz"
	.byte 0x98, 0x2d	@ PACKER: left in the buffer by Field_Prox3+0x162
