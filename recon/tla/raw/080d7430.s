.syntax unified
	.thumb
	.global Func_080d7430
	.thumb_func
Func_080d7430:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	mov r10, r1
	movs r1, #248
	lsls r1, r1, #5
	adds r6, r0, #0
	adds r1, #136
	movs r0, #120
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r3, [r3]
	adds r5, r0, #0
	mov r8, r3
	mov r0, sp
	movs r3, #0
	mov r9, r3
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r5, #0
	ldr r2, .L_080d74dc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r6, #0
	mov r1, r8
	adds r2, r5, #0
	movs r3, #1
	bl Func_080d0e1c
	movs r3, #168
	lsls r3, r3, #4
	adds r6, r5, r3
	mov r0, r10
	mov r1, r8
	adds r2, r6, #0
	movs r3, #1
	bl Func_080d0e1c
	movs r3, #168
	lsls r3, r3, #5
	adds r2, r5, r3
	adds r0, r6, #0
	adds r1, r5, #0
	movs r3, #12
	bl Func_080d0c50
	movs r3, #224
	lsls r3, r3, #4
	add r8, r3
	adds r0, r5, #0
	mov r2, r8
	movs r1, #0
	movs r3, #1
	bl Func_080d0e1c
	movs r3, #252
	lsls r3, r3, #5
	adds r2, r5, r3
	movs r3, #120
	strh r3, [r2]
	movs r3, #248
	lsls r3, r3, #5
	adds r3, #130
	adds r5, r5, r3
	movs r1, #144
	mov r3, r9
	strh r3, [r5]
	lsls r1, r1, #3
	ldr r0, .L_080d74e0
	bl Func_080145a8
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_080d74dc:
	.4byte 0x850007e2
.L_080d74e0:
	.4byte Func_080d67e8
