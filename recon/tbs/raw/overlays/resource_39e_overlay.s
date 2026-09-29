.syntax unified
	.thumb
	.section .text.x0200902c,"ax",%progbits
	.balign 4
	.global ShianJiin_RunMasterScene
	.thumb_func
ShianJiin_RunMasterScene:
	.global Func_0200102c
	.thumb_func
Func_0200102c:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r1, [pc, #276]
	movs r0, #15
	ldr r2, [pc, #276]
	bl 0x0200c414
	movs r0, #60
	bl 0x0200c3d4
	ldr r5, [pc, #268]
	adds r0, r5, #0
	bl 0x0200c49c
	cmp r6, #0
	bne .L_0200102c_0
	subs r0, r5, #1
	bl 0x0200c49c
	movs r0, #15
	ldr r1, [pc, #252]
	movs r2, #60
	bl 0x0200c4cc
	movs r2, #20
	movs r0, #15
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #15
	bl 0x0200c474
	ldr r0, [pc, #232]
	bl 0x0200c49c
	movs r2, #20
	movs r0, #15
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #4
	movs r0, #15
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #15
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #15
	movs r1, #3
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
.L_0200102c_0:
	cmp r6, #2
	bne .L_0200102c_1
	ldr r0, [pc, #176]
	bl 0x0200c49c
	movs r0, #15
	movs r1, #2
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
.L_0200102c_1:
	movs r2, #20
	movs r0, #15
	movs r1, #0
	bl 0x0200c4b4
	bl 0x02008f80
	movs r0, #15
	movs r1, #3
	bl 0x0200c474
	movs r1, #232
	movs r2, #168
	movs r0, #19
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200c44c
	movs r1, #232
	movs r2, #168
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #20
	bl 0x0200c44c
	movs r0, #19
	bl 0x0200c404
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r0, #12]
	movs r0, #19
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #60]
	movs r0, #19
	bl 0x0200c404
	ldr r3, [pc, #56]
	str r3, [r0, #24]
	movs r0, #19
	bl 0x0200c404
	movs r3, #128
	ldr r2, [r0, #80]
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r0, #124
	bl 0x0200c57c
	movs r0, #40
	bl 0x0200c3d4
	movs r0, #15
	movs r1, #216
	movs r2, #152
	bl 0x0200c434
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #6
	movs r2, #30
	bl 0x0200c4c4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0000183a
	.4byte 0x00000101
	.4byte 0x000018ae
	.4byte 0x000018ac
	.section .text.x0200a7ec,"ax",%progbits
	.balign 4
	.global FieldScene_RunScene39e_020027ec
	.thumb_func
FieldScene_RunScene39e_020027ec:
	push {r5, lr}
	bl 0x0200c3dc
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c414
	movs r2, #252
	movs r1, #168
	movs r0, #0
	lsls r2, r2, #1
	bl 0x0200c42c
	ldr r5, [pc, #676]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	bl 0x0200c524
	bl 0x0200c534
	movs r0, #0
	bl 0x0200c444
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #2
	bl 0x0200c46c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x0200c4d4
	movs r0, #60
	bl 0x0200c3d4
	movs r0, #8
	bl 0x0200c404
	movs r3, #0
	adds r0, #91
	strb r3, [r0]
	movs r0, #152
	bl 0x0200c57c
	movs r0, #8
	bl 0x0200c404
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r0, #40]
	movs r1, #1
	movs r0, #8
	bl 0x0200c454
	ldr r0, [pc, #584]
	bl 0x0200c49c
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #0
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	ldr r1, [pc, #524]
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #0
	bne .L_020027ec_0
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b 0x0200a93c
.L_020027ec_0:
	movs r0, #10
	bl 0x0200c3d4
.L_020028fe:
	movs r1, #2
	movs r0, #8
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r2, #20
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #8
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
	movs r0, #0
	ldr r1, [pc, #344]
	movs r2, #60
	bl 0x0200c4cc
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #1
	bne .L_020028fe_0
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	movs r2, #60
	bl 0x0200c4cc
	ldr r0, [pc, #304]
	bl 0x0200c49c
	movs r0, #8
	movs r1, #0
	bl 0x0200c4a4
.L_020028fe_1:
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #1
	bne .L_020028fe_0
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	movs r2, #60
	bl 0x0200c4cc
	ldr r0, [pc, #264]
	bl 0x0200c49c
	movs r0, #8
	movs r1, #0
	bl 0x0200c4a4
	b .L_020028fe_1
.L_020028fe_0:
	ldr r0, [pc, #252]
	bl 0x0200c49c
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #0
	movs r0, #8
	bl 0x0200c4a4
	movs r0, #0
	movs r1, #0
	bl 0x0200c3f4
	cmp r0, #0
	bne .L_020028fe_2
	movs r0, #10
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #0
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	ldr r3, [pc, #156]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020028fe_3
.L_020028fe_2:
	movs r0, #10
	bl 0x0200c3d4
	movs r0, #8
	movs r1, #2
	bl 0x0200c474
	ldr r3, [pc, #124]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200c4b4
.L_020028fe_3:
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #2
	movs r0, #8
	bl 0x0200c474
	movs r0, #20
	bl 0x0200c3d4
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200c4b4
	movs r1, #3
	movs r0, #0
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #3
	movs r0, #8
	bl 0x0200c45c
	movs r0, #20
	bl 0x0200c3d4
	movs r1, #5
	movs r0, #8
	bl 0x0200c454
	ldr r0, [pc, #36]
	bl 0x0200c3c4
	bl 0x0200c3e4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.2byte 0x17be
	.2byte 0x0000
	.4byte 0x00000101
	.4byte 0x000017c8
	.4byte 0x000017e0
	.4byte 0x000017c9
	.4byte 0x00000893
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
	.global gShianJiinEntrances1
gShianJiinEntrances1:
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
	.global gShianJiinEntrancesOther
gShianJiinEntrancesOther:
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
	.global gShianJiinPlacements1
gShianJiinPlacements1:
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
	.global gShianJiinPlacements
gShianJiinPlacements:
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
	.global gShianJiinPlacementsEntrance3
gShianJiinPlacementsEntrance3:
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
	.global gShianJiinEvents1
gShianJiinEvents1:
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
	.global gShianJiinEvents
gShianJiinEvents:
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
	.global gShianJiinEventsEntrance3
gShianJiinEventsEntrance3:
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
