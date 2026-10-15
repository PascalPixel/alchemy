@ Eight frames of a 16x16 explosion of 16 colours, one under another.
@ Func_080e572c unpacks them into one block of OBJ video memory and makes 32
@ sprites of them, each with the 256-colour bit cleared and OBJ palette 15;
@ its callback Func_080e4d9c copies the 16 colours of Data_080f0fa0 to OBJ
@ palette 15, and the picture carries them. Overlays 685, 6a6 and 6a7 load
@ the sheet too.
	.section .rodata
	.global FieldFx_Explosion
FieldFx_Explosion:
	.incbin "games/tla/src/graphics/field_fx/explosion.4bpp16x16.lz"
