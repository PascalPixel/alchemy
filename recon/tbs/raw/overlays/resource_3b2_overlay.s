.syntax unified
	.thumb
	.section .text.x02008cc4,"ax",%progbits
	.align 2
	.global Func_02000cc4
	.thumb_func
Func_02000cc4:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02000cc4_0
	ldr r0, [pc, #56]
	b .L_02000cc4_1
.L_02000cc4_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000cc4_2
	ldr r0, [pc, #56]
	b .L_02000cc4_1
.L_02000cc4_2:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000cc4_3
	ldr r0, [pc, #52]
	b .L_02000cc4_1
.L_02000cc4_3:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000cc4_4
	ldr r0, [pc, #52]
	b .L_02000cc4_1
.L_02000cc4_4:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000cc4_5
	ldr r0, [pc, #48]
	b .L_02000cc4_1
.L_02000cc4_5:
	ldr r0, [pc, #48]
.L_02000cc4_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000071
	.4byte 0x0200b310
	.4byte 0x00000072
	.4byte 0x0200b358
	.4byte 0x0000007b
	.4byte 0x0200b3a0
	.4byte 0x0000007c
	.4byte 0x0200b400
	.4byte 0x0000007d
	.4byte 0x0200b448
	.4byte 0x0200b478
	.section .text.x02008d48,"ax",%progbits
	.align 2
	.global Func_02000d48
	.thumb_func
Func_02000d48:
	push {lr}
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02000d48_0
	ldr r0, [pc, #40]
	b .L_02000d48_1
.L_02000d48_0:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000d48_2
	ldr r0, [pc, #40]
	b .L_02000d48_1
.L_02000d48_2:
	ldr r3, [pc, #40]
	cmp r2, r3
	bgt .L_02000d48_3
	ldr r3, [pc, #36]
	cmp r2, r3
	blt .L_02000d48_3
	ldr r0, [pc, #36]
	b .L_02000d48_1
.L_02000d48_3:
	ldr r0, [pc, #36]
.L_02000d48_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x00000071
	.4byte 0x0200b610
	.4byte 0x0000007b
	.4byte 0x0200b718
	.4byte 0x00000086
	.4byte 0x0000007e
	.4byte 0x0200b850
	.4byte 0x0200b5f8
	.section .text.x02008ec4,"ax",%progbits
	.align 2
	.global Func_02000ec4
	.thumb_func
Func_02000ec4:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02000ec4_0
	ldr r0, [pc, #56]
	b .L_02000ec4_1
.L_02000ec4_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000ec4_2
	ldr r0, [pc, #56]
	b .L_02000ec4_1
.L_02000ec4_2:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000ec4_3
	ldr r0, [pc, #52]
	b .L_02000ec4_1
.L_02000ec4_3:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000ec4_4
	ldr r0, [pc, #52]
	b .L_02000ec4_1
.L_02000ec4_4:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000ec4_5
	ldr r0, [pc, #48]
	b .L_02000ec4_1
.L_02000ec4_5:
	ldr r0, [pc, #48]
.L_02000ec4_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000071
	.4byte 0x0200b904
	.4byte 0x00000072
	.4byte 0x0200b8e0
	.4byte 0x0000007b
	.4byte 0x0200b9f4
	.4byte 0x0000007c
	.4byte 0x0200bd48
	.4byte 0x0000007d
	.4byte 0x0200bd6c
	.4byte 0x0200b880
	.section .text.x02008f70,"ax",%progbits
	.align 2
	.global Func_02000f70
	.thumb_func
Func_02000f70:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #612]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r2, r2, #1
	lsls r3, r3, #2
	ldr r6, [pc, #604]
	str r3, [r1, r2]
	adds r2, r2, r6
	movs r1, #0
	ldrsh r7, [r2, r1]
	ldr r3, [pc, #596]
	sub sp, #8
	mov r8, r2
	cmp r7, r3
	bne .L_02000f70_0
	bl 0x0200991c
	b .L_02000f70_1
.L_02000f70_0:
	ldr r3, [pc, #584]
	cmp r7, r3
	bne .L_02000f70_2
	ldr r0, [pc, #584]
	bl 0x0200b058
	cmp r0, #0
	bne .L_02000f70_3
	movs r3, #13
	str r3, [sp, #0]
	movs r5, #40
	movs r0, #0
	movs r1, #3
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200b028
	movs r3, #15
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #2
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200b028
	movs r1, #216
	movs r2, #162
	movs r0, #101
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200b108
.L_02000f70_3:
	mov r1, r8
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, r7
	bne .L_02000f70_2
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #5
	beq .L_02000f70_4
	ldr r0, [pc, #500]
	bl 0x0200b058
	cmp r0, #0
	bne .L_02000f70_4
	b .L_02000f70_1
.L_02000f70_4:
	ldr r0, [pc, #488]
	bl 0x0200b060
	movs r3, #13
	movs r2, #30
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #1
	movs r2, #1
	movs r3, #1
	bl 0x0200b028
	movs r1, #216
	movs r2, #244
	movs r0, #100
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x0200b108
	b .L_02000f70_1
.L_02000f70_2:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #440]
	cmp r2, r3
	beq .L_02000f70_5
	b .L_02000f70_6
.L_02000f70_5:
	bl 0x0200967c
	movs r0, #8
	bl 0x0200b088
	movs r3, #129
	lsls r3, r3, #16
	str r3, [r0, #56]
	movs r0, #9
	bl 0x020088c0
	movs r0, #10
	bl 0x020088c0
	movs r0, #144
	lsls r0, r0, #2
	bl 0x0200b058
	cmp r0, #0
	beq .L_02000f70_7
	movs r0, #11
	bl 0x0200b088
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000f70_8
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	movs r1, #4
	strb r3, [r2]
	bl 0x0200aff8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200b038
.L_02000f70_8:
	movs r1, #152
	movs r2, #184
	movs r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
.L_02000f70_7:
	ldr r0, [pc, #348]
	bl 0x0200b058
	cmp r0, #0
	beq .L_02000f70_9
	movs r0, #12
	bl 0x0200b088
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000f70_10
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	movs r1, #4
	strb r3, [r2]
	bl 0x0200aff8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200b038
.L_02000f70_10:
	movs r1, #160
	movs r2, #184
	movs r0, #0
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
.L_02000f70_9:
	ldr r0, [pc, #292]
	bl 0x0200b058
	cmp r0, #0
	beq .L_02000f70_11
	movs r0, #13
	bl 0x0200b088
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000f70_12
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	movs r1, #4
	strb r3, [r2]
	bl 0x0200aff8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200b038
.L_02000f70_12:
	movs r1, #192
	movs r2, #168
	movs r0, #0
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
.L_02000f70_11:
	ldr r0, [pc, #236]
	bl 0x0200b058
	cmp r0, #0
	beq .L_02000f70_13
	movs r0, #14
	bl 0x0200b088
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000f70_14
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	movs r1, #4
	strb r3, [r2]
	bl 0x0200aff8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200b038
.L_02000f70_14:
	movs r1, #144
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
	movs r1, #188
	movs r2, #160
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r3, #253
	bl 0x0200b048
.L_02000f70_13:
	ldr r0, [pc, #164]
	bl 0x0200b058
	cmp r0, #0
	bne .L_02000f70_1
	movs r0, #8
	bl 0x0200aed8
	b .L_02000f70_1
.L_02000f70_6:
	ldr r5, [pc, #148]
	cmp r2, r5
	bne .L_02000f70_15
	ldr r0, [pc, #148]
	bl 0x0200b058
	cmp r0, #0
	bne .L_02000f70_15
	movs r3, #37
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200b028
	movs r1, #150
	movs r2, #168
	movs r0, #100
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200b108
.L_02000f70_15:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	cmp r2, r5
	blt .L_02000f70_1
	ldr r3, [pc, #92]
	cmp r2, r3
	bgt .L_02000f70_1
	bl 0x02009214
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #5
	bne .L_02000f70_1
	bl 0x02009494
.L_02000f70_1:
	movs r0, #0
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000007b
	.4byte 0x0000007d
	.4byte 0x00000ef7
	.4byte 0x000008d1
	.4byte 0x00000071
	.4byte 0x00000241
	.4byte 0x00000242
	.4byte 0x00000243
	.4byte 0x00000fd7
	.4byte 0x0000007e
	.4byte 0x00000ef4
	.4byte 0x00000086
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
	.global TakaraShima_EntranceCells
TakaraShima_EntranceCells:
	.4byte 0x0017003a
	.4byte 0x00020001
	.4byte 0x003b0005
	.4byte 0x00010017
	.4byte 0x00050002
	.4byte 0x0000ffff
	.global gEffectScripts
gEffectScripts:
	.4byte 0x0200b214
	.4byte 0x0200b24c
	.4byte 0x0200b284
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
	.global TakaraShima_SceneTable01
TakaraShima_SceneTable01:
	.4byte 0xffff0001
	.4byte 0x00000118
	.4byte 0xc0000220
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000230
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x40000168
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000230
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable02
TakaraShima_SceneTable02:
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0xc0000208
	.4byte 0x00100000
	.4byte 0x01a00040
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0x00000090
	.4byte 0x00100000
	.4byte 0x01a00040
	.4byte 0x00000220
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable03
TakaraShima_SceneTable03:
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0xc0000198
	.4byte 0x00000000
	.4byte 0x02800010
	.4byte 0x000001d0
	.4byte 0xffff0002
	.4byte 0x000000c8
	.4byte 0xc0000198
	.4byte 0x00000000
	.4byte 0x02800010
	.4byte 0x000001d0
	.4byte 0xffff0063
	.4byte 0x00000110
	.4byte 0x400000e8
	.4byte 0x00000000
	.4byte 0x02800010
	.4byte 0x000001d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable04
TakaraShima_SceneTable04:
	.4byte 0xffff0001
	.4byte 0x000000b8
	.4byte 0x80000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000028
	.4byte 0x000000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable05
TakaraShima_SceneTable05:
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0x80000290
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000000d8
	.4byte 0xc0000220
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.global TakaraShima_SceneTable06
TakaraShima_SceneTable06:
	.4byte 0xffff0001
	.4byte 0x00000348
	.4byte 0x800000b0
	.4byte 0x01b00000
	.4byte 0x03700020
	.4byte 0x00000160
	.4byte 0xffff0002
	.4byte 0x000001e8
	.4byte 0x000000b0
	.4byte 0x01b00000
	.4byte 0x03700020
	.4byte 0x00000160
	.4byte 0xffff0003
	.4byte 0x000002c8
	.4byte 0x40000098
	.4byte 0x01b00000
	.4byte 0x03700020
	.4byte 0x00000160
	.4byte 0xffff0004
	.4byte 0x00000258
	.4byte 0x40000098
	.4byte 0x01b00000
	.4byte 0x03700020
	.4byte 0x00000160
	.4byte 0xffff0005
	.4byte 0x000002c8
	.4byte 0xc0000098
	.4byte 0x01b00000
	.4byte 0x03700020
	.4byte 0x00000160
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable17
TakaraShima_SceneTable17:
	.4byte 0x00000071
	.4byte 0x0014c002
	.4byte 0x00201072
	.4byte 0x00000072
	.4byte 0x00102071
	.4byte 0x0020107e
	.4byte 0x0000007b
	.4byte 0x00103086
	.4byte 0x00204086
	.4byte 0x0000007c
	.4byte 0x00102086
	.4byte 0x0020107d
	.4byte 0x0000007d
	.4byte 0x0010207c
	.4byte 0x0000007e
	.4byte 0x00102072
	.4byte 0x0020107f
	.4byte 0x00301073
	.4byte 0x00402073
	.4byte 0x0000007f
	.4byte 0x0010207e
	.4byte 0x00201080
	.4byte 0x00301074
	.4byte 0x00402074
	.4byte 0x00000080
	.4byte 0x0010207f
	.4byte 0x00201081
	.4byte 0x00301075
	.4byte 0x00402075
	.4byte 0x00000081
	.4byte 0x00102080
	.4byte 0x00201082
	.4byte 0x00301076
	.4byte 0x00402076
	.4byte 0x00000082
	.4byte 0x00102081
	.4byte 0x00201083
	.4byte 0x00301077
	.4byte 0x00402077
	.4byte 0x00000083
	.4byte 0x00102082
	.4byte 0x00201084
	.4byte 0x00301078
	.4byte 0x00402078
	.4byte 0x00000084
	.4byte 0x00102083
	.4byte 0x00201085
	.4byte 0x00301079
	.4byte 0x00402079
	.4byte 0x00000085
	.4byte 0x00102084
	.4byte 0x00201086
	.4byte 0x0030107a
	.4byte 0x0040207a
	.4byte 0x00000086
	.4byte 0x00102085
	.4byte 0x0020107c
	.4byte 0x0030107b
	.4byte 0x0040207b
	.4byte 0x000001ff
	.global TakaraShima_SceneTable07
TakaraShima_SceneTable07:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable08
TakaraShima_SceneTable08:
	.4byte 0x0fd70016
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable09
TakaraShima_SceneTable09:
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable10
TakaraShima_SceneTable10:
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable11
TakaraShima_SceneTable11:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x020093ad
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x0200938d
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02009375
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x020092b5
	.4byte 0x00000013
	.4byte 0x0ef40064
	.4byte 0x00500004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable12
TakaraShima_SceneTable12:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable13
TakaraShima_SceneTable13:
	.4byte 0x00009415
	.4byte 0x0fd70008
	.4byte 0x02008f3d
	.4byte 0x00000c15
	.4byte 0x0240000b
	.4byte 0x02009509
	.4byte 0x00000c15
	.4byte 0x0241000c
	.4byte 0x02009549
	.4byte 0x00000c15
	.4byte 0x0242000d
	.4byte 0x0200958d
	.4byte 0x00000c15
	.4byte 0x0243000e
	.4byte 0x020095d1
	.4byte 0x00004e15
	.4byte 0x08c4000f
	.4byte 0x02009625
	.4byte 0x00004e15
	.4byte 0x08c50010
	.4byte 0x0200964d
	.4byte 0x00004e15
	.4byte 0x08c60011
	.4byte 0x0200965d
	.4byte 0x00004e15
	.4byte 0x08c70012
	.4byte 0x0200966d
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff0014
	.4byte 0x02009741
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02009741
	.4byte 0x00008413
	.4byte 0x0ec00064
	.4byte 0x00200001
	.4byte 0x0000c413
	.4byte 0x0ec00064
	.4byte 0x00200001
	.4byte 0x0000a413
	.4byte 0x0ec00064
	.4byte 0x00200001
	.4byte 0x00008413
	.4byte 0x0ec10065
	.4byte 0x001000c3
	.4byte 0x0000c413
	.4byte 0x0ec10065
	.4byte 0x001000c3
	.4byte 0x0000a413
	.4byte 0x0ec10065
	.4byte 0x001000c3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable14
TakaraShima_SceneTable14:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00008602
	.4byte 0xffff0028
	.4byte 0x02009b69
	.4byte 0x00000602
	.4byte 0xffff0029
	.4byte 0x02009b95
	.4byte 0x00004602
	.4byte 0xffff002a
	.4byte 0x02009b41
	.4byte 0x0000c602
	.4byte 0xffff002b
	.4byte 0x02009b15
	.4byte 0x0000c602
	.4byte 0xffff0015
	.4byte 0x02009bbd
	.4byte 0x00004602
	.4byte 0xffff0015
	.4byte 0x02009c85
	.4byte 0x00000602
	.4byte 0xffff0015
	.4byte 0x02009b95
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte 0x02009b69
	.4byte 0x0000c602
	.4byte 0xffff0016
	.4byte 0x02009d2d
	.4byte 0x00004602
	.4byte 0xffff0016
	.4byte 0x02009de1
	.4byte 0x00000602
	.4byte 0xffff0016
	.4byte 0x02009b95
	.4byte 0x00008602
	.4byte 0xffff0016
	.4byte 0x02009b69
	.4byte 0x00008602
	.4byte 0xffff0017
	.4byte 0x02009e5d
	.4byte 0x00000602
	.4byte 0xffff0017
	.4byte 0x02009ef5
	.4byte 0x0000c602
	.4byte 0xffff0017
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0017
	.4byte 0x02009b41
	.4byte 0x00008602
	.4byte 0xffff0018
	.4byte 0x02009f79
	.4byte 0x00000602
	.4byte 0xffff0018
	.4byte 0x0200a005
	.4byte 0x0000c602
	.4byte 0xffff0018
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0018
	.4byte 0x02009b41
	.4byte 0x00008602
	.4byte 0xffff0019
	.4byte 0x0200a081
	.4byte 0x00000602
	.4byte 0xffff0019
	.4byte 0x0200a16d
	.4byte 0x0000c602
	.4byte 0xffff0019
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0019
	.4byte 0x02009b41
	.4byte 0x00008602
	.4byte 0xffff001a
	.4byte 0x0200a201
	.4byte 0x00000602
	.4byte 0xffff001a
	.4byte 0x0200a2c9
	.4byte 0x0000c602
	.4byte 0xffff001a
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte 0x02009b41
	.4byte 0x0000c602
	.4byte 0xffff001f
	.4byte 0x0200a3c5
	.4byte 0x00004602
	.4byte 0xffff001f
	.4byte 0x0200a451
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte 0x02009b95
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte 0x02009b69
	.4byte 0x0000c602
	.4byte 0xffff0020
	.4byte 0x0200a4c9
	.4byte 0x00004602
	.4byte 0xffff0020
	.4byte 0x0200a5f1
	.4byte 0x00000602
	.4byte 0xffff0020
	.4byte 0x02009b95
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte 0x02009b69
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte 0x0200a701
	.4byte 0x00004602
	.4byte 0xffff0021
	.4byte 0x0200a849
	.4byte 0x00000602
	.4byte 0xffff0021
	.4byte 0x02009b95
	.4byte 0x00008602
	.4byte 0xffff0021
	.4byte 0x02009b69
	.4byte 0x00008602
	.4byte 0xffff0022
	.4byte 0x0200a985
	.4byte 0x00000602
	.4byte 0xffff0022
	.4byte 0x0200aa99
	.4byte 0x0000c602
	.4byte 0xffff0022
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0022
	.4byte 0x02009b41
	.4byte 0x00008602
	.4byte 0xffff0023
	.4byte 0x0200ab81
	.4byte 0x00000602
	.4byte 0xffff0023
	.4byte 0x0200ac4d
	.4byte 0x0000c602
	.4byte 0xffff0023
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0023
	.4byte 0x02009b41
	.4byte 0x00008602
	.4byte 0xffff0024
	.4byte 0x0200ad0d
	.4byte 0x00000602
	.4byte 0xffff0024
	.4byte 0x0200add1
	.4byte 0x0000c602
	.4byte 0xffff0024
	.4byte 0x02009b15
	.4byte 0x00004602
	.4byte 0xffff0024
	.4byte 0x02009b41
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x020099b5
	.4byte 0x00000002
	.4byte 0xffff0016
	.4byte 0x020099c1
	.4byte 0x00000002
	.4byte 0xffff0017
	.4byte 0x020099cd
	.4byte 0x00000002
	.4byte 0xffff0018
	.4byte 0x020099d9
	.4byte 0x00000002
	.4byte 0xffff0019
	.4byte 0x020099e5
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte 0x020099f1
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte 0x020099fd
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte 0x02009a09
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte 0x02009a15
	.4byte 0x00000002
	.4byte 0xffff0022
	.4byte 0x02009a21
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte 0x02009a2d
	.4byte 0x00000002
	.4byte 0xffff0024
	.4byte 0x02009a39
	.4byte 0x00000013
	.4byte 0x0ee20064
	.4byte 0x002003e7
	.4byte 0x00000013
	.4byte 0x0ee30065
	.4byte 0x001000bd
	.4byte 0x00000013
	.4byte 0x0ee40066
	.4byte 0x0010000b
	.4byte 0x00000013
	.4byte 0x0ee50067
	.4byte 0x001000e3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable15
TakaraShima_SceneTable15:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global TakaraShima_SceneTable16
TakaraShima_SceneTable16:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x08d1000a
	.4byte 0x02008e89
	.4byte 0x00004602
	.4byte 0xffff0014
	.4byte 0x02009b41
	.4byte 0x00000013
	.4byte 0x0ee60064
	.4byte 0x00100053
	.4byte 0x00000013
	.4byte 0x0ef70065
	.4byte 0x00500007
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
