.syntax unified
	.thumb
	.section .text.x02008040,"ax",%progbits
	.balign 4
	.global Func_02000040
	.thumb_func
Func_02000040:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000040_0
	ldr r0, [pc, #36]
	b .L_02000040_1
.L_02000040_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000040_2
	ldr r0, [pc, #36]
	b .L_02000040_1
.L_02000040_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000040_3
	ldr r0, [pc, #32]
	b .L_02000040_1
.L_02000040_3:
	ldr r0, [pc, #32]
.L_02000040_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000006b
	.4byte 0x02009738
	.4byte 0x00000070
	.4byte 0x020097c8
	.4byte 0x0000006c
	.4byte 0x02009840
	.4byte 0x02009708
	.section .text.x020080a0,"ax",%progbits
	.balign 4
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {lr}
	ldr r3, [pc, #108]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #100]
	cmp r2, r3
	bne .L_020000a0_0
	ldr r0, [pc, #96]
	bl 0x02009550
	cmp r0, #0
	beq .L_020000a0_1
	ldr r0, [pc, #92]
	b .L_020000a0_2
.L_020000a0_1:
	ldr r0, [pc, #92]
	b .L_020000a0_2
.L_020000a0_0:
	ldr r3, [pc, #92]
	cmp r2, r3
	bne .L_020000a0_3
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02009550
	cmp r0, #0
	beq .L_020000a0_4
	ldr r0, [pc, #76]
	b .L_020000a0_2
.L_020000a0_4:
	ldr r0, [pc, #76]
	b .L_020000a0_2
.L_020000a0_3:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_020000a0_5
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02009550
	cmp r0, #0
	beq .L_020000a0_6
	ldr r0, [pc, #64]
	b .L_020000a0_2
.L_020000a0_6:
	ldr r0, [pc, #32]
	bl 0x02009550
	cmp r0, #0
	beq .L_020000a0_7
	ldr r0, [pc, #52]
	b .L_020000a0_2
.L_020000a0_7:
	ldr r0, [pc, #52]
	b .L_020000a0_2
.L_020000a0_5:
	ldr r0, [pc, #52]
.L_020000a0_2:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000006b
	.4byte 0x0000093e
	.4byte 0x02009ba4
	.4byte 0x02009acc
	.4byte 0x00000070
	.4byte 0x020099c4
	.4byte 0x020098ec
	.4byte 0x0000006c
	.4byte 0x02009dcc
	.4byte 0x02009d24
	.4byte 0x02009c7c
	.4byte 0x020098d4
	.section .text.x0200819c,"ax",%progbits
	.balign 4
	.global Func_0200019c
	.thumb_func
Func_0200019c:
	push {lr}
	ldr r3, [pc, #108]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #100]
	cmp r2, r3
	bne .L_0200019c_0
	ldr r0, [pc, #96]
	bl 0x02009550
	cmp r0, #0
	beq .L_0200019c_1
	ldr r0, [pc, #92]
	b .L_0200019c_2
.L_0200019c_1:
	ldr r0, [pc, #92]
	b .L_0200019c_2
.L_0200019c_0:
	ldr r3, [pc, #92]
	cmp r2, r3
	bne .L_0200019c_3
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02009550
	cmp r0, #0
	beq .L_0200019c_4
	ldr r0, [pc, #76]
	b .L_0200019c_2
.L_0200019c_4:
	ldr r0, [pc, #76]
	b .L_0200019c_2
.L_0200019c_3:
	ldr r3, [pc, #76]
	cmp r2, r3
	bne .L_0200019c_5
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02009550
	cmp r0, #0
	beq .L_0200019c_6
	ldr r0, [pc, #64]
	b .L_0200019c_2
.L_0200019c_6:
	ldr r0, [pc, #32]
	bl 0x02009550
	cmp r0, #0
	beq .L_0200019c_7
	ldr r0, [pc, #52]
	b .L_0200019c_2
.L_0200019c_7:
	ldr r0, [pc, #52]
	b .L_0200019c_2
.L_0200019c_5:
	ldr r0, [pc, #52]
.L_0200019c_2:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000006b
	.4byte 0x0000093e
	.4byte 0x02009fa0
	.4byte 0x02009e80
	.4byte 0x00000070
	.4byte 0x0200a120
	.4byte 0x0200a018
	.4byte 0x0000006c
	.4byte 0x0200a390
	.4byte 0x0200a30c
	.4byte 0x0200a24c
	.4byte 0x02009e74
	.section .text.x020087dc,"ax",%progbits
	.balign 4
	.global Func_020007dc
	.thumb_func
Func_020007dc:
	push {lr}
	bl 0x02009580
	movs r0, #158
	bl 0x020096b0
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #0
	lsls r1, r1, #8
	bl 0x020095a8
	movs r1, #3
	movs r0, #0
	bl 0x02009648
	ldr r3, [pc, #88]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #80]
	cmp r2, r3
	bne .L_020007dc_0
	movs r1, #152
	movs r2, #174
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #3
	bl 0x020095c0
	ldr r0, [pc, #64]
	movs r1, #78
	movs r2, #86
	bl 0x02009500
	b .L_020007dc_1
.L_020007dc_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_020007dc_1
	movs r0, #0
	movs r1, #248
	movs r2, #192
	bl 0x020095c0
	ldr r0, [pc, #44]
	movs r1, #74
	movs r2, #9
	bl 0x02009500
.L_020007dc_1:
	movs r0, #16
	bl 0x02009578
	movs r0, #3
	bl 0x02009670
	bl 0x02009588
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000006b
	.4byte 0x020096b8
	.4byte 0x00000070
	.4byte 0x020096ce
	.section .text.x02008328,"ax",%progbits
	.balign 4
	.global Func_02000328
	.thumb_func
Func_02000328:
	push {r5, r6, lr}
	movs r0, #0
	bl 0x020095a0
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, [pc, #44]
	ands r3, r2
	lsls r3, r3, #16
	asrs r5, r3, #16
	bl 0x02009580
	ldr r0, [pc, #36]
	bl 0x02009550
	cmp r0, #0
	beq .L_02000328_0
	ldr r0, [pc, #32]
	bl 0x02009550
	cmp r0, #0
	beq .L_02000328_1
	ldr r0, [pc, #24]
	bl 0x02009618
	movs r0, #12
	movs r1, #0
	bl 0x02009620
	b .L_02000328_2
	.4byte 0xffffc000
	.4byte 0x000008a7
	.4byte 0x000008a9
	.4byte 0x00001d23
.L_02000328_1:
	ldr r5, [pc, #292]
	adds r0, r5, #0
	bl 0x02009618
	movs r1, #0
	movs r0, #12
	bl 0x02009620
	movs r0, #0
	movs r1, #0
	bl 0x02009598
	cmp r0, #0
	bne .L_02000328_3
	movs r0, #10
	bl 0x02009578
	adds r0, r5, #1
	bl 0x02009618
	movs r0, #12
	movs r1, #0
	bl 0x02009628
	movs r2, #161
	movs r0, #12
	movs r1, #88
	lsls r2, r2, #3
	bl 0x020095c8
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009640
	movs r0, #20
	bl 0x02009578
	ldr r0, [pc, #220]
	bl 0x02009558
	b .L_02000328_2
.L_02000328_3:
	adds r0, r5, #2
	bl 0x02009618
	movs r0, #12
	movs r1, #0
	bl 0x02009628
	b .L_02000328_2
.L_02000328_0:
	movs r2, #128
	lsls r3, r5, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02000328_4
	ldr r0, [pc, #188]
	bl 0x02009618
	movs r0, #12
	movs r1, #0
	bl 0x02009628
	ldr r0, [pc, #180]
	bl 0x02009550
	cmp r0, #0
	beq .L_02000328_5
	movs r0, #235
	bl 0x02009548
	movs r1, #235
	adds r5, r0, #0
	bl 0x02009540
	movs r1, #3
	adds r6, r0, #0
	movs r0, #12
	bl 0x020095e8
	movs r2, #161
	movs r0, #12
	movs r1, #88
	lsls r2, r2, #3
	bl 0x020095c8
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009640
	ldr r3, [pc, #124]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #12
	movs r1, #0
	bl 0x02009628
	adds r1, r6, #0
	adds r0, r5, #0
	bl 0x02009570
	ldr r0, [pc, #96]
	bl 0x02009558
	movs r0, #0
	bl 0x020095a0
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r2, #163
	movs r0, #0
	lsls r2, r2, #3
	bl 0x020095c8
	movs r2, #163
	movs r0, #0
	movs r1, #72
	lsls r2, r2, #3
	bl 0x020095c8
	movs r2, #163
	movs r0, #12
	movs r1, #88
	lsls r2, r2, #3
	bl 0x020095c8
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009640
	b .L_02000328_2
.L_02000328_5:
	movs r0, #12
	movs r1, #0
	bl 0x02009628
.L_02000328_2:
	bl 0x02009588
.L_02000328_4:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00001d20
	.4byte 0x000008a9
	.4byte 0x00001d16
	.4byte 0x000008a5
	.4byte 0x03001ebc
	.4byte 0x000008a7
	.section .text.x0200886c,"ax",%progbits
	.balign 4
	.global Func_0200086c
	.thumb_func
Func_0200086c:
	push {r5, lr}
	ldr r5, [pc, #76]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #90
	bne .L_0200086c_0
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02009558
.L_0200086c_0:
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_0200086c_1
	bl 0x020088cc
	b .L_0200086c_2
.L_0200086c_1:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200086c_3
	bl 0x02008958
	b .L_0200086c_2
.L_0200086c_3:
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_0200086c_2
	bl 0x02008ad4
.L_0200086c_2:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000006b
	.4byte 0x00000070
	.4byte 0x0000006c
	.section .rodata.part1,"a",%progbits
	.4byte 0x00540062
	.4byte 0x00020002
	.4byte 0x00620005
	.4byte 0x00020052
	.4byte 0x00050002
	.4byte 0x005cffff
	.4byte 0x00020009
	.4byte 0x00050002
	.4byte 0x0007005c
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0xffffe000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000c
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x00000080
	.4byte 0x40000080
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
	.4byte 0x000000f8
	.4byte 0x40000558
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000120
	.4byte 0xc00005b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000048
	.4byte 0x00000518
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000130
	.4byte 0x40000588
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000128
	.4byte 0x40000418
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
	.4byte 0x00000188
	.4byte 0x80000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000198
	.4byte 0x80000128
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x000001b0
	.4byte 0xffff0002
	.4byte 0x00000018
	.4byte 0x000000e8
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x000001b0
	.4byte 0xffff0003
	.4byte 0x000000f0
	.4byte 0x400000d8
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x000001b0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000060
	.4byte 0xc00000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000060
	.4byte 0xc00000f8
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000110
	.4byte 0xffff0002
	.4byte 0x00000180
	.4byte 0xc00000f8
	.4byte 0x01400000
	.4byte 0x02300020
	.4byte 0x00000110
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KareiTorebi_SceneTable
KareiTorebi_SceneTable:
	.4byte 0x0000006b
	.4byte 0x00115002
	.4byte 0x0020506d
	.4byte 0x0030106c
	.4byte 0x0043a002
	.4byte 0x00000070
	.4byte 0x00116002
	.4byte 0x0020606d
	.4byte 0x0030206c
	.4byte 0x0000006c
	.4byte 0x0010306b
	.4byte 0x00203070
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00014000
	.4byte 0xffff0093
	.4byte 0x00000002
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00004000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00020000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00002000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00002000
	.4byte 0xffff00f1
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00be0000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00be0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00018000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00010000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00020000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00028000
	.4byte 0xffff00f1
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00be0000
	.4byte 0x00024000
	.4byte 0xffff00f1
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00be0000
	.4byte 0x00024000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00028000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x05800000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x04c80000
	.4byte 0x0001a000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x05180000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x05380000
	.4byte 0x00014000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x05080000
	.4byte 0x00004000
	.4byte 0xffff0091
	.4byte 0x00000002
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x05580000
	.4byte 0x00004000
	.4byte 0x0035005a
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x04780000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00014000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x0001a000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00014000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x05480000
	.4byte 0x00001000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x05080000
	.4byte 0x00019000
	.4byte 0x0035005a
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x04780000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00014000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00016000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00010000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00016000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00010000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00016000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00b80000
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
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x020087dd
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008241
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001cfb
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001cfc
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008261
	.4byte 0x00000000
	.4byte 0x0911000c
	.4byte 0x00001d12
	.4byte 0x00000000
	.4byte 0x0911000d
	.4byte 0x00001d13
	.4byte 0x00000400
	.4byte 0x08a9000c
	.4byte 0x02008329
	.4byte 0x00008400
	.4byte 0x08a9000c
	.4byte 0x02008329
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020082dd
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x020084b9
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001d00
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001d01
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001d02
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001d03
	.4byte 0x00008d15
	.4byte 0x0911000c
	.4byte 0x00001d14
	.4byte 0x00008d15
	.4byte 0x0911000d
	.4byte 0x00001d15
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001d1d
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001d1e
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008031
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x020087dd
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001f85
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001f86
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001f87
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001f88
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008031
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x020087dd
	.4byte 0x00000002
	.4byte 0x08aa0014
	.4byte 0x02008145
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008689
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001f0c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020086a9
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020086c9
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001f1d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001f1e
	.4byte 0x00008d15
	.4byte 0x18a8000b
	.4byte 0x00001f20
	.4byte 0x00008d15
	.4byte 0xffff040b
	.4byte 0x020086c9
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001f1f
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001f21
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001f22
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001f0d
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001f0e
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x02008af9
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x02008af9
	.4byte 0x00000013
	.4byte 0x0f960064
	.4byte 0x001000b7
	.4byte 0x000000d3
	.4byte 0x0f950065
	.4byte 0x00200023
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte 0x020087dd
	.4byte 0x00000002
	.4byte 0x08ab0032
	.4byte 0x02008e41
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000023e1
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000023e2
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000023e3
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000023e4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000023fd
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000023fe
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000023ff
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002400
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002401
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002402
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002403
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002404
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002405
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002406
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002407
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002408
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x02008af9
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte 0x02008af9
	.4byte 0x00000013
	.4byte 0x0f960064
	.4byte 0x001000b7
	.4byte 0x000000d3
	.4byte 0x0f950065
	.4byte 0x00200023
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200851d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001d0c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001d0d
	.4byte 0x00008d15
	.4byte 0x08a50008
	.4byte 0x00001d0e
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001d0f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001d10
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001d11
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001f0f
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001f10
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001f11
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001f12
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001f13
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001f14
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001f83
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001f84
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001f0f
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001f10
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001f11
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001f12
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001f13
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001f14
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001f83
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001f84
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000023e5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000023e6
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000023e7
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000023e8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000023e9
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000023ea
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
