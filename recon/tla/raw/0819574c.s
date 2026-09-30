.syntax unified
	.thumb
	.global Func_0819574c
	.thumb_func
Func_0819574c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r1, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	adds r6, r0, #0
	str r1, [sp, #48]
	ldr r2, [r3, #100]
	str r2, [sp, #44]
	ldr r3, [r3, #92]
	mov r8, r3
	lsls r3, r6, #1
	adds r6, r3, r6
	movs r3, #0
	str r3, [sp, #40]
	cmp r6, #0
	beq .L_08195794
	ldr r7, .L_08195a90
	adds r5, r2, #0
.L_08195780:
	adds r1, r5, #0
	adds r0, r5, #0
	mov lr, r7
	.2byte 0xf800
	ldr r1, [sp, #40]
	adds r5, #12
	adds r1, #3
	str r1, [sp, #40]
	cmp r1, r6
	bne .L_08195780
.L_08195794:
	ldr r3, [sp, #52]
	movs r2, #0
	str r2, [sp, #40]
	cmp r3, #0
	bne .L_081957a0
	b .L_08195a82
.L_081957a0:
	ldr r5, [sp, #44]
	lsls r3, r6, #2
	adds r6, r3, r5
	movs r1, #12
	adds r2, r6, #0
	adds r1, r1, r6
	adds r2, #24
	adds r3, r6, #0
	mov r10, r1
	str r2, [sp, #36]
	adds r3, #28
	mov r1, r8
	subs r2, #8
	str r3, [sp, #32]
	str r1, [sp, #8]
	str r2, [sp, #28]
	adds r5, r6, #4
	mov r11, r5
.L_081957c4:
	ldr r3, [sp, #8]
	ldr r5, [sp, #44]
	ldr r0, [r3]
	movs r2, #132
	movs r3, #128
	lsls r0, r0, #2
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r0, r5
	adds r1, r6, #0
	adds r2, #3
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, [sp, #8]
	movs r2, #132
	ldr r0, [r1, #4]
	lsls r2, r2, #24
	lsls r0, r0, #2
	adds r0, r0, r5
	mov r1, r10
	adds r2, #3
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, [sp, #8]
	ldr r1, [sp, #36]
	ldr r0, [r2, #8]
	movs r2, #132
	lsls r0, r0, #2
	lsls r2, r2, #24
	adds r0, r0, r5
	adds r2, #3
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r3, r10
	ldr r5, [sp, #32]
	ldr r0, [r3]
	ldr r3, [r6]
	mov r2, r11
	ldr r1, [r5]
	subs r0, r0, r3
	ldr r3, [r2]
	subs r1, r1, r3
	ldr r3, .L_08195a94
	mov lr, r3
	.2byte 0xf800
	ldr r1, [sp, #36]
	ldr r2, [sp, #28]
	ldr r3, [r6]
	adds r5, r0, #0
	ldr r0, [r1]
	ldr r1, [r2]
	mov r2, r11
	subs r0, r0, r3
	ldr r3, [r2]
	subs r1, r1, r3
	ldr r3, .L_08195a94
	mov lr, r3
	.2byte 0xf800
	subs r5, r5, r0
	cmp r5, #0
	bge .L_08195842
	b .L_08195a6e
.L_08195842:
	ldr r5, [sp, #8]
	movs r2, #128
	ldrb r5, [r5, #24]
	lsls r2, r2, #15
	str r5, [sp, #12]
	mov r1, r11
	ldr r3, [r6]
	mov r5, r10
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r5]
	adds r3, r3, r2
	str r3, [r5]
	ldr r1, [sp, #28]
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	ldr r5, [sp, #36]
	ldr r3, [r5]
	adds r3, r3, r2
	str r3, [r5]
	ldr r1, [sp, #32]
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	ldr r5, [sp, #28]
	mov r2, r11
	ldr r3, [r2]
	ldr r2, [r5]
	cmp r3, r2
	ble .L_081958c0
	mov r2, r10
	ldr r3, [r2]
	ldr r1, [r6]
	mov r5, r11
	eors r1, r3
	str r1, [r6]
	mov r3, r10
	ldr r2, [r2]
	eors r2, r1
	str r2, [r3]
	ldr r3, [r6]
	eors r3, r2
	str r3, [r6]
	ldr r2, [sp, #28]
	ldr r1, [r5]
	ldr r3, [r2]
	eors r1, r3
	str r1, [r5]
	ldr r3, [sp, #28]
	mov r5, r10
	ldr r2, [r3]
	eors r2, r1
	str r2, [r5, #4]
	mov r1, r11
	ldr r3, [r1]
	eors r3, r2
	str r3, [r1]
	ldr r3, [sp, #28]
	ldr r2, [r3]
.L_081958c0:
	ldr r5, [sp, #32]
	ldr r3, [r5]
	cmp r2, r3
	ble .L_0819593c
	ldr r5, [sp, #36]
	mov r2, r10
	ldr r3, [r5]
	ldr r1, [r2]
	eors r1, r3
	str r1, [r2]
	ldr r2, [r5]
	eors r2, r1
	str r2, [r5]
	mov r1, r10
	ldr r3, [r1]
	eors r3, r2
	str r3, [r1]
	ldr r5, [sp, #32]
	ldr r3, [sp, #28]
	ldr r2, [r3]
	ldr r3, [r5]
	eors r2, r3
	str r2, [r1, #4]
	ldr r1, [sp, #36]
	ldr r3, [r5]
	eors r3, r2
	str r3, [r1, #4]
	ldr r5, [sp, #28]
	mov r1, r11
	ldr r2, [r5]
	eors r2, r3
	str r2, [r5]
	ldr r3, [r1]
	cmp r3, r2
	ble .L_0819593c
	mov r2, r10
	ldr r3, [r2]
	ldr r1, [r6]
	mov r5, r11
	eors r1, r3
	str r1, [r6]
	mov r3, r10
	ldr r2, [r2]
	eors r2, r1
	str r2, [r3]
	ldr r3, [r6]
	eors r3, r2
	str r3, [r6]
	ldr r2, [sp, #28]
	ldr r1, [r5]
	ldr r3, [r2]
	eors r1, r3
	str r1, [r5]
	ldr r3, [sp, #28]
	mov r5, r10
	ldr r2, [r3]
	eors r2, r1
	str r2, [r5, #4]
	mov r1, r11
	ldr r3, [r1]
	eors r3, r2
	str r3, [r1]
.L_0819593c:
	ldr r3, [r6, #4]
	ldr r0, [r6, #16]
	asrs r2, r3, #16
	mov r8, r2
	movs r2, #30
	ldrsh r1, [r6, r2]
	asrs r5, r0, #16
	mov r9, r5
	str r1, [sp, #16]
	cmp r8, r9
	beq .L_08195962
	subs r0, r0, r3
	ldr r1, [r6, #12]
	ldr r3, [r6]
	subs r1, r1, r3
	ldr r3, .L_08195a98
	mov lr, r3
	.2byte 0xf800
	str r0, [sp, #24]
.L_08195962:
	ldr r3, [sp, #16]
	cmp r8, r3
	beq .L_0819597c
	ldr r3, [r6, #4]
	ldr r0, [r6, #28]
	ldr r1, [r6, #24]
	subs r0, r0, r3
	ldr r3, [r6]
	subs r1, r1, r3
	ldr r3, .L_08195a98
	mov lr, r3
	.2byte 0xf800
	str r0, [sp, #20]
.L_0819597c:
	ldr r7, [r6]
	mov r4, r8
	str r7, [sp, #0]
	cmp r4, r9
	beq .L_081959e6
.L_08195986:
	ldr r5, [sp, #0]
	asrs r0, r7, #16
	asrs r1, r5, #16
	cmp r1, r0
	ble .L_08195996
	eors r1, r0
	eors r0, r1
	eors r1, r0
.L_08195996:
	movs r2, #7
	ands r2, r4
	asrs r3, r4, #3
	lsls r2, r2, #3
	lsls r3, r3, #10
	adds r2, r2, r3
	mov r12, r2
	cmp r1, r0
	beq .L_081959d4
	movs r2, #7
	mov lr, r2
.L_081959ac:
	asrs r3, r1, #3
	lsls r3, r3, #6
	mov r8, r3
	adds r2, r1, #0
	mov r3, lr
	ands r2, r3
	ldr r5, [sp, #48]
	add r2, r8
	add r2, r12
	mov r8, r2
	ldrb r5, [r5, r2]
	ldr r2, [sp, #12]
	cmp r5, r2
	bcs .L_081959ce
	ldr r3, [sp, #48]
	mov r5, r8
	strb r2, [r3, r5]
.L_081959ce:
	adds r1, #1
	cmp r1, r0
	bne .L_081959ac
.L_081959d4:
	ldr r5, [sp, #0]
	ldr r1, [sp, #24]
	ldr r2, [sp, #20]
	adds r5, r5, r1
	adds r4, #1
	str r5, [sp, #0]
	adds r7, r7, r2
	cmp r4, r9
	bne .L_08195986
.L_081959e6:
	ldr r3, [sp, #16]
	cmp r9, r3
	beq .L_08195a00
	ldr r3, [r6, #16]
	ldr r0, [r6, #28]
	ldr r1, [r6, #24]
	subs r0, r0, r3
	ldr r3, [r6, #12]
	subs r1, r1, r3
	ldr r3, .L_08195a98
	mov lr, r3
	.2byte 0xf800
	str r0, [sp, #24]
.L_08195a00:
	ldr r5, [r6, #12]
	ldr r1, [sp, #16]
	mov r4, r9
	str r5, [sp, #0]
	cmp r4, r1
	beq .L_08195a6e
.L_08195a0c:
	ldr r2, [sp, #0]
	asrs r0, r7, #16
	asrs r1, r2, #16
	cmp r1, r0
	ble .L_08195a1c
	eors r1, r0
	eors r0, r1
	eors r1, r0
.L_08195a1c:
	movs r2, #7
	ands r2, r4
	asrs r3, r4, #3
	lsls r2, r2, #3
	lsls r3, r3, #10
	adds r2, r2, r3
	mov r12, r2
	cmp r1, r0
	beq .L_08195a5a
	movs r3, #7
	mov lr, r3
.L_08195a32:
	asrs r3, r1, #3
	lsls r3, r3, #6
	mov r5, lr
	adds r2, r1, #0
	mov r8, r3
	ands r2, r5
	ldr r3, [sp, #48]
	add r2, r8
	add r2, r12
	ldrb r3, [r3, r2]
	ldr r5, [sp, #12]
	mov r9, r3
	mov r8, r2
	cmp r9, r5
	bcs .L_08195a54
	ldr r3, [sp, #48]
	strb r5, [r3, r2]
.L_08195a54:
	adds r1, #1
	cmp r1, r0
	bne .L_08195a32
.L_08195a5a:
	ldr r5, [sp, #0]
	ldr r1, [sp, #24]
	ldr r2, [sp, #20]
	ldr r3, [sp, #16]
	adds r5, r5, r1
	adds r4, #1
	str r5, [sp, #0]
	adds r7, r7, r2
	cmp r4, r3
	bne .L_08195a0c
.L_08195a6e:
	ldr r5, [sp, #8]
	ldr r1, [sp, #40]
	ldr r2, [sp, #52]
	adds r5, #28
	adds r1, #1
	str r5, [sp, #8]
	str r1, [sp, #40]
	cmp r1, r2
	beq .L_08195a82
	b .L_081957c4
.L_08195a82:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08195a90:
	.4byte IwramTransformVector
.L_08195a94:
	.4byte IwramMulQ16
.L_08195a98:
	.4byte IwramRatioMulQ14
