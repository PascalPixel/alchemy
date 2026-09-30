.syntax unified
	.thumb
	.global Func_081091cc
	.thumb_func
Func_081091cc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	sub sp, #4
	mov r8, r1
	adds r7, r2, #0
	adds r5, r3, #0
	cmp r6, #0
	bne .L_08109220
	b .L_08109252
.L_081091e2:
	ldr r0, .L_0810925c
	adds r1, r6, #0
	movs r2, #0
	b .L_08109218
.L_081091ea:
	ldr r0, .L_08109260
	adds r1, r6, #0
	movs r2, #0
	b .L_08109218
.L_081091f2:
	ldr r5, .L_08109264
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	movs r3, #8
	str r3, [sp, #0]
	adds r0, r7, #0
	movs r1, #5
	adds r2, r6, #0
	movs r3, #32
	subs r5, #3
	bl UiText_DrawNumberInWindowFar
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #72
.L_08109218:
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	b .L_08109252
.L_08109220:
	adds r0, r6, #0
	bl RenderOutput_RedrawSavedRectFar
	ldr r0, .L_08109268
	adds r1, r6, #0
	add r0, r8
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	cmp r7, #0
	bne .L_08109240
	cmp r5, #1
	beq .L_081091e2
	cmp r5, #2
	beq .L_081091ea
.L_08109240:
	cmp r5, #3
	bne .L_081091f2
	ldr r0, .L_0810926c
	adds r1, r6, #0
	adds r0, r7, r0
	movs r2, #8
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_08109252:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0810925c:
	.4byte 0x0000123f
.L_08109260:
	.4byte 0x00001240
.L_08109264:
	.4byte 0x00001238
.L_08109268:
	.4byte 0x0000025f
.L_0810926c:
	.4byte 0x00001249
