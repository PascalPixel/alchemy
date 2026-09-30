.syntax unified
	.thumb
	.global Func_080fa3d4
	.thumb_func
Func_080fa3d4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	movs r3, #168
	mov r6, r8
	sub sp, #4
	adds r7, r1, #0
	movs r5, #0
	mov r10, r3
	adds r6, #76
.L_080fa3ec:
	mov r3, r10
	str r3, [sp, #0]
	adds r1, r5, #0
	movs r0, #2
	adds r2, r7, #0
	movs r3, #248
	bl RenderOutput_CreateFromResourceFar
	adds r5, #1
	stmia r6!, {r0}
	cmp r5, #7
	ble .L_080fa3ec
	movs r3, #168
	mov r6, r8
	movs r5, #8
	mov r10, r3
	adds r6, #108
.L_080fa40e:
	mov r3, r10
	str r3, [sp, #0]
	movs r3, #128
	adds r1, r5, #0
	movs r0, #2
	adds r2, r7, #0
	lsls r3, r3, #1
	bl RenderOutput_CreateFromResourceFar
	adds r5, #1
	stmia r6!, {r0}
	cmp r5, #15
	ble .L_080fa40e
	movs r3, #168
	mov r6, r8
	movs r5, #16
	mov r10, r3
	adds r6, #140
.L_080fa432:
	mov r3, r10
	str r3, [sp, #0]
	movs r3, #128
	adds r1, r5, #0
	movs r0, #2
	adds r2, r7, #0
	lsls r3, r3, #1
	bl RenderOutput_CreateFromResourceFar
	adds r5, #1
	stmia r6!, {r0}
	cmp r5, #31
	ble .L_080fa432
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
