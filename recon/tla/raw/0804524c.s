.syntax unified
	.thumb
	.global Func_0804524c
	.thumb_func
Func_0804524c:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	lsls r5, r3, #1
	movs r3, #243
	lsls r3, r3, #8
	mov r8, r1
	adds r3, #21
	movs r1, #128
	mov r9, r2
	adds r3, r3, r5
	lsls r1, r1, #3
	sub sp, #4
	movs r6, #0
	mov r11, r3
	orrs r1, r3
	mov r2, r8
	mov r3, r9
	str r6, [sp, #0]
	mov r10, r0
	bl Func_0803c378
	movs r3, #243
	lsls r3, r3, #8
	adds r3, #20
	adds r5, r5, r3
	mov r2, r8
	mov r0, r10
	adds r1, r5, #0
	mov r3, r9
	adds r2, #1
	str r6, [sp, #0]
	bl Func_0803c378
	movs r3, #2
	add r8, r3
	mov r0, r10
	mov r1, r11
	mov r2, r8
	mov r3, r9
	str r6, [sp, #0]
	bl Func_0803c378
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
	.2byte 0x0000
