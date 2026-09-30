.syntax unified
	.thumb
	.global Func_08041068
	.thumb_func
Func_08041068:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r3, [r3]
	sub sp, #4
	mov r9, r3
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #0
	movs r3, #19
	movs r1, #0
	movs r2, #30
	bl UiWindow_Create
	movs r3, #4
	ldr r7, .L_080410f4
	mov r10, r3
	movs r3, #0
	adds r6, r0, #0
	mov r8, r3
	movs r5, #2
	b .L_080410b2
.L_0804109e:
	adds r2, r5, #0
	movs r3, #29
	adds r0, r6, #0
	movs r1, #0
	str r5, [sp, #0]
	bl UiWindow_DrawDividerLine
	movs r3, #1
	adds r5, #2
	add r8, r3
.L_080410b2:
	mov r3, r10
	ldr r0, [r7]
	adds r1, r6, #0
	movs r2, #8
	bl UiText_DrawStringInWindow
	movs r3, #16
	add r10, r3
	mov r3, r8
	adds r7, #8
	cmp r3, #8
	bne .L_0804109e
	bl Func_08044460
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	adds r2, r6, #0
	str r3, [sp, #0]
	bl RenderOutput_Create
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #164
	add r3, r9
	str r0, [r3]
	add sp, #4
	adds r0, r6, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080410f4:
	.4byte Data_080aa128
