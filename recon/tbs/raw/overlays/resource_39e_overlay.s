.syntax unified
	.thumb
	.section .text.x0200834c,"ax",%progbits
	.balign 4
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_0200034c_0
	ldr r0, [pc, #16]
	b .L_0200034c_1
.L_0200034c_0:
	ldr r0, [pc, #16]
.L_0200034c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000003c
	.4byte 0x0200c7a8
	.4byte 0x0200c838
	.section .text.x02008388,"ax",%progbits
	.balign 4
	.global Func_02000388
	.thumb_func
Func_02000388:
	push {r5, lr}
	ldr r1, [pc, #112]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #104]
	cmp r2, r3
	bne .L_02000388_0
	ldr r0, [pc, #100]
	b .L_02000388_1
.L_02000388_0:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #3
	bne .L_02000388_2
	ldr r0, [pc, #88]
	b .L_02000388_1
.L_02000388_2:
	ldr r0, [pc, #88]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02000388_3
	ldr r0, [pc, #80]
	ldr r1, [pc, #76]
	adds r3, r0, #0
	adds r3, #122
	strh r1, [r3]
	adds r3, #48
	strh r1, [r3]
	adds r2, r0, #0
	movs r3, #144
	adds r2, #200
	lsls r3, r3, #17
	str r3, [r2]
	movs r3, #248
	adds r2, #8
	lsls r3, r3, #16
	str r3, [r2]
	movs r2, #133
	lsls r2, r2, #1
	adds r3, r0, r2
	adds r2, #24
	strh r1, [r3]
	adds r3, r0, r2
	strh r1, [r3]
.L_02000388_3:
	ldr r5, [pc, #36]
	adds r0, r5, #0
	bl 0x0200c3ec
	adds r0, r5, #0
.L_02000388_1:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000003c
	.4byte 0x0200c8f0
	.4byte 0x0200cae8
	.4byte 0x00000895
	.4byte 0x0200c998
	.section .text.x0200a574,"ax",%progbits
	.balign 4
	.global Func_02002574
	.thumb_func
Func_02002574:
	push {lr}
	ldr r1, [pc, #44]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02002574_0
	ldr r0, [pc, #32]
	b .L_02002574_1
.L_02002574_0:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #3
	bne .L_02002574_2
	ldr r0, [pc, #20]
	b .L_02002574_1
.L_02002574_2:
	ldr r0, [pc, #20]
.L_02002574_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000003c
	.4byte 0x0200cb90
	.4byte 0x0200d184
	.4byte 0x0200cd40
	.section .text.x0200be58,"ax",%progbits
	.balign 4
	.global Func_02003e58
	.thumb_func
Func_02003e58:
	push {r5, lr}
	ldr r5, [pc, #576]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #568]
	sub sp, #8
	cmp r2, r3
	beq .L_02003e58_0
	b .L_02003e58_1
.L_02003e58_0:
	ldr r3, [pc, #560]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #225
	adds r2, #73
	str r2, [r3]
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_02003e58_2
	ldr r0, [pc, #536]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_3
	movs r0, #8
	movs r1, #6
	bl 0x0200c454
	b .L_02003e58_4
.L_02003e58_3:
	movs r0, #8
	movs r1, #5
	bl 0x0200c454
	ldr r0, [pc, #512]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02003e58_5
	b .L_02003e58_4
.L_02003e58_5:
	ldr r0, [pc, #504]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_6
	b .L_02003e58_4
.L_02003e58_6:
	ldr r0, [pc, #496]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_7
	b .L_02003e58_4
.L_02003e58_7:
	bl 0x0200a7ec
	b .L_02003e58_4
.L_02003e58_2:
	cmp r3, #2
	beq .L_02003e58_8
	cmp r3, #4
	bne .L_02003e58_9
.L_02003e58_8:
	ldr r0, [pc, #476]
	bl 0x0200c3cc
	ldr r0, [pc, #472]
	bl 0x0200c3bc
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02003e58_10
	movs r0, #19
	bl 0x0200c404
	adds r3, r0, #0
	adds r3, #85
	strb r5, [r3]
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #12]
	str r3, [r0, #60]
	ldr r3, [pc, #444]
	movs r2, #128
	str r3, [r0, #24]
	ldr r3, [r0, #80]
	lsls r2, r2, #8
	str r2, [r0, #28]
	strh r2, [r3, #30]
	ldr r0, [pc, #436]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_11
	movs r1, #248
	movs r2, #208
	movs r0, #18
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200c44c
	ldr r0, [pc, #416]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02003e58_11
	movs r1, #128
	movs r2, #240
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #16
	bl 0x0200c44c
	movs r0, #18
	bl 0x0200c404
	ldr r5, [pc, #388]
	str r5, [r0, #108]
	movs r0, #13
	bl 0x0200c404
	str r5, [r0, #108]
	movs r0, #14
	bl 0x0200c404
	str r5, [r0, #108]
	movs r0, #15
	bl 0x0200c404
	str r5, [r0, #108]
	movs r0, #16
	bl 0x0200c404
	str r5, [r0, #108]
	b .L_02003e58_11
.L_02003e58_10:
	movs r0, #19
	bl 0x0200c404
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #12]
	str r3, [r0, #60]
	movs r1, #128
	ldr r3, [pc, #316]
	lsls r1, r1, #8
	str r3, [r0, #24]
	str r1, [r0, #28]
	movs r3, #89
	adds r3, r3, r0
	ldrb r2, [r3]
	mov r12, r3
	movs r3, #8
	orrs r3, r2
	mov r2, r12
	strb r3, [r2]
	ldr r3, [r0, #80]
	movs r2, #10
	strh r1, [r3, #30]
	movs r3, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl 0x0200c39c
.L_02003e58_11:
	movs r0, #152
	movs r1, #192
	movs r2, #224
	lsls r1, r1, #13
	lsls r2, r2, #16
	movs r3, #223
	lsls r0, r0, #17
	bl 0x02008048
	movs r0, #10
	movs r1, #5
	bl 0x0200c454
	movs r0, #11
	movs r1, #5
	bl 0x0200c454
	b .L_02003e58_4
.L_02003e58_9:
	cmp r3, #3
	bne .L_02003e58_4
	ldr r0, [pc, #220]
	bl 0x0200c3cc
	ldr r0, [pc, #216]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02003e58_13
	bl 0x0200aad0
	b .L_02003e58_4
.L_02003e58_13:
	ldr r0, [pc, #220]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02003e58_4
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200c44c
	movs r0, #9
	movs r1, #0
	movs r2, #0
.L_02003e58_12:
	bl 0x0200c44c
	b .L_02003e58_4
.L_02003e58_1:
	movs r0, #170
	bl 0x0200c564
	movs r0, #9
	bl 0x0200c404
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	movs r1, #225
	orrs r3, r2
	lsls r1, r1, #1
	strb r3, [r0]
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_02003e58_14
	ldr r0, [pc, #116]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_14
	ldr r0, [pc, #148]
	bl 0x0200c3bc
	cmp r0, #0
	bne .L_02003e58_14
	movs r3, #10
	movs r2, #24
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #84
	movs r2, #1
	movs r3, #1
	bl 0x0200c39c
.L_02003e58_14:
	ldr r0, [pc, #120]
	bl 0x0200c3bc
	cmp r0, #0
	beq .L_02003e58_4
	movs r1, #152
	movs r2, #196
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x0200c44c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c4c4
	movs r3, #10
	movs r2, #22
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #26
	movs r2, #1
	movs r3, #1
	bl 0x0200c39c
.L_02003e58_4:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000003d
	.4byte 0x03001ebc
	.4byte 0x0000088f
	.4byte 0x00000f14
	.4byte 0x00000893
	.4byte 0x00000109
	.4byte 0x0000012f
	.4byte 0x00000895
	.4byte 0x0000cccc
	.4byte 0x0000089a
	.4byte 0x0000089b
	.4byte 0x02008325
	.4byte 0x000008b2
	.4byte 0x00000894
	.4byte 0x00000892
	.section .rodata,"a",%progbits
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
	.4byte 0x0200c584
	.4byte 0x0200c5bc
	.4byte 0x0200c5f4
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000015
	.4byte 0x00009999
	.4byte 0x80010000
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00030000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008315
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x002e0042
	.4byte 0x00020003
	.4byte 0x003f0005
	.4byte 0x0003002e
	.4byte 0x00050002
	.4byte 0x003effff
	.4byte 0x00020013
	.4byte 0x00050002
	.4byte 0x00130040
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x0013003e
	.4byte 0x00020002
	.4byte 0x003c0005
	.4byte 0x00020013
	.4byte 0x00050002
	.4byte 0x0000ffff
	.4byte 0xffff0000
	.4byte 0x00000128
	.4byte 0xc00001e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000128
	.4byte 0xc00001f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000128
	.4byte 0x40000130
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000000a8
	.4byte 0x40000168
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000000e8
	.4byte 0x40000148
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
	.4byte 0x000000a8
	.4byte 0xc0000228
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000a8
	.4byte 0xc0000228
	.4byte 0x00300000
	.4byte 0x01200180
	.4byte 0x00000240
	.4byte 0xffff0002
	.4byte 0x00000100
	.4byte 0xc0000108
	.4byte 0x00300000
	.4byte 0x01700050
	.4byte 0x00000120
	.4byte 0xffff0003
	.4byte 0x000001d8
	.4byte 0xc0000228
	.4byte 0x01600000
	.4byte 0x02500180
	.4byte 0x00000240
	.4byte 0xffff0004
	.4byte 0x00000130
	.4byte 0x400000f8
	.4byte 0x00300000
	.4byte 0x01700050
	.4byte 0x00000120
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ShianJiin_XianMessages
ShianJiin_XianMessages:
	.4byte 0x0000003c
	.4byte 0x0010c002
	.4byte 0x0020103d
	.4byte 0x0030103e
	.4byte 0x0000003d
	.4byte 0x0010203c
	.4byte 0x0020a048
	.4byte 0x00302058
	.4byte 0x00408049
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0xffff0095
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0097
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0xffff0097
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00008000
	.4byte 0xffff0097
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00004000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000033
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x0000c000
	.4byte 0x00000082
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00018000
	.4byte 0x00000083
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00018000
	.4byte 0x00000083
	.4byte 0x0200c638
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x00000083
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x0001c000
	.4byte 0x00000082
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00012000
	.4byte 0x00000082
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00004000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00008000
	.4byte 0xffff0016
	.4byte 0x0200c750
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff003d
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001c000
	.4byte 0xffff0097
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x0200a779
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x0200a6d9
	.4byte 0x00000002
	.4byte 0x08940005
	.4byte 0x02008659
	.4byte 0x00000000
	.4byte 0x188f0008
	.4byte 0x000017d2
	.4byte 0x00000000
	.4byte 0x188f0009
	.4byte 0x000017d3
	.4byte 0x00000000
	.4byte 0x188f000a
	.4byte 0x000017d4
	.4byte 0x00000000
	.4byte 0x188f000b
	.4byte 0x000017d5
	.4byte 0x00000000
	.4byte 0x188f000c
	.4byte 0x02008485
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008415
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008415
	.4byte 0x00000000
	.4byte 0x08910009
	.4byte 0x00001791
	.4byte 0x00000000
	.4byte 0x08920009
	.4byte 0x02008519
	.4byte 0x00000000
	.4byte 0x0f140009
	.4byte 0x000017b5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000017bc
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001792
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001793
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008485
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200a509
	.4byte 0x00000003
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x00008d15
	.4byte 0x188f0008
	.4byte 0x000017d9
	.4byte 0x00008d15
	.4byte 0x188f0009
	.4byte 0x000017da
	.4byte 0x00008d15
	.4byte 0x188f000a
	.4byte 0x000017db
	.4byte 0x00008d15
	.4byte 0x188f000b
	.4byte 0x000017dc
	.4byte 0x00008d15
	.4byte 0x188f000c
	.4byte 0x000017dd
	.4byte 0x00008d15
	.4byte 0x08900008
	.4byte 0x0000178d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001799
	.4byte 0x00008d15
	.4byte 0x08910009
	.4byte 0x0000179a
	.4byte 0x00008d15
	.4byte 0x08920009
	.4byte 0x000017b6
	.4byte 0x00008d15
	.4byte 0x0f140009
	.4byte 0x000017b6
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000017bd
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000179b
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000179c
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000179d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a1d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004402
	.4byte 0xffff0001
	.4byte 0x0200a765
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x0200a485
	.4byte 0x00000000
	.4byte 0x188f0008
	.4byte 0x000017de
	.4byte 0x00000000
	.4byte 0x18930008
	.4byte 0x000017ce
	.4byte 0x00000000
	.4byte 0x18910008
	.4byte 0x000017b0
	.4byte 0x00000000
	.4byte 0x13000008
	.4byte 0x0200871d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000179e
	.4byte 0x00008d15
	.4byte 0x188f0008
	.4byte 0x0200a54d
	.4byte 0x00008d15
	.4byte 0x18930008
	.4byte 0x000017cf
	.4byte 0x00008d15
	.4byte 0x18910408
	.4byte 0x02008abd
	.4byte 0x00008d15
	.4byte 0xffff0408
	.4byte 0x0200871d
	.4byte 0x00000000
	.4byte 0x18950009
	.4byte 0x00001a56
	.4byte 0x00000000
	.4byte 0x1895000a
	.4byte 0x00001a57
	.4byte 0x00000000
	.4byte 0x1895000b
	.4byte 0x0200a465
	.4byte 0x00000000
	.4byte 0x1895000c
	.4byte 0x02008afd
	.4byte 0x00000000
	.4byte 0x1895000e
	.4byte 0x00001a5c
	.4byte 0x00000000
	.4byte 0x18950010
	.4byte 0x00001a5d
	.4byte 0x00000000
	.4byte 0x189b0012
	.4byte 0x0000189a
	.4byte 0x00000000
	.4byte 0x189b0009
	.4byte 0x0000189b
	.4byte 0x00000000
	.4byte 0x189b000a
	.4byte 0x0000189c
	.4byte 0x00000000
	.4byte 0x189b000b
	.4byte 0x0000189d
	.4byte 0x00000000
	.4byte 0x189b000c
	.4byte 0x02008afd
	.4byte 0x00000000
	.4byte 0x189b000d
	.4byte 0x0000189f
	.4byte 0x00000000
	.4byte 0x189b000e
	.4byte 0x000018a0
	.4byte 0x00000000
	.4byte 0x189b000f
	.4byte 0x000018a1
	.4byte 0x00000000
	.4byte 0x189b0010
	.4byte 0x000018a2
	.4byte 0x00000000
	.4byte 0x13010012
	.4byte 0x0000187f
	.4byte 0x00000000
	.4byte 0x13010010
	.4byte 0x00001880
	.4byte 0x00000000
	.4byte 0x18980012
	.4byte 0x00001864
	.4byte 0x00000000
	.4byte 0x1898000d
	.4byte 0x00001865
	.4byte 0x00000000
	.4byte 0x1898000e
	.4byte 0x00001866
	.4byte 0x00000000
	.4byte 0x1898000f
	.4byte 0x00001867
	.4byte 0x00000000
	.4byte 0x18980010
	.4byte 0x00001868
	.4byte 0x00000000
	.4byte 0x18990012
	.4byte 0x02009d51
	.4byte 0x00000000
	.4byte 0x1899000d
	.4byte 0x00001871
	.4byte 0x00000000
	.4byte 0x1899000e
	.4byte 0x00001872
	.4byte 0x00000000
	.4byte 0x1899000f
	.4byte 0x00001873
	.4byte 0x00000000
	.4byte 0x18990010
	.4byte 0x00001874
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008add
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001828
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001829
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008afd
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000182b
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000182c
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008bd5
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001830
	.4byte 0x00008d15
	.4byte 0x18950009
	.4byte 0x00001a5e
	.4byte 0x00008d15
	.4byte 0x1895000a
	.4byte 0x00001a5f
	.4byte 0x00008d15
	.4byte 0x1895000b
	.4byte 0x00001a60
	.4byte 0x00008d15
	.4byte 0x1895000c
	.4byte 0x00001a61
	.4byte 0x00008d15
	.4byte 0x1895000e
	.4byte 0x00001a62
	.4byte 0x00008d15
	.4byte 0x18950010
	.4byte 0x00001a63
	.4byte 0x00008d15
	.4byte 0x189b0012
	.4byte 0x000018ab
	.4byte 0x00008d15
	.4byte 0x189b0009
	.4byte 0x000018a3
	.4byte 0x00008d15
	.4byte 0x189b000a
	.4byte 0x000018a4
	.4byte 0x00008d15
	.4byte 0x189b000b
	.4byte 0x000018a5
	.4byte 0x00008d15
	.4byte 0x189b000c
	.4byte 0x000018a6
	.4byte 0x00008d15
	.4byte 0x189b000d
	.4byte 0x000018a7
	.4byte 0x00008d15
	.4byte 0x189b000e
	.4byte 0x000018a8
	.4byte 0x00008d15
	.4byte 0x189b000f
	.4byte 0x000018a9
	.4byte 0x00008d15
	.4byte 0x189b0010
	.4byte 0x000018aa
	.4byte 0x00008d15
	.4byte 0x13010012
	.4byte 0x00001881
	.4byte 0x00008d15
	.4byte 0x13010010
	.4byte 0x00001882
	.4byte 0x00008d15
	.4byte 0x18980012
	.4byte 0x00001869
	.4byte 0x00008d15
	.4byte 0x1898000d
	.4byte 0x0000186a
	.4byte 0x00008d15
	.4byte 0x1898000e
	.4byte 0x0000186b
	.4byte 0x00008d15
	.4byte 0x1898000f
	.4byte 0x0000186c
	.4byte 0x00008d15
	.4byte 0x18980010
	.4byte 0x0000186d
	.4byte 0x00008d15
	.4byte 0x18990012
	.4byte 0x00001875
	.4byte 0x00008d15
	.4byte 0x1899000d
	.4byte 0x00001876
	.4byte 0x00008d15
	.4byte 0x1899000e
	.4byte 0x00001877
	.4byte 0x00008d15
	.4byte 0x1899000f
	.4byte 0x00001878
	.4byte 0x00008d15
	.4byte 0x18990010
	.4byte 0x00001879
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001831
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001832
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001833
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001834
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001835
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001836
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001837
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001838
	.4byte 0x00008602
	.4byte 0xffff000a
	.4byte 0x02009335
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x02009335
	.4byte 0x00000602
	.4byte 0xffff000b
	.4byte 0x02009335
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x02009335
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte 0x020092e1
	.4byte 0x00008e15
	.4byte 0xffff0014
	.4byte 0x020093b9
	.4byte 0x00000033
	.4byte 0x0f690064
	.4byte 0x001000bb
	.4byte 0x00000023
	.4byte 0x0f680065
	.4byte 0x0010010a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a09
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a0a
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a10
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a11
	.4byte 0x00000023
	.4byte 0x0f6a0066
	.4byte 0x00200006
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
