.syntax unified
	.thumb
	.global Func_081a7a28
	.thumb_func
Func_081a7a28:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r9, r1
	mov r8, r2
	movs r1, #128
	adds r2, r3, #0
	movs r3, #128
	sub sp, #48
	lsls r1, r1, #2
	lsls r3, r3, #8
	str r1, [sp, #44]
	cmp r0, r3
	bne .L_081a7a50
	mov r1, r9
	ldrh r0, [r1]
.L_081a7a50:
	cmp r2, #1
	bne .L_081a7a5c
	movs r3, #128
	lsls r3, r3, #1
	str r3, [sp, #44]
	b .L_081a7a72
.L_081a7a5c:
	cmp r2, #2
	bne .L_081a7a72
	movs r1, #192
	lsls r1, r1, #3
	add r8, r1
	movs r1, #128
	lsls r1, r1, #1
	movs r3, #128
	str r1, [sp, #44]
	lsls r3, r3, #2
	add r9, r3
.L_081a7a72:
	movs r3, #128
	lsls r3, r3, #8
	cmp r0, r3
	bcs .L_081a7ace
	ldr r2, .L_081a7ab4
	adds r3, r0, #0
	ands r3, r2
	movs r2, #2
	mov r1, r8
	add r8, r2
	ldr r2, .L_081a7ab8
	strh r3, [r1]
	adds r3, r0, #0
	ands r3, r2
	mov r1, r8
	lsls r3, r3, #5
	strh r3, [r1]
	ldr r3, .L_081a7abc
	movs r2, #2
	add r8, r2
	ands r0, r3
	lsls r3, r0, #10
	mov r1, r8
	strh r3, [r1]
	ldr r3, [sp, #44]
	add r8, r2
	subs r3, #1
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #1
	movs r4, #128
	movs r3, #128
	b .L_081a7ac0
.L_081a7ab4:
	.4byte 0x00007c00
.L_081a7ab8:
	.4byte 0x000003e0
.L_081a7abc:
	.4byte 0x0000001f
.L_081a7ac0:
	lsls r4, r4, #24
	mov r0, r8
	lsrs r2, r2, #1
	lsls r3, r3, #19
	adds r3, #212
	subs r0, #6
	b .L_081a8136
.L_081a7ace:
	movs r3, #128
	lsls r3, r3, #13
	cmp r0, r3
	bcc .L_081a7ad8
	b .L_081a7f16
.L_081a7ad8:
	ldr r1, .L_081a7e1c
	adds r0, r0, r1
	cmp r0, #6
	bls .L_081a7ae2
	b .L_081a7ec8
.L_081a7ae2:
	ldr r2, .L_081a7e20
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_081a7aec:
	.4byte .L_081a7b08
	.4byte .L_081a7b56
	.4byte .L_081a7bf2
	.4byte .L_081a7c78
	.4byte .L_081a7d16
	.4byte .L_081a7da0
	.4byte .L_081a7e34
.L_081a7b08:
	ldr r3, [sp, #44]
	movs r2, #0
	mov r11, r2
	cmp r11, r3
	bcc .L_081a7b14
	b .L_081a813e
.L_081a7b14:
	ldr r7, .L_081a7e24
	mov r5, r8
.L_081a7b18:
	mov r1, r9
	ldrh r6, [r1]
	movs r3, #248
	lsls r0, r6, #11
	lsls r3, r3, #8
	movs r2, #2
	ands r0, r3
	movs r3, #248
	lsls r3, r3, #9
	add r9, r2
	lsls r2, r6, #7
	ands r2, r3
	movs r3, #248
	lsls r3, r3, #7
	ands r3, r6
	adds r0, r0, r2
	adds r0, r0, r3
	movs r1, #7
	mov lr, r7
	.2byte 0xf800
	adds r6, r0, #0
	strh r6, [r5]
	strh r6, [r5, #2]
	strh r6, [r5, #4]
	ldr r1, [sp, #44]
	movs r3, #1
	add r11, r3
	adds r5, #6
	cmp r11, r1
	bcc .L_081a7b18
	b .L_081a813e
.L_081a7b56:
	ldr r3, [sp, #44]
	movs r2, #0
	mov r11, r2
	cmp r11, r3
	bcc .L_081a7b62
	b .L_081a813e
.L_081a7b62:
	ldr r2, .L_081a7e28
	movs r1, #31
	mov r10, r1
.L_081a7b68:
	mov r3, r9
	ldrh r6, [r3]
	movs r1, #2
	mov r3, r10
	adds r4, r6, #0
	lsrs r0, r6, #5
	ands r4, r3
	ands r0, r3
	add r9, r1
	lsrs r3, r6, #10
	mov r1, r10
	ands r3, r1
	adds r0, r4, r0
	adds r0, r0, r3
	str r2, [sp, #4]
	ldr r3, .L_081a7e24
	movs r1, #10
	mov lr, r3
	.2byte 0xf800
	adds r6, r0, #0
	lsls r3, r6, #2
	adds r4, r3, #5
	lsls r3, r6, #1
	adds r3, r3, r6
	adds r5, r3, #5
	adds r7, r5, #0
	ldr r2, [sp, #4]
	cmp r4, #7
	bgt .L_081a7ba4
	movs r4, #8
.L_081a7ba4:
	cmp r5, #7
	bgt .L_081a7bb0
	movs r7, #8
	cmp r5, #7
	bgt .L_081a7bb0
	movs r5, #8
.L_081a7bb0:
	cmp r4, #28
	ble .L_081a7bb6
	movs r4, #28
.L_081a7bb6:
	cmp r7, #28
	ble .L_081a7bbc
	movs r7, #28
.L_081a7bbc:
	cmp r5, #28
	ble .L_081a7bc2
	movs r5, #28
.L_081a7bc2:
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r3, #2
	add r8, r3
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r3, #2
	add r8, r3
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	movs r3, #2
	add r8, r3
	ldr r3, [sp, #44]
	movs r1, #1
	add r11, r1
	cmp r11, r3
	bcc .L_081a7b68
	b .L_081a813e
.L_081a7bf2:
	ldr r2, [sp, #44]
	movs r1, #0
	mov r11, r1
	cmp r11, r2
	bcc .L_081a7bfe
	b .L_081a813e
.L_081a7bfe:
	movs r3, #31
	mov r10, r3
.L_081a7c02:
	mov r1, r9
	ldrh r6, [r1]
	mov r3, r10
	adds r4, r6, #0
	ands r4, r3
	lsrs r7, r6, #5
	lsrs r5, r6, #10
	ands r7, r3
	ands r5, r3
	lsrs r3, r4, #1
	subs r4, r4, r3
	movs r2, #2
	movs r1, #3
	adds r0, r7, #0
	add r9, r2
	str r4, [sp, #0]
	bl __divsi3
	ldr r4, [sp, #0]
	subs r7, r7, r0
	adds r4, #6
	adds r0, r4, #0
	bl Func_081a8264
	adds r7, #4
	adds r4, r0, #0
	adds r0, r7, #0
	str r4, [sp, #0]
	bl Func_081a8264
	subs r5, #6
	adds r7, r0, #0
	adds r0, r5, #0
	bl Func_081a8264
	ldr r2, .L_081a7e2c
	adds r5, r0, #0
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r2, .L_081a7e28
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	ldr r4, [sp, #0]
	mov r2, r8
	strh r3, [r2, #2]
	ldr r2, .L_081a7e30
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	movs r2, #6
	strh r3, [r1, #4]
	ldr r1, [sp, #44]
	movs r3, #1
	add r11, r3
	add r8, r2
	cmp r11, r1
	bcc .L_081a7c02
	b .L_081a813e
.L_081a7c78:
	ldr r3, [sp, #44]
	movs r2, #0
	mov r11, r2
	cmp r11, r3
	bcc .L_081a7c84
	b .L_081a813e
.L_081a7c84:
	ldr r1, .L_081a7e30
	mov r10, r1
.L_081a7c88:
	mov r2, r9
	ldrh r6, [r2]
	movs r1, #31
	adds r4, r6, #0
	movs r3, #2
	lsrs r7, r6, #5
	lsrs r5, r6, #10
	ands r4, r1
	add r9, r3
	ands r7, r1
	ands r5, r1
	cmp r4, #9
	bgt .L_081a7ca4
	movs r4, #10
.L_081a7ca4:
	cmp r7, #15
	bgt .L_081a7caa
	movs r7, #16
.L_081a7caa:
	cmp r5, #15
	bgt .L_081a7cb0
	movs r5, #16
.L_081a7cb0:
	cmp r4, #28
	ble .L_081a7cb6
	movs r4, #28
.L_081a7cb6:
	cmp r7, #24
	ble .L_081a7cbc
	movs r7, #24
.L_081a7cbc:
	cmp r5, #26
	ble .L_081a7cc2
	movs r5, #26
.L_081a7cc2:
	adds r0, r4, #0
	bl Func_081a8264
	adds r7, #2
	adds r4, r0, #0
	adds r0, r7, #0
	str r4, [sp, #0]
	bl Func_081a8264
	adds r5, #2
	adds r7, r0, #0
	adds r0, r5, #0
	bl Func_081a8264
	adds r5, r0, #0
	mov r2, r10
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	mov r1, r10
	lsls r3, r7, #1
	ldrh r3, [r1, r3]
	movs r2, #2
	add r8, r2
	mov r2, r8
	ldr r4, [sp, #0]
	strh r3, [r2]
	movs r3, #2
	add r8, r3
	lsls r3, r4, #1
	ldrh r3, [r1, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r1, [sp, #44]
	movs r3, #1
	movs r2, #2
	add r11, r3
	add r8, r2
	cmp r11, r1
	bcc .L_081a7c88
	b .L_081a813e
.L_081a7d16:
	ldr r3, [sp, #44]
	movs r2, #0
	mov r11, r2
	cmp r11, r3
	bcc .L_081a7d22
	b .L_081a813e
.L_081a7d22:
	ldr r1, .L_081a7e2c
	mov r10, r1
.L_081a7d26:
	mov r2, r9
	ldrh r6, [r2]
	movs r1, #31
	adds r4, r6, #0
	lsrs r7, r6, #5
	ands r4, r1
	ands r7, r1
	lsrs r5, r6, #10
	adds r0, r4, r7
	ands r5, r1
	movs r3, #2
	movs r1, #3
	adds r0, r0, r5
	str r4, [sp, #0]
	add r9, r3
	bl __divsi3
	bl Func_081a8264
	ldr r4, [sp, #0]
	asrs r3, r4, #1
	adds r4, r3, r0
	asrs r3, r7, #1
	adds r7, r3, r0
	asrs r3, r5, #1
	adds r5, r3, r0
	adds r0, r4, #0
	bl Func_081a8264
	adds r4, r0, #0
	adds r0, r7, #0
	str r4, [sp, #0]
	bl Func_081a8264
	adds r7, r0, #0
	adds r0, r5, #0
	bl Func_081a8264
	adds r5, r0, #0
	mov r2, r10
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	ldr r4, [sp, #0]
	mov r2, r8
	strh r3, [r2, #2]
	mov r1, r10
	lsls r3, r4, #1
	ldrh r3, [r1, r3]
	movs r1, #1
	strh r3, [r2, #4]
	ldr r2, [sp, #44]
	movs r3, #6
	add r11, r1
	add r8, r3
	cmp r11, r2
	bcc .L_081a7d26
	b .L_081a813e
.L_081a7da0:
	ldr r1, [sp, #44]
	movs r3, #0
	mov r11, r3
	cmp r11, r1
	bcc .L_081a7dac
	b .L_081a813e
.L_081a7dac:
	movs r2, #31
	mov r10, r2
.L_081a7db0:
	mov r3, r9
	ldrh r6, [r3]
	mov r2, r10
	lsrs r7, r6, #5
	lsrs r5, r6, #10
	ands r7, r2
	ands r5, r2
	adds r4, r6, #0
	ands r4, r2
	asrs r3, r7, #3
	asrs r2, r5, #3
	adds r3, r3, r2
	adds r4, r4, r3
	movs r1, #2
	adds r0, r4, #0
	add r9, r1
	bl Func_081a8264
	movs r1, #3
	adds r4, r0, #0
	adds r0, r7, #0
	str r4, [sp, #0]
	bl __divsi3
	movs r1, #3
	subs r7, r7, r0
	adds r0, r5, #0
	bl __divsi3
	ldr r1, .L_081a7e30
	subs r5, r5, r0
	lsls r3, r5, #1
	ldrh r3, [r1, r3]
	mov r2, r8
	strh r3, [r2]
	lsls r3, r7, #1
	ldrh r3, [r1, r3]
	ldr r4, [sp, #0]
	mov r1, r8
	strh r3, [r1, #2]
	ldr r2, .L_081a7e28
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	mov r2, r8
	strh r3, [r2, #4]
	ldr r2, [sp, #44]
	movs r1, #1
	movs r3, #6
	add r11, r1
	add r8, r3
	cmp r11, r2
	bcc .L_081a7db0
	b .L_081a813e
	.2byte 0x0000
.L_081a7e1c:
	.4byte 0xfffeffff
.L_081a7e20:
	.4byte .L_081a7aec
.L_081a7e24:
	.4byte IwramDivide
.L_081a7e28:
	.4byte Data_081a87ba
.L_081a7e2c:
	.4byte Data_081a87fa
.L_081a7e30:
	.4byte Data_081a877a
.L_081a7e34:
	ldr r1, [sp, #44]
	movs r3, #0
	mov r11, r3
	cmp r11, r1
	bcc .L_081a7e40
	b .L_081a813e
.L_081a7e40:
	movs r2, #31
	mov r10, r2
.L_081a7e44:
	mov r3, r9
	ldrh r6, [r3]
	mov r2, r10
	adds r4, r6, #0
	ands r4, r2
	lsrs r7, r6, #5
	ands r7, r2
	lsrs r3, r4, #1
	movs r1, #2
	subs r4, r4, r3
	adds r0, r7, #0
	add r9, r1
	lsrs r5, r6, #10
	movs r1, #3
	ands r5, r2
	str r4, [sp, #0]
	bl __divsi3
	ldr r4, [sp, #0]
	subs r7, r7, r0
	adds r4, #6
	adds r0, r4, #0
	bl Func_081a8264
	adds r7, #4
	adds r4, r0, #0
	adds r0, r7, #0
	str r4, [sp, #0]
	bl Func_081a8264
	subs r5, #6
	adds r7, r0, #0
	adds r0, r5, #0
	bl Func_081a8264
	ldr r2, .L_081a7ebc
	adds r5, r0, #0
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	mov r1, r8
	strh r3, [r1]
	ldr r2, .L_081a7ec0
	lsls r3, r7, #1
	ldrh r3, [r2, r3]
	ldr r4, [sp, #0]
	mov r2, r8
	strh r3, [r2, #2]
	ldr r2, .L_081a7ec4
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	movs r2, #6
	strh r3, [r1, #4]
	ldr r1, [sp, #44]
	movs r3, #1
	add r11, r3
	add r8, r2
	cmp r11, r1
	bcc .L_081a7e44
	b .L_081a813e
	.2byte 0x0000
.L_081a7ebc:
	.4byte Data_081a87fa
.L_081a7ec0:
	.4byte Data_081a87ba
.L_081a7ec4:
	.4byte Data_081a877a
.L_081a7ec8:
	ldr r3, [sp, #44]
	movs r2, #0
	mov r11, r2
	cmp r11, r3
	bcc .L_081a7ed4
	b .L_081a813e
.L_081a7ed4:
	ldr r4, .L_081a7ee0
	ldr r0, .L_081a7ee4
	ldr r2, .L_081a7ee8
	mov r1, r8
	b .L_081a7eec
	.2byte 0x0000
.L_081a7ee0:
	.4byte 0x00007c00
.L_081a7ee4:
	.4byte 0x000003e0
.L_081a7ee8:
	.4byte 0x0000001f
.L_081a7eec:
	mov r3, r9
	ldrh r6, [r3]
	movs r3, #2
	add r9, r3
	adds r3, r6, #0
	ands r3, r4
	strh r3, [r1]
	adds r3, r6, #0
	ands r3, r0
	lsls r3, r3, #5
	ands r6, r2
	strh r3, [r1, #2]
	lsls r3, r6, #10
	strh r3, [r1, #4]
	movs r3, #1
	add r11, r3
	ldr r3, [sp, #44]
	adds r1, #6
	cmp r11, r3
	bcc .L_081a7eec
	b .L_081a813e
.L_081a7f16:
	movs r3, #128
	lsls r3, r3, #14
	ands r3, r0
	cmp r3, #0
	beq .L_081a7fbc
	movs r3, #31
	adds r1, r0, #0
	ands r1, r3
	str r0, [sp, #40]
	lsrs r2, r0, #5
	lsrs r0, r0, #10
	str r1, [sp, #40]
	adds r1, r0, #0
	ands r2, r3
	ands r1, r3
	ldr r3, [sp, #44]
	str r2, [sp, #36]
	movs r2, #0
	mov r11, r2
	str r0, [sp, #32]
	str r1, [sp, #32]
	cmp r11, r3
	bcc .L_081a7f46
	b .L_081a813e
.L_081a7f46:
	mov r10, r8
.L_081a7f48:
	mov r1, r9
	ldrh r6, [r1]
	movs r3, #248
	lsls r0, r6, #11
	lsls r3, r3, #8
	movs r2, #2
	ands r0, r3
	movs r3, #248
	lsls r3, r3, #9
	add r9, r2
	lsls r2, r6, #7
	ands r2, r3
	movs r3, #248
	lsls r3, r3, #7
	ands r3, r6
	adds r0, r0, r2
	adds r0, r0, r3
	movs r1, #96
	ldr r3, .L_081a80b8
	mov lr, r3
	.2byte 0xf800
	ldr r2, [sp, #40]
	adds r6, r0, #0
	adds r1, r2, #0
	muls r1, r6
	ldr r3, [sp, #36]
	mov r8, r1
	ldr r1, [sp, #32]
	mov r0, r8
	adds r7, r3, #0
	muls r7, r6
	adds r5, r1, #0
	muls r5, r6
	bl Func_081a8278
	mov r8, r0
	adds r0, r7, #0
	bl Func_081a8278
	adds r7, r0, #0
	adds r0, r5, #0
	bl Func_081a8278
	mov r2, r10
	mov r3, r10
	mov r1, r8
	adds r5, r0, #0
	strh r5, [r2]
	strh r7, [r3, #2]
	strh r1, [r2, #4]
	ldr r2, [sp, #44]
	movs r1, #1
	movs r3, #6
	add r11, r1
	add r10, r3
	cmp r11, r2
	bcc .L_081a7f48
	b .L_081a813e
.L_081a7fbc:
	movs r3, #128
	lsls r3, r3, #15
	ands r3, r0
	cmp r3, #0
	bne .L_081a7fc8
	b .L_081a80c4
.L_081a7fc8:
	movs r3, #31
	str r0, [sp, #28]
	adds r1, r0, #0
	lsrs r2, r0, #5
	lsrs r0, r0, #10
	ands r1, r3
	mov r10, r0
	str r1, [sp, #28]
	mov r1, r10
	ands r2, r3
	ands r1, r3
	ldr r3, [sp, #44]
	str r2, [sp, #24]
	movs r2, #0
	mov r11, r2
	mov r10, r1
	cmp r11, r3
	bcc .L_081a7fee
	b .L_081a813e
.L_081a7fee:
	ldr r2, [sp, #24]
	ldr r1, [sp, #28]
	ldr r3, [sp, #28]
	adds r1, r1, r2
	str r1, [sp, #20]
	lsls r1, r2, #16
	mov r2, r10
	lsls r3, r3, #16
	lsls r2, r2, #16
	str r3, [sp, #16]
	str r1, [sp, #12]
	str r2, [sp, #8]
	mov r7, r8
.L_081a8008:
	mov r3, r9
	ldrh r6, [r3]
	movs r2, #31
	movs r1, #2
	adds r4, r6, #0
	lsrs r0, r6, #5
	ands r4, r2
	ands r0, r2
	lsrs r3, r6, #10
	add r9, r1
	ldr r1, [sp, #20]
	adds r0, r4, r0
	ands r3, r2
	adds r0, r0, r3
	add r1, r10
	ldr r3, .L_081a80b8
	lsls r0, r0, #4
	mov lr, r3
	.2byte 0xf800
	ldr r3, [sp, #28]
	adds r6, r0, #0
	adds r0, r3, #0
	muls r0, r6
	ldr r2, [sp, #16]
	lsrs r0, r0, #4
	asrs r1, r2, #4
	ldr r3, .L_081a80bc
	lsls r0, r0, #16
	mov lr, r3
	.2byte 0xf800
	ldr r1, [sp, #24]
	mov r8, r0
	adds r0, r1, #0
	muls r0, r6
	ldr r2, [sp, #12]
	lsrs r0, r0, #4
	asrs r1, r2, #4
	ldr r3, .L_081a80bc
	lsls r0, r0, #16
	mov lr, r3
	.2byte 0xf800
	adds r5, r0, #0
	mov r0, r10
	muls r0, r6
	ldr r2, [sp, #8]
	lsrs r0, r0, #4
	asrs r1, r2, #4
	ldr r3, .L_081a80bc
	lsls r0, r0, #16
	mov lr, r3
	.2byte 0xf800
	mov r1, r8
	lsrs r1, r1, #16
	mov r8, r1
	adds r6, r0, #0
	mov r0, r8
	bl Func_081a8264
	lsrs r5, r5, #16
	mov r8, r0
	adds r0, r5, #0
	bl Func_081a8264
	lsrs r6, r6, #16
	adds r5, r0, #0
	adds r0, r6, #0
	bl Func_081a8264
	ldr r2, .L_081a80c0
	lsls r0, r0, #1
	ldrh r3, [r2, r0]
	lsls r5, r5, #1
	strh r3, [r7]
	movs r1, #1
	ldrh r3, [r2, r5]
	add r11, r1
	strh r3, [r7, #2]
	mov r3, r8
	lsls r3, r3, #1
	mov r8, r3
	ldrh r3, [r2, r3]
	strh r3, [r7, #4]
	ldr r2, [sp, #44]
	adds r7, #6
	cmp r11, r2
	bcc .L_081a8008
	b .L_081a813e
	.2byte 0x0000
.L_081a80b8:
	.4byte IwramDivide
.L_081a80bc:
	.4byte IwramMulQ16
.L_081a80c0:
	.4byte Data_081a877a
.L_081a80c4:
	movs r3, #128
	lsls r3, r3, #16
	ands r3, r0
	cmp r3, #0
	beq .L_081a811a
	ldr r1, [sp, #44]
	movs r3, #0
	mov r11, r3
	cmp r11, r1
	bcs .L_081a813e
	ldr r4, .L_081a80e4
	ldr r0, .L_081a80e8
	ldr r2, .L_081a80ec
	mov r1, r8
	b .L_081a80f0
	.2byte 0x0000
.L_081a80e4:
	.4byte 0x00007c00
.L_081a80e8:
	.4byte 0x000003e0
.L_081a80ec:
	.4byte 0x0000001f
.L_081a80f0:
	mov r3, r9
	ldrh r6, [r3]
	movs r3, #2
	add r9, r3
	adds r3, r6, #0
	ands r3, r4
	strh r3, [r1]
	adds r3, r6, #0
	ands r3, r0
	lsls r3, r3, #5
	ands r6, r2
	strh r3, [r1, #2]
	lsls r3, r6, #10
	strh r3, [r1, #4]
	movs r3, #1
	add r11, r3
	ldr r3, [sp, #44]
	adds r1, #6
	cmp r11, r3
	bcc .L_081a80f0
	b .L_081a813e
.L_081a811a:
	cmp r2, #2
	bne .L_081a8124
	movs r1, #192
	lsls r1, r1, #3
	adds r0, r0, r1
.L_081a8124:
	ldr r3, [sp, #44]
	movs r4, #132
	lsls r2, r3, #1
	adds r2, r2, r3
	movs r3, #128
	lsls r4, r4, #24
	lsrs r2, r2, #1
	lsls r3, r3, #19
	adds r3, #212
.L_081a8136:
	mov r1, r8
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_081a813e:
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
