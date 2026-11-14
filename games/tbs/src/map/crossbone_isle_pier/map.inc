	.global Field_CrossboneIslePier
Field_CrossboneIslePier:
	map_header 0, 0, 34, 40, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIslePier, .LField_CrossboneIslePier_CELL_PICTURES
	map_offset Field_CrossboneIslePier, .LField_CrossboneIslePier_SHAPES
	map_offset Field_CrossboneIslePier, .LField_CrossboneIslePier_GRID
	map_offset Field_CrossboneIslePier, .LField_CrossboneIslePier_ANIMATION
	map_offset Field_CrossboneIslePier
	map_offset Field_CrossboneIslePier, .LField_CrossboneIslePier_MARKERS
.LField_CrossboneIslePier_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_pier/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_CrossboneIslePier_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_pier/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIslePier_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_pier/map.grid.lz"
	.balign 4, 0
.LField_CrossboneIslePier_ANIMATION:
	.incbin "games/tbs/src/map/kalay_docks/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIslePier_MARKERS:
	treasure_spot 53, 56, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteCrossboneIslePier
Tileset_PaletteCrossboneIslePier:
	.incbin "games/tbs/src/graphics/tileset/palettes/crossbone_isle_pier.gbapal.lz"
	.byte 0x50, 0x42, 0x7d	@ PACKER: left in the buffer by Field_CrossboneIslePier+0x171
	.balign 4, 0
