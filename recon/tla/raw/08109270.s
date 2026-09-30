.syntax unified
	.thumb
	.global Func_08109270
	.thumb_func
Func_08109270:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	cmp r5, #0
	beq .L_0810928a
	bl RenderOutput_RedrawSavedRectFar
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_0810928a:
	pop {r5, r6, pc}
