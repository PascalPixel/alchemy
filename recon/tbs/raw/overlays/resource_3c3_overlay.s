.syntax unified
	.thumb
	.section .text.x02008040,"ax",%progbits
	.p2align 2
	.global Func_02000040
	.thumb_func
Func_02000040:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_02000040_0
	ldr r0, [pc, #24]
	b .L_02000040_1
.L_02000040_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_02000040_2
	ldr r0, [pc, #24]
	b .L_02000040_1
.L_02000040_2:
	ldr r0, [pc, #24]
.L_02000040_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x000000aa
	.4byte 0x02008a40
	.4byte 0x000000ab
	.4byte 0x02008ad0
	.4byte 0x02008998
	.section .text.x0200808c,"ax",%progbits
	.p2align 2
	.global Func_0200008c
	.thumb_func
Func_0200008c:
	push {lr}
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_0200008c_0
	ldr r0, [pc, #40]
	b .L_0200008c_1
.L_0200008c_0:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_0200008c_2
	ldr r0, [pc, #40]
	bl 0x0200887c
	cmp r0, #0
	beq .L_0200008c_3
	ldr r0, [pc, #32]
	b .L_0200008c_1
.L_0200008c_3:
	ldr r0, [pc, #32]
	b .L_0200008c_1
.L_0200008c_2:
	ldr r0, [pc, #32]
.L_0200008c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000aa
	.4byte 0x02008ba8
	.4byte 0x000000a9
	.4byte 0x0000096f
	.4byte 0x02008c98
	.4byte 0x02008c50
	.4byte 0x02008b90
	.global Func_020000e4
	.thumb_func
Func_020000e4:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_020000e4_0
	ldr r0, [pc, #24]
	b .L_020000e4_1
.L_020000e4_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_020000e4_2
	ldr r0, [pc, #24]
	b .L_020000e4_1
.L_020000e4_2:
	ldr r0, [pc, #24]
.L_020000e4_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x000000aa
	.4byte 0x02008ddc
	.4byte 0x000000ab
	.4byte 0x02008e54
	.4byte 0x02008d10
	.global Func_02000124
	.thumb_func
Func_02000124:
	push {r5, r6, lr}
	ldr r0, [pc, #304]
	sub sp, #8
	bl 0x0200887c
	cmp r0, #0
	beq .L_02000124_0
	ldr r1, [pc, #296]
	movs r0, #226
	ldr r3, [pc, #296]
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #227
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #10
	strh r3, [r2]
.L_02000124_0:
	ldr r5, [pc, #272]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [pc, #268]
	cmp r6, r3
	bne .L_02000124_1
	ldr r0, [pc, #268]
	bl 0x0200887c
	cmp r0, #0
	beq .L_02000124_2
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x020088ec
.L_02000124_2:
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_02000124_3
	ldr r0, [pc, #236]
	bl 0x0200887c
	cmp r0, #0
	beq .L_02000124_4
	movs r0, #144
	lsls r0, r0, #2
	adds r3, r5, r0
	strh r6, [r3]
	ldr r3, [pc, #224]
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
.L_02000124_4:
	ldr r0, [pc, #220]
	bl 0x0200887c
	cmp r0, #0
	beq .L_02000124_5
	movs r0, #144
	lsls r0, r0, #2
	adds r3, r5, r0
	strh r6, [r3]
	ldr r3, [pc, #196]
	adds r2, r5, r3
	movs r3, #5
	strh r3, [r2]
.L_02000124_5:
	ldr r0, [pc, #196]
	bl 0x0200888c
.L_02000124_3:
	ldr r5, [pc, #164]
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_02000124_6
	ldr r0, [pc, #164]
	bl 0x02008884
	ldr r0, [pc, #172]
	bl 0x0200887c
	cmp r0, #0
	bne .L_02000124_6
	movs r3, #8
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #0
	movs r2, #2
	movs r3, #1
	bl 0x0200886c
.L_02000124_6:
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_02000124_7
	ldr r0, [pc, #120]
	bl 0x02008884
	b .L_02000124_7
.L_02000124_1:
	ldr r3, [pc, #124]
	cmp r6, r3
	bne .L_02000124_7
	movs r0, #8
	movs r1, #4
	bl 0x020088f4
	movs r0, #9
	movs r1, #4
	bl 0x020088f4
	movs r0, #10
	movs r1, #3
	bl 0x020088f4
	movs r0, #11
	movs r1, #4
	bl 0x020088f4
	movs r1, #3
	movs r0, #12
	bl 0x020088f4
	movs r0, #15
	bl 0x020088b4
	ldr r3, [pc, #76]
	movs r2, #56
	str r3, [r0, #28]
	movs r3, #102
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #108
	movs r1, #38
	movs r2, #1
	movs r3, #1
	bl 0x0200886c
.L_02000124_7:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000089f
	.4byte 0x02000240
	.4byte 0x00000069
	.4byte 0x000000a9
	.4byte 0x00000897
	.4byte 0x000008fb
	.4byte 0x00000242
	.4byte 0x000008fc
	.4byte 0x0000012f
	.4byte 0x0000096f
	.4byte 0x000000aa
	.4byte 0x00019999
	.section .rodata,"a",%progbits
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x00000098
	.4byte 0x400001f8
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0001
	.4byte 0x00000098
	.4byte 0xc0000208
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x80000118
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0003
	.4byte 0x00000088
	.4byte 0x40000130
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0004
	.4byte 0x00000168
	.4byte 0x00000128
	.4byte 0x01500000
	.4byte 0x02e00020
	.4byte 0x000001f0
	.4byte 0xffff0005
	.4byte 0x00000298
	.4byte 0x800001a8
	.4byte 0x01500000
	.4byte 0x02e00020
	.4byte 0x000001f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000028
	.4byte 0x00000130
	.4byte 0x00100000
	.4byte 0x01a00020
	.4byte 0x000001e0
	.4byte 0xffff0002
	.4byte 0x00000188
	.4byte 0x80000170
	.4byte 0x00100000
	.4byte 0x01a00020
	.4byte 0x000001e0
	.4byte 0xffff0003
	.4byte 0x00000028
	.4byte 0x00000390
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0xffff0004
	.4byte 0x000002a8
	.4byte 0x80000388
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0xffff0005
	.4byte 0x00000268
	.4byte 0x40000398
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0xc0000108
	.4byte 0x00180000
	.4byte 0x01180018
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000000b8
	.4byte 0x400000a8
	.4byte 0x00180000
	.4byte 0x01180018
	.4byte 0x00000100
	.4byte 0xffff0003
	.4byte 0x00000118
	.4byte 0x40000158
	.4byte 0x00d00000
	.4byte 0x02c00120
	.4byte 0x00000280
	.4byte 0xffff0004
	.4byte 0x00000298
	.4byte 0xc0000268
	.4byte 0x00d00000
	.4byte 0x02c00120
	.4byte 0x00000280
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global SuharaGate_SceneTable
SuharaGate_SceneTable:
	.4byte 0x000000a9
	.4byte 0x0011d002
	.4byte 0x002010aa
	.4byte 0x0030a069
	.4byte 0x004040aa
	.4byte 0x0052a002
	.4byte 0x000000aa
	.4byte 0x001020a9
	.4byte 0x002030aa
	.4byte 0x003020aa
	.4byte 0x004040a9
	.4byte 0x005010ab
	.4byte 0x000000ab
	.4byte 0x001050aa
	.4byte 0x002030ab
	.4byte 0x003020ab
	.4byte 0x00421002
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ee
	.4byte 0x00000007
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00025000
	.4byte 0xffff00ee
	.4byte 0x00000007
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00025000
	.4byte 0xffff00ee
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x00025000
	.4byte 0xffff00ee
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x00025000
	.4byte 0xffff00ee
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00025000
	.4byte 0x004a005b
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0098
	.4byte 0x02008980
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x02008974
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x02008974
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00004000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x020082f9
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x020082f9
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008289
	.4byte 0x00000002
	.4byte 0x096f000a
	.4byte 0x02008335
	.4byte 0x00000002
	.4byte 0x089f000a
	.4byte 0x020083c5
	.4byte 0x00000000
	.4byte 0x089f0008
	.4byte 0x0000264d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002667
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008731
	.4byte 0x00008d15
	.4byte 0x089f0008
	.4byte 0x00002652
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002669
	.4byte 0x00008d15
	.4byte 0x089f0009
	.4byte 0x00002653
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000266a
	.4byte 0x00000000
	.4byte 0x0897000a
	.4byte 0x020087ad
	.4byte 0x00008d15
	.4byte 0x0897040a
	.4byte 0x020087ad
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
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008031
	.4byte 0x0000c413
	.4byte 0x0fb40064
	.4byte 0x001000c3
	.4byte 0x00000413
	.4byte 0x0fb40064
	.4byte 0x001000c3
	.4byte 0x00008413
	.4byte 0x0fb40064
	.4byte 0x001000c3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
