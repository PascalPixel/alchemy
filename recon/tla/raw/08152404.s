.syntax unified
	.thumb
	.global Func_08152404
	.thumb_func
Func_08152404:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	mov r8, r0
	mov r9, r1
	ldr r7, [r3, #92]
	movs r6, #0
	cmp r0, #0
	beq .L_08152462
	movs r3, #3
	ands r2, r3
	movs r5, #238
	movs r1, #13
	lsls r2, r2, #2
	lsls r5, r5, #7
	negs r1, r1
	mov r10, r2
	adds r5, #220
	mov r11, r1
.L_08152436:
	mov r0, r9
	bl Func_08020040
	str r0, [r5, r7]
	cmp r0, #0
	beq .L_0815245a
	movs r3, #0
	strb r3, [r0, #26]
	adds r1, r6, #0
	bl Animation_ApplyChildArgumentFar
	ldr r2, [r5, r7]
	mov r1, r11
	ldrb r3, [r2, #9]
	ands r3, r1
	mov r1, r10
	orrs r3, r1
	strb r3, [r2, #9]
.L_0815245a:
	adds r6, #1
	adds r5, #4
	cmp r6, r8
	bne .L_08152436
.L_08152462:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
