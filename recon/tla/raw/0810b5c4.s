.syntax unified
	.thumb
	.global Func_0810b5c4
	.thumb_func
Func_0810b5c4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	adds r5, r0, #0
	bl Func_08108148
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #6
	adds r3, r6, r1
	movs r1, #1
	mov r8, r1
	movs r2, #0
	mov r9, r2
	mov r2, r8
	strb r2, [r3]
	movs r3, #129
	lsls r3, r3, #3
	adds r3, #255
	adds r2, r6, r3
	movs r3, #4
	strb r3, [r2]
	adds r0, r5, #0
	bl Object_GetByIdFar
	ldr r3, [r0, #80]
	movs r1, #128
	ldr r3, [r3, #40]
	lsls r1, r1, #3
	ldrh r2, [r3]
	adds r1, #250
	adds r3, r6, r1
	strh r2, [r3]
	ldr r1, .L_0810b654
	ldrh r0, [r3]
	mov r10, r1
	movs r2, #0
	movs r1, #0
	movs r3, #0
	bl Func_080380f8
	movs r2, #128
	lsls r2, r2, #3
	adds r2, #236
	adds r3, r6, r2
	mov r11, r0
	movs r1, #128
	ldrh r0, [r3]
	lsls r1, r1, #23
	mov r3, r9
	mov r2, r11
	str r3, [sp, #0]
	bl RenderOutput_CreateFar
	movs r3, #255
	adds r5, r0, #0
	strb r3, [r5, #15]
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #220
	mov r1, r8
	b .L_0810b658
	.2byte 0x0000
.L_0810b654:
	.4byte 0x00000000
.L_0810b658:
	adds r6, r6, r3
	strb r1, [r5, #5]
	mov r2, r10
	movs r1, #32
	negs r1, r1
	strb r2, [r5, #4]
	adds r0, r6, #0
	movs r2, #112
	mov r8, r1
	bl Func_08108aa8
	ldr r7, .L_0810b6b8
	str r5, [r6]
	adds r0, r7, #0
	bl Func_081084f4
	adds r0, r7, #1
	bl Func_081084f4
	bl Func_0810b6bc
	movs r2, #112
	adds r5, r0, #0
	mov r1, r8
	adds r0, r6, #0
	bl Func_08108aa8
	movs r2, #1
	negs r2, r2
	cmp r5, r2
	bne .L_0810b69c
	adds r0, r7, #2
	bl Func_081084f4
.L_0810b69c:
	mov r0, r11
	movs r1, #2
	bl UiWork_FinalizeFar
	bl Func_0810824c
	movs r0, #0
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0810b6b8:
	.4byte 0x000012fd
