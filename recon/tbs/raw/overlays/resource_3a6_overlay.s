.syntax unified
	.thumb
	.section .text.x02008d20,"ax",%progbits
	.balign 4
	.global Func_02000d20
	.thumb_func
Func_02000d20:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000d20_0
	ldr r0, [pc, #36]
	b .L_02000d20_1
.L_02000d20_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000d20_2
	ldr r0, [pc, #36]
	b .L_02000d20_1
.L_02000d20_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000d20_3
	ldr r0, [pc, #32]
	b .L_02000d20_1
.L_02000d20_3:
	ldr r0, [pc, #32]
.L_02000d20_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000005d
	.4byte 0x02009f9c
	.4byte 0x0000005e
	.4byte 0x0200a014
	.4byte 0x0000005f
	.4byte 0x0200a134
	.4byte 0x02009f6c
	.section .text.x02008d80,"ax",%progbits
	.balign 4
	.global Func_02000d80
	.thumb_func
Func_02000d80:
	push {lr}
	ldr r3, [pc, #44]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #36]
	cmp r2, r3
	beq 0x02008da8
	ldr r3, [pc, #32]
	cmp r2, r3
	bne .L_02000d80_0
	ldr r0, [pc, #32]
	b 0x02008daa
.L_02000d80_0:
	ldr r3, [pc, #32]
	cmp r2, r3
	bne 0x02008da8
	ldr r0, [pc, #28]
.L_02000da6:
	b .L_02000da6_0
	.2byte 0x4807
.L_02000da6_0:
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x005d
	.2byte 0x0000
	.2byte 0x005e
	.2byte 0x0000
	.2byte 0xa2c4
	.2byte 0x0200
	.2byte 0x005f
	.2byte 0x0000
	.2byte 0xa39c
	.2byte 0x0200
	.2byte 0xa234
	.2byte 0x0200
	.section .text.x0200969c,"ax",%progbits
	.balign 4
	.global Func_0200169c
	.thumb_func
Func_0200169c:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_0200169c_0
	ldr r0, [pc, #36]
	b .L_0200169c_1
.L_0200169c_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200169c_2
	ldr r0, [pc, #36]
	b .L_0200169c_1
.L_0200169c_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200169c_3
	ldr r0, [pc, #32]
	b .L_0200169c_1
.L_0200169c_3:
	ldr r0, [pc, #32]
.L_0200169c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000005d
	.4byte 0x0200a420
	.4byte 0x0000005e
	.4byte 0x0200a450
	.4byte 0x0000005f
	.4byte 0x0200a624
	.4byte 0x0200a414
	.global Func_020016f0
	.thumb_func
Func_020016f0:
	push {lr}
	ldr r3, [pc, #68]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r1, r3, r2
	movs r3, #129
	lsls r3, r3, #2
	str r3, [r1]
	ldr r3, [pc, #56]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_020016f0_0
	movs r3, #128
	lsls r3, r3, #1
	str r3, [r1]
	movs r0, #1
	bl 0x02009c1c
	movs r0, #11
	movs r1, #3
	bl 0x02009d1c
	movs r0, #12
	movs r1, #3
	bl 0x02009d1c
	ldr r0, [pc, #24]
	bl 0x02009c9c
.L_020016f0_0:
	bl 0x02009984
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000005d
	.4byte 0x0000012f
	.section .text.x02009984,"ax",%progbits
	.balign 4
	.global Func_02001984
	.thumb_func
Func_02001984:
	push {r5, r6, lr}
	ldr r2, [pc, #584]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r1, [r3, r0]
	ldr r3, [pc, #576]
	sub sp, #8
	cmp r1, r3
	beq .L_02001984_0
	b .L_02001984_1
.L_02001984_0:
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #9
	bls .L_02001984_2
	b .L_02001984_3
.L_02001984_2:
	ldr r2, [pc, #552]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	ldr r1, [sp, #896]
	lsls r0, r0, #8
	ldr r1, [sp, #896]
	lsls r0, r0, #8
	ldr r1, [sp, #896]
	lsls r0, r0, #8
	ldr r1, [sp, #896]
	lsls r0, r0, #8
	ldr r2, [sp, #8]
	lsls r0, r0, #8
	ldr r2, [sp, #8]
	lsls r0, r0, #8
	ldr r2, [sp, #8]
	lsls r0, r0, #8
	ldr r2, [sp, #400]
	lsls r0, r0, #8
	ldr r2, [sp, #400]
	lsls r0, r0, #8
	ldr r2, [sp, #400]
	lsls r0, r0, #8
	movs r0, #15
	movs r1, #3
	bl 0x02009d1c
	movs r0, #13
	movs r1, #3
	bl 0x02009d1c
	movs r0, #240
	movs r2, #232
	lsls r0, r0, #15
	lsls r2, r2, #16
	movs r1, #0
	movs r3, #223
	bl 0x020080a0
	b .L_02001984_3
	.2byte 0x2070
	.2byte 0xf000
	.2byte 0xf942
	.2byte 0x2800
	.2byte 0xd000
	.2byte 0xe0db
	.2byte 0x4873
	.2byte 0xf000
	.2byte 0xf93c
	.2byte 0x2800
	.2byte 0xd100
	.2byte 0xe0d5
	.2byte 0x2080
	.2byte 0x0080
	.2byte 0xf000
	.2byte 0xf939
	.2byte 0x4b6b
	.2byte 0x20e1
	.2byte 0x0040
	.2byte 0x181b
	.2byte 0x2100
	.2byte 0x5e5b
	.2byte 0x2b05
	.2byte 0xd102
	.2byte 0x303f
	.2byte 0xf000
	.2byte 0xf92e
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xf8ef
	.2byte 0x4868
	.2byte 0xf000
	.2byte 0xf924
	.2byte 0x2800
	.2byte 0xd000
	.2byte 0xe0bd
	.2byte 0x21c6
	.2byte 0x228c
	.2byte 0x2008
	.2byte 0x0489
	.2byte 0x0452
	.2byte 0xf000
	.2byte 0xf94e
	.2byte 0x2008
	.2byte 0xf000
	.2byte 0xf92f
	.2byte 0x4b61
	.2byte 0x66c3
	.2byte 0xe0b0
	.2byte 0x228a
	.2byte 0x0492
	.2byte 0x2100
	.2byte 0x2314
	.2byte 0x485e
	.2byte 0xf7fe
	.2byte 0xfb17
	.2byte 0x2300
	.2byte 0x2222
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x2017
	.2byte 0x2122
	.2byte 0x220d
	.2byte 0x2303
	.2byte 0xf000
	.2byte 0xf8f3
	.2byte 0xf7ff
	.2byte 0xfe73
	.2byte 0x2080
	.2byte 0x0080
	.2byte 0xf000
	.2byte 0xf8fd
	.2byte 0x2800
	.2byte 0xd009
	.2byte 0x2317
	.2byte 0x2227
	.2byte 0x9300
	.2byte 0x9201
.L_02001984_4:
	.2byte 0x2017
	.2byte 0x2129
	.2byte 0x2201
	.2byte 0x2301
	.2byte 0xf000
	.2byte 0xf8e1
	.2byte 0x4850
	.2byte 0xf000
	.2byte 0xf8ee
	.2byte 0x2800
	.2byte 0xd100
	.2byte 0xe087
	.2byte 0x231b
	.2byte 0x2229
	.2byte 0x9300
	.2byte 0x9201
	.2byte 0x201f
	.2byte 0x2127
	.2byte 0x2202
	.2byte 0x2301
	.2byte 0xf000
	.2byte 0xf8d1
	.2byte 0xe07c
.L_02001984_1:
	ldr r3, [pc, #288]
	cmp r1, r3
	bne .L_02001984_3
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #3
	bgt .L_02001984_3
	cmp r3, #1
	blt .L_02001984_3
	adds r0, #64
	bl 0x02009c8c
	cmp r0, #0
	beq .L_02001984_5
	movs r6, #4
	movs r5, #0
	movs r1, #12
	movs r2, #16
	movs r3, #1
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008528
	movs r0, #0
	movs r1, #13
	movs r2, #16
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008528
	b .L_02001984_6
.L_02001984_5:
	movs r0, #9
	bl 0x02008ba4
.L_02001984_6:
	ldr r0, [pc, #216]
	bl 0x02009c8c
	cmp r0, #0
	beq .L_02001984_7
	movs r6, #4
	movs r5, #0
	movs r1, #16
	movs r2, #16
	movs r3, #1
	movs r0, #2
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008528
	movs r0, #0
	movs r1, #16
	movs r2, #16
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008528
	b .L_02001984_8
.L_02001984_7:
	movs r0, #10
	bl 0x02008ba4
.L_02001984_8:
	ldr r0, [pc, #164]
	bl 0x02009c8c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02001984_9
	movs r3, #2
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #13
	movs r2, #19
	movs r3, #4
	bl 0x02008528
	b .L_02001984_3
.L_02001984_9:
	movs r0, #129
	lsls r0, r0, #2
	bl 0x02009c8c
	cmp r0, #0
	beq .L_02001984_10
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #13
	movs r2, #15
	movs r3, #4
	movs r0, #0
	str r5, [sp, #4]
	bl 0x02008528
	movs r3, #16
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #17
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009c6c
	movs r3, #15
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #13
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009c6c
	b .L_02001984_3
.L_02001984_10:
	movs r0, #11
	bl 0x02008ba4
	movs r0, #11
	movs r1, #3
	bl 0x02009d1c
.L_02001984_3:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000005e
	.4byte 0x020099b8
	.2byte 0x0302
	.2byte 0x0000
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x8cf9
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0x0282
	.2byte 0x0201
	.2byte 0x0000
	.4byte 0x0000005f
	.4byte 0x00000203
	.4byte 0x00000205
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
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
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
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.global HaidiaDou_WalkTargets
HaidiaDou_WalkTargets:
	.4byte 0x000002f8
	.4byte 0x00000128
	.4byte 0x000002f8
	.4byte 0x00000118
	.4byte 0x00000308
	.4byte 0x00000118
	.4byte 0x00000318
	.4byte 0x00000118
	.4byte 0x00000328
	.4byte 0x00000118
	.4byte 0x00000338
	.4byte 0x00000118
	.4byte 0x00000338
	.4byte 0x00000128
	.4byte 0x00000338
	.4byte 0x00000138
	.4byte 0x00000328
	.4byte 0x00000138
	.4byte 0x00000318
	.4byte 0x00000138
	.4byte 0x00000308
	.4byte 0x00000138
	.4byte 0x000002f8
	.4byte 0x00000138
	.global gEffectScripts
gEffectScripts:
	.4byte 0x02009da0
	.4byte 0x02009dd8
	.4byte 0x02009e10
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
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0x00100000
	.4byte 0x01500000
	.4byte 0x00000170
	.4byte 0xffff0001
	.4byte 0x00000068
	.4byte 0xc0000158
	.4byte 0x00100000
	.4byte 0x01500000
	.4byte 0x00000170
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x400000b8
	.4byte 0x00100000
	.4byte 0x01500000
	.4byte 0x00000170
	.4byte 0xffff0003
	.4byte 0x00000088
	.4byte 0xc0000178
	.4byte 0x00100000
	.4byte 0x01500000
	.4byte 0x00000170
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000148
	.4byte 0xc0000188
	.4byte 0x00300000
	.4byte 0x02200020
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0x400000a0
	.4byte 0x00300000
	.4byte 0x02200020
	.4byte 0x000001a0
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0x40000078
	.4byte 0x00300000
	.4byte 0x02200020
	.4byte 0x000001a0
	.4byte 0xffff0004
	.4byte 0x000001f8
	.4byte 0x40000088
	.4byte 0x00300000
	.4byte 0x02200020
	.4byte 0x000001a0
	.4byte 0xffff0005
	.4byte 0x00000318
	.4byte 0xc0000168
	.4byte 0x02700000
	.4byte 0x03b00010
	.4byte 0x00000180
	.4byte 0xffff0006
	.4byte 0x000002a8
	.4byte 0x400000f8
	.4byte 0x02700000
	.4byte 0x03b00010
	.4byte 0x00000180
	.4byte 0xffff0007
	.4byte 0x00000388
	.4byte 0x400000e8
	.4byte 0x02700000
	.4byte 0x03b00010
	.4byte 0x00000180
	.4byte 0xffff0008
	.4byte 0x00000198
	.4byte 0xc0000368
	.4byte 0x01380000
	.4byte 0x02c001d8
	.4byte 0x00000380
	.4byte 0xffff0009
	.4byte 0x000001e8
	.4byte 0xc00002e8
	.4byte 0x01380000
	.4byte 0x02c001d8
	.4byte 0x00000380
	.4byte 0xffff000a
	.4byte 0x00000278
	.4byte 0x40000228
	.4byte 0x01380000
	.4byte 0x02c001d8
	.4byte 0x00000380
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000c8
	.4byte 0x400001b0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000c8
	.4byte 0xc00001b0
	.4byte 0x00400000
	.4byte 0x01a00018
	.4byte 0x000001d0
	.4byte 0xffff0002
	.4byte 0x00000158
	.4byte 0x40000078
	.4byte 0x00400000
	.4byte 0x01a00018
	.4byte 0x000001d0
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0xc0000108
	.4byte 0x00400000
	.4byte 0x01a00018
	.4byte 0x000001d0
	.4byte 0xffff0004
	.4byte 0x00000218
	.4byte 0xc0000308
	.4byte 0x01900000
	.4byte 0x038001b8
	.4byte 0x00000320
	.4byte 0xffff0005
	.4byte 0x00000348
	.4byte 0xc0000308
	.4byte 0x01900000
	.4byte 0x038001b8
	.4byte 0x00000320
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global HaidiaDou_Exits
HaidiaDou_Exits:
	.4byte 0x0000005d
	.4byte 0x00104006
	.4byte 0x0020105e
	.4byte 0x00306006
	.4byte 0x0000005e
	.4byte 0x0010205d
	.4byte 0x0020505e
	.4byte 0x0030805e
	.4byte 0x0040105f
	.4byte 0x0050205e
	.4byte 0x0060905e
	.4byte 0x0070305f
	.4byte 0x0080305e
	.4byte 0x0090605e
	.4byte 0x00a0405f
	.4byte 0x0000005f
	.4byte 0x0010405e
	.4byte 0x0020505f
	.4byte 0x0030705e
	.4byte 0x0040a05e
	.4byte 0x0050205f
	.4byte 0x000001ff
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0070005d
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x01a60000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x01024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0070005d
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x02009749
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02009749
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02009749
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
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
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
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
	.4byte 0x00000602
	.4byte 0xffff0014
	.4byte 0x020098b5
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte 0x020098f5
	.4byte 0x00004602
	.4byte 0xffff0015
	.4byte 0x020090e1
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x02009bfd
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x02009bfd
	.4byte 0x00000202
	.4byte 0x02010018
	.4byte 0x020090c9
	.4byte 0x00000002
	.4byte 0x00700028
	.4byte 0x020091a1
	.4byte 0x00000002
	.4byte 0x00700029
	.4byte 0x02009259
	.4byte 0x00000002
	.4byte 0x0070002d
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x0070002e
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x0070002f
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700030
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700031
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700032
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700033
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700034
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700035
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700036
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700037
	.4byte 0x02009301
	.4byte 0x00000002
	.4byte 0x00700038
	.4byte 0x02009301
	.4byte 0x00000007
	.4byte 0xffff0008
	.4byte 0x02008cc1
	.4byte 0x00000013
	.4byte 0x0fc20064
	.4byte 0x001000cc
	.4byte 0x00000013
	.4byte 0x0fc30065
	.4byte 0x001000b5
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008de9
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008dcd
	.4byte 0x00008c15
	.4byte 0x02000009
	.4byte 0x02008e05
	.4byte 0x00008c15
	.4byte 0x0201000a
	.4byte 0x02008f79
	.4byte 0x00009115
	.4byte 0xffff0008
	.4byte 0x02009379
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
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
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02009391
	.4byte 0x00000602
	.4byte 0xffff0015
	.4byte 0x020094ad
	.4byte 0x00000202
	.4byte 0xffff0016
	.4byte 0x020095cd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
