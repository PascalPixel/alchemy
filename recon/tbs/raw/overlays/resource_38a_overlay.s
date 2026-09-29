.syntax unified
	.thumb
	.section .text.x02008314,"ax",%progbits
	.balign 4
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
	.section .text.x02008350,"ax",%progbits
	.balign 4
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
	.section .text.x020083e4,"ax",%progbits
	.balign 4
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
	.global GomaSuiro_Exits
GomaSuiro_Exits:
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
