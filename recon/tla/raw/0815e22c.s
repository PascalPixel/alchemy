.syntax unified
	.thumb
	.global Func_0815e22c
	.thumb_func
Func_0815e22c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #92]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #240
	adds r5, r5, r1
	ldr r3, [r5]
	sub sp, #24
	add r6, sp, #12
	adds r1, r6, #0
	mov r8, r0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_0815e21c
	ldr r2, [r5]
	mov r5, sp
	ldr r3, [r2, #20]
	lsls r3, r3, #1
	adds r3, #34
	ldrsh r0, [r2, r3]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r1, [r6]
	ldr r3, [r5]
	add sp, #24
	subs r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r2, r8
	adds r1, r1, r3
	str r1, [r2]
	ldr r3, [r6, #4]
	str r3, [r2, #4]
	ldr r3, [r6, #8]
	str r3, [r2, #8]
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
