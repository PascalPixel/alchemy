.syntax unified
	.thumb
	.global Func_08195a9c
	.thumb_func
Func_08195a9c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #144
	str r3, [sp, #108]
	str r1, [sp, #116]
	str r2, [sp, #112]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #100]
	adds r6, r0, #0
	str r1, [sp, #100]
	ldr r2, [r3, #92]
	ldr r3, [r3, #96]
	mov r8, r2
	str r3, [sp, #96]
	lsls r3, r6, #1
	adds r6, r3, r6
	movs r3, #0
	str r3, [sp, #104]
	cmp r6, #0
	beq .L_08195ae8
	ldr r7, .L_08195e6c
	adds r5, r1, #0
.L_08195ad4:
	adds r1, r5, #0
	adds r0, r5, #0
	mov lr, r7
	.2byte 0xf800
	ldr r1, [sp, #104]
	adds r5, #12
	adds r1, #3
	str r1, [sp, #104]
	cmp r1, r6
	bne .L_08195ad4
.L_08195ae8:
	ldr r3, [sp, #116]
	movs r2, #0
	str r2, [sp, #104]
	cmp r3, #0
	bne .L_08195af4
	b .L_08196230
.L_08195af4:
	ldr r1, [sp, #100]
	lsls r3, r6, #2
	adds r6, r3, r1
	adds r2, r6, #0
	adds r3, r6, #0
	adds r1, r6, #0
	adds r2, #12
	adds r3, #24
	adds r1, #28
	str r2, [sp, #92]
	str r3, [sp, #88]
	str r1, [sp, #84]
	mov r2, r8
	adds r3, r6, #4
	subs r1, #12
	str r2, [sp, #8]
	str r3, [sp, #80]
	str r1, [sp, #76]
.L_08195b18:
	ldr r2, [sp, #8]
	ldr r1, [sp, #100]
	ldr r0, [r2]
	movs r3, #128
	movs r2, #132
	lsls r0, r0, #2
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r0, r0, r1
	adds r3, #212
	adds r1, r6, #0
	adds r2, #3
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, [sp, #8]
	ldr r1, [sp, #100]
	ldr r0, [r2, #4]
	movs r2, #132
	lsls r0, r0, #2
	lsls r2, r2, #24
	adds r0, r0, r1
	adds r2, #3
	ldr r1, [sp, #92]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, [sp, #8]
	ldr r1, [sp, #100]
	ldr r0, [r2, #8]
	movs r2, #132
	lsls r0, r0, #2
	lsls r2, r2, #24
	adds r0, r0, r1
	adds r2, #3
	ldr r1, [sp, #88]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, [sp, #92]
	ldr r3, [r6]
	ldr r0, [r2]
	ldr r2, [sp, #80]
	subs r0, r0, r3
	ldr r3, [sp, #84]
	ldr r1, [r3]
	ldr r3, [r2]
	subs r1, r1, r3
	ldr r3, .L_08195e70
	mov lr, r3
	.2byte 0xf800
	ldr r1, [sp, #88]
	ldr r2, [sp, #76]
	ldr r3, [r6]
	adds r5, r0, #0
	ldr r0, [r1]
	ldr r1, [r2]
	ldr r2, [sp, #80]
	subs r0, r0, r3
	ldr r3, [r2]
	subs r1, r1, r3
	ldr r3, .L_08195e70
	mov lr, r3
	.2byte 0xf800
	subs r5, r5, r0
	cmp r5, #0
	bge .L_08195b9a
	b .L_0819621c
.L_08195b9a:
	ldr r1, [sp, #104]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #92]
	ldr r2, [sp, #112]
	lsls r0, r1, #1
	adds r0, r0, r1
	lsls r0, r0, #3
	str r3, [sp, #12]
	adds r0, r0, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	add r1, sp, #120
	adds r2, #6
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [r6]
	movs r2, #128
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r6]
	ldr r1, [sp, #80]
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	ldr r1, [sp, #92]
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	ldr r1, [sp, #76]
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	ldr r1, [sp, #88]
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	ldr r1, [sp, #84]
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	ldr r2, [sp, #80]
	ldr r1, [sp, #76]
	ldr r3, [r2]
	ldr r2, [r1]
	cmp r3, r2
	ble .L_08195c5e
	ldr r2, [sp, #92]
	ldr r1, [r6]
	ldr r3, [r2]
	add r7, sp, #120
	eors r1, r3
	str r1, [r6]
	ldr r3, [sp, #92]
	ldr r2, [r3]
	eors r2, r1
	str r2, [r3]
	ldr r3, [r6]
	eors r3, r2
	str r3, [r6]
	ldr r2, [sp, #80]
	ldr r1, [r2]
	ldr r2, [sp, #76]
	ldr r3, [r2]
	eors r1, r3
	ldr r3, [sp, #80]
	str r1, [r3]
	ldr r3, [sp, #76]
	ldr r2, [r3]
	eors r2, r1
	ldr r1, [sp, #92]
	str r2, [r1, #4]
	ldr r1, [sp, #80]
	ldr r3, [r1]
	eors r3, r2
	str r3, [r1]
	ldr r1, [r7, #8]
	ldr r2, [r7]
	eors r2, r1
	adds r3, r2, #0
	eors r3, r1
	eors r2, r3
	str r2, [r7]
	ldr r1, [r7, #12]
	ldr r2, [r7, #4]
	str r3, [r7, #8]
	eors r2, r1
	adds r3, r2, #0
	eors r3, r1
	eors r2, r3
	str r3, [r7, #12]
	str r2, [r7, #4]
	ldr r3, [sp, #76]
	ldr r2, [r3]
	b .L_08195c60
.L_08195c5e:
	add r7, sp, #120
.L_08195c60:
	ldr r1, [sp, #84]
	ldr r3, [r1]
	cmp r2, r3
	ble .L_08195d2a
	ldr r2, [sp, #92]
	ldr r1, [r2]
	ldr r2, [sp, #88]
	ldr r3, [r2]
	eors r1, r3
	ldr r3, [sp, #92]
	str r1, [r3]
	ldr r3, [sp, #88]
	ldr r2, [r3]
	eors r2, r1
	str r2, [r3]
	ldr r1, [sp, #92]
	ldr r3, [r1]
	eors r3, r2
	str r3, [r1]
	ldr r2, [sp, #76]
	ldr r1, [r2]
	ldr r2, [sp, #84]
	ldr r3, [r2]
	eors r1, r3
	ldr r3, [sp, #92]
	str r1, [r3, #4]
	ldr r3, [sp, #84]
	ldr r2, [r3]
	eors r2, r1
	ldr r1, [sp, #88]
	str r2, [r1, #4]
	ldr r1, [sp, #76]
	ldr r3, [r1]
	eors r3, r2
	str r3, [r1]
	ldr r1, [r7, #16]
	ldr r2, [r7, #8]
	eors r2, r1
	adds r3, r2, #0
	eors r3, r1
	eors r2, r3
	str r2, [r7, #8]
	ldr r1, [r7, #20]
	ldr r2, [r7, #12]
	str r3, [r7, #16]
	eors r2, r1
	adds r3, r2, #0
	eors r3, r1
	eors r2, r3
	str r3, [r7, #20]
	str r2, [r7, #12]
	ldr r3, [sp, #80]
	ldr r1, [sp, #76]
	ldr r2, [r3]
	ldr r3, [r1]
	cmp r2, r3
	ble .L_08195d2a
	ldr r2, [sp, #92]
	ldr r1, [r6]
	ldr r3, [r2]
	eors r1, r3
	str r1, [r6]
	ldr r3, [sp, #92]
	ldr r2, [r3]
	eors r2, r1
	str r2, [r3]
	ldr r3, [r6]
	eors r3, r2
	str r3, [r6]
	ldr r2, [sp, #80]
	ldr r1, [r2]
	ldr r2, [sp, #76]
	ldr r3, [r2]
	eors r1, r3
	ldr r3, [sp, #80]
	str r1, [r3]
	ldr r3, [sp, #76]
	ldr r2, [r3]
	eors r2, r1
	ldr r1, [sp, #92]
	str r2, [r1, #4]
	ldr r1, [sp, #80]
	ldr r3, [r1]
	eors r3, r2
	str r3, [r1]
	ldr r1, [r7, #8]
	ldr r2, [r7]
	eors r2, r1
	adds r3, r2, #0
	eors r3, r1
	eors r2, r3
	ldr r1, [r7, #12]
	str r2, [r7]
	ldr r2, [r7, #4]
	str r3, [r7, #8]
	eors r2, r1
	adds r3, r2, #0
	eors r3, r1
	eors r2, r3
	str r3, [r7, #12]
	str r2, [r7, #4]
.L_08195d2a:
	ldr r5, [r6, #16]
	ldr r0, [r6, #4]
	asrs r3, r5, #16
	str r3, [sp, #24]
	asrs r2, r0, #16
	mov r8, r2
	movs r2, #30
	ldrsh r1, [r6, r2]
	str r1, [sp, #20]
	cmp r8, r3
	beq .L_08195d74
	ldr r3, [r6]
	ldr r1, [r6, #12]
	subs r5, r5, r0
	subs r1, r1, r3
	adds r0, r5, #0
	ldr r3, .L_08195e74
	mov lr, r3
	.2byte 0xf800
	str r0, [sp, #72]
	ldr r3, [r7]
	ldr r1, [r7, #8]
	ldr r2, .L_08195e74
	subs r1, r1, r3
	adds r0, r5, #0
	mov lr, r2
	.2byte 0xf800
	str r0, [sp, #64]
	ldr r3, [r7, #4]
	ldr r1, [r7, #12]
	adds r0, r5, #0
	subs r1, r1, r3
	ldr r3, .L_08195e74
	mov lr, r3
	.2byte 0xf800
	str r0, [sp, #60]
	ldr r0, [r6, #4]
.L_08195d74:
	ldr r1, [sp, #20]
	cmp r8, r1
	beq .L_08195db0
	ldr r5, [r6, #28]
	ldr r3, [r6]
	ldr r1, [r6, #24]
	subs r5, r5, r0
	subs r1, r1, r3
	ldr r2, .L_08195e74
	adds r0, r5, #0
	mov lr, r2
	.2byte 0xf800
	str r0, [sp, #68]
	ldr r3, [r7]
	ldr r1, [r7, #16]
	adds r0, r5, #0
	subs r1, r1, r3
	ldr r3, .L_08195e74
	mov lr, r3
	.2byte 0xf800
	str r0, [sp, #56]
	ldr r1, [r7, #20]
	ldr r3, [r7, #4]
	adds r0, r5, #0
	subs r1, r1, r3
	ldr r2, .L_08195e74
	mov lr, r2
	.2byte 0xf800
	str r0, [sp, #52]
	ldr r0, [r6, #4]
.L_08195db0:
	ldr r3, [r6]
	str r3, [sp, #44]
	str r3, [sp, #48]
	ldr r1, [r7]
	str r1, [sp, #32]
	str r1, [sp, #40]
	ldr r2, [r7, #4]
	str r2, [sp, #28]
	str r2, [sp, #36]
	ldr r2, [r6, #12]
	ldr r3, [r6, #24]
	cmp r2, r3
	blt .L_08195e78
	ldr r1, [r6, #16]
	ldr r5, .L_08195e70
	subs r1, r1, r0
	ldr r0, [sp, #68]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r3, r3, r0
	cmp r3, #0
	blt .L_08195df8
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #68]
	subs r1, r1, r3
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r5, r3, r0
	b .L_08195e0c
.L_08195df8:
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #68]
	subs r1, r1, r3
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r5, r0, r3
.L_08195e0c:
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #56]
	subs r1, r1, r3
	ldr r3, .L_08195e70
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r7]
	ldr r1, [r7, #8]
	ldr r2, .L_08195e74
	subs r1, r1, r3
	subs r1, r1, r0
	adds r0, r5, #0
	mov lr, r2
	.2byte 0xf800
	str r0, [sp, #16]
	ldr r0, [sp, #52]
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	subs r1, r1, r3
	ldr r3, .L_08195e70
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r7, #4]
	ldr r1, [r7, #12]
	ldr r2, .L_08195e74
	subs r1, r1, r3
	subs r1, r1, r0
	adds r0, r5, #0
	mov lr, r2
	.2byte 0xf800
	ldr r1, [sp, #56]
	ldr r3, [sp, #64]
	ldr r2, [sp, #60]
	eors r3, r1
	eors r1, r3
	eors r3, r1
	str r3, [sp, #64]
	ldr r3, [sp, #52]
	mov r9, r0
	eors r2, r3
	eors r3, r2
	eors r2, r3
	str r1, [sp, #56]
	str r3, [sp, #52]
	str r2, [sp, #60]
	b .L_08195efe
	.2byte 0x0000
.L_08195e6c:
	.4byte IwramTransformVector
.L_08195e70:
	.4byte IwramMulQ16
.L_08195e74:
	.4byte IwramRatioMulQ14
.L_08195e78:
	ldr r1, [r6, #16]
	ldr r5, .L_08196108
	subs r1, r1, r0
	ldr r0, [sp, #68]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r0, r0, r3
	cmp r0, #0
	blt .L_08195ea6
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #68]
	subs r1, r1, r3
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r5, r0, r3
	b .L_08195eba
.L_08195ea6:
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #68]
	subs r1, r1, r3
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r5, r3, r0
.L_08195eba:
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #56]
	subs r1, r1, r3
	ldr r2, .L_08196108
	mov lr, r2
	.2byte 0xf800
	ldr r2, [r7]
	ldr r3, [r7, #8]
	adds r1, r0, #0
	subs r3, r3, r2
	subs r1, r1, r3
	adds r0, r5, #0
	ldr r3, .L_0819610c
	mov lr, r3
	.2byte 0xf800
	str r0, [sp, #16]
	ldr r2, .L_08196108
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #52]
	subs r1, r1, r3
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7, #12]
	ldr r2, [r7, #4]
	adds r1, r0, #0
	subs r3, r3, r2
	subs r1, r1, r3
	adds r0, r5, #0
	ldr r3, .L_0819610c
	mov lr, r3
	.2byte 0xf800
	mov r9, r0
.L_08195efe:
	ldr r1, [sp, #24]
	mov r0, r8
	cmp r0, r1
	beq .L_08195fc6
.L_08195f06:
	ldr r2, [sp, #12]
	ldr r1, [sp, #36]
	movs r3, #224
	lsls r3, r3, #3
	adds r2, r2, r3
	mov r12, r2
	asrs r3, r1, #16
	ldr r2, [sp, #108]
	ldr r1, [sp, #40]
	lsls r3, r2
	asrs r2, r1, #16
	adds r3, r3, r2
	add r12, r3
	ldr r2, [sp, #48]
	ldr r3, [sp, #44]
	asrs r1, r2, #16
	asrs r5, r3, #16
	movs r2, #0
	mov r8, r2
	mov lr, r2
	cmp r1, r5
	ble .L_08195f38
	eors r1, r5
	eors r5, r1
	eors r1, r5
.L_08195f38:
	movs r2, #7
	ands r2, r0
	lsrs r3, r0, #3
	lsls r2, r2, #3
	lsls r3, r3, #10
	adds r2, r2, r3
	adds r4, r1, #0
	mov r10, r2
	cmp r4, r5
	beq .L_08195f8e
	movs r3, #7
	mov r11, r3
.L_08195f50:
	mov r2, r8
	asrs r1, r2, #16
	mov r2, lr
	asrs r3, r2, #16
	ldr r2, [sp, #108]
	lsls r3, r2
	asrs r2, r4, #3
	lsls r2, r2, #6
	str r2, [sp, #4]
	adds r1, r1, r3
	mov r2, r11
	adds r3, r4, #0
	ands r3, r2
	ldr r2, [sp, #4]
	adds r3, r2, r3
	adds r2, r3, #0
	mov r3, r12
	ldrb r1, [r3, r1]
	ldr r3, [sp, #96]
	add r2, r10
	ldrb r3, [r3, r2]
	cmp r3, r1
	bcs .L_08195f82
	ldr r3, [sp, #96]
	strb r1, [r3, r2]
.L_08195f82:
	ldr r1, [sp, #16]
	adds r4, #1
	add r8, r1
	add lr, r9
	cmp r4, r5
	bne .L_08195f50
.L_08195f8e:
	ldr r2, [sp, #48]
	ldr r3, [sp, #72]
	ldr r1, [sp, #44]
	adds r2, r2, r3
	str r2, [sp, #48]
	ldr r2, [sp, #68]
	ldr r3, [sp, #40]
	adds r1, r1, r2
	str r1, [sp, #44]
	ldr r1, [sp, #64]
	ldr r2, [sp, #36]
	adds r3, r3, r1
	str r3, [sp, #40]
	ldr r3, [sp, #60]
	ldr r1, [sp, #32]
	adds r2, r2, r3
	str r2, [sp, #36]
	ldr r2, [sp, #56]
	ldr r3, [sp, #28]
	adds r1, r1, r2
	str r1, [sp, #32]
	ldr r1, [sp, #52]
	ldr r2, [sp, #24]
	adds r3, r3, r1
	adds r0, #1
	str r3, [sp, #28]
	cmp r0, r2
	bne .L_08195f06
.L_08195fc6:
	ldr r3, [sp, #24]
	ldr r1, [sp, #20]
	cmp r3, r1
	bne .L_08195fd0
	b .L_0819621c
.L_08195fd0:
	ldr r2, [r6, #28]
	ldr r3, [r6, #16]
	ldr r1, [r6, #24]
	subs r5, r2, r3
	ldr r3, [r6, #12]
	ldr r2, .L_0819610c
	subs r1, r1, r3
	mov r8, r2
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [sp, #72]
	ldr r2, [r6, #12]
	ldr r3, [r6, #24]
	cmp r2, r3
	blt .L_08196000
	ldr r3, [sp, #64]
	ldr r1, [sp, #60]
	str r3, [sp, #56]
	ldr r2, [sp, #40]
	ldr r3, [sp, #36]
	str r1, [sp, #52]
	str r2, [sp, #32]
	str r3, [sp, #28]
.L_08196000:
	ldr r3, [r7, #8]
	ldr r1, [r7, #16]
	adds r0, r5, #0
	subs r1, r1, r3
	mov lr, r8
	.2byte 0xf800
	str r0, [sp, #64]
	ldr r3, [r7, #12]
	ldr r1, [r7, #20]
	adds r0, r5, #0
	subs r1, r1, r3
	mov lr, r8
	.2byte 0xf800
	str r0, [sp, #60]
	ldr r1, [r6, #12]
	str r1, [sp, #48]
	ldr r2, [r7, #8]
	str r2, [sp, #40]
	ldr r3, [r7, #12]
	str r3, [sp, #36]
	ldr r3, [r6]
	cmp r3, r1
	bgt .L_081960d8
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r5, .L_08196108
	subs r1, r1, r3
	ldr r0, [sp, #68]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r3, r3, r0
	cmp r3, #0
	blt .L_0819605e
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #68]
	subs r1, r1, r3
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r5, r3, r0
	b .L_08196072
.L_0819605e:
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #68]
	subs r1, r1, r3
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r5, r0, r3
.L_08196072:
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #56]
	subs r1, r1, r3
	ldr r2, .L_08196108
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7]
	ldr r1, [r7, #8]
	subs r1, r1, r3
	subs r1, r1, r0
	ldr r3, .L_0819610c
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	str r0, [sp, #16]
	ldr r2, .L_08196108
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #52]
	subs r1, r1, r3
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7, #4]
	ldr r1, [r7, #12]
	subs r1, r1, r3
	subs r1, r1, r0
	ldr r3, .L_0819610c
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	ldr r2, [sp, #56]
	ldr r1, [sp, #64]
	ldr r3, [sp, #60]
	eors r1, r2
	eors r2, r1
	eors r1, r2
	str r1, [sp, #64]
	ldr r1, [sp, #52]
	str r2, [sp, #56]
	eors r3, r1
	eors r1, r3
	eors r3, r1
	str r3, [sp, #60]
	ldr r2, [sp, #32]
	ldr r3, [sp, #28]
	mov r9, r0
	str r1, [sp, #52]
	str r2, [sp, #40]
	str r3, [sp, #36]
	b .L_08196168
.L_081960d8:
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r5, .L_08196108
	subs r1, r1, r3
	ldr r0, [sp, #68]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r0, r0, r3
	cmp r0, #0
	blt .L_08196110
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #68]
	subs r1, r1, r3
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r5, r0, r3
	b .L_08196124
.L_08196108:
	.4byte IwramMulQ16
.L_0819610c:
	.4byte IwramRatioMulQ14
.L_08196110:
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #68]
	subs r1, r1, r3
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	ldr r2, [r6]
	subs r3, r3, r2
	subs r5, r3, r0
.L_08196124:
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #56]
	subs r1, r1, r3
	ldr r2, .L_08196240
	mov lr, r2
	.2byte 0xf800
	ldr r2, [r7]
	ldr r3, [r7, #8]
	adds r1, r0, #0
	subs r3, r3, r2
	subs r1, r1, r3
	adds r0, r5, #0
	ldr r3, .L_08196244
	mov lr, r3
	.2byte 0xf800
	str r0, [sp, #16]
	ldr r2, .L_08196240
	ldr r3, [r6, #4]
	ldr r1, [r6, #16]
	ldr r0, [sp, #52]
	subs r1, r1, r3
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7, #12]
	ldr r2, [r7, #4]
	adds r1, r0, #0
	subs r3, r3, r2
	subs r1, r1, r3
	adds r0, r5, #0
	ldr r3, .L_08196244
	mov lr, r3
	.2byte 0xf800
	mov r9, r0
.L_08196168:
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	cmp r0, r1
	beq .L_0819621c
.L_08196170:
	ldr r2, [sp, #12]
	ldr r1, [sp, #36]
	movs r3, #224
	lsls r3, r3, #3
	adds r7, r2, r3
	asrs r3, r1, #16
	ldr r2, [sp, #108]
	ldr r1, [sp, #40]
	lsls r3, r2
	asrs r2, r1, #16
	adds r3, r3, r2
	adds r7, r7, r3
	ldr r2, [sp, #48]
	ldr r3, [sp, #44]
	asrs r1, r2, #16
	asrs r5, r3, #16
	movs r2, #0
	mov lr, r2
	mov r12, r2
	cmp r1, r5
	ble .L_081961a0
	eors r1, r5
	eors r5, r1
	eors r1, r5
.L_081961a0:
	movs r2, #7
	ands r2, r0
	lsrs r3, r0, #3
	lsls r2, r2, #3
	lsls r3, r3, #10
	adds r2, r2, r3
	adds r4, r1, #0
	mov r8, r2
	cmp r4, r5
	beq .L_081961f4
	movs r3, #7
	mov r10, r3
.L_081961b8:
	mov r2, lr
	asrs r1, r2, #16
	mov r2, r12
	asrs r3, r2, #16
	ldr r2, [sp, #108]
	lsls r3, r2
	asrs r2, r4, #3
	lsls r2, r2, #6
	adds r1, r1, r3
	mov r11, r2
	adds r3, r4, #0
	mov r2, r10
	ands r3, r2
	add r11, r3
	ldr r3, [sp, #96]
	mov r2, r11
	add r2, r8
	ldrb r3, [r3, r2]
	ldrb r1, [r7, r1]
	mov r11, r3
	cmp r11, r1
	bcs .L_081961e8
	ldr r3, [sp, #96]
	strb r1, [r3, r2]
.L_081961e8:
	ldr r1, [sp, #16]
	adds r4, #1
	add lr, r1
	add r12, r9
	cmp r4, r5
	bne .L_081961b8
.L_081961f4:
	ldr r2, [sp, #48]
	ldr r3, [sp, #72]
	ldr r1, [sp, #44]
	adds r2, r2, r3
	str r2, [sp, #48]
	ldr r2, [sp, #68]
	ldr r3, [sp, #40]
	adds r1, r1, r2
	str r1, [sp, #44]
	ldr r1, [sp, #64]
	ldr r2, [sp, #36]
	adds r3, r3, r1
	str r3, [sp, #40]
	ldr r3, [sp, #60]
	ldr r1, [sp, #20]
	adds r2, r2, r3
	adds r0, #1
	str r2, [sp, #36]
	cmp r0, r1
	bne .L_08196170
.L_0819621c:
	ldr r2, [sp, #8]
	ldr r3, [sp, #104]
	ldr r1, [sp, #116]
	adds r2, #28
	adds r3, #1
	str r2, [sp, #8]
	str r3, [sp, #104]
	cmp r3, r1
	beq .L_08196230
	b .L_08195b18
.L_08196230:
	add sp, #144
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08196240:
	.4byte IwramMulQ16
.L_08196244:
	.4byte IwramRatioMulQ14
