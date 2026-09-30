.syntax unified
	.thumb
	.global Func_08109188
	.thumb_func
Func_08109188:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #4
	ldr r5, [r3, #12]
	cmp r5, #0
	beq .L_081091be
	adds r0, r5, #0
	bl RenderOutput_PrepareForRedrawFar
	ldr r0, .L_081091c4
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r3, .L_081091c8
	movs r1, #6
	ldr r0, [r3, #16]
	movs r3, #8
	str r3, [sp, #0]
	adds r2, r5, #0
	movs r3, #32
	bl UiText_DrawNumberInWindowFar
.L_081091be:
	add sp, #4
	pop {r5, pc}
	.2byte 0x0000
.L_081091c4:
	.4byte 0x00001237
.L_081091c8:
	.4byte gPartyState
