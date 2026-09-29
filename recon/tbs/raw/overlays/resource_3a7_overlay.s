.syntax unified
	.thumb
	.section .text.x02008754,"ax",%progbits
	.p2align 2
	.global Func_02000754
	.thumb_func
Func_02000754:
	.global FieldScene_RunSupplementalSequenceOne
	.thumb_func
FieldScene_RunSupplementalSequenceOne:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #292]
	movs r2, #178
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r2, r2, r3
	mov r10, r2
	sub sp, #16
	bl 0x02009120
	ldr r3, [pc, #280]
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02000754_0
	mov r3, r10
	str r2, [r3, #24]
	str r2, [r3, #28]
	b .L_02000754_1
.L_02000754_0:
	movs r3, #1
	negs r3, r3
	mov r2, r10
	str r3, [r2, #24]
	str r3, [r2, #28]
.L_02000754_1:
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x020090e8
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #228]
	bl 0x020090e8
	movs r0, #163
	bl 0x02009150
	ldr r3, [pc, #220]
	mov r8, r3
.L_02000754_3:
	bl 0x02009080
	mov r2, r10
	adds r5, r0, #0
	ldr r0, [r2, #36]
	bl 0x0200945c
	lsls r5, r5, #11
	lsrs r5, r5, #16
	str r0, [sp, #8]
	str r1, [sp, #12]
	adds r0, r5, #0
	bl 0x0200945c
	adds r7, r1, #0
	adds r6, r0, #0
	cmp r5, #0
	bge .L_02000754_2
	ldr r2, [pc, #184]
	ldr r3, [pc, #188]
	bl 0x020093ac
	adds r7, r1, #0
	adds r6, r0, #0
.L_02000754_2:
	adds r3, r7, #0
	adds r2, r6, #0
	ldr r0, [pc, #176]
	ldr r1, [pc, #180]
	bl 0x020093e4
	adds r3, r1, #0
	adds r2, r0, #0
	ldr r0, [sp, #8]
	ldr r1, [sp, #12]
	bl 0x020093e4
	bl 0x020094d8
	mov r3, r10
	str r0, [r3, #36]
	movs r0, #1
	bl 0x02009118
	movs r2, #1
	negs r2, r2
	add r8, r2
	mov r3, r8
	cmp r3, #0
	bge .L_02000754_3
	movs r5, #6
	movs r2, #0
	movs r6, #6
	mov r8, r2
	lsls r7, r5, #10
.L_02000754_5:
	lsls r1, r5, #5
	orrs r1, r7
	orrs r1, r6
	ldr r0, [pc, #124]
	bl 0x02009090
	movs r0, #1
	bl 0x02009118
	mov r0, r8
	movs r1, #20
	bl 0x02009060
	cmp r0, #0
	bne .L_02000754_4
	subs r6, #1
	subs r5, #1
.L_02000754_4:
	movs r3, #1
	add r8, r3
	mov r2, r8
	cmp r2, #69
	ble .L_02000754_5
	movs r3, #19
	movs r2, #91
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #83
	movs r2, #15
	movs r3, #8
	movs r0, #19
	bl 0x020090c8
	movs r0, #144
	lsls r0, r0, #1
	bl 0x02009150
	bl 0x020090b0
	bl 0x020090f0
	bl 0x02009128
	sub sp, #-16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e70
	.4byte 0x03001e40
	.4byte 0x0000e666
	.4byte 0x000001df
	.4byte 0x41f00000
	.4byte 0x00000000
	.4byte 0x40b26e97
	.4byte 0x8d4fdf3b
	.4byte 0x04000052
	.section .rodata,"a",%progbits
	.global Func_02001158
	.thumb_func
Func_02001158:
	push {r4, r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	mov r4, r8
	push {r4, r5, r6, r7}
	sub sp, #8
	mov r10, r0
	mov r9, r1
	mov r8, r2
	bl 0x0200942c
	cmp r0, #0
	beq .L_02001158_0
.L_02001158_4:
	mov r0, r10
	b .L_02001158_1
.L_02001158_0:
	mov r0, r9
	bl 0x0200942c
	cmp r0, #0
	bne .L_02001158_2
	mov r0, r10
	bl 0x0200943c
	cmp r0, #0
	beq .L_02001158_3
	mov r0, r9
	bl 0x0200943c
	cmp r0, #0
	beq .L_02001158_4
	mov r0, r10
	mov r1, r9
	ldr r2, [r0, #4]
	ldr r3, [r1, #4]
	cmp r2, r3
	beq .L_02001158_4
	bl 0x02009424
	b .L_02001158_1
.L_02001158_3:
	mov r0, r9
	bl 0x0200943c
	cmp r0, #0
	bne .L_02001158_2
	mov r0, r9
	bl 0x0200944c
	cmp r0, #0
	beq .L_02001158_5
	mov r0, r10
	bl 0x0200944c
	cmp r0, #0
	beq .L_02001158_4
	mov r2, r8
	mov r3, r10
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r4}
	stmia r2!, {r0, r4}
	mov r1, r10
	mov r4, r9
	ldr r3, [r1, #4]
	ldr r2, [r4, #4]
	mov r0, r8
	ands r3, r2
	str r3, [r0, #4]
	b .L_02001158_1
.L_02001158_5:
	mov r0, r10
	bl 0x0200944c
	cmp r0, #0
	beq .L_02001158_6
.L_02001158_2:
	mov r0, r9
	b .L_02001158_1
.L_02001158_6:
	mov r1, r10
	ldr r1, [r1, #8]
	mov r2, r9
	ldr r2, [r2, #8]
	mov lr, r1
	mov r1, r9
	mov r3, r10
	ldr r0, [r1, #12]
	ldr r1, [r1, #16]
	mov r4, lr
	ldr r6, [r3, #12]
	ldr r7, [r3, #16]
	subs r3, r4, r2
	mov r12, r2
	str r0, [sp, #0]
	str r1, [sp, #4]
	cmp r3, #0
	bge .L_02001158_7
	negs r3, r3
.L_02001158_7:
	cmp r3, #63
	bgt .L_02001158_8
	cmp lr, r12
	ble .L_02001158_9
	mov r2, r12
	mov r1, lr
	movs r0, #1
	subs r1, r1, r2
	mov r11, r0
	mov r12, r1
.L_02001158_10:
	movs r3, #1
	ldr r0, [sp, #4]
	negs r3, r3
	add r12, r3
	ldr r3, [sp, #0]
	lsls r5, r0, #31
	ldr r1, [sp, #0]
	lsrs r0, r3, #1
	adds r3, r5, #0
	mov r4, r11
	orrs r3, r0
	ldr r0, [sp, #4]
	ands r1, r4
	lsrs r4, r0, #1
	adds r0, r1, #0
	orrs r0, r3
	movs r2, #0
	str r0, [sp, #0]
	adds r0, r2, #0
	orrs r0, r4
	mov r1, r12
	str r0, [sp, #4]
	cmp r1, #0
	bne .L_02001158_10
	mov r12, lr
.L_02001158_9:
	cmp r12, lr
	ble .L_02001158_11
	movs r2, #1
	mov r11, r2
.L_02001158_12:
	movs r3, #1
	lsls r5, r7, #31
	mov r1, r11
	lsrs r0, r6, #1
	add lr, r3
	ands r1, r6
	movs r2, #0
	adds r3, r5, #0
	lsrs r4, r7, #1
	orrs r3, r0
	adds r6, r1, #0
	adds r7, r2, #0
	orrs r6, r3
	orrs r7, r4
	cmp r12, lr
	bgt .L_02001158_12
	b .L_02001158_11
.L_02001158_8:
	cmp lr, r12
	ble .L_02001158_13
	movs r0, #0
	movs r1, #0
	str r0, [sp, #0]
	str r1, [sp, #4]
	b .L_02001158_11
.L_02001158_13:
	mov lr, r12
	movs r6, #0
	movs r7, #0
.L_02001158_11:
	mov r1, r10
	mov r2, r9
	ldr r0, [r1, #4]
	ldr r3, [r2, #4]
	cmp r0, r3
	beq .L_02001158_14
	ldr r1, [sp, #0]
	ldr r2, [sp, #4]
	subs r1, r1, r6
	sbcs r2, r7
	cmp r0, #0
	bne .L_02001158_15
	ldr r3, [sp, #0]
	ldr r4, [sp, #4]
	adds r2, r7, #0
	adds r1, r6, #0
	subs r1, r1, r3
	sbcs r2, r4
.L_02001158_15:
	cmp r2, #0
	blt .L_02001158_16
	movs r3, #0
	mov r4, r8
	str r3, [r4, #4]
	mov r0, lr
	mov r3, r8
	str r0, [r4, #8]
	str r1, [r3, #12]
	str r2, [r3, #16]
	b .L_02001158_17
.L_02001158_16:
	mov r4, r8
	movs r3, #1
	mov r0, lr
	str r3, [r4, #4]
	str r0, [r4, #8]
	movs r4, #0
	negs r3, r1
	sbcs r4, r2
	mov r1, r8
	str r3, [r1, #12]
	str r4, [r1, #16]
.L_02001158_17:
	mov r2, r8
	ldr r4, [r2, #12]
	ldr r5, [r2, #16]
	ldr r1, [pc, #176]
	movs r2, #1
	negs r2, r2
	asrs r3, r2, #31
	adds r2, r2, r4
	adcs r3, r5
	cmp r3, r1
	bhi .L_02001158_18
	cmp r3, r1
	bne .L_02001158_19
	movs r0, #2
	negs r0, r0
	cmp r2, r0
	bhi .L_02001158_18
.L_02001158_19:
	lsrs r2, r4, #31
	lsls r3, r5, #1
	adds r1, r2, #0
	mov r2, r8
	orrs r1, r3
	ldr r3, [r2, #8]
	lsls r0, r4, #1
	subs r3, #1
	str r0, [r2, #12]
	str r1, [r2, #16]
	str r3, [r2, #8]
	ldr r6, [pc, #128]
	movs r2, #1
	negs r2, r2
	asrs r3, r2, #31
	adds r2, r2, r0
	adcs r3, r1
	cmp r3, r6
	bhi .L_02001158_18
	adds r5, r1, #0
	adds r4, r0, #0
	cmp r3, r6
	bne .L_02001158_19
	movs r4, #2
	negs r4, r4
	cmp r2, r4
	bhi .L_02001158_18
	adds r5, r1, #0
	adds r4, r0, #0
	b .L_02001158_19
.L_02001158_14:
	mov r1, r8
	mov r2, lr
	str r0, [r1, #4]
	str r2, [r1, #8]
	ldr r3, [sp, #0]
	ldr r4, [sp, #4]
	adds r6, r6, r3
	adcs r7, r4
	mov r4, r8
	str r6, [r4, #12]
	str r7, [r4, #16]
.L_02001158_18:
	movs r3, #3
	mov r0, r8
	str r3, [r0]
	ldr r1, [pc, #64]
	ldr r3, [r0, #16]
	cmp r3, r1
	bls .L_02001158_20
	ldr r5, [r0, #12]
	ldr r6, [r0, #16]
	movs r2, #1
	adds r3, r5, #0
	ands r3, r2
	lsls r2, r6, #31
	mov r12, r2
	lsrs r0, r5, #1
	mov r1, r12
	orrs r1, r0
	movs r4, #0
	lsrs r2, r6, #1
	mov r0, r8
	orrs r3, r1
	orrs r4, r2
	str r3, [r0, #12]
	str r4, [r0, #16]
	ldr r3, [r0, #8]
	adds r3, #1
	str r3, [r0, #8]
.L_02001158_20:
	mov r0, r8
.L_02001158_1:
	sub sp, #-8
	pop {r3, r4, r5, r6}
	mov r8, r3
	mov r9, r4
	mov r10, r5
	mov r11, r6
	pop {r4, r5, r6, r7, pc}
	.4byte 0x0fffffff
	.4byte 0x1fffffff
	push {r4, r5, r6, lr}
	sub sp, #76
	add r4, sp, #8
	add r6, sp, #56
	mov r5, sp
	str r0, [r4]
	str r1, [r4, #4]
	adds r0, r4, #0
	adds r1, r6, #0
	str r2, [r5]
	str r3, [r5, #4]
	bl 0x02009770
	add r4, sp, #36
	adds r0, r5, #0
	adds r1, r4, #0
	bl 0x02009770
	adds r1, r4, #0
	add r2, sp, #16
	adds r0, r6, #0
	bl 0x02009158
	bl 0x020095a4
	sub sp, #-76
	pop {r4, r5, r6, pc}
	.2byte 0x0000
	push {r4, r5, r6, lr}
	sub sp, #76
	add r4, sp, #8
	add r6, sp, #56
	mov r5, sp
	str r0, [r4]
	str r1, [r4, #4]
	adds r0, r4, #0
	adds r1, r6, #0
	str r2, [r5]
	str r3, [r5, #4]
	bl 0x02009770
	add r4, sp, #36
	adds r0, r5, #0
	adds r1, r4, #0
	bl 0x02009770
	ldr r3, [r4, #4]
	movs r2, #1
	eors r3, r2
	str r3, [r4, #4]
	add r2, sp, #16
	adds r1, r4, #0
	adds r0, r6, #0
	bl 0x02009158
	bl 0x020095a4
	sub sp, #-76
	pop {r4, r5, r6, pc}
	.2byte 0x0000
	.2byte 0x4800
	.2byte 0x4770
	.2byte 0x9888
	.2byte 0x0200
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #1
	bhi .L_02001158_21
	movs r2, #1
.L_02001158_21:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #4
	bne .L_02001158_22
	movs r2, #1
.L_02001158_22:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #2
	bne .L_02001158_23
	movs r2, #1
.L_02001158_23:
	adds r0, r2, #0
	pop {pc}
	push {r4, r5, lr}
	sub sp, #20
	mov r5, sp
	movs r3, #3
	lsrs r2, r0, #31
	str r3, [r5]
	str r2, [r5, #4]
	cmp r0, #0
	bne .L_02001158_24
	movs r3, #2
	str r3, [r5]
	b .L_02001158_25
.L_02001158_24:
	movs r3, #60
	str r3, [r5, #8]
	cmp r2, #0
	beq .L_02001158_26
	movs r3, #128
	lsls r3, r3, #24
	cmp r0, r3
	bne .L_02001158_27
	ldr r1, [pc, #72]
	ldr r0, [pc, #68]
	b .L_02001158_28
.L_02001158_27:
	negs r3, r0
	asrs r4, r3, #31
	b .L_02001158_29
.L_02001158_26:
	adds r3, r0, #0
	asrs r4, r0, #31
.L_02001158_29:
	str r3, [r5, #12]
	str r4, [r5, #16]
	ldr r3, [r5, #16]
	ldr r2, [pc, #56]
	cmp r3, r2
	bhi .L_02001158_25
	adds r0, r5, #0
	mov r12, r2
.L_02001158_30:
	ldr r3, [r0, #12]
	ldr r4, [r0, #16]
	lsrs r1, r3, #31
	lsls r2, r4, #1
	adds r4, r1, #0
	lsls r3, r3, #1
	orrs r4, r2
	str r3, [r0, #12]
	str r4, [r0, #16]
	ldr r3, [r0, #8]
	subs r3, #1
	str r3, [r0, #8]
	ldr r3, [r0, #16]
	cmp r3, r12
	bls .L_02001158_30
.L_02001158_25:
	adds r0, r5, #0
	bl 0x020095a4
.L_02001158_28:
	sub sp, #-20
	pop {r4, r5, pc}
	.4byte 0xc1e00000
	.4byte 0x00000000
	.4byte 0x0fffffff
	push {r4, lr}
	sub sp, #28
	mov r3, sp
	add r4, sp, #8
	str r0, [r3]
	str r1, [r3, #4]
	adds r0, r3, #0
	adds r1, r4, #0
	bl 0x02009770
	adds r0, r4, #0
	bl 0x02009564
	cmp r0, #0
	bne .L_02001158_31
	adds r0, r4, #0
	bl 0x02009544
	cmp r0, #0
	beq .L_02001158_32
.L_02001158_31:
	movs r0, #0
	b .L_02001158_33
.L_02001158_32:
	adds r0, r4, #0
	bl 0x02009554
	cmp r0, #0
	bne .L_02001158_34
	ldr r3, [r4, #8]
	movs r0, #0
	cmp r3, #0
	blt .L_02001158_33
	cmp r3, #30
	ble .L_02001158_35
.L_02001158_34:
	ldr r3, [r4, #4]
	negs r0, r3
	orrs r0, r3
	ldr r3, [pc, #28]
	lsrs r0, r0, #31
	adds r0, r0, r3
	b .L_02001158_33
.L_02001158_35:
	movs r2, #60
	subs r2, r2, r3
	ldr r0, [r4, #12]
	ldr r1, [r4, #16]
	bl 0x02009574
	ldr r3, [r4, #4]
	cmp r3, #0
	beq .L_02001158_33
	negs r0, r0
.L_02001158_33:
	sub sp, #-28
	pop {r4, pc}
	.4byte 0x7fffffff
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #1
	bhi .L_02001158_36
	movs r2, #1
.L_02001158_36:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #4
	bne .L_02001158_37
	movs r2, #1
.L_02001158_37:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #2
	bne .L_02001158_38
	movs r2, #1
.L_02001158_38:
	adds r0, r2, #0
	pop {pc}
	push {r4, r5, r6, lr}
	adds r6, r2, #0
	cmp r6, #0
	beq .L_02001158_39
	movs r3, #32
	subs r3, r3, r6
	cmp r3, #0
	bgt .L_02001158_40
	negs r3, r3
	adds r4, r1, #0
	movs r5, #0
	lsrs r4, r3
	b .L_02001158_41
.L_02001158_40:
	adds r2, r1, #0
	lsls r2, r3
	adds r3, r0, #0
	lsrs r3, r6
	adds r5, r1, #0
	adds r4, r3, #0
	lsrs r5, r6
	orrs r4, r2
.L_02001158_41:
	adds r1, r5, #0
	adds r0, r4, #0
.L_02001158_39:
	pop {r4, r5, r6, pc}
	push {r4, r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r4, r0, #0
	ldr r3, [r4, #4]
	sub sp, #8
	ldr r5, [r4, #12]
	ldr r6, [r4, #16]
	mov r10, r3
	movs r7, #0
	bl 0x02009740
	cmp r0, #0
	beq .L_02001158_42
	ldr r2, [pc, #228]
	ldr r1, [pc, #220]
	adds r4, r6, #0
	adds r3, r5, #0
	orrs r4, r2
	ldr r7, [pc, #220]
	b .L_02001158_43
.L_02001158_42:
	adds r0, r4, #0
	bl 0x02009750
	cmp r0, #0
	bne .L_02001158_44
	adds r0, r4, #0
	bl 0x02009760
	cmp r0, #0
	beq .L_02001158_45
	movs r5, #0
	movs r6, #0
	b .L_02001158_46
.L_02001158_45:
	adds r3, r6, #0
	orrs r3, r5
	cmp r3, #0
	beq .L_02001158_46
	ldr r0, [r4, #8]
	ldr r2, [pc, #184]
	cmp r0, r2
	bge .L_02001158_47
	subs r2, r2, r0
	cmp r2, #56
	ble .L_02001158_48
	movs r5, #0
	movs r6, #0
	b .L_02001158_49
.L_02001158_48:
	movs r3, #0
	mov r8, r3
	movs r3, #1
	lsls r3, r2
	subs r3, #1
	asrs r4, r3, #31
	ands r4, r6
	ands r3, r5
	orrs r3, r4
	cmp r3, #0
	beq .L_02001158_50
	movs r3, #1
	mov r8, r3
.L_02001158_50:
	adds r1, r6, #0
	adds r0, r5, #0
	bl 0x02009574
	movs r4, #0
	mov r3, r8
	adds r5, r0, #0
	adds r6, r1, #0
	orrs r5, r3
	orrs r6, r4
.L_02001158_49:
	movs r3, #255
	adds r1, r5, #0
	ands r1, r3
	movs r2, #0
	cmp r1, #128
	bne .L_02001158_51
	cmp r2, #0
	bne .L_02001158_51
	adds r3, #1
	adds r1, r5, #0
	ands r1, r3
	adds r3, r2, #0
	orrs r3, r1
	cmp r3, #0
	beq .L_02001158_52
	movs r3, #128
	movs r4, #0
	b .L_02001158_53
.L_02001158_51:
	movs r3, #127
	movs r4, #0
.L_02001158_53:
	adds r5, r5, r3
	adcs r6, r4
.L_02001158_52:
	ldr r3, [pc, #80]
	cmp r6, r3
	bls .L_02001158_54
	movs r7, #1
	b .L_02001158_54
.L_02001158_47:
	movs r3, #128
	lsls r3, r3, #3
	cmp r0, r3
	blt .L_02001158_55
.L_02001158_44:
	ldr r7, [pc, #56]
	movs r5, #0
	movs r6, #0
	b .L_02001158_46
.L_02001158_55:
	ldr r3, [pc, #60]
	adds r1, r5, #0
	adds r7, r0, r3
	movs r3, #255
	ands r1, r3
	movs r2, #0
	cmp r1, #128
	bne .L_02001158_56
	cmp r2, #0
	bne .L_02001158_56
	adds r3, #1
	adds r1, r5, #0
	ands r1, r3
	adds r3, r2, #0
	orrs r3, r1
	cmp r3, #0
	beq .L_02001158_57
	movs r3, #128
	movs r4, #0
	b .L_02001158_58
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x000007ff
	.4byte 0xfffffc02
	.4byte 0x0fffffff
	.4byte 0x000003ff
.L_02001158_56:
	movs r3, #127
	movs r4, #0
.L_02001158_58:
	adds r5, r5, r3
	adcs r6, r4
.L_02001158_57:
	ldr r3, [pc, #96]
	cmp r6, r3
	bls .L_02001158_54
	lsls r1, r6, #31
	lsrs r2, r5, #1
	adds r3, r1, #0
	orrs r3, r2
	lsrs r4, r6, #1
	adds r6, r4, #0
	adds r5, r3, #0
	adds r7, #1
.L_02001158_54:
	lsls r1, r6, #24
	lsrs r2, r5, #8
	adds r3, r1, #0
	orrs r3, r2
	lsrs r4, r6, #8
.L_02001158_43:
	adds r6, r4, #0
	adds r5, r3, #0
.L_02001158_46:
	mov r0, sp
	ldr r3, [r0, #4]
	ldr r2, [pc, #60]
	ldr r1, [pc, #64]
	ands r3, r2
	ands r1, r6
	orrs r3, r1
	str r3, [r0, #4]
	ldr r3, [pc, #40]
	ldrh r2, [r0, #6]
	ands r7, r3
	ldr r3, [pc, #52]
	lsls r1, r7, #4
	ands r3, r2
	orrs r3, r1
	strh r3, [r0, #6]
	mov r3, r10
	ldrb r2, [r0, #7]
	lsls r1, r3, #7
	movs r3, #127
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #7]
	ldr r3, [r0, #4]
	str r5, [r0, #4]
	str r3, [r0]
	ldr r1, [r0, #4]
	ldr r0, [r0]
	sub sp, #-8
	b .L_02001158_59
	.4byte 0x000007ff
	.4byte 0x1fffffff
	.4byte 0xfff00000
	.4byte 0x000fffff
	.4byte 0xffff800f
.L_02001158_59:
	pop {r3, r4}
	mov r8, r3
	mov r10, r4
	pop {r4, r5, r6, r7, pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #1
	bhi .L_02001158_60
	movs r2, #1
.L_02001158_60:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #4
	bne .L_02001158_61
	movs r2, #1
.L_02001158_61:
	adds r0, r2, #0
	pop {pc}
	push {lr}
	ldr r3, [r0]
	movs r2, #0
	cmp r3, #2
	bne .L_02001158_62
	movs r2, #1
.L_02001158_62:
	adds r0, r2, #0
	pop {pc}
	push {r4, r5, r6, r7, lr}
	ldr r4, [r0, #4]
	sub sp, #8
	mov r2, sp
	str r4, [r2]
	ldr r3, [r0]
	str r3, [r2, #4]
	lsls r3, r3, #12
	lsrs r6, r3, #12
	ldrh r3, [r2, #6]
	lsls r3, r3, #17
	adds r7, r1, #0
	lsrs r1, r3, #21
	ldrb r3, [r2, #7]
	lsrs r3, r3, #7
	adds r5, r4, #0
	str r3, [r7, #4]
	cmp r1, #0
	bne .L_02001158_63
	adds r3, r4, #0
	orrs r3, r6
	cmp r3, #0
	bne .L_02001158_64
	movs r3, #2
	str r3, [r7]
	b .L_02001158_65
.L_02001158_64:
	ldr r3, [pc, #132]
	lsrs r1, r5, #24
	lsls r2, r6, #8
	adds r4, r1, #0
	str r3, [r7, #8]
	orrs r4, r2
	lsls r3, r5, #8
	adds r6, r4, #0
	adds r5, r3, #0
	movs r3, #3
	str r3, [r7]
	ldr r3, [pc, #116]
	cmp r6, r3
	bhi .L_02001158_66
	mov r12, r3
.L_02001158_67:
	lsrs r1, r5, #31
	lsls r2, r6, #1
	adds r4, r1, #0
	lsls r3, r5, #1
	orrs r4, r2
	adds r6, r4, #0
	adds r5, r3, #0
	ldr r3, [r7, #8]
	subs r3, #1
	str r3, [r7, #8]
	cmp r6, r12
	bls .L_02001158_67
	b .L_02001158_66
.L_02001158_63:
	ldr r2, [pc, #84]
	cmp r1, r2
	bne .L_02001158_68
	adds r3, r4, #0
	orrs r3, r6
	cmp r3, #0
	bne .L_02001158_69
	movs r3, #4
	str r3, [r7]
	b .L_02001158_65
.L_02001158_69:
	movs r2, #128
	lsls r2, r2, #12
	adds r4, r6, #0
	movs r3, #0
	ands r4, r2
	orrs r3, r4
	cmp r3, #0
	beq .L_02001158_70
	movs r3, #1
.L_02001158_70:
	str r3, [r7]
.L_02001158_66:
	str r5, [r7, #12]
	str r6, [r7, #16]
	b .L_02001158_65
.L_02001158_68:
	ldr r2, [pc, #44]
	adds r3, r1, r2
	lsrs r1, r5, #24
	lsls r2, r6, #8
	adds r4, r1, #0
	orrs r4, r2
	ldr r1, [pc, #36]
	ldr r2, [pc, #36]
	str r3, [r7, #8]
	movs r3, #3
	str r3, [r7]
	orrs r4, r2
	lsls r3, r5, #8
	str r3, [r7, #12]
	str r4, [r7, #16]
.L_02001158_65:
	sub sp, #-8
	pop {r4, r5, r6, r7, pc}
	.4byte 0xfffffc02
	.4byte 0x0fffffff
	.4byte 0x000007ff
	.4byte 0xfffffc01
	.4byte 0x00000000
	.4byte 0x10000000
	.global KuupuappuDou_DirectionSteps
KuupuappuDou_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global KuupuappuDou_PuffScript
KuupuappuDou_PuffScript:
	.4byte 0x0000001b
AlchemyRuntime_02001888:
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouEntrancesOther
gKuupuappuDouEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouEntrances1
gKuupuappuDouEntrances1:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001a8
	.4byte 0x400000d8
	.4byte 0x00480000
	.4byte 0x01e00008
	.4byte 0x00000140
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x40000068
	.4byte 0x00480000
	.4byte 0x01e00008
	.4byte 0x00000140
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0xc0000100
	.4byte 0x00480000
	.4byte 0x01e00008
	.4byte 0x00000140
	.4byte 0xffff0004
	.4byte 0x000000b8
	.4byte 0xc0000108
	.4byte 0x00480000
	.4byte 0x01e00008
	.4byte 0x00000140
	.4byte 0xffff0005
	.4byte 0x00000158
	.4byte 0x400001d0
	.4byte 0x01180000
	.4byte 0x02800168
	.4byte 0x00000280
	.4byte 0xffff0006
	.4byte 0x000001e8
	.4byte 0x400001e8
	.4byte 0x01180000
	.4byte 0x02800168
	.4byte 0x00000280
	.4byte 0xffff0007
	.4byte 0x00000248
	.4byte 0x40000260
	.4byte 0x01180000
	.4byte 0x02800168
	.4byte 0x00000280
	.4byte 0xffff0008
	.4byte 0x00000158
	.4byte 0xc0000278
	.4byte 0x01180000
	.4byte 0x02800168
	.4byte 0x00000280
	.4byte 0xffff000d
	.4byte 0x00000198
	.4byte 0x400001d8
	.4byte 0x01180000
	.4byte 0x02800168
	.4byte 0x00000280
	.4byte 0xffff0009
	.4byte 0x00000058
	.4byte 0x400002c0
	.4byte 0x00280000
	.4byte 0x01600278
	.4byte 0x00000390
	.4byte 0xffff000c
	.4byte 0x000000b8
	.4byte 0x40000360
	.4byte 0x00280000
	.4byte 0x01600278
	.4byte 0x00000390
	.4byte 0xffff000a
	.4byte 0x000001c8
	.4byte 0x40000320
	.4byte 0x01500000
	.4byte 0x024002b8
	.4byte 0x00000358
	.4byte 0xffff000b
	.4byte 0x000001c8
	.4byte 0xc0000348
	.4byte 0x01500000
	.4byte 0x024002b8
	.4byte 0x00000358
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouEntrances2
gKuupuappuDouEntrances2:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000088
	.4byte 0x40000068
	.4byte 0x00280000
	.4byte 0x01900008
	.4byte 0x00000148
	.4byte 0xffff0002
	.4byte 0x00000158
	.4byte 0x40000098
	.4byte 0x00280000
	.4byte 0x01900008
	.4byte 0x00000148
	.4byte 0xffff0003
	.4byte 0x00000158
	.4byte 0xc00000b8
	.4byte 0x00280000
	.4byte 0x01900008
	.4byte 0x00000148
	.4byte 0xffff0004
	.4byte 0x000000a8
	.4byte 0xc0000148
	.4byte 0x00280000
	.4byte 0x01900008
	.4byte 0x00000148
	.4byte 0xffff0005
	.4byte 0x00000038
	.4byte 0x400001e0
	.4byte 0x00080000
	.4byte 0x00f80198
	.4byte 0x00000290
	.4byte 0xffff0006
	.4byte 0x00000098
	.4byte 0x400001d8
	.4byte 0x00080000
	.4byte 0x00f80198
	.4byte 0x00000290
	.4byte 0xffff0007
	.4byte 0x00000078
	.4byte 0x40000240
	.4byte 0x00080000
	.4byte 0x00f80198
	.4byte 0x00000290
	.4byte 0xffff0008
	.4byte 0x00000158
	.4byte 0x400001c8
	.4byte 0x01180000
	.4byte 0x02200158
	.4byte 0x000002a0
	.4byte 0xffff0009
	.4byte 0x000001a8
	.4byte 0xc0000298
	.4byte 0x01180000
	.4byte 0x02200158
	.4byte 0x000002a0
	.4byte 0xffff000e
	.4byte 0x000001c8
	.4byte 0x400001d8
	.4byte 0x01180000
	.4byte 0x02200158
	.4byte 0x000002a0
	.4byte 0xffff000a
	.4byte 0x000000c8
	.4byte 0x40000308
	.4byte 0x00680000
	.4byte 0x015802b8
	.4byte 0x000003b0
	.4byte 0xffff000b
	.4byte 0x00000108
	.4byte 0x40000328
	.4byte 0x00680000
	.4byte 0x015802b8
	.4byte 0x000003b0
	.4byte 0xffff000d
	.4byte 0x00000218
	.4byte 0xc00000a8
	.4byte 0x01a00000
	.4byte 0x02900018
	.4byte 0x000000b8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouEntrances3
gKuupuappuDouEntrances3:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0xc0000298
	.4byte 0x00c80000
	.4byte 0x01b80208
	.4byte 0x000002a8
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0x40000258
	.4byte 0x00c80000
	.4byte 0x01b80208
	.4byte 0x000002a8
	.4byte 0xffff0003
	.4byte 0x00000058
	.4byte 0x40000238
	.4byte 0x00080000
	.4byte 0x00f801d8
	.4byte 0x00000278
	.4byte 0xffff0004
	.4byte 0x000000a8
	.4byte 0x40000220
	.4byte 0x00080000
	.4byte 0x00f801d8
	.4byte 0x00000278
	.4byte 0xffff0005
	.4byte 0x000000f8
	.4byte 0xc0000150
	.4byte 0x00800000
	.4byte 0x01780028
	.4byte 0x00000180
	.4byte 0xffff0006
	.4byte 0x00000118
	.4byte 0x40000068
	.4byte 0x00800000
	.4byte 0x01780028
	.4byte 0x00000180
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x002f01bf
	.4byte 0x01cf030f
	.4byte 0x031f003f
	.4byte 0x000affff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouExitsOther
gKuupuappuDouExitsOther:
	.4byte 0x000001ff
	.global gKuupuappuDouExits1
gKuupuappuDouExits1:
	.4byte 0x00000060
	.4byte 0x00102062
	.4byte 0x00201061
	.4byte 0x00305060
	.4byte 0x0040d060
	.4byte 0x00503060
	.4byte 0x00606061
	.4byte 0x0070a061
	.4byte 0x00809060
	.4byte 0x00908060
	.4byte 0x00a0d017
	.4byte 0x00a0b060
	.4byte 0x00b0c060
	.4byte 0x00c0b060
	.4byte 0x00d04060
	.4byte 0x000001ff
	.global gKuupuappuDouExits2
gKuupuappuDouExits2:
	.4byte 0x00000061
	.4byte 0x00102060
	.4byte 0x00206062
	.4byte 0x00308061
	.4byte 0x00405061
	.4byte 0x00504061
	.4byte 0x00606060
	.4byte 0x00703062
	.4byte 0x00803061
	.4byte 0x0090b061
	.4byte 0x00a07060
	.4byte 0x00b09061
	.4byte 0x00d0d061
	.4byte 0x00e0e061
	.4byte 0x000001ff
	.global gKuupuappuDouExits3
gKuupuappuDouExits3:
	.4byte 0x00000062
	.4byte 0x0010e017
	.4byte 0x00201060
	.4byte 0x00307061
	.4byte 0x00405062
	.4byte 0x00504062
	.4byte 0x00602061
	.4byte 0x000001ff
	.global gKuupuappuDouPlacementsOther
gKuupuappuDouPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouPlacements1
gKuupuappuDouPlacements1:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouPlacements2
gKuupuappuDouPlacements2:
	.4byte 0xffff012b
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x036c0000
	.4byte 0x00004000
	.4byte 0xffff012b
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x037c0000
	.4byte 0x00004000
	.4byte 0xffff00f0
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00004000
	.4byte 0xffff00f0
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00004000
	.4byte 0xffff00f0
	.4byte 0x00000001
	.4byte 0x01840000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00004000
	.4byte 0xffff00f0
	.4byte 0x00000001
	.4byte 0x01a40000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00004000
	.4byte 0xffff00f0
	.4byte 0x00000001
	.4byte 0x01a40000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00004000
	.4byte 0xffff00f0
	.4byte 0x00000001
	.4byte 0x01c40000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00004000
	.4byte 0x030000f8
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouPlacements3
gKuupuappuDouPlacements3:
	.4byte 0xffff012b
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00004000
	.4byte 0xffff012b
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00ec0000
	.4byte 0x00004000
	.4byte 0xffff012b
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00004000
	.4byte 0xffff00f0
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0xffff00f0
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff00f0
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00004000
	.4byte 0xffff00f0
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouEventsOther
gKuupuappuDouEventsOther:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouEvents1
gKuupuappuDouEvents1:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte FieldScene_RunScene3a7SequenceB
	.4byte 0x00000003
	.4byte 0xffff0063
	.4byte SceneState_SetFlag953
	.4byte 0x00000013
	.4byte 0x0ef30064
	.4byte 0x00500003
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouEvents2
gKuupuappuDouEvents2:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000013
	.4byte 0x0fc40064
	.4byte 0x00100082
	.4byte 0x00000003
	.4byte 0xffff0063
	.4byte SceneState_SetFlag953
	.4byte 0x00008c15
	.4byte 0x09a90009
	.4byte FieldScene_RunScene3a7SequenceA
	.4byte 0x00000202
	.4byte 0xffff003c
	.4byte FieldScene_RunFlag9a9GuardedScene
	.4byte 0x00001815
	.4byte 0x02000010
	.4byte SceneState_ApplyRectAndMarkActor16
	.4byte 0x00001815
	.4byte 0x02010011
	.4byte SceneState_ConfigureRegion26_30AndMarkActor17
	.4byte 0x00001815
	.4byte 0x02020012
	.4byte SceneState_ConfigureRegion26_30AndClearActor18Mode
	.4byte 0x00001815
	.4byte 0x02030013
	.4byte SceneState_ApplyRectAndSetupActor19
	.4byte 0x00001815
	.4byte 0x02040014
	.4byte SceneActor_SetupSlotTwenty
	.4byte 0x00001815
	.4byte 0x02050015
	.4byte SceneActor_MarkSlot21AndSetFlag205
	.4byte 0x00004e15
	.4byte 0x03000016
	.4byte SceneState_ApplyFlag300
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKuupuappuDouEvents3
gKuupuappuDouEvents3:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte FieldScene_RunGuardedStep9AAAfterSetup
	.4byte 0x00008c15
	.4byte 0x09aa000a
	.4byte FieldScene_RunGuardedStep9AA
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte Resource3a7_NoOpCallback
	.4byte 0x00000003
	.4byte 0xffff0063
	.4byte SceneState_SetFlag953
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global KuupuappuDou_ScheduleFrames
KuupuappuDou_ScheduleFrames:
	.4byte 0x00000014
	.4byte 0x00000050
	.4byte 0x00000073
	.4byte 0x000000aa
	.global KuupuappuDou_ScheduleTimer
KuupuappuDou_ScheduleTimer:
	.4byte 0x00000000
	.global KuupuappuDou_ScheduleIndex
KuupuappuDou_ScheduleIndex:
	.4byte 0x00000000
	.global KuupuappuDou_TickCounter
KuupuappuDou_TickCounter:
	.4byte 0x00000000
	.global KuupuappuDou_TickValue
KuupuappuDou_TickValue:
	.4byte 0x00000000
