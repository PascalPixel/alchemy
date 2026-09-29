.syntax unified
	.thumb
	.section .text.x020083ac,"ax",%progbits
	.balign 4
	.global Func_020003ac
	.thumb_func
Func_020003ac:
	push {lr}
	ldr r3, [pc, #56]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_020003ac_0
	ldr r0, [pc, #44]
	b .L_020003ac_1
.L_020003ac_0:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_020003ac_2
	ldr r0, [pc, #44]
	b .L_020003ac_1
.L_020003ac_2:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_020003ac_3
	ldr r0, [pc, #40]
	b .L_020003ac_1
.L_020003ac_3:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020003ac_4
	ldr r0, [pc, #40]
	b .L_020003ac_1
.L_020003ac_4:
	ldr r0, [pc, #40]
.L_020003ac_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000059
	.4byte 0x0200a174
	.4byte 0x0000005a
	.4byte 0x0200a1d4
	.4byte 0x0000005b
	.4byte 0x0200a234
	.4byte 0x0000005c
	.4byte 0x0200a2dc
	.4byte 0x0200a12c
	.section .text.x0200841c,"ax",%progbits
	.balign 4
	.global Func_0200041c
	.thumb_func
Func_0200041c:
	push {r5, lr}
	ldr r5, [pc, #84]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_0200041c_0
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #5
	bne .L_0200041c_0
	ldr r0, [pc, #60]
	bl 0x02009d4c
.L_0200041c_0:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_0200041c_1
	ldr r0, [pc, #44]
	b .L_0200041c_2
.L_0200041c_1:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_0200041c_3
	ldr r0, [pc, #44]
	b .L_0200041c_2
.L_0200041c_3:
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_0200041c_4
	ldr r0, [pc, #36]
	b .L_0200041c_2
.L_0200041c_4:
	ldr r0, [pc, #36]
.L_0200041c_2:
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000005b
	.4byte 0x0000090a
	.4byte 0x00000059
	.4byte 0x0200a3c8
	.4byte 0x0000005a
	.4byte 0x0200a410
	.4byte 0x0200a4b8
	.4byte 0x0200a3b0
	.section .text.x020084e4,"ax",%progbits
	.balign 4
	.global Func_020004e4
	.thumb_func
Func_020004e4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r0, [pc, #900]
	sub sp, #12
	bl 0x02009d54
	ldr r3, [pc, #896]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #888]
	cmp r2, r3
	bne .L_020004e4_0
	movs r3, #22
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #8
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #21
	str r3, [sp, #4]
	movs r5, #23
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009d14
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009d1c
	movs r3, #16
	movs r2, #42
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #36
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #14
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	b .L_020004e4_1
.L_020004e4_0:
	ldr r3, [pc, #748]
	cmp r2, r3
	beq .L_020004e4_2
	b .L_020004e4_3
.L_020004e4_2:
	movs r3, #42
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #11
	str r3, [sp, #4]
	movs r5, #20
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009d14
	movs r3, #13
	str r3, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009d1c
	movs r3, #14
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r2, #56
	movs r3, #18
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r2
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #22
	str r3, [sp, #4]
	movs r5, #7
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009d14
	movs r6, #24
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009d1c
	movs r3, #23
	str r3, [sp, #4]
	movs r5, #44
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009d14
	movs r3, #25
	str r3, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009d1c
	movs r3, #38
	str r3, [sp, #0]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	str r6, [sp, #4]
	bl 0x02009d14
	movs r3, #26
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #17
	movs r2, #35
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #50
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r5, #34
	movs r6, #43
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009d14
	movs r3, #45
	str r3, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009d1c
	movs r3, #6
	movs r2, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #27
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #70
	movs r1, #68
	movs r2, #4
	movs r3, #2
	str r6, [sp, #0]
	bl 0x02009d14
	b .L_020004e4_1
.L_020004e4_3:
	ldr r3, [pc, #392]
	cmp r2, r3
	bne .L_020004e4_1
	movs r3, #16
	str r3, [sp, #4]
	movs r6, #8
	movs r0, #69
	movs r1, #99
	movs r2, #4
	movs r3, #2
	str r6, [sp, #0]
	bl 0x02009d24
	movs r1, #6
	movs r2, #20
	str r1, [sp, #0]
	str r2, [sp, #4]
	mov r8, r1
	mov r10, r2
	movs r0, #69
	movs r1, #99
	movs r2, #4
	movs r3, #2
	bl 0x02009d24
	movs r3, #23
	str r3, [sp, #4]
	movs r5, #10
	movs r0, #69
	movs r1, #99
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009d24
	movs r3, #14
	str r3, [sp, #4]
	movs r0, #69
	movs r1, #99
	movs r2, #4
	movs r3, #2
	str r6, [sp, #0]
	bl 0x02009d1c
	movs r3, #18
	mov r1, r8
	str r1, [sp, #0]
	str r3, [sp, #4]
	movs r0, #69
	movs r1, #99
	movs r2, #4
	movs r3, #2
	bl 0x02009d1c
	mov r2, r8
	mov r3, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #69
	movs r1, #99
	movs r2, #4
	movs r3, #1
	bl 0x02009d1c
	movs r3, #21
	str r3, [sp, #4]
	movs r0, #69
	movs r1, #99
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009d1c
	movs r5, #32
	movs r0, #0
	movs r1, #121
	movs r2, #5
	movs r3, #7
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #43
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #121
	movs r2, #5
	movs r3, #7
	str r5, [sp, #4]
	bl 0x02009d14
	movs r6, #9
	movs r5, #5
	movs r0, #6
	movs r1, #120
	movs r2, #3
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #44
	str r3, [sp, #0]
	movs r0, #9
	movs r1, #120
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009d14
	mov r1, r8
	str r1, [sp, #4]
	movs r0, #9
	movs r1, #0
	movs r2, #3
	movs r3, #3
	str r6, [sp, #0]
	bl 0x02009d1c
.L_020004e4_1:
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x02009da4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009da4
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009da4
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x02009da4
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009da4
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009da4
	movs r5, #100
.L_020004e4_4:
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	adds r5, #1
	bl 0x02009e24
	cmp r5, #107
	ble .L_020004e4_4
	ldr r3, [pc, #64]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #68]
	cmp r2, r3
	beq .L_020004e4_5
	movs r3, #128
	movs r1, #128
	lsls r3, r3, #8
	lsls r1, r1, #7
	movs r2, #128
	str r3, [sp, #4]
	str r1, [sp, #8]
	movs r3, #128
	movs r1, #128
	lsls r2, r2, #9
	movs r0, #0
	lsls r1, r1, #11
	lsls r3, r3, #6
	str r2, [sp, #0]
	bl 0x02009e14
.L_020004e4_5:
	sub sp, #-12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x00000201
	.4byte 0x02000240
	.4byte 0x00000059
	.4byte 0x0000005a
	.4byte 0x0000005b
	.4byte 0x0000005c
	.global Func_0200088c
	.thumb_func
Func_0200088c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #128
	lsls r0, r0, #2
	sub sp, #8
	bl 0x02009d54
	ldr r0, [pc, #892]
	bl 0x02009d4c
	ldr r3, [pc, #888]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #880]
	cmp r2, r3
	bne .L_0200088c_0
	movs r3, #7
	str r3, [sp, #4]
	movs r6, #22
	movs r0, #64
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r6, [sp, #0]
	bl 0x02009d14
	movs r3, #8
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #68
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #21
	str r3, [sp, #4]
	movs r5, #23
	movs r0, #72
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009d14
	movs r0, #72
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009d1c
	movs r3, #16
	movs r2, #42
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #36
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #80
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #14
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #84
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r1, #200
	movs r2, #182
	movs r0, #9
	lsls r1, r1, #17
	b .L_0200088c_1
.L_0200088c_0:
	ldr r3, [pc, #732]
	cmp r2, r3
	beq .L_0200088c_2
	b .L_0200088c_3
.L_0200088c_2:
	movs r3, #42
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #11
	str r3, [sp, #4]
	movs r5, #20
	movs r0, #68
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009d14
	movs r6, #12
	movs r0, #68
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009d1c
	movs r3, #14
	str r3, [sp, #0]
	movs r0, #72
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r6, [sp, #4]
	bl 0x02009d14
	movs r2, #56
	movs r3, #18
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r10, r2
	movs r0, #76
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #22
	str r3, [sp, #4]
	movs r5, #7
	movs r0, #80
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009d14
	movs r6, #23
	movs r0, #80
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009d1c
	movs r3, #44
	str r3, [sp, #0]
	mov r8, r3
	movs r0, #84
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r6, [sp, #4]
	bl 0x02009d14
	mov r1, r8
	str r1, [sp, #0]
	movs r5, #24
	movs r0, #84
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d1c
	movs r3, #38
	str r3, [sp, #0]
	movs r0, #88
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #26
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #92
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #17
	movs r2, #35
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #96
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #50
	movs r2, #36
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #100
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r5, #34
	movs r6, #43
	movs r0, #104
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009d14
	mov r2, r8
	str r2, [sp, #4]
	movs r0, #104
	movs r1, #126
	movs r2, #4
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009d1c
	movs r3, #6
	movs r2, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #108
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	movs r3, #27
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #112
	movs r1, #126
	movs r2, #4
	movs r3, #2
	bl 0x02009d14
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #116
	movs r3, #2
	movs r1, #126
	movs r2, #4
	str r6, [sp, #0]
	bl 0x02009d14
	movs r1, #176
	movs r2, #204
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x02009da4
	movs r1, #184
	movs r2, #198
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl 0x02009da4
	movs r1, #144
	movs r2, #190
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009da4
	movs r1, #144
	movs r2, #179
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x02009da4
	movs r1, #162
	movs r2, #204
	movs r0, #13
	lsls r1, r1, #18
.L_0200088c_1:
	lsls r2, r2, #17
	bl 0x02009da4
	b .L_0200088c_4
.L_0200088c_3:
	ldr r3, [pc, #308]
	cmp r2, r3
	bne .L_0200088c_4
	movs r1, #8
	movs r3, #14
	str r1, [sp, #0]
	str r3, [sp, #4]
	mov r8, r1
	movs r0, #64
	movs r1, #124
	movs r2, #4
	movs r3, #4
	bl 0x02009d14
	movs r3, #18
	str r3, [sp, #4]
	movs r6, #6
	movs r0, #68
	movs r1, #124
	movs r2, #4
	movs r3, #4
	str r6, [sp, #0]
	bl 0x02009d14
	movs r3, #20
	str r3, [sp, #4]
	movs r0, #68
	movs r1, #124
	movs r2, #4
	movs r3, #1
	str r6, [sp, #0]
	bl 0x02009d1c
	movs r3, #10
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #72
	movs r1, #124
	movs r2, #4
	movs r3, #4
	bl 0x02009d14
	mov r2, r8
	str r2, [sp, #0]
	movs r5, #32
	movs r0, #10
	movs r1, #121
	movs r2, #5
	movs r3, #7
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #43
	str r3, [sp, #0]
	movs r0, #5
	movs r1, #121
	movs r2, #5
	movs r3, #7
	str r5, [sp, #4]
	bl 0x02009d14
	movs r5, #9
	movs r7, #5
	movs r0, #0
	movs r1, #120
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl 0x02009d14
	movs r3, #44
	str r3, [sp, #0]
	movs r0, #3
	movs r3, #1
	movs r1, #120
	movs r2, #3
	str r7, [sp, #4]
	bl 0x02009d14
	movs r1, #168
	movs r2, #184
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl 0x02009da4
	movs r1, #128
	movs r2, #158
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009da4
	movs r0, #6
	movs r1, #0
	movs r2, #3
	movs r3, #3
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009d1c
	ldr r0, [pc, #104]
	bl 0x02009d44
	cmp r0, #0
	bne .L_0200088c_4
	movs r0, #0
	movs r1, #119
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	str r7, [sp, #4]
	bl 0x02009d14
.L_0200088c_4:
	movs r5, #100
.L_0200088c_5:
	movs r1, #1
	movs r2, #1
	adds r0, r5, #0
	negs r1, r1
	negs r2, r2
	adds r5, #1
	bl 0x02009e24
	cmp r5, #107
	ble .L_0200088c_5
	bl 0x02009e2c
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	beq .L_0200088c_6
	bl 0x02009e1c
.L_0200088c_6:
	sub sp, #-8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x00000201
	.4byte 0x02000240
	.4byte 0x00000059
	.4byte 0x0000005a
	.4byte 0x0000005b
	.4byte 0x0000090a
	.4byte 0x0000005c
	.global Func_02000c38
	.thumb_func
Func_02000c38:
	push {lr}
	movs r0, #8
	movs r1, #2
	bl 0x02009dc4
	ldr r0, [pc, #28]
	movs r1, #5
	bl 0x02009e0c
	ldr r3, [pc, #24]
	ldr r2, [pc, #24]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	movs r0, #53
	movs r1, #5
	bl 0x02009e04
	pop {r0}
	bx r0
	.4byte 0x0000005b
	.4byte 0x02000240
	.4byte 0x0000022b
	.section .text.x02008cd0,"ax",%progbits
	.balign 4
	.global Func_02000cd0
	.thumb_func
Func_02000cd0:
	push	{r5, r6, lr}
	ldr	r0, [pc, #268]
	ldr	r3, [pc, #268]
	ldr	r2, [pc, #272]
	ldr	r6, [r3, #0]
	adds	r3, r2, r0
	movs	r0, #0
	ldrsh	r1, [r3, r0]
	movs	r3, #100
	adds	r0, r1, #0
	muls	r0, r3
	movs	r1, #139
	lsls	r1, r1, #2
	adds	r2, r2, r1
	movs	r3, #0
	ldrsh	r1, [r2, r3]
	bl 0x02009c84
	adds	r5, r0, #0
	ldr	r0, [pc, #244]
	bl 0x02009d44
	cmp	r0, #0
	bne.n	.L_02000dda
	ldr	r0, [pc, #236]
	bl 0x02009d44
	cmp	r0, #0
	beq.n	.L_02000d28
	cmp	r5, #74
	bgt.n	.L_02000d28
	ldr	r0, [pc, #224]
	bl 0x02009d54
	ldr	r0, [pc, #220]
	bl 0x02009d54
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x02009d54
	ldr	r0, [pc, #212]
	bl 0x02009d54
.L_02000d28:
	ldr	r0, [pc, #208]
	bl 0x02009d44
	cmp	r0, #0
	beq.n	.L_02000d50
	cmp	r5, #49
	bgt.n	.L_02000d50
	ldr	r0, [pc, #196]
	bl 0x02009d54
	ldr	r0, [pc, #180]
	bl 0x02009d54
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x02009d54
	ldr	r0, [pc, #172]
	bl 0x02009d54
.L_02000d50:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02009d44
	cmp	r0, #0
	beq.n	.L_02000d7c
	cmp	r5, #24
	bgt.n	.L_02000d7c
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02009d54
	ldr	r0, [pc, #136]
	bl 0x02009d54
	movs	r0, #193
	lsls	r0, r0, #2
	bl 0x02009d54
	ldr	r0, [pc, #128]
	bl 0x02009d54
.L_02000d7c:
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02009d44
	cmp	r0, #0
	bne.n	.L_02000d9e
	cmp	r5, #24
	ble.n	.L_02000d9e
	movs	r0, #192
	lsls	r0, r0, #2
	bl 0x02009d4c
	movs	r0, #193
	lsls	r0, r0, #1
	adds	r2, r6, r0
	movs	r3, #1
	strh	r3, [r2, #0]
.L_02000d9e:
	ldr	r0, [pc, #92]
	bl 0x02009d44
	cmp	r0, #0
	bne.n	.L_02000dbc
	cmp	r5, #49
	ble.n	.L_02000dbc
	ldr	r0, [pc, #76]
	bl 0x02009d4c
	movs	r1, #193
	lsls	r1, r1, #1
	adds	r2, r6, r1
	movs	r3, #2
	strh	r3, [r2, #0]
.L_02000dbc:
	ldr	r0, [pc, #48]
	bl 0x02009d44
	cmp	r0, #0
	bne.n	.L_02000dda
	cmp	r5, #74
	ble.n	.L_02000dda
	ldr	r0, [pc, #36]
	bl 0x02009d4c
	movs	r3, #193
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #3
	strh	r3, [r2, #0]
.L_02000dda:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x00000232
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000201
	.4byte 0x00000302
	.4byte 0x00000303
	.4byte 0x00000305
	.2byte 0x0301
	.2byte 0x0000
	.global Func_02000e00
	.thumb_func
Func_02000e00:
	push {lr}
	ldmia r0!, {r3}
	ldmia r1!, {r4}
	ldr r2, [r0, #4]
	subs r4, r4, r3
	ldr r3, [r1]
	subs r3, r3, r2
	asrs r3, r3, #16
	asrs r4, r4, #16
	adds r2, r3, #0
	muls r2, r3
	adds r0, r4, #0
	muls r0, r4
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, [pc, #8]
	bl 0x02009e58
	pop {r1}
	bx r1
	.4byte 0x030001d8
	.global Func_02000e2c
	.thumb_func
Func_02000e2c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #492]
	movs	r2, #240
	ldr	r3, [r3, #0]
	movs	r0, #128
	sub	sp, #104
	movs	r1, #60
	lsls	r2, r2, #16
	lsls	r0, r0, #2
	str	r3, [sp, #20]
	str	r1, [sp, #16]
	mov	fp, r2
	bl 0x02009d4c
	movs	r0, #1
	bl 0x02009c78
	ldr	r3, [pc, #464]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #456]
	cmp	r2, r3
	bne.n	.L_02000e74
	ldr	r2, [pc, #452]
	movs	r7, #3
	mov	r8, r2
	b.n	.L_02000e88
.L_02000e74:
	ldr	r3, [pc, #448]
	cmp	r2, r3
	bne.n	.L_02000e82
	ldr	r3, [pc, #448]
	movs	r7, #5
	mov	r8, r3
	b.n	.L_02000e88
.L_02000e82:
	ldr	r1, [pc, #444]
	movs	r7, #2
	mov	r8, r1
.L_02000e88:
	mov	r9, r7
	mov	r5, r8
	cmp	r7, #0
	beq.n	.L_02000eb8
	movs	r6, #0
.L_02000e92:
	movs	r0, #0
	bl 0x02009d74
	adds	r1, r5, #0
	adds	r0, #8
	bl 0x02008e00
	cmp	r0, fp
	bgt.n	.L_02000eac
	mov	r2, r9
	subs	r2, r2, r7
	mov	fp, r0
	mov	sl, r2
.L_02000eac:
	adds	r6, #8
	mov	r3, r8
	subs	r7, #1
	adds	r5, r3, r6
	cmp	r7, #0
	bne.n	.L_02000e92
.L_02000eb8:
	mov	r1, sl
	lsls	r1, r1, #1
	mov	sl, r1
	movs	r2, #128
	movs	r1, #128
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	movs	r0, #0
	bl 0x02009d7c
	movs	r0, #0
	bl 0x02009d74
	mov	r2, sl
	lsls	r3, r2, #2
	add	r3, r8
	movs	r2, #0
	ldr	r1, [r3, #0]
	ldr	r3, [r3, #4]
	bl 0x02009d04
	movs	r0, #0
	bl 0x02009d74
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r0, #152
	bl 0x02009e44
	movs	r0, #0
	bl 0x02009d74
	adds	r5, r0, #0
	movs	r0, #0
	bl 0x02009d74
	ldr	r1, [r0, #12]
	adds	r0, r5, #0
	bl 0x02008324
	movs	r0, #241
	bl 0x02009e44
	movs	r0, #0
	bl 0x02009d74
	movs	r3, #214
	add	r4, sp, #64
	strh	r3, [r4, #24]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r4, #8]
	ldr	r3, [pc, #288]
	str	r3, [r4, #12]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r4, #16]
	ldr	r3, [pc, #280]
	str	r3, [r4, #20]
	movs	r3, #224
	ldr	r5, [r0, #8]
	lsls	r3, r3, #13
	ldr	r1, [r0, #12]
	ldr	r2, [r0, #16]
	movs	r6, #0
	str	r3, [sp, #8]
	adds	r0, r5, #0
	movs	r3, #0
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	str	r4, [sp, #12]
	bl 0x0200813c
	movs	r1, #130
	movs	r0, #0
	lsls	r1, r1, #1
	movs	r2, #0
	bl 0x02009dec
	movs	r1, #18
	movs	r0, #0
	bl 0x02009dac
	ldr	r1, [pc, #232]
	ldr	r3, [sp, #20]
	ldr	r5, [pc, #232]
	adds	r6, r3, r1
	movs	r7, #0
.L_02000f6a:
	movs	r3, #150
	lsls	r3, r3, #2
	strh	r3, [r6, #0]
	ldr	r2, [sp, #16]
	subs	r2, #1
	str	r2, [sp, #16]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	beq.n	.L_02000f98
	subs	r3, r2, #5
	strh	r3, [r5, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bgt.n	.L_02000f8e
	strh	r7, [r5, #0]
	b.n	.L_02000f98
.L_02000f8e:
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L_02000f98
	movs	r3, #1
	str	r3, [sp, #16]
.L_02000f98:
	movs	r0, #1
	bl 0x02009c8c
	ldr	r1, [sp, #16]
	cmp	r1, #0
	bne.n	.L_02000f6a
	movs	r0, #0
	bl 0x02009d74
	movs	r3, #214
	add	r4, sp, #24
	strh	r3, [r4, #24]
	ldr	r3, [pc, #144]
	movs	r2, #128
	str	r3, [r4, #12]
	ldr	r3, [pc, #144]
	lsls	r2, r2, #8
	str	r2, [r4, #8]
	str	r2, [r4, #16]
	str	r3, [r4, #20]
	ldr	r3, [sp, #16]
	ldr	r2, [r0, #16]
	ldr	r1, [r0, #12]
	ldr	r5, [r0, #8]
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r3, #224
	lsls	r3, r3, #13
	str	r3, [sp, #8]
	adds	r0, r5, #0
	movs	r3, #0
	str	r4, [sp, #12]
	bl 0x0200813c
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x02009e44
	movs	r0, #152
	bl 0x02009e44
	movs	r0, #0
	bl 0x02009d74
	movs	r3, #192
	lsls	r3, r3, #11
	str	r3, [r0, #40]
	movs	r1, #1
	movs	r0, #0
	bl 0x02009dac
	movs	r0, #10
	bl 0x02009d5c
	ldr	r2, [pc, #68]
	ldr	r1, [sp, #20]
	adds	r3, r1, r2
	add	r1, sp, #16
	ldrh	r1, [r1, #0]
	movs	r0, #0
	strh	r1, [r3, #0]
	bl 0x02009c78
	add	sp, #104
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000059
	.4byte 0x02009f30
	.4byte 0x0000005a
	.4byte 0x02009f48
	.4byte 0x02009f70
	.4byte 0x0000cccc
	.4byte 0x00013333
	.4byte 0x00000cba
	.2byte 0x0472
	.2byte 0x0200
	.section .text.x02009638,"ax",%progbits
	.balign 4
	.global Func_02001638
	.thumb_func
Func_02001638:
	push {r5, lr}
	ldr r0, [pc, #532]
	movs r3, #139
	lsls r3, r3, #2
	adds r2, r0, r3
	adds r3, #44
	strh r3, [r2]
	ldr r2, [pc, #524]
	movs r1, #0
	adds r3, r0, r2
	strh r1, [r3]
	movs r3, #140
	lsls r3, r3, #2
	adds r2, r0, r3
	ldr r3, [pc, #512]
	strh r3, [r2]
	movs r2, #224
	lsls r2, r2, #1
	adds r5, r0, r2
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, [pc, #504]
	sub sp, #8
	cmp r2, r3
	bne .L_02001638_0
	b .L_02001638_1
.L_02001638_0:
	ldr r3, [pc, #496]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	bl 0x02009c1c
	movs r1, #200
	ldr r0, [pc, #480]
	lsls r1, r1, #4
	bl 0x02009c94
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, [pc, #472]
	cmp r2, r3
	bne .L_02001638_2
	movs r3, #64
	movs r5, #126
	str r3, [sp, #0]
	movs r0, #22
	movs r1, #7
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #68
	str r3, [sp, #0]
	movs r0, #8
	movs r1, #10
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #72
	str r3, [sp, #0]
	movs r0, #23
	movs r1, #21
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #76
	str r3, [sp, #0]
	movs r0, #16
	movs r1, #42
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #80
	str r3, [sp, #0]
	movs r0, #36
	movs r1, #44
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #84
	str r3, [sp, #0]
	movs r0, #14
	movs r1, #55
	b .L_02001638_3
.L_02001638_2:
	ldr r3, [pc, #368]
	cmp r2, r3
	bne .L_02001638_4
	movs r3, #64
	movs r5, #126
	str r3, [sp, #0]
	movs r0, #42
	movs r1, #5
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #68
	str r3, [sp, #0]
	movs r0, #20
	movs r1, #11
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #72
	str r3, [sp, #0]
	movs r0, #14
	movs r1, #12
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #76
	str r3, [sp, #0]
	movs r0, #56
	movs r1, #18
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #80
	str r3, [sp, #0]
	movs r0, #7
	movs r1, #22
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #84
	str r3, [sp, #0]
	movs r0, #44
	movs r1, #23
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #88
	str r3, [sp, #0]
	movs r0, #38
	movs r1, #24
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #92
	str r3, [sp, #0]
	movs r0, #26
	movs r1, #28
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #96
	str r3, [sp, #0]
	movs r0, #17
	movs r1, #35
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #100
	str r3, [sp, #0]
	movs r0, #50
	movs r1, #36
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #104
	str r3, [sp, #0]
	movs r0, #34
	movs r1, #43
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #108
	str r3, [sp, #0]
	movs r0, #6
	movs r1, #46
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #112
	str r3, [sp, #0]
	movs r0, #27
	movs r1, #55
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #116
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #56
.L_02001638_3:
	movs r2, #4
	movs r3, #2
	str r5, [sp, #4]
	bl 0x02009d14
	b .L_02001638_5
.L_02001638_4:
	ldr r3, [pc, #112]
	cmp r2, r3
	bne .L_02001638_5
	movs r0, #169
	bl 0x02009e3c
	movs r3, #64
	movs r5, #124
	str r3, [sp, #0]
	movs r0, #8
	movs r1, #14
	movs r2, #4
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #68
	str r3, [sp, #0]
	movs r0, #6
	movs r1, #18
	movs r2, #4
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009d14
	movs r3, #72
	str r3, [sp, #0]
	movs r0, #10
	movs r1, #21
	movs r2, #4
	movs r3, #4
	str r5, [sp, #4]
	bl 0x02009d14
.L_02001638_5:
	bl 0x020084e4
.L_02001638_1:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000022e
	.4byte 0x00000119
	.4byte 0x0000005c
	.4byte 0x03001ebc
	.4byte 0x02008cd1
	.4byte 0x00000059
	.4byte 0x0000005a
	.4byte 0x0000005b
	.section .text.x020098a4,"ax",%progbits
	.balign 4
	.global Func_020018a4
	.thumb_func
Func_020018a4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #72]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	ldr	r2, [pc, #68]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	sub	sp, #12
	lsrs	r3, r3, #5
	str	r3, [sp, #8]
	ldr	r3, [pc, #60]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_020018da
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #36]
	adds	r4, r2, #0
	strh	r3, [r2, #0]
	b.n	.L_02001932
.L_020018da:
	movs	r0, #130
	lsls	r0, r0, #1
	bl 0x02009d44
	cmp	r0, #0
	beq.n	.L_0200190c
	ldr	r4, [pc, #32]
	movs	r5, #0
	ldrsh	r3, [r4, r5]
	ldrh	r2, [r4, #0]
	cmp	r3, #0
	ble.n	.L_02001932
	subs	r3, r2, #1
	strh	r3, [r4, #0]
	b.n	.L_02001932
	.4byte 0x00000002
	.4byte 0x0200a6d0
	.4byte 0x03001b10
	.4byte 0x0200b030
	.2byte 0xa6be
	.2byte 0x0200
.L_0200190c:
	ldr	r4, [pc, #56]
	movs	r0, #0
	ldrsh	r3, [r4, r0]
	ldrh	r2, [r4, #0]
	cmp	r3, #1
	bgt.n	.L_02001932
	adds	r3, r2, #1
	movs	r1, #128
	strh	r3, [r4, #0]
	lsls	r1, r1, #9
	lsls	r3, r3, #16
	cmp	r3, r1
	bne.n	.L_02001932
	ldr	r3, [pc, #36]
	ldr	r0, [pc, #36]
	ldr	r1, [pc, #40]
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_02001932:
	movs	r2, #0
	ldrsh	r0, [r4, r2]
	cmp	r0, #0
	bne.n	.L_02001960
	ldr	r3, [pc, #32]
	movs	r5, #0
	ldrsh	r0, [r3, r5]
	bl 0x02009cc4
	b.n	.L_02001be4
	.2byte 0x0000
	.4byte 0x0200a6be
	.4byte 0x040000d4
	.4byte 0x02009f80
	.4byte 0x050003c0
	.4byte 0x80000010
	.2byte 0xa6d0
	.2byte 0x0200
.L_02001960:
	ldr	r3, [pc, #132]
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L_0200198c
	ldr	r2, [pc, #128]
	adds	r3, r1, r2
	ldrb	r2, [r3, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #5
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r1, r1, r3
	lsls	r3, r0, #19
	asrs	r2, r3, #16
	adds	r1, #38
	movs	r4, #0
.L_02001982:
	adds	r4, #1
	strh	r2, [r1, #0]
	adds	r1, #4
	cmp	r4, #143
	bls.n	.L_02001982
.L_0200198c:
	movs	r0, #144
	lsls	r0, r0, #4
	bl 0x02009cac
	mov	r9, r0
	ldr	r3, [pc, #88]
	ldr	r0, [pc, #88]
	mov	r1, r9
	ldr	r2, [pc, #88]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, r9
	adds	r3, #12
	ldr	r5, [pc, #60]
	str	r3, [sp, #4]
	mov	sl, r3
	movs	r4, #6
	mov	fp, r5
.L_020019b0:
	mov	r1, sl
	movs	r0, #0
	ldrsh	r2, [r1, r0]
	movs	r3, #248
	lsls	r2, r2, #16
	lsls	r3, r3, #13
	ands	r3, r2
	lsrs	r3, r3, #16
	mov	r8, r3
	lsrs	r5, r2, #21
	mov	r3, fp
	lsrs	r2, r2, #26
	ands	r2, r3
	ands	r5, r3
	ldr	r3, [pc, #44]
	movs	r0, #0
	ldrsh	r6, [r3, r0]
	movs	r1, #3
	adds	r0, r6, #0
	adds	r7, r2, #0
	str	r4, [sp, #0]
	bl 0x02009c84
	movs	r1, #6
	add	r8, r0
	b.n	.L_02001a00
	.4byte 0x0000001f
	.4byte 0x03001ecc
	.4byte 0x00000539
	.4byte 0x040000d4
	.4byte 0x02009f80
	.4byte 0x80000010
	.2byte 0xa6bc
	.2byte 0x0200
.L_02001a00:
	adds	r0, r6, #0
	bl 0x02009c84
	subs	r7, #20
	subs	r0, r7, r0
	adds	r7, r0, #0
	adds	r7, #20
	ldr	r4, [sp, #0]
	cmp	r6, #60
	ble.n	.L_02001a30
	ldr	r3, [pc, #208]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001a30
	lsls	r0, r6, #6
	movs	r1, #120
	bl 0x02009c84
	adds	r0, r5, r0
	adds	r5, r0, #0
	ldr	r4, [sp, #0]
	subs	r5, #32
.L_02001a30:
	mov	r1, r8
	cmp	r1, #31
	bls.n	.L_02001a3a
	movs	r2, #31
	mov	r8, r2
.L_02001a3a:
	cmp	r5, #31
	bls.n	.L_02001a40
	movs	r5, #31
.L_02001a40:
	cmp	r7, #31
	bls.n	.L_02001a46
	movs	r7, #31
.L_02001a46:
	lsls	r2, r5, #5
	lsls	r3, r7, #10
	orrs	r3, r2
	mov	r5, r8
	mov	r0, sl
	orrs	r3, r5
	movs	r1, #2
	adds	r4, #1
	strh	r3, [r0, #0]
	add	sl, r1
	cmp	r4, #11
	bls.n	.L_020019b0
	ldr	r2, [sp, #4]
	ldr	r5, [pc, #136]
	mov	r6, r9
	ldr	r1, [r2, #0]
	adds	r0, r5, #0
	adds	r6, #16
	adds	r5, #4
	bl 0x02009ce4
	adds	r0, r5, #0
	ldr	r1, [r6, #0]
	adds	r5, #4
	adds	r6, #4
	bl 0x02009ce4
	ldr	r1, [r6, #0]
	adds	r0, r5, #0
	bl 0x02009ce4
	ldr	r2, [pc, #104]
	ldr	r0, [pc, #108]
	adds	r3, r2, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	lsls	r0, r3, #4
	subs	r0, r0, r3
	movs	r3, #139
	lsls	r3, r3, #2
	adds	r2, r2, r3
	movs	r3, #0
	ldrsh	r1, [r2, r3]
	lsls	r0, r0, #3
	bl 0x02009c84
	ldr	r5, [pc, #84]
	movs	r1, #236
	strh	r0, [r5, #0]
	lsls	r1, r1, #15
	lsls	r0, r0, #16
	cmp	r0, r1
	ble.n	.L_02001ab6
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #0]
.L_02001ab6:
	ldr	r1, [pc, #68]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	ldrh	r2, [r1, #0]
	cmp	r3, #0
	beq.n	.L_02001ad4
	adds	r3, r2, #0
	subs	r3, #8
	strh	r2, [r5, #0]
	strh	r3, [r1, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bgt.n	.L_02001ad4
	ldr	r3, [pc, #16]
	strh	r3, [r1, #0]
.L_02001ad4:
	ldr	r3, [pc, #40]
	ldr	r0, [pc, #44]
	mov	r1, r9
	ldr	r2, [pc, #44]
	b.n	.L_02001b0c
	.2byte 0x0000
	.4byte 0x00000077
	.4byte 0x00000000
	.4byte 0x03001e40
	.4byte 0x050003cc
	.4byte 0x02000240
	.4byte 0x00000232
	.4byte 0x0200a6bc
	.4byte 0x0200a6c0
	.4byte 0x040000d4
	.4byte 0x0200a730
	.2byte 0x0240
	.2byte 0x8400
.L_02001b0c:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #228]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #118
	bgt.n	.L_02001b52
	ldr	r3, [pc, #224]
	movs	r5, #0
	ldrsh	r2, [r3, r5]
	movs	r3, #128
	mov	r1, r9
	movs	r4, #12
	subs	r3, r3, r2
	adds	r1, #80
	ldr	r0, [pc, #212]
	cmp	r4, r3
	bcs.n	.L_02001b48
	movs	r2, #7
	mov	ip, r3
.L_02001b34:
	adds	r3, r4, #0
	ands	r3, r2
	str	r0, [r1, #32]
	stmia	r1!, {r0}
	cmp	r3, #7
	bne.n	.L_02001b42
	adds	r1, #32
.L_02001b42:
	adds	r4, #1
	cmp	r4, ip
	bcc.n	.L_02001b34
.L_02001b48:
	mov	r0, r9
	ldr	r3, [r0, #0]
	str	r3, [r1, #0]
	ldr	r3, [r0, #32]
	str	r3, [r1, #32]
.L_02001b52:
	movs	r2, #144
	lsls	r2, r2, #3
	ldr	r0, [pc, #172]
	mov	r1, r9
	add	r2, r9
	movs	r4, #0
.L_02001b5e:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	cmp	r3, #0
	beq.n	.L_02001b68
	strb	r3, [r1, #0]
.L_02001b68:
	adds	r4, #1
	adds	r1, #1
	cmp	r4, r0
	bls.n	.L_02001b5e
	ldr	r3, [pc, #148]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #144
	mov	r2, r9
	lsls	r1, r1, #3
	bl 0x02009ccc
	ldr	r2, [pc, #136]
	ldr	r5, [pc, #140]
	mov	r8, r2
	movs	r4, #0
	movs	r7, #0
	movs	r6, #8
.L_02001b8c:
	ldr	r3, [pc, #132]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	lsls	r3, r3, #3
	adds	r2, r3, #0
	ldr	r3, [pc, #128]
	subs	r2, #16
	ands	r2, r3
	cmp	r4, #4
	bne.n	.L_02001ba6
	movs	r1, #128
	lsls	r1, r1, #23
	mov	r8, r1
.L_02001ba6:
	movs	r3, #0
	str	r3, [r5, #0]
	lsls	r3, r2, #16
	orrs	r3, r6
	mov	r2, r8
	orrs	r3, r2
	str	r3, [r5, #4]
	ldr	r0, [sp, #8]
	movs	r3, #228
	lsls	r3, r3, #8
	orrs	r3, r0
	ldr	r0, [pc, #80]
	str	r3, [r5, #8]
	adds	r0, r7, r0
	movs	r1, #255
	str	r4, [sp, #0]
	bl 0x02009cdc
	ldr	r1, [sp, #8]
	ldr	r4, [sp, #0]
	adds	r1, #8
	adds	r4, #1
	adds	r5, #12
	str	r1, [sp, #8]
	adds	r7, #12
	adds	r6, #32
	cmp	r4, #4
	bls.n	.L_02001b8c
	mov	r0, r9
	bl 0x02009cb4
.L_02001be4:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0200a6c0
	.4byte 0x0200a6bc
	.4byte 0xeeeeeeee
	.4byte 0x0000047f
	.4byte 0x0200a6d0
	.4byte 0x80008000
	.4byte 0x0200a6e0
	.4byte 0x0200a6be
	.2byte 0x01ff
	.2byte 0x0000
	.global Func_02001c1c
	.thumb_func
Func_02001c1c:
	push {r5, lr}
	ldr r1, [pc, #52]
	ldr r0, [pc, #52]
	bl 0x02009cbc
	ldr r5, [pc, #52]
	bl 0x02009cd4
	movs r1, #144
	strh r0, [r5]
	lsls r0, r0, #16
	lsls r1, r1, #3
	movs r2, #0
	asrs r0, r0, #16
	bl 0x02009ccc
	ldr r2, [pc, #16]
	ldr r3, [pc, #32]
	strh r2, [r3]
	ldr r3, [pc, #32]
	ldr r1, [pc, #32]
	strh r2, [r3]
	ldr r0, [pc, #32]
	bl 0x02009c94
	b .L_02001c1c_0
	.4byte 0x00000000
	.4byte 0x0200a730
	.4byte 0x02009fa0
	.4byte 0x0200a6d0
	.4byte 0x0200a6be
	.4byte 0x0200b030
	.4byte 0x00000c76
	.4byte 0x020098a5
.L_02001c1c_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global SceneState_SetHalfwordB030
	.thumb_func
SceneState_SetHalfwordB030:
	ldr r3, [pc, #4]
	strh r0, [r3]
	bx lr
	.2byte 0x0000
	.4byte 0x0200b030
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x01800000
	.4byte 0x00800000
	.4byte 0x01200000
	.4byte 0x02b00000
	.4byte 0x02600000
	.4byte 0x02d00000
	.4byte 0x02c00000
	.4byte 0x00600000
	.4byte 0x01c00000
	.4byte 0x01d00000
	.4byte 0x03400000
	.4byte 0x02500000
	.4byte 0x00800000
	.4byte 0x02f00000
	.4byte 0x01d00000
	.4byte 0x03800000
	.4byte 0x00c00000
	.4byte 0x00e00000
	.4byte 0x00c00000
	.4byte 0x01600000
	.4byte 0x377f10a0
	.4byte 0x121722bb
	.4byte 0x0ccc1172
	.4byte 0x7df07ef7
	.4byte 0x610b7ce7
	.4byte 0x50007c00
	.4byte 0x00147fff
	.4byte 0x2f1f0000
	.4byte 0x5c020100
	.4byte 0x3fc01eef
	.4byte 0x9de2abb8
	.4byte 0xd1dc7b0e
	.4byte 0x20cfafbd
	.4byte 0x4957900f
	.4byte 0xdef71d33
	.4byte 0xeb6f047a
	.4byte 0xebcc0813
	.4byte 0xe429f3ef
	.4byte 0xd2cbcc01
	.4byte 0x47c3de7b
	.4byte 0x38a3e160
	.4byte 0x1c7c1cfc
	.4byte 0x87dfcf87
	.4byte 0x9f07dfcf
	.4byte 0x7f3e0fbf
	.4byte 0x7dfcf81f
	.4byte 0x81f633e0
	.4byte 0xb881f027
	.4byte 0x041f87c0
	.4byte 0xfeb0433a
	.4byte 0xe91188ff
	.4byte 0x94499517
	.4byte 0x00a9cd3e
	.4byte 0xa6710fe9
	.4byte 0xa87fe42f
	.4byte 0x623ec04f
	.4byte 0xa923a250
	.4byte 0x755804c6
	.4byte 0x1a402388
	.4byte 0xe8cbe91d
	.4byte 0xfa4fe7b0
	.4byte 0xf281322f
	.4byte 0x452f81b2
	.4byte 0x5fffa48a
	.4byte 0x21c70ce4
	.4byte 0x02ab32ae
	.4byte 0xd8139124
	.4byte 0xff9c0981
	.4byte 0xfffc0d83
	.4byte 0x97fcbe53
	.4byte 0x0020673f
	.4byte 0xfe2534ae
	.4byte 0x435a2670
	.4byte 0x7046447c
	.4byte 0x039f8c62
	.4byte 0x330e38f8
	.4byte 0x71c61f00
	.4byte 0x3e3f9ede
	.4byte 0x38f01f18
	.4byte 0x09c38f80
	.4byte 0xe0be607c
	.4byte 0x7f01f223
	.4byte 0xfb49f0cf
	.4byte 0xcfc06c84
	.4byte 0x93e3dc08
	.4byte 0xbf9f0df3
	.4byte 0x7dfcf8cf
	.4byte 0x33efe7c6
	.4byte 0xaaca477e
	.4byte 0xad4cd315
	.4byte 0xe1c7cf1a
	.4byte 0x9c3d9e33
	.4byte 0xbb7da652
	.4byte 0x55126398
	.4byte 0x014000c5
	.4byte 0x394900b0
	.4byte 0x58ad26ae
	.4byte 0x88d26ef7
	.4byte 0xd55e23de
	.4byte 0x45eb29bb
	.4byte 0x5bfedbad
	.4byte 0x9acf33d4
	.4byte 0xef139d45
	.4byte 0x573ad45d
	.4byte 0x73ad5623
	.4byte 0xbb8eaaf7
	.4byte 0x4e6b83de
	.4byte 0xa30f8aca
	.4byte 0xef1a9aa6
	.4byte 0x1595a261
	.2byte 0xa5df
	push	{r0, r3, r6, lr}
	ldr	r2, [r6, #40]
	add	sl, r3
	ldrsb	r1, [r5, r2]
	strh	r5, [r4, #12]
	stmia	r0!, {r3, r4, r5, r6, r7}
	lsls	r0, r7, #17
	cmp	r3, r0
	stmia	r0!, {r0, r4, r6, r7}
	cmp	r2, #7
	ldr	r5, [pc, #280]
	adds	r1, #19
	ldmia	r5, {r1, r5, r6}
	lsrs	r3, r6, #28
	ldr	r6, [pc, #684]
	orrs	r2, r5
	lsrs	r1, r0, #22
	push	{r1, r3, r4, r6, lr}
	.2byte 0xf0e8
	.2byte 0xee19
	str	r2, [r0, #92]
	subs	r6, #160
	add	sp, #356
	lsls	r5, r0, #16
	ldrb	r3, [r1, #16]
	.2byte 0xff00
	.2byte 0x0000
	.global gEffectScripts
gEffectScripts:
	ldr	r6, [sp, #544]
	lsls	r0, r0, #8
	ldr	r6, [sp, #768]
	lsls	r0, r0, #8
	ldr	r6, [sp, #992]
	lsls	r0, r0, #8
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0100
	movs	r0, r0
	lsls	r4, r4, #1
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0100
	movs	r0, r0
	lsls	r4, r4, #1
	ands	r0, r0
	.2byte 0x0000
.L_02002152:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0100
	movs	r0, r0
	lsls	r4, r4, #1
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r1, r0
.L_0200218e:
	.2byte 0xffff
	.2byte 0x03d8
	movs	r0, r0
	lsls	r0, r7, #14
	strh	r0, [r0, #0]
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_0200219e:
	lsls	r0, r7, #15
	lsls	r0, r7, #15
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0190
	movs	r0, r0
	movs	r0, r3
	ands	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #15
	lsls	r0, r7, #15
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0100
	movs	r0, r0
	lsls	r4, r4, #1
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_020021e6:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0390
	movs	r0, r0
	lsls	r0, r5, #15
	.2byte 0xc000
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r7, #15
.L_02002200:
	lsls	r0, r7, #15
.L_02002202:
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x0018
	movs	r0, r0
	lsls	r0, r1, #9
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r0, r7, #15
	lsls	r0, r7, #15
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_0200222c:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0100
	movs	r0, r0
	lsls	r4, r4, #1
	ands	r0, r0
.L_02002240:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
.L_02002246:
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x05e7
	movs	r0, r0
.L_02002254:
	lsls	r0, r5, #1
	strh	r0, [r0, #0]
	movs	r0, r0
	movs	r0, r0
	movs	r0, r1
	lsls	r0, r7, #23
	lsls	r0, r7, #7
	movs	r0, r0
	movs	r2, r0
	.2byte 0xffff
	.2byte 0x02d7
	movs	r0, r0
.L_0200226c:
	lsls	r0, r5, #1
	ands	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r0, r1
	lsls	r0, r7, #23
.L_02002278:
	lsls	r0, r7, #7
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0257
	movs	r0, r0
	lsls	r0, r5, #4
	ands	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_0200228c:
	movs	r0, r1
	lsls	r0, r7, #23
	lsls	r0, r7, #7
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x00a7
	movs	r0, r0
	lsls	r0, r7, #1
	ands	r0, r0
.L_020022a0:
	movs	r0, r0
	movs	r0, r0
	movs	r0, r1
	lsls	r0, r7, #23
	lsls	r0, r7, #7
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x00a7
	movs	r0, r0
	lsls	r0, r1, #2
	.2byte 0xc000
	movs	r0, r0
	movs	r0, r0
	movs	r0, r1
.L_020022be:
	lsls	r0, r7, #23
	lsls	r0, r7, #7
	movs	r0, r0
.L_020022c4:
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020022d2:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x01a8
	movs	r0, r0
	lsls	r0, r7, #12
	strh	r0, [r0, #0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0xffff
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x01a8
	movs	r0, r0
	lsls	r0, r1, #15
	.2byte 0xc000
	movs	r0, r0
	lsls	r0, r6, #4
	lsls	r0, r6, #8
.L_02002306:
	lsls	r0, r6, #9
	lsls	r0, r0, #16
	movs	r0, r0
	movs	r2, r0
.L_0200230e:
	.2byte 0xffff
	.2byte 0x0228
	movs	r0, r0
	lsls	r0, r1, #12
	.2byte 0xc000
	movs	r0, r0
	lsls	r0, r6, #4
	lsls	r0, r6, #8
	lsls	r0, r6, #9
	lsls	r0, r0, #16
.L_02002322:
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x01b8
	movs	r0, r0
	lsls	r0, r7, #4
	.2byte 0xc000
	movs	r0, r0
	movs	r0, r4
	movs	r0, r2
	lsls	r0, r6, #7
	lsls	r0, r0, #5
	movs	r0, r0
.L_0200233c:
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0088
	movs	r0, r0
	lsls	r0, r5, #4
	.2byte 0xc000
.L_02002348:
	movs	r0, r0
	movs	r0, r4
	movs	r0, r2
	lsls	r0, r6, #7
	lsls	r0, r0, #5
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02002362:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_0200236c:
	.global RamakanSabaku_Exits
RamakanSabaku_Exits:
	lsls	r1, r3, #1
	movs	r0, r0
	movs	r0, #2
	movs	r1, r2
	asrs	r2, r3, #1
	movs	r0, r4
	lsls	r2, r3, #1
	movs	r0, r0
	movs	r0, #89
	movs	r0, r2
.L_02002380:
	asrs	r3, r3, #1
	movs	r0, r4
	lsls	r3, r3, #1
	movs	r0, r0
	movs	r0, #90
	movs	r0, r2
	movs	r0, #92
	movs	r0, r4
	asrs	r4, r3, #1
	movs	r0, r6
.L_02002394:
	adds	r0, #92
	lsls	r0, r0, #1
	lsls	r4, r3, #1
	movs	r0, r0
	movs	r0, #91
	movs	r0, r2
	adds	r0, #91
	movs	r0, r4
	str	r2, [r0, r0]
	movs	r3, r6
.L_020023a8:
	eors	r3, r3
	lsls	r0, r0, #1
	lsls	r7, r7, #7
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020023bc:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r3, r0, #3
	lsrs	r2, r1, #4
	movs	r1, r0
	movs	r0, r0
	movs	r0, r0
	movs	r0, r1
	movs	r0, r0
.L_020023d6:
	movs	r0, r0
	movs	r0, r0
	movs	r0, r1
	ands	r0, r0
	movs	r0, r0
	lsls	r1, r0, #3
.L_020023e2:
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r2, #6
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r4, r5, #5
	ands	r0, r0
.L_020023f6:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02002408:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r3, r0, #3
	lsrs	r2, r1, #4
	movs	r1, r0
	movs	r0, r0
	movs	r0, r0
	movs	r0, r1
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	movs	r0, r1
	ands	r0, r0
	movs	r0, r0
	lsls	r1, r0, #3
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r4, #5
.L_02002434:
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r4, r1, #3
	ands	r0, r0
	movs	r0, r0
	lsls	r1, r0, #3
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r4, #11
.L_0200244c:
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r4, r1, #6
	ands	r0, r0
	movs	r0, r0
	lsls	r1, r0, #3
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
.L_02002460:
	movs	r0, r0
	lsls	r0, r2, #2
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_0200246a:
	lsls	r4, r7, #5
	ands	r0, r0
	movs	r0, r0
	lsls	r1, r0, #3
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r0, #9
	movs	r0, r0
.L_0200247e:
	movs	r0, r0
	movs	r0, r0
	lsls	r4, r1, #11
	ands	r0, r0
	movs	r0, r0
	lsls	r5, r3, #1
	lsls	r7, r5, #1
	movs	r1, r0
	.2byte 0x0000
	.2byte 0x0000
.L_02002492:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
.L_020024a4:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_020024ae:
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	lsls	r3, r0, #3
	lsrs	r2, r1, #4
	movs	r1, r0
	movs	r0, r0
.L_020024c0:
	movs	r0, r0
	lsls	r0, r5, #2
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r5, #1
	ands	r0, r0
	movs	r0, r0
	lsls	r1, r0, #3
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r0, r0
	lsls	r0, r0, #2
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	lsls	r4, r7, #4
	ands	r0, r0
.L_020024e6:
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
.L_02002500:
	.global RamakanSabaku_Events
RamakanSabaku_Events:
	movs	r1, r0
	movs	r0, r0
	movs	r1, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r2, r0
.L_02002512:
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r3, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r4, r0
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r5, r0
	.2byte 0xffff
	.2byte 0x0001
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r6, r0
	.2byte 0xffff
	.2byte 0x0002
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r7, r0
	.2byte 0xffff
	.2byte 0x0003
	movs	r0, r0
	movs	r1, r0
	movs	r0, r0
	movs	r0, r1
	.2byte 0xffff
	.2byte 0x0004
	movs	r0, r0
	movs	r2, r0
	movs	r0, r0
	movs	r1, r1
	.2byte 0xffff
	.2byte 0x8499
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r4, r2
	lsls	r0, r0, #8
	ldrh	r5, [r5, #48]
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r5, r2
	.2byte 0xffff
	.2byte 0x9055
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	movs	r6, r2
	lsls	r0, r0, #8
	str	r1, [sp, #516]
	lsls	r0, r0, #8
	movs	r2, r0
.L_02002592:
	movs	r0, r0
	movs	r6, r3
	.2byte 0xffff
	.2byte 0x92fd
	lsls	r0, r0, #8
.L_0200259c:
	movs	r2, r0
	movs	r0, r0
	movs	r0, r5
	.2byte 0xffff
	.2byte 0x94b1
.L_020025a6:
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r0, r1
	lsrs	r2, r1, #4
.L_020025b0:
	ldrh	r1, [r7, #32]
	lsls	r0, r0, #8
	movs	r0, r0
	movs	r0, r0
	movs	r5, r1
.L_020025ba:
	.2byte 0xffff
	.2byte 0x8315
	lsls	r0, r0, #8
	movs	r3, r2
	movs	r0, r0
.L_020025c4:
	lsls	r4, r4, #1
	lsrs	r3, r7, #29
	movs	r4, r4
	movs	r0, r2
	movs	r3, r0
	movs	r0, r0
	lsls	r5, r4, #1
	lsls	r0, r2, #13
	movs	r0, r0
	movs	r0, r6
	movs	r3, r2
	movs	r0, r0
.L_020025dc:
	lsls	r6, r4, #1
	lsrs	r4, r7, #29
	lsls	r4, r0, #3
	movs	r0, r2
	movs	r3, r0
	movs	r0, r0
	lsls	r7, r4, #1
	lsls	r1, r2, #13
	movs	r0, r0
.L_020025ee:
	movs	r0, r6
.L_020025f0:
	movs	r3, r2
	movs	r0, r0
	lsls	r1, r5, #1
	lsrs	r5, r7, #29
	lsls	r1, r1, #12
	movs	r0, r4
	movs	r3, r2
	movs	r0, r0
	lsls	r2, r5, #1
	lsrs	r6, r7, #29
.L_02002604:
	lsls	r7, r6, #2
	movs	r0, r2
	movs	r3, r2
	movs	r0, r0
	lsls	r3, r5, #1
	lsrs	r7, r7, #29
	lsls	r3, r0, #3
	movs	r0, r2
	str	r0, [sp, #532]
	str	r0, [r0, r0]
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x888d
	lsls	r0, r0, #8
	str	r0, [sp, #532]
	ands	r0, r0
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x84e5
	lsls	r0, r0, #8
	movs	r6, r0
	movs	r0, r0
	movs	r1, r0
	lsls	r1, r0, #8
	ldrh	r5, [r5, #34]
	lsls	r0, r0, #8
	movs	r6, r0
	movs	r0, r0
.L_0200263c:
	movs	r2, r0
	lsls	r1, r0, #8
	ldrh	r5, [r5, #34]
	lsls	r0, r0, #8
	movs	r6, r0
	movs	r0, r0
	movs	r3, r0
	lsls	r1, r0, #8
	ldrh	r5, [r5, #34]
	lsls	r0, r0, #8
.L_02002650:
	movs	r2, r0
.L_02002652:
	movs	r0, r0
	lsls	r4, r4, #1
	.2byte 0xffff
	.2byte 0x9055
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	lsls	r5, r4, #1
	.2byte 0xffff
	.2byte 0x9055
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	lsls	r6, r4, #1
	.2byte 0xffff
	.2byte 0x9055
	lsls	r0, r0, #8
	movs	r2, r0
.L_02002676:
	movs	r0, r0
.L_02002678:
	lsls	r7, r4, #1
	.2byte 0xffff
	.2byte 0x9055
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	lsls	r0, r5, #1
	.2byte 0xffff
	.2byte 0x9055
.L_0200268a:
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	lsls	r1, r5, #1
	.2byte 0xffff
	.2byte 0x9055
	lsls	r0, r0, #8
	movs	r2, r0
	movs	r0, r0
	lsls	r2, r5, #1
	.2byte 0xffff
	.2byte 0x9055
	lsls	r0, r0, #8
.L_020026a4:
	movs	r2, r0
	movs	r0, r0
	lsls	r3, r5, #1
	.2byte 0xffff
	.2byte 0x9055
.L_020026ae:
	lsls	r0, r0, #8
	.2byte 0xffff
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
