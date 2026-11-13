	.include "games/tbs/include/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_CrossboneIsleCave
Field_CrossboneIsleCave:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 120, 120, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_CELL_PICTURES
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_SHAPES
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_GRID
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_ANIMATION
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_BLEND
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_MARKERS
.LField_CrossboneIsleCave_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_cave/cell_pictures_ja.delta1.lz"
	.balign 4, 0
.LField_CrossboneIsleCave_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_cave/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleCave_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_cave/map_ja.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleCave_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_first_water_beast/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleCave_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_CrossboneIsleCave_MARKERS:
	treasure_spot 0, 0, 100
	treasure_spot 0, 1, 100
	treasure_spots_end
	.else
	map_header 0, 0, 120, 120, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_CELL_PICTURES
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_SHAPES
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_GRID
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_ANIMATION
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_BLEND
	map_offset Field_CrossboneIsleCave, .LField_CrossboneIsleCave_MARKERS
.LField_CrossboneIsleCave_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_cave/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_CrossboneIsleCave_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_cave/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleCave_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_cave/map.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleCave_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_first_water_beast/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleCave_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_CrossboneIsleCave_MARKERS:
	treasure_spot 0, 0, 100
	treasure_spot 0, 1, 100
	treasure_spots_end
	.endif
	.balign 4, 0
	.global Tileset_PaletteCrossboneIsleCave
Tileset_PaletteCrossboneIsleCave:
	.incbin "games/tbs/src/graphics/tileset/palettes/crossbone_isle_cave.gbapal.lz"
	.ifdef TBS_EDITION_JA
	.byte 0x5c	@ PACKER: left in the buffer by Field_CrossboneIsleCave+0x1a7
	.else
	.byte 0xc8	@ PACKER: left in the buffer by Field_CrossboneIsleCave+0x1a7
	.endif
