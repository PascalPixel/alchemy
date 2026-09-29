.syntax unified
	.thumb
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [r0, #80]
	movs r3, #3
	ldrb r2, [r0, #9]
	ands r1, r3
	movs r3, #13
	negs r3, r3
	lsls r1, r1, #2
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
	bx lr
	.2byte 0x0000
	.global OverlayObject_PrepareObject
	.thumb_func
OverlayObject_PrepareObject:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x020086dc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000048_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x020086ec
	adds r0, r5, #0
	movs r1, #14
	bl 0x02008754
	adds r0, r5, #0
	movs r1, #1
	bl 0x020086f4
	adds r0, r5, #0
	b .L_02000048_1
.L_02000048_0:
	movs r0, #0
.L_02000048_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x020086dc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020000a0_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x020086ec
	adds r0, r5, #0
	movs r1, #15
	bl 0x02008754
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_020000a0_1
.L_020000a0_0:
	movs r0, #0
.L_020000a0_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.section .text.x02008314,"ax",%progbits
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000314_0
	ldr r0, [pc, #16]
	b .L_02000314_1
.L_02000314_0:
	ldr r0, [pc, #16]
.L_02000314_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001d
	.4byte 0x020088d8
	.4byte 0x02008818
	.global Func_02000344
	.thumb_func
Func_02000344:
	movs r0, #0
	bx lr
	.global Func_02000348
	.thumb_func
Func_02000348:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008920
	.global Func_02000350
	.thumb_func
Func_02000350:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000350_0
	ldr r0, [pc, #16]
	b .L_02000350_1
.L_02000350_0:
	ldr r0, [pc, #16]
.L_02000350_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001d
	.4byte 0x02008978
	.4byte 0x02008948
	.global Func_02000380
	.thumb_func
Func_02000380:
	push {lr}
	movs r0, #8
	sub sp, #8
	bl 0x02008724
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_02000380_0
	ldr r2, [pc, #72]
	adds r3, r3, r2
.L_02000380_0:
	asrs r3, r3, #20
	cmp r3, #24
	bne .L_02000380_1
	movs r0, #8
	bl 0x020084f4
	movs r0, #8
	bl 0x02008724
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r2, #17
	movs r3, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #74
	movs r2, #9
	movs r3, #3
	movs r0, #19
	bl 0x020086e4
	movs r0, #8
	bl 0x02008724
	movs r1, #0
	bl 0x020086ec
	ldr r0, [pc, #12]
	bl 0x02008714
.L_02000380_1:
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0x00000864
	.global Func_020003e4
	.thumb_func
Func_020003e4:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_020003e4_0
	ldr r0, [pc, #16]
	b .L_020003e4_1
.L_020003e4_0:
	ldr r0, [pc, #16]
.L_020003e4_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001d
	.4byte 0x020089f0
	.4byte 0x02008990
	.global Func_02000414
	.thumb_func
Func_02000414:
	push {lr}
	ldr r3, [pc, #152]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r3, r3, #2
	lsls r2, r2, #1
	str r3, [r1, r2]
	ldr r1, [pc, #140]
	ldrsh r2, [r1, r2]
	ldr r3, [pc, #140]
	sub sp, #8
	cmp r2, r3
	bne .L_02000414_0
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_02000414_1
	ldr r0, [pc, #124]
	bl 0x0200871c
	b .L_02000414_0
.L_02000414_1:
	movs r0, #8
	bl 0x02008724
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [pc, #104]
	bl 0x0200870c
	cmp r0, #0
	beq .L_02000414_0
	movs r1, #173
	movs r2, #146
	lsls r2, r2, #17
	lsls r1, r1, #17
	movs r0, #8
	bl 0x02008744
	movs r0, #8
	bl 0x02008724
	movs r1, #0
	bl 0x020086ec
	movs r0, #8
	bl 0x02008724
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #8
	bl 0x0200874c
	movs r3, #19
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
	movs r1, #74
	movs r2, #9
	movs r3, #3
	bl 0x020086e4
.L_02000414_0:
	movs r0, #0
	sub sp, #-8
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000001c
	.4byte 0x0000012f
	.4byte 0x00000864
	.global Func_020004c4
	.thumb_func
Func_020004c4:
	ldr r2, [r0, #80]
	ldr r1, [pc, #8]
	ldrh r3, [r2, #30]
	adds r3, r3, r1
	strh r3, [r2, #30]
	bx lr
	.4byte 0xfffff800
	.global Func_020004d4
	.thumb_func
Func_020004d4:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	adds r6, r1, #0
	movs r5, #60
.L_020004d4_1:
	cmp r5, #0
	beq .L_020004d4_0
	movs r0, #1
	bl 0x020086b4
	ldr r3, [r7, #12]
	subs r5, #1
	cmp r3, r6
	bgt .L_020004d4_1
.L_020004d4_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_020004f4
	.thumb_func
Func_020004f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	mov r11, r0
	bl 0x02008724
	adds r6, r0, #0
	adds r5, r6, #0
	movs r3, #0
	adds r5, #85
	strb r3, [r5]
	mov r8, r3
.L_020004f4_0:
	movs r0, #1
	bl 0x020086b4
	ldr r2, [r6, #80]
	ldr r1, [pc, #376]
	ldrh r3, [r2, #30]
	adds r3, r3, r1
	strh r3, [r2, #30]
	ldr r3, [r6, #80]
	ldrh r0, [r3, #30]
	bl 0x020086c4
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r6, #8]
	asrs r0, r0, #1
	subs r3, r3, r0
	str r3, [r6, #8]
	movs r2, #1
	movs r3, #128
	lsls r3, r3, #24
	add r8, r2
	str r3, [r6, #56]
	mov r3, r8
	cmp r3, #17
	bls .L_020004f4_0
	ldr r3, [pc, #336]
	movs r1, #192
	movs r2, #192
	str r3, [r6, #108]
	mov r0, r11
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200872c
	movs r1, #188
	movs r2, #144
	lsls r1, r1, #1
	mov r0, r11
	lsls r2, r2, #1
	bl 0x02008734
	ldr r3, [pc, #308]
	str r3, [r6, #72]
	movs r3, #3
	strb r3, [r5]
	adds r3, r6, #0
	adds r3, #34
	movs r2, #0
	strb r2, [r3]
	mov r0, r11
	bl 0x0200873c
	movs r1, #0
	adds r0, r6, #0
	bl 0x020084d4
	movs r0, #188
	bl 0x0200875c
	movs r0, #160
	lsls r0, r0, #11
	movs r2, #128
	adds r1, r0, #0
	lsls r2, r2, #9
	bl 0x020086fc
	movs r0, #141
	bl 0x0200875c
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #248]
	bl 0x020086fc
	movs r4, #0
	add r7, sp, #56
	mov r8, r4
	mov r10, r7
	mov r9, r4
.L_020004f4_2:
	mov r1, r8
	lsls r5, r1, #12
	adds r0, r5, #0
	bl 0x020086c4
	mov r2, r10
	mov r3, r9
	str r0, [r2]
	str r3, [r2, #4]
	adds r0, r5, #0
	bl 0x020086bc
	mov r4, r10
	ldr r2, [r4]
	str r0, [r4, #8]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_020004f4_1
	adds r3, r2, #3
.L_020004f4_1:
	lsrs r5, r0, #31
	adds r5, r0, r5
	asrs r3, r3, #2
	asrs r5, r5, #1
	subs r3, r2, r3
	subs r5, r0, r5
	str r3, [r7]
	str r5, [r7, #8]
	ldr r4, [r7, #4]
	ldr r1, [r6, #12]
	ldr r2, [r6, #16]
	ldr r0, [r6, #8]
	str r4, [sp, #0]
	mov r4, r9
	str r5, [sp, #4]
	str r4, [sp, #8]
	str r4, [sp, #12]
	bl 0x0200813c
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #16
	bls .L_020004f4_2
	movs r3, #160
	lsls r3, r3, #11
	movs r1, #173
	movs r2, #146
	str r3, [r6, #40]
	lsls r2, r2, #1
	mov r0, r11
	lsls r1, r1, #1
	bl 0x02008734
	mov r0, r11
	bl 0x0200873c
	adds r0, r6, #0
	movs r1, #0
	bl 0x020084d4
	mov r3, r9
	str r3, [r6, #108]
	ldr r2, [r6, #80]
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r2, #30]
	add r4, sp, #16
	movs r3, #214
	strh r3, [r4, #24]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r4, #8]
	ldr r3, [pc, #84]
	str r3, [r4, #12]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r4, #16]
	ldr r3, [pc, #80]
	str r3, [r4, #20]
	mov r3, r9
	ldr r2, [r6, #16]
	ldr r1, [r6, #12]
	ldr r0, [r6, #8]
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #224
	lsls r3, r3, #13
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #12]
	bl 0x0200813c
	movs r0, #154
	bl 0x0200875c
	mov r0, r11
	movs r1, #3
	bl 0x0200874c
	bl 0x02008704
	sub sp, #-68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffffff00
	.4byte 0x020084c5
	.4byte 0x0000cccc
	.4byte 0x0000e666
	.4byte 0x00013333
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
	.global gEffectScripts
gEffectScripts:
	.4byte 0x02008764
	.4byte 0x0200879c
	.4byte 0x020087d4
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000178
	.4byte 0xc0000308
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0x40000228
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0003
	.4byte 0x00000258
	.4byte 0x40000208
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0004
	.4byte 0x00000178
	.4byte 0x400000a8
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0005
	.4byte 0x00000266
	.4byte 0xc0000350
	.4byte 0x01e00000
	.4byte 0x02d0025d
	.4byte 0x00000384
	.4byte 0xffff0063
	.4byte 0x000001a8
	.4byte 0x80000118
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x0000009a
	.4byte 0xc000015e
	.4byte 0x00100000
	.4byte 0x02780030
	.4byte 0x00000168
	.4byte 0xffff0002
	.4byte 0x00000228
	.4byte 0xc0000138
	.4byte 0x00100000
	.4byte 0x02780030
	.4byte 0x00000168
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00102019
	.4byte 0x0020101b
	.4byte 0x0030201b
	.4byte 0x0040101d
	.4byte 0x0050b005
	.4byte 0x0000001d
	.4byte 0x0010401c
	.4byte 0x00228002
	.4byte 0x000001ff
	.4byte 0xffff00d4
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000004
	.4byte 0x00000013
	.4byte 0x0f280064
	.4byte 0x001000bf
	.4byte 0x00008c15
	.4byte 0x08640008
	.4byte 0x02008381
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
