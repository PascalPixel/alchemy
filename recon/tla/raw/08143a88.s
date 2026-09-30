.syntax unified
	.thumb
	.global Func_08143a88
	.thumb_func
Func_08143a88:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	bl Func_081435e0
	ldr r3, .L_08143ac0
	movs r2, #128
	orrs r5, r3
	ldr r3, .L_08143ac4
	lsls r2, r2, #19
	adds r2, #12
	mov r8, r3
	movs r3, #128
	strh r5, [r2]
	lsls r3, r3, #2
	movs r5, #0
	mov r12, r5
	movs r7, #0
	mov lr, r3
	movs r6, #0
.L_08143ab4:
	movs r3, #128
	lsls r3, r3, #1
	movs r4, #0
	adds r0, r6, r3
	lsls r1, r7, #1
	b .L_08143ac8
.L_08143ac0:
	.4byte 0x00004784
.L_08143ac4:
	.4byte 0x06003800
.L_08143ac8:
	adds r3, r0, #0
	orrs r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r10, r3
	mov r3, r8
	adds r2, r5, r3
	adds r4, #1
	mov r3, r10
	strh r3, [r2]
	add r0, lr
	adds r1, #2
	adds r5, #2
	cmp r4, #8
	bne .L_08143ac8
	ldr r1, .L_08143af4
	ldr r2, .L_08143af0
	movs r4, #0
	b .L_08143af8
	.2byte 0x0000
.L_08143af0:
	.4byte 0x00000000
.L_08143af4:
	.4byte 0x06003800
.L_08143af8:
	adds r3, r5, r1
	adds r4, #1
	strh r2, [r3]
	adds r5, #2
	cmp r4, #8
	bne .L_08143af8
	movs r3, #128
	lsls r3, r3, #5
	adds r6, r6, r3
	movs r3, #1
	add r12, r3
	mov r3, r12
	adds r7, #8
	cmp r3, #16
	bne .L_08143ab4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
