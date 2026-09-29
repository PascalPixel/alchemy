.syntax unified
	.thumb
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_02000030_0
	ldr r0, [pc, #24]
	b .L_02000030_1
.L_02000030_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_02000030_2
	ldr r0, [pc, #24]
	b .L_02000030_1
.L_02000030_2:
	ldr r0, [pc, #24]
.L_02000030_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000064
	.4byte 0x020084d0
	.4byte 0x00000065
	.4byte 0x020086c8
	.4byte 0x020084a0
	.section .text.x0200807c,"ax",%progbits
	.p2align 2
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {r5, lr}
	ldr r1, [pc, #72]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #64]
	cmp r2, r3
	bne .L_0200007c_0
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #9
	blt .L_0200007c_1
	cmp r3, #15
	ble .L_0200007c_2
	cmp r3, #17
	bne .L_0200007c_1
.L_0200007c_2:
	ldr r5, [pc, #40]
	b .L_0200007c_3
.L_0200007c_1:
	ldr r5, [pc, #40]
.L_0200007c_3:
	adds r0, r5, #0
	bl 0x02008420
	adds r0, r5, #0
	b .L_0200007c_4
.L_0200007c_0:
	ldr r3, [pc, #32]
	cmp r2, r3
	bne .L_0200007c_5
	ldr r0, [pc, #28]
	b .L_0200007c_4
.L_0200007c_5:
	ldr r0, [pc, #28]
.L_0200007c_4:
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000064
	.4byte 0x020088d4
	.4byte 0x0200879c
	.4byte 0x00000065
	.4byte 0x02008a0c
	.4byte 0x02008784
	.global Func_020000e4
	.thumb_func
Func_020000e4:
	push {lr}
	ldr r1, [pc, #64]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_020000e4_0
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #9
	blt .L_020000e4_1
	cmp r3, #15
	ble .L_020000e4_2
	cmp r3, #17
	bne .L_020000e4_1
.L_020000e4_2:
	ldr r0, [pc, #32]
	b .L_020000e4_3
.L_020000e4_1:
	ldr r0, [pc, #32]
	b .L_020000e4_3
.L_020000e4_0:
	ldr r3, [pc, #32]
	cmp r2, r3
	bne .L_020000e4_4
	ldr r0, [pc, #28]
	b .L_020000e4_3
.L_020000e4_4:
	ldr r0, [pc, #28]
.L_020000e4_3:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000064
	.4byte 0x02008c88
	.4byte 0x02008a48
	.4byte 0x00000065
	.4byte 0x02008eb0
	.4byte 0x02008a3c
	.section .text.x02008308,"ax",%progbits
	.p2align 2
	.global Func_02000308
	.thumb_func
Func_02000308:
	push {lr}
	ldr r3, [pc, #32]
	movs r2, #224
	ldr r1, [r3]
	ldr r3, [pc, #28]
	lsls r2, r2, #1
	str r3, [r1, r2]
	ldr r3, [pc, #28]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_02000308_0
	bl 0x0200833c
.L_02000308_0:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000209
	.4byte 0x02000240
	.4byte 0x00000064
	.section .rodata,"a",%progbits
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
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
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000090
	.4byte 0xc00000c0
	.4byte 0x00050000
	.4byte 0x00f5002d
	.4byte 0x000000e6
	.4byte 0xffff0002
	.4byte 0x00000190
	.4byte 0xc00000c0
	.4byte 0x00ff0000
	.4byte 0x01e5002d
	.4byte 0x000000e6
	.4byte 0xffff0003
	.4byte 0x00000270
	.4byte 0xc00000f0
	.4byte 0x01f40000
	.4byte 0x02df002d
	.4byte 0x00000113
	.4byte 0xffff0004
	.4byte 0x00000080
	.4byte 0xc00001b0
	.4byte 0x00230000
	.4byte 0x01130122
	.4byte 0x000001d4
	.4byte 0xffff0005
	.4byte 0x00000180
	.4byte 0xc0000180
	.4byte 0x01310000
	.4byte 0x022100ff
	.4byte 0x000001e5
	.4byte 0xffff0006
	.4byte 0x000001d0
	.4byte 0xc00001c0
	.4byte 0x01310000
	.4byte 0x022100ff
	.4byte 0x000001e5
	.4byte 0xffff0007
	.4byte 0x000002d0
	.4byte 0xc00001f0
	.4byte 0x02760000
	.4byte 0x036b0131
	.4byte 0x00000217
	.4byte 0xffff0008
	.4byte 0x00000320
	.4byte 0xc00001b8
	.4byte 0x02760000
	.4byte 0x036b0131
	.4byte 0x00000217
	.4byte 0xffff0009
	.4byte 0x00000030
	.4byte 0xc0000280
	.4byte 0x00000000
	.4byte 0x013101ef
	.4byte 0x00000307
	.4byte 0xffff000a
	.4byte 0x000000e0
	.4byte 0xc00002e0
	.4byte 0x00000000
	.4byte 0x013101ef
	.4byte 0x00000307
	.4byte 0xffff000b
	.4byte 0x00000198
	.4byte 0x40000268
	.4byte 0x01630000
	.4byte 0x02b2020d
	.4byte 0x000002c6
	.4byte 0xffff000c
	.4byte 0x00000228
	.4byte 0x40000288
	.4byte 0x01630000
	.4byte 0x02b2020d
	.4byte 0x000002c6
	.4byte 0xffff000d
	.4byte 0x00000278
	.4byte 0x40000288
	.4byte 0x01630000
	.4byte 0x02b2020d
	.4byte 0x000002c6
	.4byte 0xffff000e
	.4byte 0x00000228
	.4byte 0xc00002a8
	.4byte 0x01630000
	.4byte 0x02b2020d
	.4byte 0x000002c6
	.4byte 0xffff000f
	.4byte 0x00000278
	.4byte 0xc00002a8
	.4byte 0x01630000
	.4byte 0x02b2020d
	.4byte 0x000002c6
	.4byte 0xffff0010
	.4byte 0x00000328
	.4byte 0x40000268
	.4byte 0x02e90000
	.4byte 0x03a7023a
	.4byte 0x000002d0
	.4byte 0xffff0011
	.4byte 0x000001b8
	.4byte 0xc0000368
	.4byte 0x01540000
	.4byte 0x020802e4
	.4byte 0x00000384
	.4byte 0xffff0012
	.4byte 0x00000248
	.4byte 0xc0000368
	.4byte 0x021c0000
	.4byte 0x02da02e4
	.4byte 0x00000384
	.4byte 0xffff0013
	.4byte 0x00000348
	.4byte 0x40000318
	.4byte 0x02c60000
	.4byte 0x03ac02e4
	.4byte 0x00000384
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0xc0000130
	.4byte 0x006e0000
	.4byte 0x01860023
	.4byte 0x00000159
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x40000080
	.4byte 0x006e0000
	.4byte 0x01860023
	.4byte 0x00000159
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KareiHeya_Exits
KareiHeya_Exits:
	.4byte 0x00000064
	.4byte 0x00101063
	.4byte 0x00202063
	.4byte 0x00303063
	.4byte 0x00404063
	.4byte 0x0050c063
	.4byte 0x00605063
	.4byte 0x00706063
	.4byte 0x0080d063
	.4byte 0x0090b064
	.4byte 0x00a07063
	.4byte 0x00b09064
	.4byte 0x00c11064
	.4byte 0x00d12064
	.4byte 0x00e13064
	.4byte 0x00f10064
	.4byte 0x0100f064
	.4byte 0x0110c064
	.4byte 0x0120d064
	.4byte 0x0130e064
	.4byte 0x00000065
	.4byte 0x00108063
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00650000
	.4byte 0x00000000
	.4byte 0x00b50000
	.4byte 0x00003000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00a70000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x0000b000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x017f0000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x0000b000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x015a0000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x0000f000
	.4byte 0x0000007e
	.4byte 0x00000002
	.4byte 0x023d0000
	.4byte 0x00000000
	.4byte 0x00a30000
	.4byte 0x0000b000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x029e0000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00005000
	.4byte 0x0000007f
	.4byte 0x00000001
	.4byte 0x00b70000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00005000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01710000
	.4byte 0x00003000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x02740000
	.4byte 0x00007000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x030c0000
	.4byte 0x00000000
	.4byte 0x02790000
	.4byte 0x00013000
	.4byte 0x0000008f
	.4byte 0x00000001
	.4byte 0x031b0000
	.4byte 0x00000000
	.4byte 0x03260000
	.4byte 0x0000d000
	.4byte 0x0000008f
	.4byte 0x00000001
	.4byte 0x03590000
	.4byte 0x00000000
	.4byte 0x03490000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02730000
	.4byte 0x00011000
	.4byte 0x00000080
	.4byte 0x00000002
	.4byte 0x01f90000
	.4byte 0x00000000
	.4byte 0x02990000
	.4byte 0x00000000
	.4byte 0x00000084
	.4byte 0x00000001
	.4byte 0x00420000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x0001b000
	.4byte 0x00000094
	.4byte 0x00000001
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00005000
	.4byte 0x000000ab
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00025000
	.4byte 0x000000ab
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x023a0000
	.4byte 0x00005000
	.4byte 0x00000072
	.4byte 0x00000001
	.4byte 0x00cf0000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00008000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02830000
	.4byte 0x00007000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x00f70000
	.4byte 0x00000000
	.4byte 0x02e00000
	.4byte 0x0001b000
	.4byte 0x0000009d
	.4byte 0x00000001
	.4byte 0x01070000
	.4byte 0x00000000
	.4byte 0x02730000
	.4byte 0x00007000
	.4byte 0x00000043
	.4byte 0x00000001
	.4byte 0x01a10000
	.4byte 0x00000000
	.4byte 0x032a0000
	.4byte 0x00001000
	.4byte 0x00000042
	.4byte 0x00000001
	.4byte 0x01cb0000
	.4byte 0x00000000
	.4byte 0x03340000
	.4byte 0x00009000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x00005000
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
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x02008241
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x0000c602
	.4byte 0xffff0013
	.4byte 0x02008241
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001abf
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001ac0
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001ac3
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001ac4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001ac7
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001ac8
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001acb
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001acc
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200816d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001ae6
	.4byte 0x00000000
	.4byte 0x09110012
	.4byte 0x00001ae7
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001afe
	.4byte 0x00000000
	.4byte 0x09110013
	.4byte 0x00001ae8
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001aff
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001ac1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001ac2
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001ac5
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001ac6
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001ac9
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001aca
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001acd
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001ace
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001af5
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001af6
	.4byte 0x00008d15
	.4byte 0x09110012
	.4byte 0x00001af7
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001b03
	.4byte 0x00008d15
	.4byte 0x09110013
	.4byte 0x00001af8
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001b04
	.4byte 0x00000033
	.4byte 0x0f830064
	.4byte 0x0020000b
	.4byte 0x00000173
	.4byte 0x0f840065
	.4byte 0x001000e2
	.4byte 0x00000033
	.4byte 0x0f850066
	.4byte 0x001000bc
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029a8
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029a9
	.4byte 0x00000173
	.4byte 0xffff0065
	.4byte 0x004029aa
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x004029ab
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x004029ac
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x02008241
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte 0x02008241
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200818d
	.4byte 0x00000000
	.4byte 0x09110009
	.4byte 0x00001ada
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001afc
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001adb
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001adc
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008145
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001ade
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001adf
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001ae0
	.4byte 0x00000000
	.4byte 0x09110010
	.4byte 0x00001ae1
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001afd
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001ae2
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001ae9
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001aea
	.4byte 0x00008d15
	.4byte 0x09110008
	.4byte 0x00001aeb
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001b00
	.4byte 0x00008d15
	.4byte 0x09110009
	.4byte 0x00001aec
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001b01
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001aed
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001aee
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001aef
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001af0
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001af1
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001af2
	.4byte 0x00008d15
	.4byte 0x09110010
	.4byte 0x00001af3
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001b02
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001af4
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001af9
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001afa
	.4byte 0x00000033
	.4byte 0x0f830064
	.4byte 0x0020000b
	.4byte 0x00000173
	.4byte 0x0f840065
	.4byte 0x001000e2
	.4byte 0x00000033
	.4byte 0x0f850066
	.4byte 0x001000bc
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029a8
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029a9
	.4byte 0x00000173
	.4byte 0xffff0065
	.4byte 0x004029aa
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x004029ab
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x004029ac
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020081fd
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a90
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x007d0007
	.4byte 0x00020001
	.4byte 0x00080005
	.4byte 0x0001007d
	.4byte 0x00050002
	.4byte 0x0000ffff
	.global KareiHeya_ArrivalPlacements
KareiHeya_ArrivalPlacements:
	.4byte 0x02008ee0
	.4byte 0x00620022
	.4byte 0x02008ee0
	.4byte 0x00620027
	.4byte 0x02008ee0
	.4byte 0x00600032
	.4byte 0x02008ee0
	.4byte 0x006b0034
