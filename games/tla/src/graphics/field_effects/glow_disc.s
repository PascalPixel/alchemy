@ A 64x64 disc of 256 colours, shaded in colours 224 to 255: the 4,096
@ bytes of the shared effect scene's sprite canvas (battle_fx_work[1]),
@ which Func_080eb824 copies to OBJ video memory. Func_080e572c, 080e5d5c,
@ 080e7844 and overlay 685 unpack it there. In Func_080e572c the callback
@ Func_080e4d9c writes 32 colours it computes from the sine table to OBJ
@ palettes 14 and 15 each frame, from a counter the scene starts at 0; the
@ picture carries the shared OBJ palette and those colours of the first
@ frame.
	.section .rodata
	.global FieldEffect_GlowDisc
FieldEffect_GlowDisc:
	.incbin "games/tla/src/graphics/field_effects/glow_disc.8bpp.lz"
