.syntax unified
	.thumb
	.global Func_0802493c
	.thumb_func
Func_0802493c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #20]
	sub sp, #72
	add r5, sp, #12
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_08024920
	ldr r6, [r5]
	mov r0, sp
	adds r0, #16
	str r0, [sp, #8]
	cmp r6, #0
	bne .L_0802496a
	b .L_08024c2e
.L_0802496a:
	ldr r3, [r6, #12]
	ldr r2, [r6, #8]
	str r3, [sp, #4]
	adds r3, r6, #0
	adds r3, #97
	ldr r0, [r6, #16]
	ldrb r3, [r3]
	mov r11, r2
	mov r9, r0
	cmp r3, #0
	beq .L_08024982
	b .L_08024b32
.L_08024982:
	movs r2, #0
	str r2, [sp, #0]
	movs r0, #128
	ldr r3, [r6, #56]
	lsls r0, r0, #24
	cmp r3, r0
	beq .L_08024a7e
	mov r2, r11
	subs r0, r3, r2
	cmp r0, #0
	bge .L_080249a0
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_080249a0:
	ldr r3, [r6, #64]
	mov r2, r9
	asrs r7, r0, #16
	subs r0, r3, r2
	cmp r0, #0
	bge .L_080249b4
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_080249b4:
	asrs r0, r0, #16
	mov r8, r0
	mov r2, r8
	mov r3, r8
	muls r3, r2
	adds r0, r7, #0
	muls r0, r7
	adds r0, r0, r3
	ldr r3, .L_08024c3c
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_08024c40
	lsls r0, r0, #16
	cmp r0, r2
	bgt .L_08024a02
	ldr r3, [r6, #56]
	mov r0, r11
	subs r7, r3, r0
	ldr r3, [r6, #64]
	mov r2, r9
	subs r2, r3, r2
	adds r1, r7, #0
	ldr r3, .L_08024c44
	adds r0, r7, #0
	mov r8, r2
	mov lr, r3
	.2byte 0xf800
	mov r1, r8
	adds r5, r0, #0
	ldr r2, .L_08024c44
	mov r0, r8
	mov lr, r2
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	ldr r3, .L_08024c3c
	mov lr, r3
	.2byte 0xf800
	lsls r0, r0, #8
.L_08024a02:
	cmp r0, #0
	bne .L_08024a10
	ldr r0, [r6, #56]
	ldr r2, [r6, #64]
	mov r11, r0
	mov r9, r2
	b .L_08024ae4
.L_08024a10:
	ldr r3, .L_08024c48
	ldr r1, [r6, #52]
	mov lr, r3
	.2byte 0xf800
	adds r5, r0, #0
	ldr r0, .L_08024c44
	adds r1, r5, #0
	mov r10, r0
	adds r0, r7, #0
	mov lr, r10
	.2byte 0xf800
	ldr r3, [r6, #36]
	adds r1, r5, #0
	adds r3, r3, r0
	str r3, [r6, #36]
	mov r0, r8
	mov lr, r10
	.2byte 0xf800
	ldr r3, [r6, #44]
	ldr r7, [r6, #36]
	adds r3, r3, r0
	str r3, [r6, #44]
	adds r1, r7, #0
	adds r0, r7, #0
	mov r8, r3
	mov lr, r10
	.2byte 0xf800
	mov r1, r8
	adds r5, r0, #0
	mov r0, r8
	mov lr, r10
	.2byte 0xf800
	adds r5, r5, r0
	adds r0, r5, #0
	ldr r2, .L_08024c3c
	mov lr, r2
	.2byte 0xf800
	ldr r1, [r6, #48]
	lsls r0, r0, #8
	cmp r0, r1
	ble .L_08024ae4
	ldr r3, .L_08024c48
	mov lr, r3
	.2byte 0xf800
	adds r5, r0, #0
	adds r1, r5, #0
	adds r0, r7, #0
	mov lr, r10
	.2byte 0xf800
	adds r1, r5, #0
	str r0, [r6, #36]
	mov r0, r8
	mov lr, r10
	.2byte 0xf800
	b .L_08024ae2
.L_08024a7e:
	ldr r7, [r6, #36]
	ldr r0, [r6, #44]
	adds r3, r7, #0
	orrs r3, r0
	mov r8, r0
	cmp r3, #0
	beq .L_08024ae4
	ldr r2, .L_08024c44
	adds r1, r7, #0
	mov r10, r2
	adds r0, r7, #0
	mov lr, r10
	.2byte 0xf800
	mov r1, r8
	adds r5, r0, #0
	mov r0, r8
	mov lr, r10
	.2byte 0xf800
	adds r5, r5, r0
	ldr r3, .L_08024c3c
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	lsls r0, r0, #8
	cmp r0, #0
	beq .L_08024ade
	ldr r3, [r6, #52]
	subs r1, r0, r3
	cmp r1, #0
	bge .L_08024ac2
	ldr r3, [sp, #0]
	str r3, [r6, #36]
	str r3, [r6, #44]
	b .L_08024ae4
.L_08024ac2:
	ldr r3, .L_08024c48
	mov lr, r3
	.2byte 0xf800
	adds r5, r0, #0
	adds r1, r5, #0
	adds r0, r7, #0
	mov lr, r10
	.2byte 0xf800
	adds r1, r5, #0
	str r0, [r6, #36]
	mov r0, r8
	mov lr, r10
	.2byte 0xf800
	b .L_08024ae2
.L_08024ade:
	ldr r0, [sp, #0]
	str r0, [r6, #36]
.L_08024ae2:
	str r0, [r6, #44]
.L_08024ae4:
	adds r3, r6, #0
	adds r3, #85
	ldrb r2, [r3]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08024b32
	ldr r3, [r6, #20]
	ldr r2, [sp, #4]
	cmp r2, r3
	ble .L_08024b06
	ldr r2, [r6, #40]
	ldr r3, [r6, #72]
	subs r2, r2, r3
	str r2, [r6, #40]
	adds r0, r2, #0
	b .L_08024b34
.L_08024b06:
	ldr r0, [r6, #40]
	cmp r0, #0
	bge .L_08024b34
	str r3, [sp, #4]
	ldr r3, .L_08024c44
	ldr r1, [r6, #68]
	mov lr, r3
	.2byte 0xf800
	adds r3, r0, #0
	negs r0, r3
	adds r1, r0, #0
	str r0, [r6, #40]
	cmp r1, #0
	bge .L_08024b24
	adds r1, r3, #0
.L_08024b24:
	ldr r3, [r6, #72]
	cmp r1, r3
	bgt .L_08024b34
	movs r3, #0
	str r3, [r6, #40]
	movs r0, #0
	b .L_08024b34
.L_08024b32:
	ldr r0, [r6, #40]
.L_08024b34:
	ldr r2, [sp, #4]
	ldr r3, [r6, #36]
	adds r2, r2, r0
	str r2, [sp, #4]
	add r11, r3
	ldr r3, [r6, #44]
	adds r1, r6, #0
	adds r1, #86
	add r9, r3
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_08024b92
	cmp r3, #17
	beq .L_08024b6c
	cmp r3, #17
	bgt .L_08024b5a
	cmp r3, #16
	beq .L_08024b60
	b .L_08024b92
.L_08024b5a:
	cmp r3, #18
	beq .L_08024b7a
	b .L_08024b92
.L_08024b60:
	ldr r2, [r6, #56]
	cmp r11, r2
	beq .L_08024b8e
	ldr r3, [r6, #8]
	mov r0, r11
	b .L_08024b84
.L_08024b6c:
	ldr r2, [r6, #60]
	ldr r3, [sp, #4]
	cmp r3, r2
	beq .L_08024b8e
	ldr r3, [r6, #12]
	ldr r0, [sp, #4]
	b .L_08024b84
.L_08024b7a:
	ldr r2, [r6, #64]
	cmp r9, r2
	beq .L_08024b8e
	ldr r3, [r6, #16]
	mov r0, r9
.L_08024b84:
	subs r3, r3, r2
	subs r2, r0, r2
	eors r3, r2
	cmp r3, #0
	bge .L_08024b92
.L_08024b8e:
	movs r2, #1
	str r2, [sp, #0]
.L_08024b92:
	ldr r3, [sp, #0]
	cmp r3, #0
	beq .L_08024bce
	adds r3, r6, #0
	adds r3, #88
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08024bc0
	movs r3, #0
	str r3, [r6, #36]
	str r3, [r6, #44]
	adds r3, r6, #0
	adds r3, #85
	ldr r0, [r6, #56]
	ldr r2, [r6, #64]
	ldrb r3, [r3]
	mov r11, r0
	mov r9, r2
	cmp r3, #0
	bne .L_08024bc0
	ldr r0, [r6, #60]
	str r0, [sp, #4]
	str r3, [r6, #40]
.L_08024bc0:
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r6, #56]
	str r3, [r6, #60]
	str r3, [r6, #64]
	movs r3, #0
	strb r3, [r1]
.L_08024bce:
	mov r2, r11
	str r2, [r6, #8]
	ldr r3, [sp, #4]
	mov r0, r9
	str r3, [r6, #12]
	str r0, [r6, #16]
	adds r3, r6, #0
	adds r3, #90
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08024c20
	ldr r2, [r6, #36]
	ldr r3, [r6, #44]
	mov r11, r2
	mov r9, r3
	cmp r2, #0
	bne .L_08024bf8
	cmp r3, #0
	beq .L_08024c20
.L_08024bf8:
	mov r0, r9
	mov r1, r11
	bl ArcTan2
	ldrh r3, [r6, #6]
	movs r2, #128
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_08024c14
	movs r0, #128
	lsls r0, r0, #5
.L_08024c14:
	ldr r2, .L_08024c4c
	cmp r0, r2
	bge .L_08024c1c
	ldr r0, .L_08024c4c
.L_08024c1c:
	adds r3, r3, r0
	strh r3, [r6, #6]
.L_08024c20:
	ldr r0, [sp, #8]
	ldmia r0!, {r6}
	adds r3, r0, #0
	str r3, [sp, #8]
	cmp r6, #0
	beq .L_08024c2e
	b .L_0802496a
.L_08024c2e:
	add sp, #72
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08024c3c:
	.4byte IwramFillWords + 0x74
.L_08024c40:
	.4byte 0x00ffffff
.L_08024c44:
	.4byte IwramMulQ16
.L_08024c48:
	.4byte IwramRatioMulQ14
.L_08024c4c:
	.4byte 0xfffff000
