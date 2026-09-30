.syntax unified
	.thumb
	.global Func_080dbe08
	.thumb_func
Func_080dbe08:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	mov r9, r3
	adds r6, r0, #0
	mov r8, r1
	mov r10, r2
	bl Func_080dbde8
	mov r1, r8
	adds r5, r0, #0
	mov r2, r10
	adds r0, r6, #0
	bl Func_080dbb78
	mov r3, r9
	orrs r5, r3
	strb r5, [r0, #3]
	lsls r5, r5, #24
	lsrs r5, r5, #24
	adds r0, r5, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
	.2byte 0x0000
