.syntax unified
	.thumb
	.global Func_0802bd08
	.thumb_func
Func_0802bd08:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r0, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r6, #0
	mov r8, r3
	ldr r3, [r3]
	sub sp, #4
	adds r4, r2, #0
	mov r12, r6
	cmp r3, #0
	beq .L_0802bd2e
	ldmia r3!, {r6}
	ldr r3, [r3, #4]
	mov r12, r3
.L_0802bd2e:
	asrs r1, r1, #4
	lsls r3, r1, #5
	mov r2, r12
	asrs r5, r5, #4
	asrs r0, r0, #3
	asrs r4, r4, #3
	adds r7, r3, r5
	asrs r2, r2, #24
	asrs r3, r0, #31
	mov r12, r2
	asrs r5, r4, #31
	lsrs r2, r3, #31
	mov lr, r3
	adds r2, r0, r2
	lsrs r3, r5, #31
	movs r1, #31
	asrs r2, r2, #1
	adds r3, r4, r3
	ands r2, r1
	asrs r3, r3, #1
	ands r3, r1
	lsls r2, r2, #5
	adds r2, r2, r3
	movs r3, #164
	lsls r3, r3, #1
	lsls r2, r2, #1
	adds r2, r2, r3
	asrs r6, r6, #24
	mov r3, r8
	strh r7, [r3, r2]
	subs r3, r6, r4
	cmp r3, #0
	blt .L_0802bd76
	cmp r3, #1
	ble .L_0802bd7c
	b .L_0802bdc6
.L_0802bd76:
	subs r3, r4, r6
	cmp r3, #1
	bgt .L_0802bdc6
.L_0802bd7c:
	mov r2, r12
	subs r3, r2, r0
	cmp r3, #0
	blt .L_0802bd8a
	cmp r3, #1
	ble .L_0802bd92
	b .L_0802bdc6
.L_0802bd8a:
	mov r2, r12
	subs r3, r0, r2
	cmp r3, #1
	bgt .L_0802bdc6
.L_0802bd92:
	mov r3, lr
	lsrs r6, r5, #31
	lsrs r5, r3, #31
	adds r6, r4, r6
	adds r5, r0, r5
	movs r2, #1
	asrs r6, r6, #1
	asrs r5, r5, #1
	mov r8, r2
	str r2, [sp, #0]
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	movs r0, #0
	bl Func_0802b878
	movs r2, #128
	lsls r2, r2, #3
	adds r3, r7, r2
	mov r2, r8
	str r2, [sp, #0]
	movs r0, #1
	adds r1, r6, #0
	adds r2, r5, #0
	bl Func_0802b878
.L_0802bdc6:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
