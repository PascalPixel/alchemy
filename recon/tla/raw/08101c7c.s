.syntax unified
	.thumb
	.global Func_08101c7c
	.thumb_func
Func_08101c7c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r3, #0
	ldr r3, [sp, #32]
	adds r6, r2, #0
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	mov r7, r10
	lsls r7, r7, #12
	mov r8, r3
	mov r10, r7
	cmp r0, #0
	bge .L_08101ca8
	adds r6, r6, r0
	movs r0, #0
.L_08101ca8:
	adds r3, r0, r6
	cmp r3, #29
	ble .L_08101cb2
	movs r3, #30
	subs r6, r3, r0
.L_08101cb2:
	cmp r1, #0
	bge .L_08101cba
	adds r5, r5, r1
	movs r1, #0
.L_08101cba:
	adds r3, r1, r5
	cmp r3, #29
	ble .L_08101cc4
	movs r3, #20
	subs r5, r3, r1
.L_08101cc4:
	cmp r6, #0
	ble .L_08101d24
	cmp r5, #0
	ble .L_08101d24
	lsls r3, r0, #1
	lsls r2, r1, #6
	add r3, r8
	adds r2, r2, r3
	movs r0, #2
	mov r12, r2
	mov r9, r0
.L_08101cda:
	mov r0, r12
	adds r4, r6, #0
	adds r0, #8
	cmp r4, #0
	beq .L_08101d0a
	ldr r7, .L_08101d30
	movs r3, #15
	mov lr, r3
	mov r11, r7
.L_08101cec:
	ldrh r2, [r0]
	mov r7, lr
	lsrs r3, r2, #12
	ands r3, r7
	cmp r3, #15
	bne .L_08101d02
	mov r3, r11
	ands r2, r3
	mov r7, r10
	orrs r2, r7
	strh r2, [r0]
.L_08101d02:
	subs r4, #1
	adds r0, #2
	cmp r4, #0
	bne .L_08101cec
.L_08101d0a:
	lsrs r3, r1, #2
	mov r0, r8
	mov r2, r9
	lsls r2, r3
	ldrb r3, [r0, #3]
	orrs r2, r3
	strb r2, [r0, #3]
	subs r5, #1
	movs r3, #64
	add r12, r3
	adds r1, #1
	cmp r5, #0
	bne .L_08101cda
.L_08101d24:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08101d30:
	.4byte 0xffff0fff
