.syntax unified
	.thumb
	.global Func_08040cc4
	.thumb_func
Func_08040cc4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r3, [r3]
	sub sp, #4
	mov r8, r3
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #6
	movs r1, #2
	movs r2, #18
	movs r3, #16
	bl UiWindow_Create
	movs r5, #3
	adds r7, r0, #0
	movs r6, #3
.L_08040cec:
	adds r2, r5, #0
	adds r0, r7, #0
	movs r1, #0
	movs r3, #17
	subs r6, #1
	str r5, [sp, #0]
	bl UiWindow_DrawDividerLine
	adds r5, #3
	cmp r6, #0
	bge .L_08040cec
	ldr r5, .L_08040d6c
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #8
	movs r3, #4
	bl UiText_DrawResource
	adds r0, r5, #1
	adds r1, r7, #0
	movs r2, #8
	movs r3, #28
	bl UiText_DrawResource
	adds r0, r5, #2
	adds r1, r7, #0
	movs r2, #8
	movs r3, #52
	bl UiText_DrawResource
	adds r0, r5, #3
	adds r1, r7, #0
	movs r2, #8
	movs r3, #76
	adds r5, #4
	bl UiText_DrawResource
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #8
	movs r3, #100
	bl UiText_DrawResource
	bl Func_08044460
	movs r1, #128
	movs r6, #0
	lsls r1, r1, #23
	adds r2, r7, #0
	movs r3, #0
	str r6, [sp, #0]
	bl RenderOutput_Create
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #164
	add r3, r8
	str r0, [r3]
	add sp, #4
	adds r0, r7, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08040d6c:
	.4byte 0x0000115b
