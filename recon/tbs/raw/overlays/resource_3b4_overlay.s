.syntax unified
	.thumb
	.section .text.x020089f0,"ax",%progbits
	.p2align 2
	.global Func_020009f0
	.thumb_func
Func_020009f0:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020009f0_0
	ldr r0, [pc, #36]
	b .L_020009f0_1
.L_020009f0_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020009f0_2
	ldr r0, [pc, #36]
	b .L_020009f0_1
.L_020009f0_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020009f0_3
	ldr r0, [pc, #32]
	b .L_020009f0_1
.L_020009f0_3:
	ldr r0, [pc, #32]
.L_020009f0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000075
	.4byte 0x0200a898
	.4byte 0x00000076
	.4byte 0x0200a8e0
	.4byte 0x00000078
	.4byte 0x0200a928
	.4byte 0x0200a868
	.section .text.x02008a50,"ax",%progbits
	.p2align 2
	.global Func_02000a50
	.thumb_func
Func_02000a50:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000a50_0
	ldr r0, [pc, #36]
	b 0x02008a7e
.L_02000a50_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne 0x02008a72
.L_02000a6e:
	ldr r0, [pc, #36]
	b .L_02000a6e_0
	.2byte 0x4b09
	.2byte 0x429a
	.2byte 0xd101
	.2byte 0x4808
	.2byte 0xe000
	.2byte 0x4808
.L_02000a6e_0:
	pop {r1}
.L_02000a80:
	bx r1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0075
	.2byte 0x0000
	.2byte 0xa9b0
	.2byte 0x0200
	.2byte 0x0076
	.2byte 0x0000
	.2byte 0xaa40
	.2byte 0x0200
	.2byte 0x0078
	.2byte 0x0000
	.2byte 0xaad0
	.2byte 0x0200
	.2byte 0xa998
	.2byte 0x0200
	.section .text.x02008ee0,"ax",%progbits
	.p2align 2
	.global Func_02000ee0
	.thumb_func
Func_02000ee0:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000ee0_0
	ldr r0, [pc, #36]
	b .L_02000ee0_1
.L_02000ee0_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000ee0_2
	ldr r0, [pc, #36]
	b .L_02000ee0_1
.L_02000ee0_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000ee0_3
	ldr r0, [pc, #32]
	b .L_02000ee0_1
.L_02000ee0_3:
	ldr r0, [pc, #32]
.L_02000ee0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000075
	.4byte 0x0200abb4
	.4byte 0x00000076
	.4byte 0x0200acb0
	.4byte 0x00000078
	.4byte 0x0200adac
	.4byte 0x0200aba8
	.section .text.x02009f78,"ax",%progbits
	.p2align 2
	.global Func_02001f78
	.thumb_func
Func_02001f78:
	push {r5, lr}
	ldr r3, [pc, #72]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r2, r2, #1
	lsls r3, r3, #2
	str r3, [r1, r2]
	ldr r3, [pc, #60]
	adds r5, r3, r2
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, [pc, #56]
	ldrh r1, [r5]
	cmp r2, r3
	bne .L_02001f78_0
	bl 0x0200a188
	ldrh r1, [r5]
.L_02001f78_0:
	lsls r3, r1, #16
	ldr r2, [pc, #44]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02001f78_1
	bl 0x0200a290
	ldrh r1, [r5]
.L_02001f78_1:
	lsls r3, r1, #16
	ldr r2, [pc, #32]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02001f78_2
	bl 0x0200a334
.L_02001f78_2:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000075
	.4byte 0x00000076
	.4byte 0x00000078
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
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
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000040
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000378
	.4byte 0x00000358
	.4byte 0x00000368
	.4byte 0x00000358
	.4byte 0x00000358
	.4byte 0x00000358
	.4byte 0x00000348
	.4byte 0x00000358
	.4byte 0x00000338
	.4byte 0x00000358
	.4byte 0x00000338
	.4byte 0x00000348
	.4byte 0x00000338
	.4byte 0x00000338
	.4byte 0x00000348
	.4byte 0x00000338
	.4byte 0x00000358
	.4byte 0x00000338
	.4byte 0x00000368
	.4byte 0x00000338
	.4byte 0x00000378
	.4byte 0x00000338
	.4byte 0x00000378
	.4byte 0x00000348
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
	.4byte 0xffff0001
	.4byte 0x000000c8
	.4byte 0xc00003c8
	.4byte 0x00180000
	.4byte 0x01f80230
	.4byte 0x000003f0
	.4byte 0xffff0002
	.4byte 0x00000058
	.4byte 0xc00003c8
	.4byte 0x00180000
	.4byte 0x01f80230
	.4byte 0x000003f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000c8
	.4byte 0xc0000198
	.4byte 0x00280000
	.4byte 0x01d00000
	.4byte 0x000001b8
	.4byte 0xffff0002
	.4byte 0x00000058
	.4byte 0xc0000188
	.4byte 0x00280000
	.4byte 0x01d00000
	.4byte 0x000001b8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000308
	.4byte 0xc00003b8
	.4byte 0x02000000
	.4byte 0x03a00220
	.4byte 0x000003e0
	.4byte 0xffff0002
	.4byte 0x00000298
	.4byte 0xc00003b8
	.4byte 0x02000000
	.4byte 0x03a00220
	.4byte 0x000003e0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000075
	.4byte 0x00103080
	.4byte 0x00204080
	.4byte 0x00000076
	.4byte 0x00103081
	.4byte 0x00204081
	.4byte 0x00000078
	.4byte 0x00103083
	.4byte 0x00204083
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00024000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00024000
	.4byte 0x0036005a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02009199
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020091d9
	.4byte 0x00009415
	.4byte 0xffff0008
	.4byte 0x02009205
	.4byte 0x00009415
	.4byte 0xffff0009
	.4byte 0x02009239
	.4byte 0x00009415
	.4byte 0xffff000a
	.4byte 0x0200926d
	.4byte 0x00009415
	.4byte 0xffff000b
	.4byte 0x020092a1
	.4byte 0x00009415
	.4byte 0xffff000c
	.4byte 0x020092d5
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x02009071
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte 0x02009109
	.4byte 0x00000002
	.4byte 0xffff0034
	.4byte 0x02009115
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte 0x02009121
	.4byte 0x00000002
	.4byte 0xffff0036
	.4byte 0x0200915d
	.4byte 0x00004602
	.4byte 0xffff0035
	.4byte 0x02008f35
	.4byte 0x00004602
	.4byte 0xffff0036
	.4byte 0x02008f35
	.4byte 0x00000013
	.4byte 0x0eca0064
	.4byte 0x0020014d
	.4byte 0x00000013
	.4byte 0x0ecb0065
	.4byte 0x0010010b
	.4byte 0x00000013
	.4byte 0x0ecc0066
	.4byte 0x001000c0
	.4byte 0x00000013
	.4byte 0x0ecd0067
	.4byte 0x001000e2
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte 0x020095cd
	.4byte 0x00000002
	.4byte 0xffff0036
	.4byte 0x020095d9
	.4byte 0x00000002
	.4byte 0xffff003d
	.4byte 0x020095f1
	.4byte 0x00000002
	.4byte 0xffff003e
	.4byte 0x02009695
	.4byte 0x00000006
	.4byte 0xffff0060
	.4byte 0x02009839
	.4byte 0x00000006
	.4byte 0xffff005c
	.4byte 0x0200949d
	.4byte 0x00000006
	.4byte 0xffff005b
	.4byte 0x0200938d
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x020098c1
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020098d1
	.4byte 0x00009115
	.4byte 0xffff0008
	.4byte 0x02008aa5
	.4byte 0x00009115
	.4byte 0xffff0009
	.4byte 0x02008aa5
	.4byte 0x00009115
	.4byte 0xffff000a
	.4byte 0x02008aa5
	.4byte 0x00009115
	.4byte 0xffff000b
	.4byte 0x02008aa5
	.4byte 0x00009115
	.4byte 0xffff000c
	.4byte 0x02008aa5
	.4byte 0x00000013
	.4byte 0x0ece0064
	.4byte 0x002001bc
	.4byte 0x00000013
	.4byte 0x0ecf0065
	.4byte 0x001000e3
	.4byte 0x00000013
	.4byte 0x0ed00066
	.4byte 0x00100060
	.4byte 0x00000013
	.4byte 0x0ed10067
	.4byte 0x001000ba
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte 0x02009d65
	.4byte 0x00000202
	.4byte 0xffff0033
	.4byte 0x02009b61
	.4byte 0x00000202
	.4byte 0xffff0034
	.4byte 0x02009c5d
	.4byte 0x00000202
	.4byte 0xffff0035
	.4byte 0x02009e61
	.4byte 0x00000202
	.4byte 0xffff003d
	.4byte 0x02009b61
	.4byte 0x00000202
	.4byte 0xffff0036
	.4byte 0x02009d79
	.4byte 0x00000202
	.4byte 0xffff003c
	.4byte 0x020098e1
	.4byte 0x00000202
	.4byte 0xffff0046
	.4byte 0x020099cd
	.4byte 0x00000002
	.4byte 0x00360014
	.4byte 0x02008ad1
	.4byte 0x00000002
	.4byte 0x00360015
	.4byte 0x02008b69
	.4byte 0x00000002
	.4byte 0x00360016
	.4byte 0x02008ccd
	.4byte 0x00000002
	.4byte 0x0036001e
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x0036001f
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360020
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360021
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360022
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360023
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360024
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360025
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360026
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360027
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360028
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360029
	.4byte 0x02008e51
	.4byte 0x00000007
	.4byte 0xffff000f
	.4byte 0x020089dd
	.4byte 0x00009115
	.4byte 0xffff000f
	.4byte 0x02008ec9
	.4byte 0x00001815
	.4byte 0x0200000d
	.4byte 0x02009985
	.4byte 0x00008c15
	.4byte 0x02010008
	.4byte 0x02009ac9
	.4byte 0x00008c15
	.4byte 0x02060009
	.4byte 0x02009c29
	.4byte 0x00008c15
	.4byte 0x0203000a
	.4byte 0x02009cf9
	.4byte 0x00008c15
	.4byte 0x0204000b
	.4byte 0x02009e55
	.4byte 0x00008c15
	.4byte 0x0205000c
	.4byte 0x02009e75
	.4byte 0x00000013
	.4byte 0x0ed60068
	.4byte 0x0020029a
	.4byte 0x00000013
	.4byte 0x0ed70069
	.4byte 0x001000bd
	.4byte 0x00000013
	.4byte 0x0ed8006a
	.4byte 0x001000bc
	.4byte 0x00000013
	.4byte 0x0ed9006b
	.4byte 0x00100026
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraAshiba_SlotColumns
TakaraAshiba_SlotColumns:
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000000
	.global TakaraAshiba_IconTimer
TakaraAshiba_IconTimer:
	.4byte 0x00000000
