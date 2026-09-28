.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl 0x02009690
	pop {r0}
	bx r0
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
	.global Func_02000094
	.thumb_func
Func_02000094:
	movs r0, #0
	bx lr
	.global Func_02000098
	.thumb_func
Func_02000098:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020098a0
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
	.global Func_02000144
	.thumb_func
Func_02000144:
	push {lr}
	bl 0x02009580
	ldr r0, [pc, #68]
	bl 0x02009558
	movs r1, #196
	movs r2, #148
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x020095c8
	movs r0, #8
	ldr r1, [pc, #48]
	ldr r2, [pc, #52]
	bl 0x020095a8
	movs r1, #204
	movs r2, #148
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x020095c8
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x02009640
	movs r0, #20
	bl 0x02009578
	bl 0x02009588
	pop {r0}
	bx r0
	.4byte 0x000008aa
	.4byte 0x00013333
	.4byte 0x00009999
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
	.global Func_02000240
	.thumb_func
Func_02000240:
	push {lr}
	bl 0x02009580
	ldr r0, [pc, #20]
	bl 0x02009618
	movs r1, #0
	movs r0, #8
	bl 0x02009638
	bl 0x02009588
	pop {r0}
	bx r0
	.4byte 0x00001cf8
	.global Func_02000260
	.thumb_func
Func_02000260:
	push {lr}
	bl 0x02009580
	ldr r0, [pc, #100]
	bl 0x02009550
	cmp r0, #0
	bne .L_02000260_0
	ldr r0, [pc, #92]
	bl 0x02009618
	movs r1, #0
	movs r0, #11
	bl 0x02009620
	movs r0, #0
	movs r1, #0
	bl 0x02009598
	cmp r0, #0
	bne .L_02000260_1
	movs r0, #11
	movs r1, #0
	bl 0x02009628
	ldr r0, [pc, #56]
	bl 0x02009558
	b .L_02000260_2
.L_02000260_1:
	ldr r3, [pc, #56]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #11
	movs r1, #0
	bl 0x02009628
	b .L_02000260_2
.L_02000260_0:
	ldr r0, [pc, #32]
	bl 0x02009618
	movs r0, #11
	movs r1, #0
	bl 0x02009628
.L_02000260_2:
	bl 0x02009588
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000008a6
	.4byte 0x00001cfd
	.4byte 0x03001ebc
	.4byte 0x00001cfe
	.global Func_020002dc
	.thumb_func
Func_020002dc:
	push {lr}
	movs r0, #0
	bl 0x020095a0
	bl 0x02009580
	ldr r0, [pc, #48]
	bl 0x02009550
	cmp r0, #0
	beq .L_020002dc_0
	ldr r0, [pc, #44]
	bl 0x02009550
	cmp r0, #0
	beq .L_020002dc_0
	ldr r0, [pc, #36]
	bl 0x02009618
	movs r1, #0
	movs r0, #12
	bl 0x02009620
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009640
.L_020002dc_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000008a7
	.4byte 0x000008a9
	.4byte 0x00001d23
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
	.global Func_020004b8
	.thumb_func
Func_020004b8:
	push {lr}
	bl 0x02009580
	ldr r0, [pc, #72]
	bl 0x02009550
	cmp r0, #0
	beq .L_020004b8_0
	ldr r0, [pc, #64]
	bl 0x02009618
	movs r0, #13
	movs r1, #0
	bl 0x02009620
	b .L_020004b8_1
.L_020004b8_0:
	ldr r0, [pc, #52]
	bl 0x02009550
	cmp r0, #0
	beq .L_020004b8_2
	ldr r0, [pc, #48]
	bl 0x02009618
	movs r0, #13
	movs r1, #0
	bl 0x02009628
	b .L_020004b8_1
.L_020004b8_2:
	ldr r0, [pc, #36]
	bl 0x02009618
	movs r0, #13
	movs r1, #0
	bl 0x02009628
.L_020004b8_1:
	bl 0x02009588
	pop {r0}
	bx r0
	.4byte 0x000008a7
	.4byte 0x00001d1f
	.4byte 0x000008a5
	.4byte 0x00001d1b
	.4byte 0x00001d19
	.global Func_0200051c
	.thumb_func
Func_0200051c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #150
	lsls r2, r2, #2
	sub sp, #4
	mov r8, r2
	bl 0x02009580
	ldr r0, [pc, #320]
	bl 0x02009550
	cmp r0, #0
	beq .L_0200051c_0
	ldr r0, [pc, #312]
	bl 0x02009618
	movs r0, #8
	movs r1, #0
	bl 0x02009628
	b .L_0200051c_1
.L_0200051c_0:
	ldr r0, [pc, #300]
	bl 0x02009618
	movs r1, #0
	movs r0, #8
	bl 0x02009620
	movs r0, #0
	movs r1, #0
	bl 0x02009598
	cmp r0, #1
	bne .L_0200051c_2
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009630
	b .L_0200051c_3
.L_0200051c_2:
	ldr r7, [pc, #268]
	movs r3, #236
	ldr r2, [r7]
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	mov r0, r8
	movs r1, #5
	bl 0x02009538
	movs r1, #0
	movs r0, #8
	bl 0x02009620
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #11
	movs r3, #4
	movs r0, #19
	bl 0x02009518
	adds r5, r0, #0
	adds r1, r5, #0
	ldr r0, [pc, #220]
	movs r2, #0
	movs r3, #0
	bl 0x02009528
	ldr r6, [pc, #212]
	movs r3, #8
	ldr r0, [r6, #16]
	movs r1, #6
	str r3, [sp, #0]
	adds r2, r5, #0
	movs r3, #24
	bl 0x02009530
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl 0x02009598
	cmp r0, #1
	bne .L_0200051c_4
	adds r0, r5, #0
	movs r1, #2
	bl 0x02009520
	movs r1, #4
	movs r0, #0
	bl 0x020095e8
	movs r0, #10
	bl 0x02009578
	b .L_0200051c_5
.L_0200051c_4:
	ldr r3, [r6, #16]
	cmp r8, r3
	bls .L_0200051c_6
	adds r0, r5, #0
	movs r1, #2
	bl 0x02009520
	movs r1, #3
	movs r0, #0
	bl 0x020095e8
	movs r0, #10
	bl 0x02009578
	ldr r2, [r7]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	movs r0, #113
	strh r3, [r2]
	bl 0x020096b0
.L_0200051c_5:
	movs r0, #8
	movs r1, #0
	bl 0x02009628
	b .L_0200051c_3
.L_0200051c_6:
	adds r0, r5, #0
	movs r1, #2
	bl 0x02009520
	movs r1, #3
	movs r0, #0
	bl 0x020095e8
	movs r0, #10
	bl 0x02009578
	ldr r3, [r7]
	movs r2, #236
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	adds r2, #3
	strh r2, [r3]
	movs r0, #8
	movs r1, #0
	bl 0x02009628
	movs r1, #0
	movs r0, #235
	bl 0x02009590
	ldr r0, [pc, #28]
	bl 0x02009558
	mov r3, r8
	negs r0, r3
	bl 0x02009568
.L_0200051c_3:
	bl 0x02009588
.L_0200051c_1:
	sub sp, #-4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x000008a5
	.4byte 0x00001d0b
	.4byte 0x00001d04
	.4byte 0x03001ebc
	.4byte 0x00000c8a
	.4byte 0x02000240
	.global Func_02000688
	.thumb_func
Func_02000688:
	push {lr}
	bl 0x02009580
	ldr r0, [pc, #20]
	bl 0x02009618
	movs r1, #0
	movs r0, #8
	bl 0x02009638
	bl 0x02009588
	pop {r0}
	bx r0
	.4byte 0x00001f09
	.global Func_020006a8
	.thumb_func
Func_020006a8:
	push {lr}
	bl 0x02009580
	ldr r0, [pc, #20]
	bl 0x02009618
	movs r1, #0
	movs r0, #10
	bl 0x02009638
	bl 0x02009588
	pop {r0}
	bx r0
	.4byte 0x00001f15
	.global Func_020006c8
	.thumb_func
Func_020006c8:
	push {lr}
	bl 0x02009580
	ldr r0, [pc, #248]
	bl 0x02009550
	cmp r0, #0
	beq .L_020006c8_0
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x02009608
	movs r0, #20
	bl 0x02009578
	ldr r0, [pc, #224]
	bl 0x02009618
	movs r0, #11
	movs r1, #0
	bl 0x02009628
	bl 0x02009588
	b .L_020006c8_1
.L_020006c8_0:
	movs r0, #20
	bl 0x02009578
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #50
	bl 0x02009650
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x02009608
	movs r0, #20
	bl 0x02009578
	ldr r0, [pc, #176]
	bl 0x02009618
	movs r0, #11
	movs r1, #0
	bl 0x02009628
	ldr r0, [pc, #164]
	bl 0x02009550
	cmp r0, #0
	beq .L_020006c8_2
	movs r0, #20
	bl 0x02009578
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #40
	bl 0x02009650
	movs r1, #0
	movs r0, #11
	bl 0x02009620
	movs r0, #0
	movs r1, #0
	bl 0x02009598
	cmp r0, #0
	bne .L_020006c8_3
	movs r0, #20
	bl 0x02009578
	movs r0, #11
	movs r1, #0
	bl 0x02009628
	ldr r0, [pc, #92]
	bl 0x02009558
	b .L_020006c8_4
.L_020006c8_3:
	movs r0, #10
	bl 0x02009578
	ldr r3, [pc, #92]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r0, #11
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x02009640
	movs r0, #30
	bl 0x02009578
	b .L_020006c8_4
.L_020006c8_2:
	movs r0, #10
	bl 0x02009578
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x02009640
	movs r0, #30
	bl 0x02009578
.L_020006c8_4:
	bl 0x02009588
.L_020006c8_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000008a8
	.4byte 0x00001f1c
	.4byte 0x00001f18
	.4byte 0x000008a6
	.4byte 0x03001ebc
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
	.global Func_020008cc
	.thumb_func
Func_020008cc:
	push {r5, lr}
	ldr r5, [pc, #116]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_020008cc_0
	ldr r0, [pc, #104]
	bl 0x02009550
	cmp r0, #0
	bne .L_020008cc_0
	ldr r0, [pc, #92]
	bl 0x02009558
	bl 0x02008ba0
.L_020008cc_0:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_020008cc_1
	ldr r0, [pc, #72]
	bl 0x02009550
	cmp r0, #0
	bne .L_020008cc_1
	ldr r0, [pc, #68]
	bl 0x02009560
.L_020008cc_1:
	ldr r0, [pc, #64]
	bl 0x02009550
	cmp r0, #0
	beq .L_020008cc_2
	ldr r0, [pc, #52]
	bl 0x02009550
	cmp r0, #0
	bne .L_020008cc_2
	movs r1, #176
	movs r2, #163
	movs r0, #12
	lsls r1, r1, #15
	lsls r2, r2, #19
	bl 0x020095d8
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009640
.L_020008cc_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000008ac
	.4byte 0x00000109
	.4byte 0x000008a9
	.4byte 0x00000911
	.global Func_02000958
	.thumb_func
Func_02000958:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x02008af8
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02009550
	cmp r0, #0
	beq .L_02000958_0
	movs r0, #12
	movs r1, #2
	bl 0x02009610
.L_02000958_0:
	ldr r3, [pc, #272]
	movs r1, #225
	lsls r1, r1, #1
	adds r5, r3, r1
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #3
	bne .L_02000958_1
	ldr r0, [pc, #260]
	bl 0x02009560
	ldrh r2, [r5]
.L_02000958_1:
	lsls r3, r2, #16
	movs r2, #128
	lsls r2, r2, #9
	cmp r3, r2
	bne .L_02000958_2
	ldr r0, [pc, #244]
	bl 0x02009560
.L_02000958_2:
	ldr r0, [pc, #240]
	bl 0x02009550
	cmp r0, #0
	beq .L_02000958_3
	movs r1, #204
	movs r2, #148
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x020095d8
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009640
.L_02000958_3:
	ldr r0, [pc, #208]
	bl 0x02009550
	cmp r0, #0
	beq .L_02000958_4
	movs r1, #140
	movs r2, #148
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x020095d8
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009640
	movs r1, #144
	movs r2, #140
	movs r0, #16
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x020095d8
	movs r1, #224
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009640
	movs r1, #232
	movs r2, #152
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x020095d8
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009640
	movs r1, #240
	movs r2, #156
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x020095d8
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl 0x02009640
	movs r0, #10
	bl 0x020095a0
	adds r3, r0, #0
	movs r1, #0
	adds r3, #89
	adds r2, r0, #0
	strb r1, [r3]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r0, #80]
	ldrb r3, [r2, #9]
	movs r5, #12
	orrs r3, r5
	strb r3, [r2, #9]
	ldr r3, [r0, #80]
	adds r3, #38
	strb r1, [r3]
	movs r3, #192
	ldr r2, [r0, #80]
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r0, #11
	bl 0x020095a0
	ldr r6, [pc, #24]
	adds r3, r0, #0
	adds r3, #35
	strb r6, [r3]
	ldr r2, [r0, #80]
	ldrb r3, [r2, #9]
	orrs r3, r5
	strb r3, [r2, #9]
	ldr r2, [r0, #80]
	ldrb r3, [r2, #21]
	orrs r3, r5
	strb r3, [r2, #21]
	b .L_02000958_4
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x0000012f
	.4byte 0x000008aa
	.4byte 0x000008ab
.L_02000958_4:
	movs r0, #149
	lsls r0, r0, #4
	bl 0x02009550
	cmp r0, #0
	beq .L_02000958_5
	movs r3, #14
	movs r5, #18
	str r3, [sp, #0]
	movs r0, #18
	movs r1, #18
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009508
	movs r3, #15
	str r3, [sp, #0]
	movs r0, #18
	movs r1, #18
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x02009508
.L_02000958_5:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ad4
	.thumb_func
Func_02000ad4:
	push {lr}
	ldr r3, [pc, #24]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	ldr r0, [pc, #12]
	bl 0x02009560
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000012f
	.global Func_02000af8
	.thumb_func
Func_02000af8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #14
	sub sp, #8
	bl 0x020095a0
	ldr r0, [r0, #8]
	mov r8, r0
	mov r3, r8
	asrs r3, r3, #20
	movs r0, #15
	mov r8, r3
	bl 0x020095a0
	movs r3, #5
	ldr r5, [r0, #8]
	movs r6, #11
	str r3, [sp, #0]
	movs r0, #5
	movs r1, #12
	movs r2, #5
	movs r3, #1
	str r6, [sp, #4]
	bl 0x02009508
	asrs r5, r5, #20
	movs r0, #1
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009508
	mov r3, r8
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #1
	str r6, [sp, #4]
	bl 0x02009508
	movs r0, #14
	bl 0x02008b68
	movs r0, #15
	bl 0x02008b68
	sub sp, #-8
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02000b68
	.thumb_func
Func_02000b68:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl 0x020095a0
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x020095a0
	movs r1, #0
	bl 0x02009510
	adds r0, r5, #0
	movs r1, #3
	bl 0x02009648
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r6, #35
	ldrb r2, [r6]
	movs r3, #2
	orrs r3, r2
	strb r3, [r6]
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ba0
	.thumb_func
Func_02000ba0:
	push {lr}
	bl 0x02009580
	bl 0x02009688
	movs r1, #164
	movs r2, #178
	lsls r1, r1, #17
	lsls r2, r2, #19
	movs r0, #8
	bl 0x020095d8
	movs r0, #8
	bl 0x020095a0
	movs r3, #1
	adds r0, #91
	strb r3, [r0]
	bl 0x02009678
	bl 0x02009680
	movs r0, #20
	bl 0x02009578
	movs r1, #16
	movs r3, #128
	lsls r3, r3, #8
	negs r1, r1
	movs r2, #0
	movs r0, #1
	bl 0x02009698
	movs r0, #1
	bl 0x020095d0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #0
	bl 0x02009640
	movs r0, #20
	bl 0x02009578
	ldr r0, [pc, #556]
	bl 0x02009618
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009640
	movs r0, #1
	ldr r1, [pc, #540]
	ldr r2, [pc, #540]
	bl 0x020095a8
	movs r2, #178
	movs r0, #1
	movs r1, #232
	lsls r2, r2, #3
	bl 0x020095c8
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009640
	movs r0, #184
	movs r1, #1
	movs r2, #180
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #19
	lsls r0, r0, #16
	bl 0x02009660
	bl 0x02009668
	movs r0, #10
	bl 0x02009578
	movs r0, #1
	movs r1, #6
	movs r2, #15
	bl 0x020095f0
	movs r2, #40
	movs r0, #1
	movs r1, #6
	bl 0x020095f0
	movs r1, #0
	movs r0, #1
	bl 0x02009628
	movs r0, #20
	bl 0x02009578
	movs r0, #132
	movs r1, #1
	movs r2, #181
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #19
	lsls r0, r0, #17
	bl 0x02009660
	bl 0x02009668
	movs r0, #20
	bl 0x02009578
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #50
	bl 0x02009650
	movs r0, #8
	ldr r1, [pc, #412]
	ldr r2, [pc, #412]
	bl 0x020095a8
	movs r1, #132
	movs r2, #178
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #3
	bl 0x020095c8
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x02009640
	movs r0, #10
	bl 0x02009578
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x02009640
	movs r0, #20
	bl 0x02009578
	movs r0, #10
	bl 0x02009578
	movs r1, #4
	movs r0, #8
	bl 0x020095e8
	movs r0, #10
	bl 0x02009578
	movs r1, #0
	movs r0, #8
	bl 0x02009628
	movs r0, #20
	bl 0x02009578
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #1
	bl 0x02009650
	movs r0, #30
	bl 0x02009578
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x02009640
	movs r0, #50
	bl 0x02009578
	movs r1, #0
	movs r0, #1
	bl 0x02009628
	movs r0, #20
	bl 0x02009578
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #1
	bl 0x02009640
	movs r0, #30
	bl 0x02009578
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020095a8
	movs r1, #132
	movs r2, #183
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #3
	bl 0x020095c8
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009640
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009640
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl 0x02009640
	movs r0, #30
	bl 0x02009578
	movs r1, #0
	movs r0, #1
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #3
	movs r0, #0
	bl 0x020095e8
	movs r0, #30
	bl 0x02009578
	movs r1, #3
	movs r0, #1
	bl 0x020095e8
	movs r0, #30
	bl 0x02009578
	movs r0, #1
	ldr r1, [pc, #140]
	ldr r2, [pc, #144]
	bl 0x020095a8
	movs r0, #1
	movs r1, #2
	bl 0x020095e0
	movs r0, #0
	bl 0x020095a0
	cmp r0, #0
	beq .L_02000ba0_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x020095b8
.L_02000ba0_0:
	movs r0, #1
	bl 0x020095d0
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x020095d8
	movs r0, #20
	bl 0x02009578
	movs r0, #8
	bl 0x020095a0
	movs r3, #0
	adds r0, #91
	strb r3, [r0]
	movs r1, #2
	movs r0, #8
	bl 0x020095b0
	movs r0, #8
	bl 0x020095a0
	ldr r2, [r0, #8]
	cmp r2, #0
	bge .L_02000ba0_1
	ldr r3, [pc, #56]
	adds r2, r2, r3
.L_02000ba0_1:
	adds r3, r0, #0
	asrs r2, r2, #16
	adds r3, #100
	strh r2, [r3]
	ldr r2, [r0, #16]
	cmp r2, #0
	bge .L_02000ba0_2
	ldr r3, [pc, #36]
	adds r2, r2, r3
.L_02000ba0_2:
	adds r3, r0, #0
	asrs r2, r2, #16
	adds r3, #102
	strh r2, [r3]
	bl 0x02009588
	pop {r0}
	bx r0
	.4byte 0x00001f89
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x0000ffff
	.global Func_02000e40
	.thumb_func
Func_02000e40:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r0, [pc, #1016]
	bl 0x02009558
	bl 0x02009580
	bl 0x02009688
	ldr r0, [pc, #1008]
	bl 0x02009618
	movs r0, #11
	bl 0x020095a0
	movs r1, #0
	mov r10, r1
	adds r3, r0, #0
	mov r2, r10
	adds r3, #35
	strb r2, [r3]
	ldr r2, [r0, #80]
	movs r1, #12
	ldrb r3, [r2, #9]
	mov r8, r1
	mov r1, r8
	orrs r3, r1
	strb r3, [r2, #9]
	ldr r2, [r0, #80]
	ldrb r3, [r2, #21]
	orrs r3, r1
	strb r3, [r2, #21]
	movs r0, #232
	movs r1, #1
	movs r2, #152
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl 0x02009660
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020095a8
	movs r2, #136
	movs r0, #0
	movs r1, #216
	lsls r2, r2, #1
	bl 0x020095c8
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #0
	bl 0x02009640
	bl 0x02009668
	movs r0, #20
	bl 0x02009578
	movs r1, #129
	movs r2, #50
	movs r0, #13
	lsls r1, r1, #1
	bl 0x02009650
	movs r1, #0
	movs r0, #13
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r0, #10
	ldr r1, [pc, #872]
	movs r2, #50
	bl 0x02009650
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009640
	movs r0, #10
	movs r1, #4
	movs r2, #13
	bl 0x020095f0
	movs r2, #30
	movs r0, #10
	movs r1, #4
	bl 0x020095f0
	movs r1, #0
	movs r0, #10
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #2
	movs r0, #11
	bl 0x02009600
	movs r0, #20
	bl 0x02009578
	movs r1, #0
	movs r0, #11
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #4
	movs r0, #13
	bl 0x020095e8
	movs r0, #20
	bl 0x02009578
	movs r1, #0
	movs r0, #13
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r0, #10
	ldr r1, [pc, #764]
	movs r2, #55
	bl 0x02009650
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x020095a8
	movs r0, #10
	movs r1, #16
	movs r2, #0
	bl 0x020096a8
	movs r0, #10
	movs r1, #7
	movs r2, #0
	bl 0x020095f0
	movs r1, #24
	movs r2, #0
	movs r0, #10
	bl 0x020096a8
	movs r0, #10
	bl 0x020095a0
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #16
	strb r3, [r0]
	negs r1, r1
	movs r2, #0
	movs r0, #10
	bl 0x020096a0
	movs r0, #153
	bl 0x020096b0
	movs r0, #13
	ldr r1, [pc, #680]
	ldr r2, [pc, #680]
	bl 0x020095a8
	movs r2, #0
	movs r1, #16
	movs r0, #13
	bl 0x020096a8
	movs r0, #10
	bl 0x02009578
	movs r1, #1
	movs r0, #10
	bl 0x020095e0
	movs r0, #10
	bl 0x020095a0
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	movs r1, #129
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #13
	bl 0x02009658
	movs r1, #2
	movs r0, #13
	bl 0x020095f8
	movs r0, #155
	bl 0x020096b0
	movs r0, #10
	bl 0x02009578
	movs r0, #155
	bl 0x020096b0
	movs r0, #10
	bl 0x02009578
	movs r0, #155
	bl 0x020096b0
	movs r0, #10
	bl 0x02009578
	movs r0, #20
	bl 0x02009578
	movs r0, #13
	ldr r1, [pc, #576]
	ldr r2, [pc, #580]
	bl 0x020095a8
	movs r1, #6
	movs r2, #0
	movs r0, #13
	bl 0x020095f0
	movs r0, #159
	bl 0x020096b0
	movs r1, #8
	movs r2, #0
	negs r1, r1
	movs r0, #13
	bl 0x020096a8
	movs r0, #20
	bl 0x02009578
	movs r1, #0
	movs r0, #10
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #129
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #70
	bl 0x02009650
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020095a8
	movs r1, #8
	movs r0, #16
	negs r1, r1
	movs r2, #0
	bl 0x020096a8
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #16
	bl 0x02009640
	movs r0, #30
	bl 0x02009578
	movs r1, #4
	movs r0, #16
	bl 0x020095e8
	movs r0, #20
	bl 0x02009578
	movs r1, #0
	movs r0, #16
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x02009640
	movs r0, #35
	bl 0x02009578
	movs r1, #2
	movs r0, #10
	bl 0x02009600
	movs r0, #20
	bl 0x02009578
	movs r1, #0
	movs r0, #10
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #16
	bl 0x02009640
	movs r0, #55
	bl 0x02009578
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #16
	bl 0x02009640
	movs r0, #30
	bl 0x02009578
	movs r1, #0
	movs r0, #16
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl 0x02009640
	movs r0, #20
	bl 0x02009578
	movs r1, #129
	movs r2, #50
	movs r0, #11
	lsls r1, r1, #1
	bl 0x02009650
	movs r1, #0
	movs r0, #11
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #2
	movs r0, #13
	bl 0x02009600
	movs r0, #20
	bl 0x02009578
	movs r1, #0
	movs r0, #13
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #13
	bl 0x02009640
	movs r0, #60
	bl 0x02009578
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
.L_02001164:
	movs r0, #13
	bl 0x02009640
	movs r0, #30
	bl 0x02009578
	movs r1, #0
	movs r0, #13
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009640
	movs r2, #0
	movs r1, #0
	movs r0, #11
	bl 0x02009640
	movs r0, #20
	bl 0x02009578
	movs r1, #2
	movs r0, #10
	bl 0x02009600
	movs r0, #20
	bl 0x02009578
	movs r0, #10
	movs r1, #0
	bl 0x02009628
	movs r0, #10
	ldr r1, [pc, #164]
	ldr r2, [pc, #176]
	bl 0x020095a8
	movs r0, #10
	movs r1, #8
	movs r2, #0
	bl 0x020096a8
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x020095a8
	movs r1, #8
	movs r0, #16
	negs r1, r1
	movs r2, #16
	bl 0x020096a8
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #8
	bl 0x02009640
	movs r1, #0
	movs r0, #16
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
.L_020011fc:
	movs r0, #10
	bl 0x02009650
	movs r0, #10
	bl 0x020095a0
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	ldr r1, [pc, #84]
	movs r0, #10
	ldr r2, [pc, #68]
	bl 0x020095a8
	movs r1, #8
	movs r2, #0
	negs r1, r1
	movs r0, #10
	bl 0x020096a8
	movs r0, #10
	bl 0x020095a0
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #20
	bl 0x02009578
	movs r1, #2
	movs r0, #10
	b .L_020011fc_0
	.2byte 0x0000
	.2byte 0x08ab
	.2byte 0x0000
	.2byte 0x23eb
	.2byte 0x0000
	.2byte 0x0107
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0002
	.2byte 0x3333
	.2byte 0x0001
	.4byte 0x00006666
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0000
	.4byte 0x0000cccc
.L_020011fc_0:
	bl 0x02009600
	movs r0, #20
	bl 0x02009578
	movs r1, #0
	movs r0, #10
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #4
	movs r0, #16
	bl 0x020095e8
	movs r0, #20
	bl 0x02009578
	movs r1, #0
	movs r0, #16
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r0, #10
	bl 0x020095a0
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	ldr r1, [pc, #568]
	movs r0, #10
	ldr r2, [pc, #568]
	bl 0x020095a8
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #10
	bl 0x020096a8
	movs r0, #10
	bl 0x020095a0
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r2, #0
	movs r1, #0
	movs r0, #10
	bl 0x02009640
	movs r0, #20
	bl 0x02009578
	movs r1, #4
	movs r0, #10
	bl 0x020095e8
	movs r0, #20
	bl 0x02009578
	movs r0, #10
	ldr r1, [pc, #508]
	ldr r2, [pc, #512]
	bl 0x020095a8
	movs r0, #10
	movs r1, #8
	movs r2, #0
	bl 0x020096a8
	movs r0, #10
	movs r1, #6
	movs r2, #0
	bl 0x020095f0
	movs r1, #24
	movs r2, #0
	movs r0, #10
	bl 0x020096a8
	movs r0, #133
	bl 0x020096b0
	movs r2, #0
	movs r0, #16
	movs r1, #6
	bl 0x020095f0
	ldr r1, [pc, #464]
	movs r0, #16
	bl 0x020095b0
	movs r0, #10
	bl 0x020095a0
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r1, #6
	movs r0, #10
	movs r2, #0
	bl 0x020095f0
	movs r1, #12
	negs r1, r1
	movs r2, #4
	movs r0, #10
	bl 0x020096a8
	movs r0, #10
	bl 0x020095a0
	adds r1, r0, #0
	adds r3, r1, #0
	mov r2, r10
	adds r3, #89
	strb r2, [r3]
	adds r2, r1, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r1, #80]
	ldrb r3, [r2, #9]
	mov r0, r8
	orrs r3, r0
	strb r3, [r2, #9]
	ldr r3, [r1, #80]
	mov r2, r10
	adds r3, #38
	strb r2, [r3]
	movs r5, #192
	ldr r3, [r1, #80]
.L_02001384:
	lsls r5, r5, #8
	movs r1, #12
	strh r5, [r3, #30]
	movs r0, #10
	negs r1, r1
	movs r2, #4
	bl 0x020096a8
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl 0x02009640
	movs r0, #10
	bl 0x020095a0
	adds r0, #90
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r0, #159
	bl 0x020096b0
	movs r0, #20
	bl 0x02009578
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #50
	bl 0x02009650
	movs r1, #192
	adds r2, r5, #0
	movs r0, #11
	lsls r1, r1, #9
	bl 0x020095a8
	movs r0, #11
	movs r1, #24
	movs r2, #0
	bl 0x020096a8
	movs r2, #0
	adds r1, r5, #0
	movs r0, #11
	bl 0x02009640
	movs r0, #10
	bl 0x02009578
	movs r1, #0
	movs r0, #11
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #4
	movs r0, #16
	bl 0x020095e8
	movs r0, #20
	bl 0x02009578
	movs r1, #0
	movs r0, #16
	bl 0x02009628
	movs r0, #20
	bl 0x02009578
	movs r0, #16
	ldr r1, [pc, #208]
	ldr r2, [pc, #212]
	bl 0x020095a8
	movs r1, #8
	movs r2, #0
	negs r1, r1
	movs r0, #16
	bl 0x020096a8
	movs r0, #20
	bl 0x02009578
	movs r1, #0
	movs r0, #16
	bl 0x02009628
	movs r0, #10
	bl 0x02009578
	movs r1, #2
	movs r0, #10
	bl 0x02009600
	movs r0, #20
	bl 0x02009578
	movs r0, #10
	bl 0x02009578
	movs r2, #0
.L_02001456:
	movs r1, #0
	movs r0, #16
	bl 0x02009640
	movs r0, #40
	bl 0x02009578
	movs r1, #3
	movs r0, #13
	bl 0x020095e8
	movs r0, #10
	bl 0x02009578
	movs r1, #3
	movs r0, #13
	bl 0x020095e8
	movs r0, #20
	bl 0x02009578
	movs r0, #10
	bl 0x02009578
	movs r1, #128
	movs r2, #128
	movs r0, #16
	lsls r1, r1, #9
	lsls r2, r2, #8
.L_02001490:
	bl 0x020095a8
	movs r2, #24
	movs r0, #16
	movs r1, #24
	negs r2, r2
	bl 0x020096a8
	movs r0, #16
	movs r1, #8
	movs r2, #0
	bl 0x020096a8
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #16
	bl 0x02009640
	movs r0, #20
	bl 0x02009578
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020095a8
	movs r2, #8
	movs r1, #0
	negs r2, r2
	movs r0, #13
	bl 0x020096a8
	movs r0, #10
	bl 0x02009578
	bl 0x02009588
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0001
	.2byte 0xe666
	.2byte 0x0000
	.2byte 0x96e4
	.2byte 0x0200
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/IMPORT.INC"
	.section .rodata,"a",%progbits
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
