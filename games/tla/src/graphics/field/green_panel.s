@ One 64x64 sprite of 16 colours: a green panel in a window frame, drawn in
@ its top-left 32x32. Overlay 6b3's Func_02000098 unpacks it into one block
@ of OBJ video memory, makes one 64x64 sprite of it with the 256-colour bit
@ cleared and OBJ palette 14, and copies Data_02000874 to that palette: the
@ window palette's 16 colours, which the picture carries.
	.section .rodata
	.global Field_GreenPanel
Field_GreenPanel:
	.incbin "games/tla/src/graphics/field/green_panel.4bpp.lz"
	.byte 0x0a	@ PACKER: left in the buffer by FieldFx_FireColumn+0xaf
