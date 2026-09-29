.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.balign 4
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_0200004c_0
	ldr r0, [pc, #16]
	b .L_0200004c_1
.L_0200004c_0:
	ldr r0, [pc, #16]
.L_0200004c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b3
	.4byte 0x02009690
	.4byte 0x020096b0
	.section .text.x02008084,"ax",%progbits
	.balign 4
	.global Func_02000084
	.thumb_func
Func_02000084:
	push {lr}
	ldr r3, [pc, #40]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #32]
	cmp r2, r3
	bne .L_02000084_0
	ldr r0, [pc, #28]
	bl 0x020091dc
	cmp r0, #0
	beq .L_02000084_1
	ldr r0, [pc, #24]
	b .L_02000084_2
.L_02000084_1:
	ldr r0, [pc, #24]
	b .L_02000084_2
.L_02000084_0:
	ldr r0, [pc, #24]
.L_02000084_2:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x000000b4
	.4byte 0x000009a7
	.4byte 0x02009974
	.4byte 0x0200989c
	.4byte 0x02009734
	.section .text.x020084b0,"ax",%progbits
	.balign 4
	.global Func_020004b0
	.thumb_func
Func_020004b0:
	push {lr}
	ldr r3, [pc, #56]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_020004b0_0
	ldr r0, [pc, #44]
	bl 0x020091dc
	cmp r0, #0
	beq .L_020004b0_1
	ldr r0, [pc, #40]
	b .L_020004b0_2
.L_020004b0_1:
	ldr r0, [pc, #40]
	b .L_020004b0_2
.L_020004b0_0:
	ldr r0, [pc, #28]
	bl 0x020091dc
	cmp r0, #0
	beq .L_020004b0_3
	ldr r0, [pc, #28]
	b .L_020004b0_2
.L_020004b0_3:
	ldr r0, [pc, #28]
.L_020004b0_2:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000b4
	.4byte 0x000009a7
	.4byte 0x0200a010
	.4byte 0x02009eb4
	.4byte 0x02009ca4
	.4byte 0x02009a94
	.section .text.x0200904c,"ax",%progbits
	.global Func_0200104c
	.thumb_func
Func_0200104c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, [pc, #356]
	movs r1, #225
	lsls r1, r1, #1
	adds r1, r1, r7
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r11, r1
	cmp r3, #90
	bne .L_0200104c_0
	ldr r0, [pc, #340]
	bl 0x020091e4
.L_0200104c_0:
	ldr r3, [pc, #340]
	movs r2, #224
	ldr r1, [r3]
	ldr r3, [pc, #336]
	lsls r2, r2, #1
	str r3, [r1, r2]
	ldrsh r3, [r7, r2]
	mov r9, r3
	ldr r3, [pc, #332]
	cmp r9, r3
	bne .L_0200104c_1
	movs r0, #20
	bl 0x02009214
	movs r2, #0
	mov r10, r2
	adds r3, r0, #0
	adds r3, #35
	mov r1, r10
	strb r1, [r3]
	adds r2, r0, #0
	adds r2, #89
	ldrb r3, [r2]
	movs r6, #4
	orrs r3, r6
	strb r3, [r2]
	ldr r1, [r0, #80]
	movs r5, #13
	ldrb r2, [r1, #9]
	negs r5, r5
	adds r3, r5, #0
	ands r3, r2
	movs r2, #8
	mov r8, r2
	mov r2, r8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r0, #18
	bl 0x02009214
	adds r3, r0, #0
	adds r3, #35
	mov r1, r10
	strb r1, [r3]
	adds r2, r0, #0
	adds r2, #89
	ldrb r3, [r2]
	orrs r3, r6
	strb r3, [r2]
	ldr r1, [r0, #80]
	ldrb r2, [r1, #9]
	adds r3, r5, #0
	ands r3, r2
	mov r2, r8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r0, #19
	bl 0x02009214
	adds r2, r0, #0
	adds r2, #89
	ldrb r3, [r2]
	orrs r6, r3
	adds r3, r0, #0
	adds r3, #35
	mov r1, r10
	strb r6, [r2]
	strb r1, [r3]
	ldr r2, [r0, #80]
	ldrb r3, [r2, #9]
	ands r5, r3
	mov r3, r8
	orrs r5, r3
	strb r5, [r2, #9]
	movs r1, #6
	movs r0, #15
	bl 0x0200924c
	mov r3, r11
	movs r1, #0
	ldrsh r2, [r3, r1]
	cmp r2, #12
	bne .L_0200104c_1
	movs r1, #226
	lsls r1, r1, #1
	adds r3, r7, r1
	mov r1, r9
	strh r1, [r3]
	movs r1, #227
	lsls r1, r1, #1
	adds r3, r7, r1
	strh r2, [r3]
.L_0200104c_1:
	ldr r5, [pc, #148]
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #156]
	cmp r2, r3
	bne .L_0200104c_2
	movs r0, #13
	bl 0x02009214
	movs r2, #89
	adds r2, r2, r0
	mov r12, r2
	ldrb r2, [r2]
	movs r3, #4
	orrs r3, r2
	mov r2, r12
	strb r3, [r2]
	adds r3, r0, #0
	movs r1, #0
	adds r3, #35
	strb r1, [r3]
	ldr r1, [r0, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	movs r0, #192
	strb r3, [r1, #9]
	lsls r0, r0, #2
	bl 0x020091dc
	cmp r0, #0
	beq .L_0200104c_3
	ldr r1, [pc, #96]
	movs r0, #14
	bl 0x02009224
.L_0200104c_3:
	movs r3, #225
	lsls r3, r3, #1
	adds r5, r5, r3
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #99
	bne .L_0200104c_2
	bl 0x02008508
	movs r0, #12
	bl 0x02009214
	movs r1, #6
	bl 0x020092ec
	movs r0, #11
	bl 0x02009214
	movs r1, #6
	bl 0x020092ec
	movs r3, #21
	strh r3, [r5]
.L_0200104c_2:
	movs r0, #0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000009a7
	.4byte 0x03001ebc
	.4byte 0x00000209
	.4byte 0x000000b3
	.4byte 0x000000b4
	.4byte 0x02009314
	.section .rodata,"a",%progbits
	.global gRariberoPoseAction
gRariberoPoseAction:
	.4byte 0x00000015
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00026666
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00013333
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global gRariberoEntrances
gRariberoEntrances:
	.4byte 0xffff0001
	.4byte 0x00000090
	.4byte 0xc00000a8
	.4byte 0x00000000
	.4byte 0x00f00008
	.4byte 0x000000c0
	.4byte 0xffff0002
	.4byte 0x00000190
	.4byte 0xc00000a8
	.4byte 0x01000000
	.4byte 0x01f00008
	.4byte 0x000000c0
	.4byte 0xffff0003
	.4byte 0x00000288
	.4byte 0xc0000048
	.4byte 0x01e80000
	.4byte 0x02d80008
	.4byte 0x000000b8
	.4byte 0xffff0004
	.4byte 0x00000080
	.4byte 0xc00001d8
	.4byte 0x00000000
	.4byte 0x00f000d8
	.4byte 0x000001e8
	.4byte 0xffff0005
	.4byte 0x00000088
	.4byte 0x40000128
	.4byte 0x00000000
	.4byte 0x00f000d8
	.4byte 0x000001e8
	.4byte 0xffff0006
	.4byte 0x00000170
	.4byte 0xc00001d8
	.4byte 0x00f00000
	.4byte 0x01e000d8
	.4byte 0x000001e8
	.4byte 0xffff0007
	.4byte 0x00000188
	.4byte 0x40000128
	.4byte 0x00f00000
	.4byte 0x01e000d8
	.4byte 0x000001e8
	.4byte 0xffff0008
	.4byte 0x00000288
	.4byte 0xc0000118
	.4byte 0x01e00000
	.4byte 0x02d000d8
	.4byte 0x000001e8
	.4byte 0xffff0009
	.4byte 0x00000060
	.4byte 0xc00002a8
	.4byte 0x00080000
	.4byte 0x01380208
	.4byte 0x000002b8
	.4byte 0xffff000a
	.4byte 0x000000c8
	.4byte 0x40000258
	.4byte 0x00080000
	.4byte 0x01380208
	.4byte 0x000002b8
	.4byte 0xffff000b
	.4byte 0x000001b0
	.4byte 0xc00002d8
	.4byte 0x01380000
	.4byte 0x02280208
	.4byte 0x000002e8
	.4byte 0xffff000c
	.4byte 0x000001b0
	.4byte 0xc0000276
	.4byte 0x01380000
	.4byte 0x02280208
	.4byte 0x000002e8
	.4byte 0xffff0015
	.4byte 0x00000060
	.4byte 0xc00000a8
	.4byte 0x00080000
	.4byte 0x01380008
	.4byte 0x000000c0
	.4byte 0xffff0016
	.4byte 0x000000d8
	.4byte 0x40000058
	.4byte 0x00080000
	.4byte 0x01380008
	.4byte 0x000000c0
	.4byte 0xffff0017
	.4byte 0x000000e0
	.4byte 0xc0000188
	.4byte 0x00080000
	.4byte 0x013800e8
	.4byte 0x000001a0
	.4byte 0xffff0018
	.4byte 0x00000058
	.4byte 0x40000138
	.4byte 0x00080000
	.4byte 0x013800e8
	.4byte 0x000001a0
	.4byte 0xffff0019
	.4byte 0x00000180
	.4byte 0xc0000170
	.4byte 0x01400000
	.4byte 0x027800f0
	.4byte 0x00000190
	.4byte 0xffff001a
	.4byte 0x000001c8
	.4byte 0xc0000178
	.4byte 0x01400000
	.4byte 0x027800f0
	.4byte 0x00000190
	.4byte 0xffff001b
	.4byte 0x000001f8
	.4byte 0xc0000178
	.4byte 0x01400000
	.4byte 0x027800f0
	.4byte 0x00000190
	.4byte 0xffff001c
	.4byte 0x000001d8
	.4byte 0x40000048
	.4byte 0x01580000
	.4byte 0x02480008
	.4byte 0x000000a8
	.4byte 0xffff001d
	.4byte 0x000002e8
	.4byte 0x40000048
	.4byte 0x02780000
	.4byte 0x03680008
	.4byte 0x000000a8
	.4byte 0xffff001e
	.4byte 0x00000340
	.4byte 0xc0000368
	.4byte 0x01180000
	.4byte 0x039001f8
	.4byte 0x00000380
	.4byte 0xffff001f
	.4byte 0x000001d0
	.4byte 0x40000340
	.4byte 0x01180000
	.4byte 0x039001f8
	.4byte 0x00000380
	.4byte 0xffff0063
	.4byte 0x00000060
	.4byte 0xc00000a8
	.4byte 0x00080000
	.4byte 0x01380008
	.4byte 0x000000c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001c00c4
	.4byte 0x00cc0253
	.4byte 0x025b0024
	.4byte 0x000affff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001c00d4
	.4byte 0x00dc0053
	.4byte 0x005b0024
	.4byte 0x0016ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gRariberoExits
gRariberoExits:
	.4byte 0x000000b3
	.4byte 0x001020b2
	.4byte 0x0020a0b2
	.4byte 0x003050b3
	.4byte 0x004050b2
	.4byte 0x005030b3
	.4byte 0x006040b2
	.4byte 0x007080b3
	.4byte 0x008070b3
	.4byte 0x009090b2
	.4byte 0x00a0b0b2
	.4byte 0x00b060b2
	.4byte 0x000000b4
	.4byte 0x015070b2
	.4byte 0x016080b2
	.4byte 0x017030b2
	.4byte 0x018190b4
	.4byte 0x019180b4
	.4byte 0x01a1c0b4
	.4byte 0x01b1d0b4
	.4byte 0x01c1a0b4
	.4byte 0x01d1b0b4
	.4byte 0x01e010b2
	.4byte 0x01f010bc
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x0000c000
	.4byte 0xffff008f
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001e000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00016000
	.4byte 0xffff0067
	.4byte 0x00000003
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02260000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00024000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00004000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x005c0000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00003000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x00440000
	.4byte 0x00000000
	.4byte 0x01540000
	.4byte 0x00004000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00014000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00004000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0046
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00012000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00012000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x0002c000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00004000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00004000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00008000
	.4byte 0xffff0046
	.4byte 0x00000006
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00014000
	.4byte 0xffff0039
	.4byte 0x00000006
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00024000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00004000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x0001c000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00004000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte 0x00000001
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
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x0200848d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000026c9
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000026ca
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000026cb
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000026cc
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000026cd
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000026ce
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000026cf
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000026d0
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000026d1
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000026d2
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000026d6
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000026d7
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000026d8
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000026d9
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000026da
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000026db
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000026dc
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000026dd
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000026de
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000026df
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x020080c9
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0200815d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020081c9
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008235
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000026e6
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000026e8
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000026ea
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000026f7
	.4byte 0x00000033
	.4byte 0x0fb90064
	.4byte 0x001000e5
	.4byte 0x00000033
	.4byte 0x0fba0065
	.4byte 0x001000bb
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029be
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029bf
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x004029c0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x0200848d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028d2
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000028d3
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028d4
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000028d5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000028d6
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000028d7
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000028d8
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000028d9
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000028da
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000028db
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000028dc
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000028dd
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000028de
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000028df
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000028e0
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000028e1
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000028e2
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000028e3
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000028e4
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000028e5
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x020080c9
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0200815d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020081c9
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008235
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000028f1
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000028f3
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000028f5
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000028fd
	.4byte 0x00000033
	.4byte 0x0fb90064
	.4byte 0x001000e5
	.4byte 0x00000033
	.4byte 0x0fba0065
	.4byte 0x001000bb
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029be
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029bf
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x004029c0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0017
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0018
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0019
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001b
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001c
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001d
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x0200848d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000026d3
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000026d4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000026d5
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000026e0
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000026e1
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000026e2
	.4byte 0x00000000
	.4byte 0x0300000e
	.4byte 0x02008309
	.4byte 0x00008d15
	.4byte 0x0300040e
	.4byte 0x02008309
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200829d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000026f0
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000026f1
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000026f2
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000026f3
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000026f4
	.4byte 0x00000033
	.4byte 0x0fbb0066
	.4byte 0x001000e5
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x004029c1
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x004029c2
	.4byte 0x00000173
	.4byte 0xffff00cd
	.4byte 0x004029c3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0017
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0018
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff0019
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001b
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001c
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001d
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x0200848d
	.4byte 0x00000002
	.4byte 0x09bc0033
	.4byte 0x020083f5
	.4byte 0x00000000
	.4byte 0x09ba000c
	.4byte 0x02008469
	.4byte 0x00008d15
	.4byte 0x09ba000c
	.4byte 0x0000288d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028e6
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000028e7
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000028e8
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000028e9
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000028ea
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028eb
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000028ec
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000028ed
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000028ee
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000028ef
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200829d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000028f7
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000028f8
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000028f9
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000028fa
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000028fb
	.4byte 0x00000033
	.4byte 0x0fbb0066
	.4byte 0x001000e5
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x004029c1
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x004029c2
	.4byte 0x00000173
	.4byte 0xffff00cd
	.4byte 0x004029c3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
