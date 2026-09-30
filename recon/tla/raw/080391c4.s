.syntax unified
	.thumb
	.global Func_080391c4
	.thumb_func
Func_080391c4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #12
	ldrsh r5, [r0, r2]
	ldrh r2, [r0, #10]
	ldr r3, [r3, #60]
	mov r8, r2
	ldrh r2, [r0, #22]
	mov r10, r3
	movs r3, #14
	ldrsh r6, [r0, r3]
	movs r3, #0
	strh r3, [r0, #26]
	movs r3, #8
	ands r3, r2
	sub sp, #4
	ldrh r7, [r0, #8]
	cmp r3, #0
	beq .L_08039236
	movs r3, #32
	ands r3, r2
	cmp r3, #0
	beq .L_08039216
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	mov r3, r8
	bl Func_0803a084
	movs r1, #240
	ldr r3, .L_08039254
	ldr r0, .L_08039258
	lsls r1, r1, #4
	ldr r2, .L_0803925c
	mov lr, r3
	.2byte 0xf800
	b .L_08039224
.L_08039216:
	movs r1, #240
	ldr r3, .L_08039254
	ldr r0, .L_08039258
	lsls r1, r1, #4
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
.L_08039224:
	movs r3, #0
	str r3, [sp, #0]
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	mov r3, r8
	bl Func_0803a2b0
	b .L_08039242
.L_08039236:
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	mov r3, r8
	bl Func_0803a084
.L_08039242:
	movs r3, #1
	mov r2, r10
	strb r3, [r2, #3]
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08039254:
	.4byte IwramFillWords
.L_08039258:
	.4byte 0x06002500
.L_0803925c:
	.4byte 0x44444444
