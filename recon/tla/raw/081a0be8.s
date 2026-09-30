.syntax unified
	.thumb
	.global Func_081a0be8
	.thumb_func
Func_081a0be8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r6, r2, #0
	sub sp, #32
	mov r8, r1
	bl Runtime_BumpAllocateAlternatePool
	movs r1, #192
	adds r7, r0, #0
	ldr r3, .L_081a0ca0
	lsls r1, r1, #4
	mov lr, r3
	.2byte 0xf800
	adds r1, r7, #0
	adds r2, r6, #0
	adds r0, r5, #0
	bl UiIcon_CopyResourceToSlotFar + 0x8
	movs r6, #192
	mov r10, r0
	lsls r6, r6, #3
	adds r0, r7, #0
	adds r1, r7, #0
.L_081a0c20:
	ldrb r3, [r1, #1]
	ldrb r2, [r1]
	lsls r3, r3, #4
	orrs r2, r3
	subs r6, #1
	strb r2, [r0]
	adds r1, #2
	adds r0, #1
	cmp r6, #0
	bne .L_081a0c20
	ldr r3, .L_081a0ca4
	movs r6, #0
	mov r12, r3
	mov r3, r8
	lsls r5, r3, #5
	adds r1, r7, #0
.L_081a0c40:
	mov r3, r12
	adds r0, r5, r3
	adds r2, r1, #0
	movs r4, #7
.L_081a0c48:
	ldr r3, [r2]
	subs r4, #1
	adds r2, #96
	stmia r0!, {r3}
	cmp r4, #0
	bge .L_081a0c48
	adds r6, #1
	adds r5, #32
	adds r1, #4
	cmp r6, #23
	ble .L_081a0c40
	mov r3, r8
	lsls r1, r3, #5
	movs r3, #192
	lsls r3, r3, #2
	adds r5, r7, r3
	ldr r3, .L_081a0ca8
	movs r6, #0
	mov r12, r3
.L_081a0c6e:
	mov r3, r12
	adds r0, r1, r3
	adds r2, r5, #0
	movs r4, #7
.L_081a0c76:
	ldr r3, [r2]
	subs r4, #1
	adds r2, #96
	stmia r0!, {r3}
	cmp r4, #0
	bge .L_081a0c76
	adds r6, #1
	adds r1, #32
	adds r5, #4
	cmp r6, #23
	ble .L_081a0c6e
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r10
	add sp, #32
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081a0ca0:
	.4byte IwramClearWords
.L_081a0ca4:
	.4byte 0x06010000
.L_081a0ca8:
	.4byte 0x06010300
