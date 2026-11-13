	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_TretTreeBranch
Field_TretTreeBranch:
	map_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_TretTreeBranch, .LField_TretTreeBranch_CELL_PICTURES
	map_offset Field_TretTreeBranch, .LField_TretTreeBranch_SHAPES
	map_offset Field_TretTreeBranch, .LField_TretTreeBranch_GRID
	map_offset Field_TretTreeBranch, .LField_TretTreeBranch_ANIMATION
	map_offset Field_TretTreeBranch, .LField_TretTreeBranch_BLEND
	map_offset Field_TretTreeBranch, .LField_TretTreeBranch_MARKERS
.LField_TretTreeBranch_CELL_PICTURES:
	.incbin "games/tbs/src/map/tret_tree_branch/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TretTreeBranch_SHAPES:
	.incbin "games/tbs/src/map/tret_tree_branch/shapes.table.lz"
	.balign 4, 0
.LField_TretTreeBranch_GRID:
	.incbin "games/tbs/src/map/tret_tree_branch/map.grid.lz"
	.balign 4, 0
.LField_TretTreeBranch_ANIMATION:
	.incbin "games/tbs/src/map/kolima/tileset_anims.script.lz"
	.balign 4, 0
.LField_TretTreeBranch_BLEND:
	.incbin "games/tbs/src/map/tret_tree_branch/blend.script.lz"
	.balign 4, 0
.LField_TretTreeBranch_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_TretTreeBranchPalette
Tileset_TretTreeBranchPalette:
	.incbin "games/tbs/src/graphics/tileset/tret_tree_branch/palette.gbapal.lz"
	.byte 0x9b, 0x46	@ PACKER: left in the buffer by Field_TretTreeBranch+0x16a
	.global Tileset_TretTreeBranchTilesA
Tileset_TretTreeBranchTilesA:
	.incbin "games/tbs/src/graphics/tileset/tret_tree_branch/tiles_a.4bpp.mtf"
	.byte 0xe0, 0x91, 0x0f	@ PACKER: left in the buffer by Tileset_TreeRoomsTilesB+0x1d59
	.global Tileset_TretTreeBranchTilesB
Tileset_TretTreeBranchTilesB:
	.incbin "games/tbs/src/graphics/tileset/tret_tree_branch/tiles_b.4bpp.mtf"
	.byte 0x2f	@ PACKER: left in the buffer by Tileset_TretTreeBranchTilesA+0x13f
	.global Tileset_TretTreeBranchTilesC
Tileset_TretTreeBranchTilesC:
	.incbin "games/tbs/src/graphics/tileset/tret_tree_branch/tiles_c.4bpp.mtf"
	.byte 0x23, 0xe7	@ PACKER: left in the buffer by Tileset_TreeRoomsTilesB+0x27fe
	.global Tileset_TretTreeBranchTilesD
Tileset_TretTreeBranchTilesD:
	.incbin "games/tbs/src/graphics/tileset/tret_tree_branch/tiles_d.4bpp.mtf"
	.byte 0x09	@ PACKER: left in the buffer by Tileset_TretTreeBranchTilesC+0x13f
