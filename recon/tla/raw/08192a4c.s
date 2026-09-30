.syntax unified
	.thumb
	.global Func_08192a4c
	.thumb_func
Func_08192a4c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #64
	mov r0, r9
	add r3, sp, #60
	str r0, [r3]
	adds r3, r0, #0
	movs r1, #0
	str r3, [sp, #36]
	str r1, [sp, #32]
	str r1, [sp, #12]
	ldr r1, [sp, #36]
	subs r3, #16
	mov r2, sp
	mov r9, r3
	adds r2, #40
	movs r3, #4
	movs r5, #24
	movs r0, #8
	subs r1, #24
	str r2, [sp, #28]
	str r3, [sp, #16]
	str r5, [sp, #8]
	str r0, [sp, #20]
	str r1, [sp, #24]
	add r7, sp, #48
.L_08192a8a:
	mov r2, r9
	ldr r4, [r2]
	ldr r3, [sp, #8]
	ldr r1, [r4, r3]
	cmp r1, #0
	bne .L_08192a98
	b .L_08192c4a
.L_08192a98:
	ldr r5, [sp, #12]
	movs r6, #15
	ldr r3, [r4, r5]
	ldr r5, [sp, #16]
	asrs r0, r3, #16
	ldr r2, [r4, r5]
	mov lr, r0
	asrs r0, r2, #16
	mov r12, r0
	movs r0, #254
	movs r5, #0
	lsls r0, r0, #15
	ands r6, r1
	mov r10, r5
	cmp r3, r0
	bhi .L_08192ac0
	cmp r2, #0
	blt .L_08192ac0
	cmp r2, r0
	ble .L_08192acc
.L_08192ac0:
	mov r0, r9
	ldr r2, [r0]
	ldr r1, [sp, #8]
	movs r3, #0
	str r3, [r2, r1]
	b .L_08192c4a
.L_08192acc:
	movs r3, #16
	ands r3, r1
	cmp r3, #0
	beq .L_08192b06
	ldr r3, [sp, #36]
	ldr r5, [sp, #20]
	subs r3, #12
	ldr r2, [r3]
	ldr r3, [r4, r5]
	subs r2, r2, r3
	cmp r2, #32
	ble .L_08192af4
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #7
	ldr r0, .L_08192c7c
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r0, r0, r3
	mov r10, r0
.L_08192af4:
	movs r1, #192
	lsls r1, r1, #5
	adds r1, #16
	cmp r10, r1
	bne .L_08192b06
	ldr r2, [sp, #8]
	movs r3, #0
	str r3, [r4, r2]
	b .L_08192c4a
.L_08192b06:
	mov r3, r9
	ldr r5, [r3]
	ldr r0, [sp, #8]
	movs r2, #128
	ldr r3, [r5, r0]
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08192bfc
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Func_081969f8
	movs r1, #0
	mov r2, r9
	ldr r3, [sp, #36]
	mov r10, r1
	ldr r5, [sp, #20]
	ldr r1, [r2]
	subs r3, #12
	ldr r2, [r3]
	ldr r3, [r1, r5]
	adds r6, r0, #0
	subs r2, r2, r3
	cmp r2, #24
	ble .L_08192b48
	ldr r0, .L_08192c80
	lsls r3, r2, #11
	adds r0, r0, r3
	mov r10, r0
.L_08192b48:
	ldr r3, [sp, #40]
	ldr r2, .L_08192c84
	movs r5, #248
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_08192c88
	ldr r0, [sp, #28]
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	lsls r5, r5, #5
	mov r2, r10
	str r3, [sp, #40]
	adds r5, #16
	adds r3, r1, r2
	adds r3, r3, r5
	str r3, [r0, #4]
	bl Func_08014ee0
	mov r2, r9
	ldr r5, [sp, #12]
	ldr r1, [r2]
	ldr r2, .L_08192c8c
	ldr r3, [r1, r5]
	movs r0, #0
	adds r3, r3, r2
	str r3, [r7]
	adds r3, r5, #0
	adds r3, #4
	ldr r3, [r1, r3]
	mov r11, r0
	adds r3, r3, r2
	str r3, [r7, #4]
	str r0, [r7, #8]
	adds r0, r7, #0
	bl Func_08015128
	mov r1, r9
	ldr r5, [sp, #12]
	ldr r3, [r1]
	adds r5, #20
	ldr r3, [r3, r5]
	mov r2, r11
	str r3, [r7]
	str r3, [r7, #4]
	str r2, [r7, #8]
	adds r0, r7, #0
	bl Func_080151ac
	mov r3, r9
	ldr r2, [r3]
	movs r0, #128
	ldr r3, [r2, r5]
	lsls r0, r0, #6
	adds r3, r3, r0
	str r3, [r2, r5]
	movs r3, #4
	str r3, [r6]
	ldr r1, [sp, #28]
	ldr r3, .L_08192c90
	mov r2, r8
	str r3, [r6, #8]
	str r1, [r6, #16]
	str r2, [r6, #12]
	mov r1, r8
	movs r2, #4
	ldr r0, .L_08192c94
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
	movs r3, #224
	lsls r3, r3, #6
	cmp r10, r3
	bne .L_08192c2a
	mov r5, r9
	ldr r3, [r5]
	ldr r0, [sp, #8]
	mov r1, r11
	str r1, [r3, r0]
	b .L_08192c2a
.L_08192bfc:
	ldr r3, [sp, #36]
	ldr r2, .L_08192c98
	lsls r4, r6, #1
	subs r3, #28
	ldr r0, [r3]
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	movs r2, #224
	add r1, r10
	adds r1, r5, r1
	lsls r2, r2, #3
	mov r3, lr
	mov r5, r12
	adds r1, r1, r2
	lsrs r2, r6, #1
	subs r2, r3, r2
	str r6, [sp, #0]
	subs r3, r5, r6
	str r4, [sp, #4]
	ldr r5, [sp, #24]
	ldr r4, [r5]
	mov lr, r4
	.2byte 0xf800
.L_08192c2a:
	ldr r2, [sp, #20]
	mov r0, r9
	ldr r1, [r0]
	ldr r5, [sp, #12]
	adds r2, #4
	ldr r2, [r1, r2]
	ldr r3, [r1, r5]
	adds r3, r3, r2
	str r3, [r1, r5]
	ldr r2, [sp, #20]
	ldr r0, [sp, #16]
	adds r2, #8
	ldr r3, [r1, r0]
	ldr r2, [r1, r2]
	adds r3, r3, r2
	str r3, [r1, r0]
.L_08192c4a:
	ldr r1, [sp, #16]
	ldr r2, [sp, #12]
	ldr r3, [sp, #8]
	ldr r5, [sp, #20]
	ldr r0, [sp, #32]
	adds r1, #28
	adds r2, #28
	adds r3, #28
	adds r5, #28
	adds r0, #1
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r3, [sp, #8]
	str r5, [sp, #20]
	str r0, [sp, #32]
	cmp r0, #32
	beq .L_08192c6e
	b .L_08192a8a
.L_08192c6e:
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08192c7c:
	.4byte 0xffff9fc0
.L_08192c80:
	.4byte 0xffff4000
.L_08192c84:
	.4byte 0xffffff00
.L_08192c88:
	.4byte 0xffff00ff
.L_08192c8c:
	.4byte 0xffc00000
.L_08192c90:
	.4byte Data_08199f98
.L_08192c94:
	.4byte Data_08199f88
.L_08192c98:
	.4byte Data_08197410
