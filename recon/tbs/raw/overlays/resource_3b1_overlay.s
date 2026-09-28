.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	movs r1, #1
	adds r5, r0, #0
	bl 0x0200e450
	movs r3, #0
	str r3, [r5, #8]
	str r3, [r5, #12]
	str r3, [r5, #16]
	str r3, [r5, #36]
	str r3, [r5, #40]
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #60]
	str r3, [r5, #56]
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.global Func_02000058
	.thumb_func
Func_02000058:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #102
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_02000058_0
	bl 0x0200e430
	ldr r3, [r5, #12]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	ldr r2, [pc, #56]
	subs r3, r3, r0
	adds r3, r3, r2
	str r3, [r5, #12]
	cmp r3, #0
	bge .L_02000058_1
	movs r3, #0
	b .L_02000058_2
.L_02000058_0:
	bl 0x0200e430
	ldr r3, [r5, #12]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r0
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	str r3, [r5, #12]
	cmp r3, r2
	ble .L_02000058_1
	movs r3, #1
.L_02000058_2:
	strh r3, [r6]
.L_02000058_1:
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xffff8000
	.global Func_020000b0
	.thumb_func
Func_020000b0:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x0200e430
	lsls r0, r0, #6
	lsrs r0, r0, #16
	cmp r0, #6
	bne .L_020000b0_0
	movs r3, #192
	lsls r3, r3, #6
	b .L_020000b0_1
.L_020000b0_0:
	cmp r0, #9
	bne .L_020000b0_2
	movs r3, #160
	lsls r3, r3, #7
.L_020000b0_1:
	strh r3, [r5, #6]
.L_020000b0_2:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.global StagedActor_AdvanceCounter98
	.thumb_func
StagedActor_AdvanceCounter98:
	.global Func_020000d8
	.thumb_func
Func_020000d8:
	push {lr}
	adds r1, r0, #0
	adds r1, #98
	ldrb r3, [r1]
	movs r2, #160
	adds r3, #1
	strb r3, [r1]
	lsls r2, r2, #23
	lsls r3, r3, #24
	cmp r3, r2
	bls .L_020000d8_0
	adds r2, r0, #0
	adds r2, #102
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020000d8_0:
	pop {r0}
	bx r0
	.global StagedActor_CountdownUntilPositionUnset
	.thumb_func
StagedActor_CountdownUntilPositionUnset:
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {lr}
	ldr r3, [r0, #76]
	cmp r3, #0
	beq .L_020000fc_0
	subs r3, #1
	str r3, [r0, #76]
	b .L_020000fc_1
.L_020000fc_0:
	movs r0, #1
	b .L_020000fc_2
.L_020000fc_1:
	movs r2, #128
	ldr r3, [r0, #56]
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020000fc_3
	ldr r2, [r0, #60]
	cmp r2, r3
	bne .L_020000fc_3
	ldr r3, [r0, #64]
	movs r0, #1
	cmp r3, r2
	beq .L_020000fc_2
.L_020000fc_3:
	movs r0, #0
.L_020000fc_2:
	pop {r1}
	bx r1
	.section .text.x02008284,"ax",%progbits
	.align 2
	.global UpdateActorNineEffectMode
	.thumb_func
UpdateActorNineEffectMode:
	.global Func_02000284
	.thumb_func
Func_02000284:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #9
	bl 0x0200e4e8
	adds r3, r5, #0
	adds r3, #91
	ldrb r6, [r3]
	cmp r6, #0
	bne .L_02000284_0
	adds r7, r0, #0
	adds r7, #99
	ldrb r1, [r7]
	cmp r1, #1
	bne .L_02000284_1
	movs r2, #208
	lsls r2, r2, #8
	strh r2, [r5, #6]
	ldr r3, [pc, #40]
	adds r2, r5, #0
	adds r2, #98
	strb r1, [r2]
	b .L_02000284_2
.L_02000284_1:
	cmp r1, #2
	bne .L_02000284_3
	movs r2, #98
	adds r2, r2, r5
	ldrb r3, [r2]
	mov r8, r2
	cmp r3, #0
	beq .L_02000284_4
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200e450
.L_02000284_4:
	mov r3, r8
	strb r6, [r3]
	strb r6, [r7]
	b .L_02000284_0
	.2byte 0x0000
	.4byte 0x00000000
.L_02000284_3:
	cmp r1, #3
	bne .L_02000284_0
	ldr r3, [pc, #12]
	strh r6, [r5, #6]
.L_02000284_2:
	strb r3, [r7]
.L_02000284_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000000
	.global Func_020002f4
	.thumb_func
Func_020002f4:
	push {r5, r6, lr}
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #89
	movs r3, #8
	ldr r5, [r6, #80]
	movs r1, #0
	strb r3, [r2]
	bl 0x0200e480
	movs r2, #13
	ldrb r1, [r5, #9]
	negs r2, r2
	adds r3, r2, #0
	ands r3, r1
	movs r1, #4
	orrs r3, r1
	strb r3, [r5, #9]
	ldrb r3, [r5, #21]
	ands r2, r3
	orrs r2, r1
	adds r1, r6, #0
	adds r1, #35
	strb r2, [r5, #21]
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r6, #0
	movs r1, #15
	bl 0x0200e5a0
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.global Func_02000340
	.thumb_func
Func_02000340:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000340_0
	ldr r0, [pc, #16]
	b .L_02000340_1
.L_02000340_0:
	ldr r0, [pc, #16]
.L_02000340_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000006f
	.4byte 0x0200e984
	.4byte 0x0200e96c
	.global Func_02000370
	.thumb_func
Func_02000370:
	movs r0, #0
	bx lr
	.global Func_02000374
	.thumb_func
Func_02000374:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200eb94
	.section .text.x0200854c,"ax",%progbits
	.align 2
	.global Func_0200054c
	.thumb_func
Func_0200054c:
	push {lr}
	ldr r3, [pc, #228]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #22
	bhi .L_0200054c_0
	ldr r2, [pc, #212]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	strh r4, [r0, #46]
	lsls r0, r0, #8
	strh r4, [r0, #46]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r0, [r6, #46]
	lsls r0, r0, #8
	strh r2, [r0, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r4, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r4, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r4, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r0, [r5, #48]
	lsls r0, r0, #8
	strh r4, [r5, #48]
	lsls r0, r0, #8
	strh r0, [r6, #46]
	lsls r0, r0, #8
	movs r0, #138
	lsls r0, r0, #4
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200054c_1
	ldr r0, [pc, #104]
	b .L_0200054c_2
.L_0200054c_1:
	ldr r0, [pc, #104]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200054c_3
	ldr r0, [pc, #100]
	bl 0x0200e4a0
	cmp r0, #0
	bne .L_0200054c_3
	ldr r0, [pc, #92]
	b .L_0200054c_2
.L_0200054c_3:
	ldr r0, [pc, #92]
	b .L_0200054c_2
	.2byte 0x4814
	.2byte 0xf005
	.2byte 0xff55
	.2byte 0x2800
	.2byte 0xd001
	.2byte 0x4815
	.2byte 0xe017
	.2byte 0x4815
	.2byte 0xe015
	.2byte 0x208a
	.2byte 0x0100
	.2byte 0xf005
	.2byte 0xff4b
	.2byte 0x2800
	.2byte 0xd001
	.2byte 0x4812
	.2byte 0xe00d
	.2byte 0x480c
	.2byte 0xf005
	.2byte 0xff44
	.2byte 0x2800
	.2byte 0xd001
	.2byte 0x480f
	.2byte 0xe006
	.2byte 0x480f
	.2byte 0xe004
	.2byte 0x480f
	.2byte 0xe002
	.2byte 0x480f
	.2byte 0xe000
.L_0200054c_0:
	ldr r0, [pc, #60]
.L_0200054c_2:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008568
	.4byte 0x0200f6fc
	.4byte 0x00000928
	.4byte 0x0000093e
	.4byte 0x0200f570
	.4byte 0x0200f444
	.2byte 0xfedc
	.2byte 0x0200
	.2byte 0xf9c0
	.2byte 0x0200
	.2byte 0xf930
	.2byte 0x0200
	.2byte 0xf984
	.2byte 0x0200
	.2byte 0xf81c
	.2byte 0x0200
	.2byte 0xfb58
	.2byte 0x0200
	.2byte 0xfd44
	.2byte 0x0200
	.4byte 0x0200f420
	.global Func_02000670
	.thumb_func
Func_02000670:
	push {lr}
	bl 0x0200e4c8
	bl 0x0200e648
	ldr r0, [pc, #120]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000670_0
	ldr r0, [pc, #112]
	bl 0x0200e5a8
	movs r0, #10
	movs r1, #0
	bl 0x0200e5b8
	b .L_02000670_1
.L_02000670_0:
	ldr r0, [pc, #100]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000670_2
	ldr r0, [pc, #96]
	bl 0x0200e5a8
	movs r1, #0
	movs r0, #10
	bl 0x0200e5b0
	movs r0, #0
	movs r1, #0
	bl 0x0200e4e0
	cmp r0, #0
	bne .L_02000670_3
	bl 0x02009f3c
	b .L_02000670_1
.L_02000670_3:
	movs r0, #10
	movs r1, #2
	bl 0x0200e578
	movs r0, #10
	movs r1, #0
	bl 0x0200e5b8
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	b .L_02000670_1
.L_02000670_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
	movs r0, #10
	movs r1, #0
	bl 0x0200e5b8
.L_02000670_1:
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000921
	.4byte 0x00001dd4
	.4byte 0x00000922
	.4byte 0x00001d91
	.4byte 0x00001d31
	.global Func_02000708
	.thumb_func
Func_02000708:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #20]
	bl 0x0200e5a8
	movs r1, #0
	movs r0, #12
	bl 0x0200e5c8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x00001dd1
	.global Func_02000728
	.thumb_func
Func_02000728:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #164]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000728_0
	ldr r0, [pc, #156]
	bl 0x0200e5a8
	movs r0, #8
	bl 0x0200c86c
	movs r1, #208
	movs r2, #60
	movs r0, #8
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r1, #4
	movs r0, #8
	bl 0x0200e560
	movs r0, #8
	bl 0x0200c86c
	movs r0, #8
	movs r1, #3
	bl 0x0200e560
	b .L_02000728_1
.L_02000728_0:
	ldr r0, [pc, #112]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000728_2
	ldr r0, [pc, #108]
	bl 0x0200e5a8
	movs r0, #8
	movs r1, #0
	bl 0x0200e5b8
	b .L_02000728_1
.L_02000728_2:
	ldr r0, [pc, #96]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000728_3
	ldr r0, [pc, #88]
	bl 0x0200e5a8
	movs r0, #8
	movs r1, #0
	bl 0x0200e5b8
	ldr r0, [pc, #64]
	bl 0x0200e4a0
	cmp r0, #0
	bne .L_02000728_1
	ldr r0, [pc, #68]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000728_1
	ldr r3, [pc, #64]
	movs r1, #185
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	b .L_02000728_1
.L_02000728_3:
	ldr r0, [pc, #52]
	bl 0x0200e5a8
	movs r0, #8
	movs r1, #0
	bl 0x0200e5b8
.L_02000728_1:
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x00000928
	.4byte 0x00001eb2
	.4byte 0x00000925
	.4byte 0x00001e06
	.4byte 0x00000921
	.4byte 0x00001dcd
	.4byte 0x00000924
	.4byte 0x03001ebc
	.4byte 0x00001d30
	.global Func_020007f8
	.thumb_func
Func_020007f8:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #152]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020007f8_0
	movs r1, #2
	movs r0, #8
	bl 0x0200e578
	ldr r0, [pc, #136]
	bl 0x0200e5a8
	movs r0, #8
	bl 0x0200c86c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x0200e588
	movs r1, #0
	movs r0, #8
	bl 0x0200e5b0
	movs r0, #0
	movs r1, #0
	bl 0x0200e4e0
	cmp r0, #0
	bne .L_020007f8_1
	movs r0, #40
	bl 0x0200e4c0
	movs r0, #8
	bl 0x0200c86c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #8
	bl 0x0200c880
	movs r0, #8
	movs r1, #0
	bl 0x0200e5b8
	b .L_020007f8_2
.L_020007f8_1:
	ldr r3, [pc, #68]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	movs r0, #8
	movs r1, #0
	bl 0x0200e5b8
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e5d0
	b .L_020007f8_2
.L_020007f8_0:
	ldr r0, [pc, #32]
	bl 0x0200e5a8
	movs r0, #8
	movs r1, #0
	bl 0x0200e5b8
.L_020007f8_2:
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000925
	.4byte 0x00001e13
	.4byte 0x03001ebc
	.4byte 0x00001d4e
	.global Func_020008a8
	.thumb_func
Func_020008a8:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #48]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020008a8_0
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	movs r0, #10
	movs r1, #0
	bl 0x0200e5c8
	b .L_020008a8_1
.L_020008a8_0:
	ldr r0, [pc, #28]
	bl 0x0200e5a8
	movs r0, #10
	movs r1, #0
	bl 0x0200e5b8
.L_020008a8_1:
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000925
	.4byte 0x00001e19
	.4byte 0x00001d50
	.global Func_020008ec
	.thumb_func
Func_020008ec:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020008ec_0
	bl 0x020092dc
	adds r5, r0, #0
	bl 0x0200e4c8
	adds r0, r5, #0
	bl 0x02009190
	ldr r0, [pc, #136]
	bl 0x0200e5a8
	movs r0, #8
	bl 0x0200c86c
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_020008ec_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r5, #0
	bl 0x0200e518
.L_020008ec_1:
	adds r0, r5, #0
	bl 0x0200e540
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	bl 0x0200e4d0
	b .L_020008ec_2
.L_020008ec_0:
	ldr r0, [pc, #72]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020008ec_3
	movs r2, #153
	ldr r1, [pc, #64]
	lsls r2, r2, #4
	b .L_020008ec_4
.L_020008ec_3:
	ldr r0, [pc, #64]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020008ec_5
	ldr r1, [pc, #48]
	ldr r2, [pc, #56]
	b .L_020008ec_4
.L_020008ec_5:
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020008ec_6
	ldr r1, [pc, #32]
	ldr r2, [pc, #48]
.L_020008ec_4:
	movs r0, #8
	bl 0x02009804
	b .L_020008ec_2
.L_020008ec_6:
	ldr r1, [pc, #20]
	ldr r2, [pc, #40]
	movs r0, #8
	bl 0x02009804
.L_020008ec_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00001e9e
	.4byte 0x0000092b
	.4byte 0x00001e78
	.4byte 0x0000092a
	.4byte 0x00000917
	.4byte 0x00000929
	.4byte 0x00000935
	.4byte 0x0000092c
	.global Func_020009b4
	.thumb_func
Func_020009b4:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020009b4_0
	bl 0x020092dc
	adds r5, r0, #0
	bl 0x0200e4c8
	adds r0, r5, #0
	bl 0x02009190
	ldr r0, [pc, #136]
	bl 0x0200e5a8
	movs r0, #10
	bl 0x0200c86c
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_020009b4_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r5, #0
	bl 0x0200e518
.L_020009b4_1:
	adds r0, r5, #0
	bl 0x0200e540
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	bl 0x0200e4d0
	b .L_020009b4_2
.L_020009b4_0:
	ldr r0, [pc, #72]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020009b4_3
	ldr r1, [pc, #68]
	ldr r2, [pc, #68]
	b .L_020009b4_4
.L_020009b4_3:
	ldr r0, [pc, #68]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020009b4_5
	ldr r1, [pc, #52]
	ldr r2, [pc, #60]
	b .L_020009b4_4
.L_020009b4_5:
	ldr r0, [pc, #60]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020009b4_6
	ldr r1, [pc, #36]
	ldr r2, [pc, #52]
.L_020009b4_4:
	movs r0, #10
	bl 0x02009804
	b .L_020009b4_2
.L_020009b4_6:
	ldr r1, [pc, #24]
	ldr r2, [pc, #44]
	movs r0, #10
	bl 0x02009804
.L_020009b4_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001e9f
	.4byte 0x0000092b
	.4byte 0x00001e7b
	.4byte 0x00000992
	.4byte 0x0000092a
	.4byte 0x00000919
	.4byte 0x00000929
	.4byte 0x00000937
	.4byte 0x0000092e
	.global Func_02000a80
	.thumb_func
Func_02000a80:
	push {r5, lr}
	movs r0, #138
	lsls r0, r0, #4
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000a80_0
	bl 0x0200e4c8
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl 0x0200e5f0
	movs r0, #40
	bl 0x0200e4c0
	ldr r0, [pc, #184]
	bl 0x0200e5a8
	movs r0, #11
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	b .L_02000a80_1
.L_02000a80_0:
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000a80_2
	bl 0x020092dc
	adds r5, r0, #0
	bl 0x0200e4c8
	adds r0, r5, #0
	bl 0x02009190
	ldr r0, [pc, #140]
	bl 0x0200e5a8
	movs r0, #11
	bl 0x0200c86c
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02000a80_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r5, #0
	bl 0x0200e518
.L_02000a80_3:
	adds r0, r5, #0
	bl 0x0200e540
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	bl 0x0200e4d0
	b .L_02000a80_1
.L_02000a80_2:
	ldr r0, [pc, #76]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000a80_4
	ldr r1, [pc, #72]
	ldr r2, [pc, #72]
	b .L_02000a80_5
.L_02000a80_4:
	ldr r0, [pc, #72]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000a80_6
	ldr r1, [pc, #56]
	ldr r2, [pc, #64]
	b .L_02000a80_5
.L_02000a80_6:
	ldr r0, [pc, #64]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000a80_7
	ldr r1, [pc, #40]
	ldr r2, [pc, #56]
.L_02000a80_5:
	movs r0, #11
	bl 0x02009804
	b .L_02000a80_1
.L_02000a80_7:
	ldr r1, [pc, #28]
	ldr r2, [pc, #48]
	movs r0, #11
	bl 0x02009804
.L_02000a80_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001f47
	.4byte 0x00001ea0
	.4byte 0x0000092b
	.4byte 0x00001e7e
	.4byte 0x00000993
	.4byte 0x0000092a
	.4byte 0x0000091a
	.4byte 0x00000929
	.4byte 0x00000938
	.4byte 0x0000092f
	.global Func_02000b84
	.thumb_func
Func_02000b84:
	push {r5, lr}
	bl 0x0200e4c8
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000b84_0
	bl 0x020092dc
	adds r5, r0, #0
	bl 0x02009190
	ldr r0, [pc, #256]
	bl 0x0200e5a8
	movs r0, #12
	bl 0x0200c86c
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02000b84_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r5, #0
	bl 0x0200e518
.L_02000b84_1:
	adds r0, r5, #0
	bl 0x0200e540
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	b .L_02000b84_2
.L_02000b84_0:
	movs r1, #2
	movs r0, #12
	bl 0x0200e580
	movs r0, #20
	bl 0x0200e4c0
	ldr r0, [pc, #184]
	bl 0x0200e5a8
	movs r1, #0
	movs r0, #12
	bl 0x0200e5b0
	movs r0, #0
	movs r1, #0
	bl 0x0200e4e0
	cmp r0, #0
	bne .L_02000b84_3
	movs r0, #12
	bl 0x0200c86c
	movs r0, #12
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02000b84_4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl 0x0200e518
.L_02000b84_4:
	movs r0, #12
	bl 0x0200e540
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a8
	ldr r0, [pc, #100]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000b84_5
	ldr r0, [pc, #96]
	bl 0x0200e4a8
	b .L_02000b84_2
.L_02000b84_5:
	ldr r0, [pc, #92]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000b84_6
	ldr r0, [pc, #84]
	bl 0x0200e4a8
	b .L_02000b84_2
.L_02000b84_6:
	ldr r0, [pc, #80]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000b84_7
	ldr r0, [pc, #76]
	bl 0x0200e4a8
	b .L_02000b84_2
.L_02000b84_7:
	movs r0, #147
	lsls r0, r0, #4
	bl 0x0200e4a8
	b .L_02000b84_2
.L_02000b84_3:
	ldr r3, [pc, #60]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #12
	bl 0x0200c86c
.L_02000b84_2:
	bl 0x0200e4d0
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00001ea1
	.4byte 0x00001e81
	.4byte 0x0000092b
	.4byte 0x00000994
	.4byte 0x0000092a
	.4byte 0x0000091b
	.4byte 0x00000929
	.4byte 0x00000939
	.4byte 0x03001ebc
	.global Func_02000cc8
	.thumb_func
Func_02000cc8:
	push {r5, lr}
	bl 0x0200e4c8
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000cc8_0
	bl 0x020092dc
	adds r5, r0, #0
	bl 0x02009190
	ldr r0, [pc, #260]
	bl 0x0200e5a8
	movs r0, #9
	bl 0x0200c86c
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02000cc8_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r5, #0
	bl 0x0200e518
.L_02000cc8_1:
	adds r0, r5, #0
	bl 0x0200e540
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	b .L_02000cc8_2
.L_02000cc8_0:
	ldr r0, [pc, #204]
	bl 0x0200e5a8
	movs r2, #60
	movs r0, #9
	movs r1, #0
	bl 0x0200e5c0
	movs r0, #9
	movs r1, #1
	bl 0x0200e580
	movs r1, #0
	movs r0, #9
	bl 0x0200e5b0
	movs r0, #0
	movs r1, #0
	bl 0x0200e4e0
	cmp r0, #0
	bne .L_02000cc8_3
	movs r0, #9
	bl 0x0200c86c
	movs r0, #9
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02000cc8_4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl 0x0200e518
.L_02000cc8_4:
	movs r0, #9
	bl 0x0200e540
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a8
	ldr r0, [pc, #100]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000cc8_5
	ldr r0, [pc, #96]
	bl 0x0200e4a8
	b .L_02000cc8_2
.L_02000cc8_5:
	ldr r0, [pc, #92]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000cc8_6
	ldr r0, [pc, #84]
	bl 0x0200e4a8
	b .L_02000cc8_2
.L_02000cc8_6:
	ldr r0, [pc, #80]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000cc8_7
	ldr r0, [pc, #76]
	bl 0x0200e4a8
	b .L_02000cc8_2
.L_02000cc8_7:
	ldr r0, [pc, #72]
	bl 0x0200e4a8
	b .L_02000cc8_2
.L_02000cc8_3:
	ldr r3, [pc, #68]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #9
	bl 0x0200c86c
.L_02000cc8_2:
	bl 0x0200e4d0
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001ea2
	.4byte 0x00001e84
	.4byte 0x0000092b
	.4byte 0x00000991
	.4byte 0x0000092a
	.4byte 0x00000918
	.4byte 0x00000929
	.4byte 0x00000936
	.4byte 0x0000092d
	.4byte 0x03001ebc
	.global Func_02000e14
	.thumb_func
Func_02000e14:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000e14_0
	bl 0x020092dc
	adds r5, r0, #0
	bl 0x0200e4c8
	adds r0, r5, #0
	bl 0x02009190
	ldr r0, [pc, #136]
	bl 0x0200e5a8
	movs r0, #13
	bl 0x0200c86c
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02000e14_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r5, #0
	bl 0x0200e518
.L_02000e14_1:
	adds r0, r5, #0
	bl 0x0200e540
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	bl 0x0200e4d0
	b .L_02000e14_2
.L_02000e14_0:
	ldr r0, [pc, #72]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000e14_3
	ldr r1, [pc, #68]
	ldr r2, [pc, #68]
	b .L_02000e14_4
.L_02000e14_3:
	ldr r0, [pc, #68]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000e14_5
	ldr r1, [pc, #52]
	ldr r2, [pc, #60]
	b .L_02000e14_4
.L_02000e14_5:
	ldr r0, [pc, #60]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000e14_6
	ldr r1, [pc, #36]
	ldr r2, [pc, #52]
.L_02000e14_4:
	movs r0, #13
	bl 0x02009804
	b .L_02000e14_2
.L_02000e14_6:
	ldr r1, [pc, #24]
	ldr r2, [pc, #44]
	movs r0, #13
	bl 0x02009804
.L_02000e14_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001ea3
	.4byte 0x0000092b
	.4byte 0x00001e88
	.4byte 0x00000995
	.4byte 0x0000092a
	.4byte 0x0000091c
	.4byte 0x00000929
	.4byte 0x0000093a
	.4byte 0x00000931
	.global Func_02000ee0
	.thumb_func
Func_02000ee0:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000ee0_0
	bl 0x020092dc
	adds r5, r0, #0
	bl 0x0200e4c8
	adds r0, r5, #0
	bl 0x02009190
	ldr r0, [pc, #136]
	bl 0x0200e5a8
	movs r0, #14
	bl 0x0200c86c
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02000ee0_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r5, #0
	bl 0x0200e518
.L_02000ee0_1:
	adds r0, r5, #0
	bl 0x0200e540
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	bl 0x0200e4d0
	b .L_02000ee0_2
.L_02000ee0_0:
	ldr r0, [pc, #72]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000ee0_3
	ldr r1, [pc, #68]
	ldr r2, [pc, #68]
	b .L_02000ee0_4
.L_02000ee0_3:
	ldr r0, [pc, #68]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000ee0_5
	ldr r1, [pc, #52]
	ldr r2, [pc, #60]
	b .L_02000ee0_4
.L_02000ee0_5:
	ldr r0, [pc, #60]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000ee0_6
	ldr r1, [pc, #36]
	ldr r2, [pc, #52]
.L_02000ee0_4:
	movs r0, #14
	bl 0x02009804
	b .L_02000ee0_2
.L_02000ee0_6:
	ldr r1, [pc, #24]
	ldr r2, [pc, #44]
	movs r0, #14
	bl 0x02009804
.L_02000ee0_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001ea4
	.4byte 0x0000092b
	.4byte 0x00001e8b
	.4byte 0x00000996
	.4byte 0x0000092a
	.4byte 0x0000091d
	.4byte 0x00000929
	.4byte 0x0000093b
	.4byte 0x00000932
	.global Func_02000fac
	.thumb_func
Func_02000fac:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000fac_0
	bl 0x020092dc
	adds r5, r0, #0
	bl 0x0200e4c8
	adds r0, r5, #0
	bl 0x02009190
	ldr r0, [pc, #136]
	bl 0x0200e5a8
	movs r0, #15
	bl 0x0200c86c
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02000fac_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r5, #0
	bl 0x0200e518
.L_02000fac_1:
	adds r0, r5, #0
	bl 0x0200e540
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	bl 0x0200e4d0
	b .L_02000fac_2
.L_02000fac_0:
	ldr r0, [pc, #72]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000fac_3
	ldr r1, [pc, #68]
	ldr r2, [pc, #68]
	b .L_02000fac_4
.L_02000fac_3:
	ldr r0, [pc, #68]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000fac_5
	ldr r1, [pc, #52]
	ldr r2, [pc, #60]
	b .L_02000fac_4
.L_02000fac_5:
	ldr r0, [pc, #60]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02000fac_6
	ldr r1, [pc, #36]
	ldr r2, [pc, #52]
.L_02000fac_4:
	movs r0, #15
	bl 0x02009804
	b .L_02000fac_2
.L_02000fac_6:
	ldr r1, [pc, #24]
	ldr r2, [pc, #44]
	movs r0, #15
	bl 0x02009804
.L_02000fac_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001ea5
	.4byte 0x0000092b
	.4byte 0x00001e8e
	.4byte 0x00000997
	.4byte 0x0000092a
	.4byte 0x0000091e
	.4byte 0x00000929
	.4byte 0x0000093c
	.4byte 0x00000933
	.global Func_02001078
	.thumb_func
Func_02001078:
	push {r5, lr}
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001078_0
	bl 0x020092dc
	adds r5, r0, #0
	bl 0x0200e4c8
	adds r0, r5, #0
	bl 0x02009190
	ldr r0, [pc, #136]
	bl 0x0200e5a8
	movs r0, #16
	bl 0x0200c86c
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02001078_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r5, #0
	bl 0x0200e518
.L_02001078_1:
	adds r0, r5, #0
	bl 0x0200e540
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	bl 0x0200e4d0
	b .L_02001078_2
.L_02001078_0:
	ldr r0, [pc, #72]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001078_3
	ldr r1, [pc, #68]
	ldr r2, [pc, #68]
	b .L_02001078_4
.L_02001078_3:
	ldr r0, [pc, #68]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001078_5
	ldr r1, [pc, #52]
	ldr r2, [pc, #60]
	b .L_02001078_4
.L_02001078_5:
	ldr r0, [pc, #60]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001078_6
	ldr r1, [pc, #36]
	ldr r2, [pc, #52]
.L_02001078_4:
	movs r0, #16
	bl 0x02009804
	b .L_02001078_2
.L_02001078_6:
	ldr r1, [pc, #24]
	ldr r2, [pc, #44]
	movs r0, #16
	bl 0x02009804
.L_02001078_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001ea6
	.4byte 0x0000092b
	.4byte 0x00001e91
	.4byte 0x00000998
	.4byte 0x0000092a
	.4byte 0x0000091f
	.4byte 0x00000929
	.4byte 0x0000093d
	.4byte 0x00000934
	.global Func_02001144
	.thumb_func
Func_02001144:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #68]
	movs r2, #12
	movs r4, #12
	ldr r3, [r3]
	negs r2, r2
	negs r4, r4
	adds r4, r4, r1
	adds r2, r2, r0
	adds r6, r0, #0
	movs r5, #8
	mov lr, r2
	adds r6, #12
	mov r12, r4
	adds r1, #12
	adds r3, #52
.L_02001144_2:
	ldmia r3!, {r0}
	movs r7, #10
	ldrsh r2, [r0, r7]
	movs r7, #18
	ldrsh r4, [r0, r7]
	cmp lr, r2
	bge .L_02001144_0
	cmp r6, r2
	ble .L_02001144_0
	cmp r12, r4
	bge .L_02001144_0
	cmp r1, r4
	bgt .L_02001144_1
.L_02001144_0:
	adds r5, #1
	cmp r5, #65
	bls .L_02001144_2
	movs r0, #0
.L_02001144_1:
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.section .text.x02009280,"ax",%progbits
	.align 2
	.global SceneActor_CheckBucketOffsetPoint
	.thumb_func
SceneActor_CheckBucketOffsetPoint:
	.global Func_02001280
	.thumb_func
Func_02001280:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	movs r0, #0
	sub sp, #12
	bl 0x0200e4e8
	ldr r3, [pc, #72]
	lsls r5, r5, #2
	adds r6, r0, #0
	ldr r3, [r3, r5]
	movs r2, #10
	ldrsh r1, [r6, r2]
	asrs r2, r3, #16
	adds r5, r1, r2
	lsls r3, r3, #16
	movs r1, #18
	ldrsh r2, [r6, r1]
	asrs r3, r3, #16
	adds r7, r2, r3
	adds r0, r5, #0
	adds r1, r7, #0
	bl 0x02009144
	cmp r0, #0
	bne .L_02001280_0
	mov r1, sp
	lsls r3, r5, #16
	str r3, [r1]
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	lsls r3, r7, #16
	str r3, [r1, #8]
	adds r0, r6, #0
	bl 0x0200e478
	cmp r0, #0
	beq .L_02001280_1
.L_02001280_0:
	movs r0, #0
	b .L_02001280_2
.L_02001280_1:
	movs r0, #1
.L_02001280_2:
	sub sp, #-12
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x0200e668
	.global Func_020012dc
	.thumb_func
Func_020012dc:
	push {r5, lr}
	ldr r0, [pc, #56]
	movs r5, #0
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020012dc_0
	movs r5, #3
	b .L_020012dc_1
.L_020012dc_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020012dc_2
	movs r5, #2
	b .L_020012dc_1
.L_020012dc_2:
	ldr r0, [pc, #32]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020012dc_1
	movs r5, #1
.L_020012dc_1:
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200cfa8
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000092b
	.4byte 0x0000092a
	.4byte 0x00000929
	.global Func_02001324
	.thumb_func
Func_02001324:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001324_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_02001324_1
.L_02001324_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001324_2
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	b .L_02001324_1
.L_02001324_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
.L_02001324_1:
	movs r0, #18
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x0000092c
	.4byte 0x00001ece
	.4byte 0x00000935
	.4byte 0x00001ecf
	.4byte 0x00001ed0
	.global Func_02001378
	.thumb_func
Func_02001378:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001378_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_02001378_1
.L_02001378_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001378_2
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	b .L_02001378_1
.L_02001378_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
.L_02001378_1:
	movs r0, #19
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x0000092d
	.4byte 0x00001ece
	.4byte 0x00000936
	.4byte 0x00001ecf
	.4byte 0x00001ed0
	.global Func_020013cc
	.thumb_func
Func_020013cc:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020013cc_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_020013cc_1
.L_020013cc_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020013cc_2
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	b .L_020013cc_1
.L_020013cc_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
.L_020013cc_1:
	movs r0, #20
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x0000092e
	.4byte 0x00001ece
	.4byte 0x00000937
	.4byte 0x00001ecf
	.4byte 0x00001ed0
	.global Func_02001420
	.thumb_func
Func_02001420:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #40]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001420_0
	ldr r0, [pc, #32]
	bl 0x0200e5a8
	b .L_02001420_1
.L_02001420_0:
	ldr r0, [pc, #28]
	bl 0x0200e5a8
.L_02001420_1:
	movs r0, #21
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000092f
	.4byte 0x00001ed1
	.4byte 0x00001ed2
	.global Func_0200145c
	.thumb_func
Func_0200145c:
	push {lr}
	bl 0x0200e4c8
	movs r0, #147
	lsls r0, r0, #4
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200145c_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_0200145c_1
.L_0200145c_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200145c_2
	ldr r0, [pc, #36]
	bl 0x0200e5a8
	b .L_0200145c_1
.L_0200145c_2:
	ldr r0, [pc, #32]
	bl 0x0200e5a8
.L_0200145c_1:
	movs r0, #22
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001ece
	.4byte 0x00000939
	.4byte 0x00001ecf
	.4byte 0x00001ed0
	.global Func_020014b0
	.thumb_func
Func_020014b0:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020014b0_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_020014b0_1
.L_020014b0_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020014b0_2
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	b .L_020014b0_1
.L_020014b0_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
.L_020014b0_1:
	movs r0, #23
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x00000931
	.4byte 0x00001ece
	.4byte 0x0000093a
	.4byte 0x00001ecf
	.4byte 0x00001ed0
	.global Func_02001504
	.thumb_func
Func_02001504:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001504_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_02001504_1
.L_02001504_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001504_2
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	b .L_02001504_1
.L_02001504_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
.L_02001504_1:
	movs r0, #24
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x00000932
	.4byte 0x00001ece
	.4byte 0x0000093b
	.4byte 0x00001ecf
	.4byte 0x00001ed0
	.global Func_02001558
	.thumb_func
Func_02001558:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #40]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001558_0
	ldr r0, [pc, #32]
	bl 0x0200e5a8
	b .L_02001558_1
.L_02001558_0:
	ldr r0, [pc, #28]
	bl 0x0200e5a8
.L_02001558_1:
	movs r0, #25
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000933
	.4byte 0x00001ed1
	.4byte 0x00001ed2
	.global Func_02001594
	.thumb_func
Func_02001594:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001594_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_02001594_1
.L_02001594_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001594_2
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	b .L_02001594_1
.L_02001594_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
.L_02001594_1:
	movs r0, #18
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x0000092c
	.4byte 0x00001edb
	.4byte 0x00000935
	.4byte 0x00001edc
	.4byte 0x00001edd
	.global Func_020015e8
	.thumb_func
Func_020015e8:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020015e8_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_020015e8_1
.L_020015e8_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020015e8_2
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	b .L_020015e8_1
.L_020015e8_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
.L_020015e8_1:
	movs r0, #19
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x0000092d
	.4byte 0x00001edb
	.4byte 0x00000936
	.4byte 0x00001edc
	.4byte 0x00001edd
	.global Func_0200163c
	.thumb_func
Func_0200163c:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200163c_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_0200163c_1
.L_0200163c_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200163c_2
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	b .L_0200163c_1
.L_0200163c_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
.L_0200163c_1:
	movs r0, #20
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x0000092e
	.4byte 0x00001edb
	.4byte 0x00000937
	.4byte 0x00001edc
	.4byte 0x00001edd
	.global Func_02001690
	.thumb_func
Func_02001690:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #40]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001690_0
	ldr r0, [pc, #32]
	bl 0x0200e5a8
	b .L_02001690_1
.L_02001690_0:
	ldr r0, [pc, #28]
	bl 0x0200e5a8
.L_02001690_1:
	movs r0, #21
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000092f
	.4byte 0x00001ede
	.4byte 0x00001edf
	.global Func_020016cc
	.thumb_func
Func_020016cc:
	push {lr}
	bl 0x0200e4c8
	movs r0, #147
	lsls r0, r0, #4
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020016cc_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_020016cc_1
.L_020016cc_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020016cc_2
	ldr r0, [pc, #36]
	bl 0x0200e5a8
	b .L_020016cc_1
.L_020016cc_2:
	ldr r0, [pc, #32]
	bl 0x0200e5a8
.L_020016cc_1:
	movs r0, #22
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001edb
	.4byte 0x00000939
	.4byte 0x00001edc
	.4byte 0x00001edd
	.global Func_02001720
	.thumb_func
Func_02001720:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001720_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_02001720_1
.L_02001720_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001720_2
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	b .L_02001720_1
.L_02001720_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
.L_02001720_1:
	movs r0, #23
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x00000931
	.4byte 0x00001edb
	.4byte 0x0000093a
	.4byte 0x00001edc
	.4byte 0x00001edd
	.global Func_02001774
	.thumb_func
Func_02001774:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #56]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001774_0
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	b .L_02001774_1
.L_02001774_0:
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001774_2
	ldr r0, [pc, #40]
	bl 0x0200e5a8
	b .L_02001774_1
.L_02001774_2:
	ldr r0, [pc, #36]
	bl 0x0200e5a8
.L_02001774_1:
	movs r0, #24
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.4byte 0x00000932
	.4byte 0x00001edb
	.4byte 0x0000093b
	.4byte 0x00001edc
	.4byte 0x00001edd
	.global Func_020017c8
	.thumb_func
Func_020017c8:
	push {lr}
	bl 0x0200e4c8
	ldr r0, [pc, #40]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020017c8_0
	ldr r0, [pc, #32]
	bl 0x0200e5a8
	b .L_020017c8_1
.L_020017c8_0:
	ldr r0, [pc, #28]
	bl 0x0200e5a8
.L_020017c8_1:
	movs r0, #25
	movs r1, #0
	bl 0x0200e5b8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000933
	.4byte 0x00001ede
	.4byte 0x00001edf
	.global Func_02001804
	.thumb_func
Func_02001804:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r6, r0, #0
	adds r7, r2, #0
	bl 0x0200e4c8
	adds r0, r5, #0
	bl 0x0200e5a8
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200e5b0
	movs r0, #0
	movs r1, #0
	bl 0x0200e4e0
	cmp r0, #0
	bne .L_02001804_0
	adds r0, r6, #0
	bl 0x0200c86c
	adds r0, r6, #0
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02001804_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r0, r6, #0
	bl 0x0200e518
.L_02001804_1:
	adds r0, r6, #0
	bl 0x0200e540
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200e4a8
	adds r0, r7, #0
	bl 0x0200e4a8
	b .L_02001804_2
.L_02001804_0:
	ldr r3, [pc, #28]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	adds r0, r6, #0
	bl 0x0200c86c
.L_02001804_2:
	bl 0x0200e4d0
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.global Func_02001894
	.thumb_func
Func_02001894:
	push {lr}
	movs r0, #0
	bl 0x0200e4e8
	ldr r2, [pc, #180]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bls .L_02001894_0
	ldr r0, [pc, #168]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001894_1
	ldr r0, [pc, #164]
	bl 0x0200e4a0
	cmp r0, #0
	bne .L_02001894_1
	movs r0, #17
	bl 0x0200e658
	b .L_02001894_2
.L_02001894_1:
	movs r0, #15
	bl 0x0200e658
	b .L_02001894_2
.L_02001894_0:
	bl 0x0200e4c8
	ldr r0, [pc, #132]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001894_3
	ldr r0, [pc, #128]
	bl 0x0200e5a8
	b .L_02001894_4
.L_02001894_3:
	movs r0, #138
	lsls r0, r0, #4
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001894_5
	ldr r0, [pc, #112]
	bl 0x0200e5a8
	b .L_02001894_4
.L_02001894_5:
	ldr r0, [pc, #92]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001894_6
	ldr r0, [pc, #96]
	bl 0x0200e5a8
	b .L_02001894_4
.L_02001894_6:
	ldr r0, [pc, #92]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001894_7
	ldr r0, [pc, #88]
	bl 0x0200e5a8
	b .L_02001894_4
.L_02001894_7:
	ldr r0, [pc, #84]
	bl 0x0200e5a8
.L_02001894_4:
	ldr r0, [pc, #48]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001894_8
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	bne .L_02001894_8
	movs r0, #17
	movs r1, #0
	bl 0x0200e5b8
	b .L_02001894_9
.L_02001894_8:
	movs r0, #15
	movs r1, #0
	bl 0x0200e5b8
.L_02001894_9:
	bl 0x0200e4d0
.L_02001894_2:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffffe000
	.4byte 0x00000928
	.4byte 0x0000093e
	.4byte 0x00001f81
	.4byte 0x00001f48
	.4byte 0x00001f7f
	.4byte 0x00000925
	.4byte 0x00001f7d
	.4byte 0x00001f7b
	.section .text.x02009a08,"ax",%progbits
	.align 2
	.global Func_02001a08
	.thumb_func
Func_02001a08:
	push {lr}
	ldr r0, [pc, #68]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02001a08_0
	bl 0x0200e4c8
	movs r0, #8
	bl 0x0200e5d8
	ldr r0, [pc, #52]
	movs r1, #1
	movs r2, #8
	bl 0x0200e490
	movs r0, #0
	ldr r1, [pc, #44]
	ldr r2, [pc, #44]
	bl 0x0200e4f8
	movs r1, #204
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #134
	bl 0x0200e530
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl 0x0200c880
	bl 0x0200e4d0
.L_02001a08_0:
	pop {r0}
	bx r0
	.4byte 0x00000301
	.4byte 0x00001e48
	.4byte 0x00019999
	.4byte 0x0000cccc
	.global Func_02001a60
	.thumb_func
Func_02001a60:
	push {lr}
	ldr r0, [pc, #184]
	bl 0x0200e4a0
	cmp r0, #0
	beq 0x02009b16
	bl 0x0200e4c8
	bl 0x0200e648
	ldr r0, [pc, #168]
	ldr r1, [pc, #172]
	bl 0x0200e5f8
	movs r0, #224
	movs r1, #1
	ldr r3, [pc, #164]
	ldr r2, [pc, #168]
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200c8ac
	ldr r0, [pc, #160]
	bl 0x0200e5a8
	movs r0, #8
	bl 0x0200c86c
.L_02001a98:
	movs r0, #10
	bl 0x0200c86c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #8
	bl 0x0200c880
	movs r0, #8
	bl 0x0200c86c
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200c880
	movs r0, #10
	bl 0x0200c86c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #9
	bl 0x0200c880
	movs r0, #9
	bl 0x0200c86c
	movs r2, #20
	movs r1, #0
	movs r0, #8
	bl 0x0200e5d0
	movs r0, #8
	bl 0x0200c86c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #9
	bl 0x0200c880
	movs r0, #9
	bl 0x0200c86c
	movs r0, #10
	bl 0x0200c86c
	movs r0, #8
	bl 0x0200c86c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200c880
	movs r0, #8
	bl 0x0200c86c
	movs r0, #146
	lsls r0, r0, #4
	bl 0x0200e4a8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0922
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0x0028
	.2byte 0x1000
	.2byte 0x0000
	.2byte 0x027e
	.2byte 0x1d26
	.2byte 0x0000
	.global Func_02001b34
	.thumb_func
Func_02001b34:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	ldr r0, [pc, #952]
	bl 0x0200e4a0
	cmp r0, #0
	bne .L_02001b34_0
	b 0x02009eea
.L_02001b34_0:
	bl 0x0200e4c8
	bl 0x0200e648
	ldr r0, [pc, #936]
	ldr r1, [pc, #936]
	bl 0x0200e5f8
	movs r1, #1
.L_02001b60:
	movs r2, #232
	lsls r2, r2, #17
	ldr r3, [pc, #928]
	ldr r0, [pc, #932]
	negs r1, r1
	bl 0x0200c8ac
	movs r1, #1
	movs r0, #13
	bl 0x0200e580
	ldr r0, [pc, #920]
	bl 0x0200e5a8
	ldr r3, [pc, #916]
	mov r8, r3
	mov r0, r8
	bl 0x0200c86c
	movs r3, #208
	lsls r3, r3, #8
	mov r10, r3
	movs r0, #12
	mov r1, r10
	bl 0x0200c880
	movs r1, #129
	ldr r6, [pc, #896]
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r1, #2
	movs r0, #12
	bl 0x0200e578
	adds r0, r6, #0
	bl 0x0200c86c
	movs r0, #14
	movs r1, #1
	bl 0x0200e580
	movs r2, #20
	ldr r0, [pc, #864]
	movs r1, #0
	bl 0x0200e5c0
	movs r0, #12
	movs r1, #0
	bl 0x0200c880
	movs r0, #12
	ldr r1, [pc, #848]
	movs r2, #40
	ldr r5, [pc, #840]
	bl 0x0200e5e8
	movs r2, #40
.L_02001bd8:
	movs r0, #14
	ldr r1, [pc, #840]
	bl 0x0200e5e8
	movs r1, #3
	movs r0, #14
	bl 0x0200e578
	adds r0, r5, #0
	bl 0x0200c86c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200e5f0
	movs r0, #40
	bl 0x0200e4c0
	movs r1, #3
	movs r0, #12
	bl 0x0200e578
	adds r0, r6, #0
	bl 0x0200c86c
	movs r1, #1
	movs r0, #14
	bl 0x0200e580
	adds r0, r5, #0
	bl 0x0200c86c
	movs r3, #176
	lsls r3, r3, #8
	mov r9, r3
	mov r1, r9
	movs r0, #14
	bl 0x0200c880
	adds r0, r5, #0
	bl 0x0200c86c
	movs r0, #12
	mov r1, r10
	bl 0x0200c880
	movs r1, #128
	movs r2, #30
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r1, #1
	movs r0, #12
	bl 0x0200e578
	adds r0, r6, #0
	bl 0x0200c86c
	movs r1, #4
	movs r0, #13
	bl 0x0200e560
	mov r0, r8
	bl 0x0200c86c
	movs r1, #2
.L_02001c60:
	movs r0, #13
	bl 0x0200e578
	mov r0, r8
	bl 0x0200c86c
	movs r1, #4
	movs r0, #12
	bl 0x0200e550
	adds r0, r6, #0
	bl 0x0200c86c
	movs r1, #4
	movs r0, #14
	bl 0x0200e560
	adds r0, r5, #0
	bl 0x0200c86c
	movs r3, #128
	lsls r3, r3, #8
	mov r11, r3
	movs r0, #14
	mov r1, r11
	bl 0x0200c880
	movs r0, #14
	movs r1, #2
	bl 0x0200e578
	adds r0, r5, #0
.L_02001ca0:
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200e5e8
	adds r0, r6, #0
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r0, #14
	ldr r1, [pc, #600]
	movs r2, #0
	bl 0x0200e5e8
	movs r2, #60
	movs r0, #13
	ldr r1, [pc, #588]
	bl 0x0200e5e8
	movs r1, #2
	movs r0, #14
	bl 0x0200e578
	adds r0, r5, #0
	bl 0x0200c86c
	movs r0, #14
	mov r1, r9
	bl 0x0200c880
	movs r1, #1
	movs r0, #14
	bl 0x0200e580
	adds r0, r5, #0
	bl 0x0200c86c
	movs r3, #192
	lsls r3, r3, #6
	mov r8, r3
	movs r0, #13
	mov r1, r8
	bl 0x0200c880
	movs r0, #13
	ldr r1, [pc, #524]
	movs r2, #0
	bl 0x0200e5e8
	movs r2, #60
	movs r0, #12
	ldr r1, [pc, #512]
	bl 0x0200e5e8
	movs r1, #1
	movs r0, #13
	bl 0x0200e580
	movs r0, #13
	bl 0x0200c86c
	movs r2, #40
	movs r0, #14
	ldr r1, [pc, #492]
	bl 0x0200e5e8
	movs r1, #1
	movs r0, #14
	bl 0x0200e580
	adds r0, r5, #0
	bl 0x0200c86c
	movs r0, #12
	mov r1, r10
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200e5d0
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl 0x0200e5d0
	movs r0, #13
	mov r1, r8
	bl 0x0200c880
	movs r0, #12
	movs r1, #2
	bl 0x0200e580
	adds r0, r6, #0
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #7
	movs r0, #14
	bl 0x0200e5d0
	adds r0, r5, #0
	bl 0x0200c86c
	movs r0, #12
	movs r1, #2
	bl 0x0200e578
	movs r1, #2
	movs r0, #13
.L_02001da0:
	bl 0x0200e580
	movs r0, #60
	bl 0x0200e4c0
	movs r1, #1
	movs r0, #13
	bl 0x0200e580
	movs r0, #13
	bl 0x0200c86c
	movs r1, #3
	movs r0, #14
	bl 0x0200e560
	adds r0, r5, #0
	bl 0x0200c86c
	movs r1, #129
	movs r2, #40
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r1, #2
	movs r0, #12
	bl 0x0200e580
	adds r0, r6, #0
	bl 0x0200c86c
	movs r1, #3
	movs r0, #13
	bl 0x0200e560
	movs r0, #13
	bl 0x0200c86c
	movs r2, #40
	movs r0, #14
	mov r1, r9
	bl 0x0200e5d0
	movs r0, #14
	movs r1, #3
	bl 0x0200e550
	movs r0, #13
	movs r1, #3
	bl 0x0200e560
	movs r0, #14
	ldr r1, [pc, #284]
	ldr r2, [pc, #284]
	bl 0x0200e4f8
	ldr r2, [pc, #280]
	movs r0, #13
	ldr r1, [pc, #272]
	bl 0x0200e4f8
	ldr r5, [pc, #272]
	movs r0, #14
	adds r1, r5, #0
	bl 0x0200e500
	adds r1, r5, #0
	movs r0, #13
	bl 0x0200e500
	movs r0, #20
	bl 0x0200e4c0
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e5d0
	ldr r1, [pc, #188]
	ldr r2, [pc, #240]
	movs r0, #0
	bl 0x0200e4f8
	movs r0, #0
	bl 0x0200e4e8
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r2, #130
	strb r3, [r0]
	movs r1, #184
	lsls r2, r2, #2
	movs r0, #0
	bl 0x0200e530
	movs r0, #1
	bl 0x0200e4c0
	movs r0, #0
	bl 0x0200e4e8
.L_02001e72:
	adds r0, #90
.L_02001e74:
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	mov r1, r11
	movs r0, #0
	movs r2, #20
	bl 0x0200e5d0
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #12
	movs r1, #4
	movs r2, #20
	bl 0x0200e570
	movs r1, #160
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r1, #2
	movs r0, #12
	bl 0x0200e578
	movs r0, #12
	bl 0x0200c86c
	ldr r2, [pc, #116]
	movs r0, #12
	ldr r1, [pc, #108]
	bl 0x0200e4f8
	adds r1, r5, #0
	movs r0, #12
	bl 0x0200e500
	movs r0, #40
	bl 0x0200e4c0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #0
	bl 0x0200e5d0
	movs r0, #12
	bl 0x0200e508
	ldr r0, [pc, #84]
	bl 0x0200e4a8
	bl 0x0200e4d0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0911
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0002
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x0014
	.2byte 0x1000
	.2byte 0x0000
	.2byte 0x05b7
	.2byte 0x1d56
	.2byte 0x0000
	.2byte 0x200d
	.2byte 0x0000
	.2byte 0x800c
	.2byte 0x0000
	.2byte 0xa00e
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
	.4byte 0x00019999
	.4byte 0x0000cccc
	.2byte 0xe6a8
	.2byte 0x0200
	.2byte 0x3333
	.2byte 0x0001
	.4byte 0x00000922
	.global Func_02001f3c
	.thumb_func
Func_02001f3c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #28
	bl 0x0200e660
	ldr r0, [pc, #1016]
	ldr r1, [pc, #1020]
	bl 0x0200e5f8
	movs r0, #228
	movs r2, #162
	movs r1, #1
	ldr r3, [pc, #1012]
	lsls r2, r2, #18
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200c8ac
	movs r1, #1
	movs r0, #9
	bl 0x0200e580
	ldr r0, [pc, #996]
	bl 0x0200e5a8
	movs r0, #9
	bl 0x0200c86c
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200e5d0
.L_02001fb8:
	movs r2, #40
	movs r0, #9
	ldr r1, [pc, #924]
	bl 0x0200e5e8
	movs r1, #2
	movs r0, #9
	bl 0x0200e578
	movs r0, #9
	bl 0x0200c86c
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #208
	movs r2, #20
	movs r0, #13
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r1, #1
	movs r0, #11
	bl 0x0200e580
	ldr r3, [pc, #868]
	mov r9, r3
	mov r0, r9
	bl 0x0200c86c
	movs r1, #129
	movs r2, #20
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r1, #2
	movs r0, #13
	bl 0x0200e578
	movs r0, #13
	bl 0x0200c86c
	ldr r1, [pc, #836]
	movs r2, #60
	movs r0, #9
	bl 0x0200e5e8
	movs r0, #9
	bl 0x0200c86c
	movs r1, #130
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200e5e8
	ldr r0, [pc, #812]
	bl 0x0200c86c
	movs r0, #8
	movs r1, #1
	bl 0x0200e580
	movs r7, #192
	movs r1, #3
	movs r0, #8
	bl 0x0200e550
	lsls r7, r7, #6
	movs r0, #8
	bl 0x0200c86c
	adds r1, r7, #0
	movs r0, #12
	bl 0x0200c880
	ldr r0, [pc, #772]
	bl 0x0200c86c
	movs r3, #176
	lsls r3, r3, #8
	mov r11, r3
	mov r1, r11
	movs r0, #11
	bl 0x0200c880
	movs r1, #3
	movs r0, #11
	bl 0x0200e560
	movs r0, #10
	bl 0x0200e4c0
	movs r0, #13
	movs r1, #1
	bl 0x0200e580
.L_0200208c:
	movs r1, #3
	movs r0, #13
	bl 0x0200e560
	movs r0, #13
	bl 0x0200c86c
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #160
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #7
	bl 0x0200e5d0
	movs r3, #160
	lsls r3, r3, #7
	mov r10, r3
	mov r1, r10
	movs r0, #11
	bl 0x0200c880
	movs r0, #13
	ldr r1, [pc, #680]
	ldr r2, [pc, #680]
	bl 0x0200e4f8
	movs r0, #12
	ldr r1, [pc, #676]
	ldr r2, [pc, #668]
	bl 0x0200e4f8
	movs r1, #222
	movs r2, #167
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e528
	movs r1, #236
	movs r2, #167
	lsls r2, r2, #2
	lsls r1, r1, #1
	movs r0, #13
	bl 0x0200e530
	movs r0, #12
	bl 0x0200e540
	movs r1, #1
	movs r0, #12
	bl 0x0200e550
	movs r0, #80
.L_02002100:
	bl 0x0200e4c0
	movs r3, #208
	lsls r3, r3, #8
	mov r8, r3
	mov r1, r8
	movs r0, #12
	bl 0x0200c880
	movs r2, #60
	movs r0, #12
	ldr r1, [pc, #608]
	bl 0x0200e5e8
	movs r1, #1
	movs r0, #11
	bl 0x0200e580
	movs r0, #20
	bl 0x0200e4c0
	movs r2, #40
	ldr r0, [pc, #588]
	movs r1, #0
	bl 0x0200e5c0
	movs r0, #11
	movs r1, #2
	bl 0x0200e580
	mov r1, r8
	movs r2, #0
	movs r0, #11
	bl 0x0200e5d0
	mov r0, r9
	bl 0x0200c86c
	mov r1, r8
	movs r0, #12
	movs r2, #0
	bl 0x0200e5d0
	movs r2, #60
.L_02002158:
	movs r0, #9
	ldr r1, [pc, #540]
	bl 0x0200e5e8
	movs r1, #4
	movs r0, #11
	bl 0x0200e550
	movs r0, #20
	bl 0x0200e4c0
	mov r0, r9
	bl 0x0200c86c
	movs r1, #3
	movs r0, #9
	bl 0x0200e550
	movs r0, #9
	bl 0x0200c86c
	mov r1, r8
.L_02002184:
	movs r2, #0
	movs r0, #13
	bl 0x0200e5d0
	movs r1, #129
	movs r0, #13
.L_02002190:
	lsls r1, r1, #1
	bl 0x0200e5f0
	movs r2, #20
	movs r1, #2
	movs r0, #13
	bl 0x0200e570
	movs r0, #13
	bl 0x0200c86c
	movs r1, #3
	movs r0, #9
	bl 0x0200e560
	movs r0, #9
	ldr r6, [pc, #460]
	bl 0x0200c86c
	mov r1, r8
	movs r0, #11
	bl 0x0200c880
	movs r1, #1
	movs r0, #12
	bl 0x0200e580
	adds r0, r6, #0
	bl 0x0200c86c
.L_020021cc:
	movs r2, #40
	movs r0, #8
	ldr r1, [pc, #400]
	bl 0x0200e5e8
	movs r1, #3
	movs r0, #8
	bl 0x0200e550
	movs r0, #8
	bl 0x0200c86c
	movs r1, #129
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200e5e8
	movs r2, #0
	movs r1, #4
	movs r0, #13
	bl 0x0200e570
	movs r0, #13
	bl 0x0200c86c
	movs r1, #3
	movs r0, #9
	bl 0x0200e550
	movs r0, #9
	bl 0x0200c86c
	movs r1, #1
	movs r0, #11
	bl 0x0200e580
	mov r0, r9
	bl 0x0200c86c
	movs r1, #129
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
.L_02002224:
	bl 0x0200e5e8
	movs r0, #8
	movs r1, #0
	bl 0x0200e5b8
	movs r0, #11
	movs r1, #2
	bl 0x0200e578
	mov r0, r9
	movs r1, #0
	movs r2, #40
	bl 0x0200e5c0
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #0
.L_0200224a:
	bl 0x0200e5e8
	mov r1, r10
	movs r2, #20
	movs r0, #9
	bl 0x0200e5d0
	movs r0, #9
	movs r1, #2
	bl 0x0200e578
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x0200e5c0
	movs r1, #3
	movs r0, #11
	bl 0x0200e550
	movs r0, #20
	bl 0x0200e4c0
	movs r1, #128
	movs r2, #40
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r1, #2
	movs r0, #12
	bl 0x0200e578
	ldr r5, [pc, #236]
	adds r0, r6, #0
	bl 0x0200c86c
	movs r2, #20
	mov r1, r10
.L_02002298:
	movs r0, #11
	bl 0x0200e5d0
	adds r0, r5, #0
	bl 0x0200c86c
	movs r1, #2
	movs r0, #12
	bl 0x0200e578
	adds r0, r6, #0
	bl 0x0200c86c
	movs r0, #11
	movs r1, #3
	bl 0x0200e560
	movs r1, #1
	movs r0, #11
	bl 0x0200e580
	adds r0, r5, #0
	bl 0x0200c86c
	movs r1, #129
	movs r2, #60
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r1, #1
	movs r0, #9
	bl 0x0200e580
	movs r0, #9
	bl 0x0200c86c
	movs r0, #11
	ldr r1, [pc, #144]
	movs r2, #40
	bl 0x0200e5e8
	movs r2, #20
	mov r1, r8
	movs r0, #11
	bl 0x0200e5d0
	movs r1, #3
	movs r0, #9
	bl 0x0200e550
	movs r0, #9
	bl 0x0200c86c
	movs r2, #20
	movs r0, #11
	ldr r1, [pc, #80]
	bl 0x0200e5e8
	movs r1, #2
	movs r0, #11
	bl 0x0200e578
	mov r0, r9
	bl 0x0200c86c
	movs r1, #132
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200e5e8
	movs r0, #9
	bl 0x0200c86c
	movs r0, #8
	movs r1, #1
	bl 0x0200e580
	movs r1, #3
	movs r0, #8
	bl 0x0200e550
	movs r0, #8
	bl 0x0200c86c
	movs r2, #40
	mov r1, r8
	b .L_02002298_0
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0002
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x0014
	.2byte 0x1000
	.2byte 0x1d93
	.2byte 0x0000
	.4byte 0x00000103
	.2byte 0x100b
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x900c
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.4byte 0x00000101
	.2byte 0x400b
	.2byte 0x0000
	.2byte 0x100c
	.2byte 0x0000
.L_02002298_0:
	movs r0, #9
	bl 0x0200e5d0
	movs r0, #9
	bl 0x0200c86c
	movs r1, #1
	movs r0, #12
	bl 0x0200e580
	movs r0, #20
	bl 0x0200e4c0
	adds r0, r6, #0
	bl 0x0200c86c
	mov r1, r10
	movs r0, #11
	movs r2, #0
	bl 0x0200e5d0
	mov r1, r10
	movs r0, #9
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r2, #40
	mov r1, r11
	movs r0, #10
	bl 0x0200e5d0
	movs r1, #1
	movs r0, #11
	bl 0x0200e580
	adds r0, r5, #0
	bl 0x0200c86c
	movs r0, #12
	movs r1, #3
	bl 0x0200e550
.L_020023f0:
	movs r2, #20
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200e5c0
	movs r1, #2
	movs r0, #9
	bl 0x0200e580
	movs r0, #9
	bl 0x0200c86c
	movs r1, #132
	movs r2, #40
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r1, #3
	movs r0, #12
	bl 0x0200e550
	adds r0, r6, #0
	bl 0x0200c86c
	movs r1, #3
	movs r0, #8
	bl 0x0200e560
	movs r0, #8
	bl 0x0200c86c
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200e5d0
	movs r0, #19
	bl 0x0200e660
	movs r0, #8
	movs r1, #2
	bl 0x0200e578
	ldr r5, [pc, #836]
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #80
	movs r0, #8
.L_02002454:
	bl 0x0200e5e8
	adds r0, r5, #0
	bl 0x0200c86c
	movs r0, #12
	ldr r1, [pc, #816]
	movs r2, #0
	bl 0x0200e5e8
	movs r0, #11
	ldr r1, [pc, #808]
	movs r2, #0
	bl 0x0200e5e8
	movs r0, #13
.L_02002474:
	ldr r1, [pc, #796]
	movs r2, #0
	bl 0x0200e5e8
	movs r0, #10
	ldr r1, [pc, #788]
	movs r2, #0
	bl 0x0200e5e8
	movs r0, #0
	ldr r1, [pc, #776]
	movs r2, #40
	bl 0x0200e5e8
	mov r1, r8
	movs r0, #12
	movs r2, #0
	bl 0x0200e5d0
	mov r1, r8
	movs r0, #11
	movs r2, #0
	bl 0x0200e5d0
	mov r1, r11
	movs r0, #13
	movs r2, #0
	bl 0x0200e5d0
	mov r1, r11
	movs r0, #10
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #192
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200e5f0
	movs r2, #40
	movs r0, #8
	movs r1, #4
	bl 0x0200e570
	movs r0, #8
	movs r1, #2
	bl 0x0200e578
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200e5b8
	movs r0, #8
	ldr r1, [pc, #684]
	ldr r2, [pc, #684]
	bl 0x0200e4f8
	movs r0, #8
	ldr r1, [pc, #680]
	ldr r2, [pc, #684]
	bl 0x0200e530
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #231
	ldr r2, [pc, #652]
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200e530
	mov r1, r11
	movs r0, #9
	bl 0x0200c880
	movs r1, #128
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r1, #2
	movs r0, #9
	bl 0x0200e578
	ldr r0, [pc, #620]
	bl 0x0200c86c
	ldr r1, [pc, #588]
	movs r2, #60
	movs r0, #11
	bl 0x0200e5e8
	movs r0, #11
	bl 0x0200c86c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #12
	bl 0x0200e5e8
	adds r0, r6, #0
	bl 0x0200c86c
	movs r0, #8
	ldr r1, [pc, #580]
	movs r2, #20
	bl 0x0200e5e8
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl 0x0200e570
	movs r2, #20
	mov r1, r10
	movs r0, #8
	bl 0x0200e5d0
	movs r0, #8
	bl 0x0200c86c
	movs r0, #28
	bl 0x0200e660
	movs r1, #3
	movs r0, #8
	bl 0x0200e578
	movs r0, #8
	bl 0x0200c86c
	movs r2, #60
	ldr r1, [pc, #496]
	movs r0, #13
	bl 0x0200e5e8
	movs r0, #13
	bl 0x0200c86c
	adds r1, r7, #0
	movs r0, #8
	bl 0x0200c880
	movs r1, #4
	movs r0, #8
	bl 0x0200e560
	movs r0, #8
	bl 0x0200c86c
	movs r1, #222
	movs r2, #157
	lsls r2, r2, #2
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200e530
	mov r1, r8
	movs r0, #12
	bl 0x0200c880
	ldr r0, [pc, #472]
	bl 0x0200c86c
	movs r2, #20
	mov r1, r10
	movs r0, #8
	bl 0x0200e5d0
	movs r1, #3
	movs r0, #8
	bl 0x0200e560
	movs r0, #8
	bl 0x0200c86c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #11
	bl 0x0200e5e8
	mov r0, r9
	bl 0x0200c86c
	movs r2, #40
	movs r0, #13
	ldr r1, [pc, #424]
	bl 0x0200e5e8
	movs r1, #2
	movs r0, #13
	bl 0x0200e578
	movs r0, #13
	bl 0x0200c86c
	adds r1, r7, #0
	movs r0, #9
	bl 0x0200c880
	movs r1, #4
	movs r0, #9
	bl 0x0200e560
	ldr r0, [pc, #392]
	bl 0x0200c86c
	movs r0, #12
	movs r1, #0
	bl 0x0200c880
	movs r1, #1
	movs r0, #8
	bl 0x0200e580
	movs r0, #8
	bl 0x0200c86c
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #12
	ldr r1, [pc, #356]
	movs r2, #0
	bl 0x0200e5e8
	movs r2, #60
	movs r0, #9
	ldr r1, [pc, #344]
	bl 0x0200e5e8
	ldr r0, [pc, #340]
	ldr r1, [pc, #344]
	bl 0x0200e5f8
	movs r0, #232
	movs r2, #170
	movs r3, #128
	movs r1, #1
	lsls r3, r3, #21
	lsls r2, r2, #18
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200c8ac
	movs r0, #10
	movs r1, #1
	bl 0x0200e580
	movs r1, #0
	movs r0, #10
	bl 0x0200c880
	movs r0, #10
	bl 0x0200c86c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #129
.L_020026aa:
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #10
	bl 0x0200e5e8
	movs r0, #10
.L_020026b6:
	bl 0x0200c86c
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200e5d0
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200e5e8
	movs r2, #40
	movs r1, #4
	movs r0, #10
	bl 0x0200e570
	movs r0, #10
	bl 0x0200c86c
	movs r1, #1
	movs r0, #10
	bl 0x0200e580
	movs r0, #10
	bl 0x0200c86c
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #219
	movs r0, #13
	lsls r1, r1, #1
	ldr r2, [pc, #196]
	bl 0x0200e528
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	mov r1, r11
	movs r0, #9
	movs r2, #0
	bl 0x0200e5d0
	adds r1, r7, #0
	movs r0, #12
	movs r2, #0
	bl 0x0200e5d0
	mov r1, r11
	movs r2, #0
	movs r0, #11
	bl 0x0200e5d0
	movs r0, #17
	bl 0x0200e660
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #244
	movs r0, #10
	lsls r1, r1, #1
	ldr r2, [pc, #128]
	bl 0x0200e530
	mov r1, r11
	movs r2, #0
	movs r0, #10
	bl 0x0200e5d0
	movs r0, #13
	bl 0x0200e540
	movs r0, #13
	movs r1, #1
	bl 0x0200e550
	movs r0, #13
	mov r1, r8
	movs r2, #0
	bl 0x0200e5d0
.L_02002776:
	bl 0x0200e650
	ldr r0, [pc, #88]
	bl 0x0200e4a8
	pop {r3, r5, r6, r7}
.L_02002782:
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x8008
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x01db
	.2byte 0x0000
	.2byte 0x0256
	.2byte 0x0000
	.2byte 0x026a
	.2byte 0x0000
	.2byte 0x8009
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
	.2byte 0x900c
	.2byte 0x0000
	.2byte 0x0107
	.2byte 0x0000
	.2byte 0x1009
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0001
	.2byte 0x2666
	.2byte 0x0000
	.2byte 0x0293
	.2byte 0x0000
	.2byte 0x02ae
	.2byte 0x0000
	.2byte 0x0921
	.2byte 0x0000
	.global Func_020027d8
	.thumb_func
Func_020027d8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl 0x0200e4c8
	bl 0x0200e648
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200e588
	movs r1, #128
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200e5e8
	ldr r5, [pc, #200]
	movs r1, #3
	movs r0, #8
	bl 0x0200e578
	ldr r0, [pc, #192]
	bl 0x0200e5a8
	adds r0, r5, #0
	bl 0x0200c86c
	movs r0, #9
	movs r1, #1
	bl 0x0200e578
	movs r0, #12
	movs r1, #1
	bl 0x0200e578
	movs r0, #11
	movs r1, #1
	bl 0x0200e578
	movs r0, #13
	movs r1, #1
	bl 0x0200e578
	movs r0, #10
	movs r1, #1
	bl 0x0200e580
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
.L_02002842:
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #208
	movs r0, #12
	lsls r1, r1, #8
.L_0200284e:
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #208
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #176
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r0, #8
	movs r1, #1
	bl 0x0200e580
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200e5b0
	movs r0, #0
	movs r1, #0
	bl 0x0200e4e0
	cmp r0, #0
	bne .L_0200284e_0
	movs r1, #2
	movs r0, #9
	bl 0x0200e580
	ldr r0, [pc, #48]
	bl 0x0200c86c
	movs r1, #132
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200e5e8
	adds r0, r5, #0
	bl 0x0200c86c
	ldr r3, [pc, #28]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_0200284e_1
	.2byte 0x0000
	.2byte 0x1008
	.2byte 0x0000
	.2byte 0x1ddb
	.2byte 0x0000
	.4byte 0x00009009
	.4byte 0x03001ebc
.L_0200284e_0:
	ldr r3, [pc, #828]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	movs r1, #1
	movs r0, #9
	bl 0x0200e580
	ldr r0, [pc, #808]
	bl 0x0200c86c
	movs r0, #8
	movs r1, #2
	bl 0x0200e578
	ldr r0, [pc, #800]
	bl 0x0200c86c
.L_0200284e_1:
	movs r2, #40
	movs r0, #13
	ldr r1, [pc, #792]
	bl 0x0200e5e8
.L_0200290e:
	ldr r0, [pc, #792]
	ldr r1, [pc, #792]
	bl 0x0200e5f8
	movs r0, #236
	movs r1, #1
.L_0200291a:
	movs r2, #159
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200e600
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r6, #176
	movs r1, #236
	ldr r2, [pc, #756]
	lsls r6, r6, #8
	movs r0, #13
	lsls r1, r1, #1
	bl 0x0200e530
	adds r1, r6, #0
	movs r0, #13
	bl 0x0200c880
	movs r0, #13
	bl 0x0200c86c
	movs r3, #160
	lsls r3, r3, #7
	mov r10, r3
	movs r0, #8
	mov r1, r10
	bl 0x0200c880
	movs r0, #8
	movs r1, #3
	bl 0x0200e560
	movs r0, #9
	movs r1, #3
	bl 0x0200e560
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r2, #20
	movs r0, #13
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r0, #11
	movs r1, #3
	bl 0x0200e550
	movs r1, #3
	movs r0, #13
	bl 0x0200e560
	movs r0, #20
	bl 0x0200e4c0
	movs r0, #12
	movs r1, #1
	bl 0x0200e580
	movs r3, #192
	lsls r3, r3, #6
	mov r8, r3
	movs r0, #12
	mov r1, r8
	bl 0x0200c880
	ldr r0, [pc, #636]
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r0, #11
	adds r1, r6, #0
	movs r2, #20
	bl 0x0200e5d0
	movs r5, #208
	movs r2, #40
	ldr r1, [pc, #616]
	movs r0, #11
	bl 0x0200e5e8
	lsls r5, r5, #8
	movs r0, #11
	bl 0x0200c86c
	ldr r7, [pc, #604]
	movs r0, #12
	adds r1, r5, #0
	bl 0x0200c880
	movs r1, #4
	movs r0, #12
	bl 0x0200e550
	adds r0, r7, #0
	bl 0x0200c86c
	movs r0, #13
	adds r1, r6, #0
	bl 0x0200c880
	movs r1, #1
	movs r0, #13
	bl 0x0200e580
	movs r0, #13
	bl 0x0200c86c
	movs r1, #128
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r0, #9
	mov r1, r8
	bl 0x0200c880
	movs r1, #1
	movs r0, #9
	bl 0x0200e580
	movs r0, #9
	bl 0x0200c86c
	movs r1, #3
	movs r0, #12
	bl 0x0200e560
	adds r0, r7, #0
	bl 0x0200c86c
	movs r1, #2
	movs r0, #8
	bl 0x0200e580
	movs r0, #8
	bl 0x0200c86c
	movs r0, #12
	adds r1, r5, #0
	bl 0x0200c880
	movs r1, #3
	movs r0, #12
	bl 0x0200e560
	adds r0, r7, #0
	bl 0x0200c86c
	movs r0, #11
	movs r1, #2
	bl 0x0200e580
	adds r1, r6, #0
	movs r0, #11
	bl 0x0200c880
	movs r0, #11
	bl 0x0200c86c
	movs r1, #0
	movs r0, #12
	bl 0x0200c880
	adds r0, r7, #0
	bl 0x0200c86c
	movs r0, #8
	mov r1, r8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #11
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #13
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #243
	movs r2, #152
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e530
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_0200291a_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200e548
.L_0200291a_0:
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #243
	movs r2, #156
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e530
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #1
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_0200291a_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200e548
.L_0200291a_1:
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #243
	movs r2, #160
	movs r0, #2
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e530
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #2
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_0200291a_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200e548
.L_0200291a_2:
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #243
	movs r2, #164
	movs r0, #3
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e530
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200e5d0
	movs r1, #132
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200e5e8
	adds r0, r7, #0
	bl 0x0200c86c
	movs r1, #1
	movs r0, #9
	bl 0x0200e580
	ldr r0, [pc, #156]
	bl 0x0200c86c
	movs r0, #8
	movs r1, #3
	bl 0x0200e560
	mov r1, r10
	movs r0, #8
	bl 0x0200c880
	movs r0, #8
	bl 0x0200c86c
	movs r0, #8
	mov r1, r8
	bl 0x0200c880
	movs r1, #0
	movs r0, #8
	bl 0x0200e5b0
	movs r0, #0
	movs r1, #0
	bl 0x0200e4e0
	cmp r0, #1
	bne .L_0200291a_3
	movs r1, #2
	movs r0, #8
	bl 0x0200e578
	movs r0, #8
	bl 0x0200c86c
	movs r1, #3
	movs r0, #12
	bl 0x0200e560
	adds r0, r7, #0
	bl 0x0200c86c
	movs r0, #9
	movs r1, #1
	bl 0x0200e578
	movs r2, #40
	ldr r0, [pc, #28]
	movs r1, #0
	bl 0x0200e5c0
	ldr r3, [pc, #16]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200291a_4
	.4byte 0x03001ebc
	.4byte 0x00009009
	.2byte 0x9008
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x1999
	.2byte 0x0000
	.4byte 0x00000296
	.4byte 0x0000100c
	.4byte 0x00000101
	.4byte 0x0000900c
	.4byte 0x00001009
.L_0200291a_3:
	ldr r3, [pc, #876]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #3
	strh r3, [r2]
	movs r0, #8
	movs r1, #3
	bl 0x0200e578
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x0200e5c0
.L_0200291a_4:
	movs r1, #1
	movs r0, #13
	bl 0x0200e580
	movs r0, #13
	bl 0x0200c86c
	movs r0, #8
	movs r1, #1
	bl 0x0200e580
	movs r3, #160
	lsls r3, r3, #7
	mov r8, r3
	mov r1, r8
	movs r0, #8
	bl 0x0200c880
	movs r6, #176
	movs r0, #8
	lsls r6, r6, #8
	bl 0x0200c86c
	movs r0, #13
	movs r1, #1
	bl 0x0200e580
	movs r0, #13
	adds r1, r6, #0
	bl 0x0200c880
	movs r2, #20
	movs r0, #13
	movs r1, #0
	bl 0x0200e5c0
	movs r0, #8
	movs r1, #3
	bl 0x0200e560
	movs r0, #8
	ldr r1, [pc, #764]
	ldr r2, [pc, #768]
	bl 0x0200e4f8
	ldr r5, [pc, #764]
	movs r1, #236
	movs r2, #158
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #8
	bl 0x0200e530
	adds r0, r5, #0
	bl 0x0200c86c
	movs r2, #40
	movs r0, #13
	ldr r1, [pc, #744]
	bl 0x0200e5e8
	movs r1, #2
	movs r0, #13
	bl 0x0200e578
	movs r0, #13
	bl 0x0200c86c
	movs r0, #8
	movs r1, #4
	bl 0x0200e550
	movs r2, #40
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200e5c0
	movs r0, #11
	movs r1, #1
	bl 0x0200e580
	adds r1, r6, #0
	movs r0, #11
	bl 0x0200c880
	ldr r0, [pc, #692]
	bl 0x0200c86c
	movs r1, #129
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200e5e8
	movs r0, #10
	ldr r1, [pc, #676]
	ldr r2, [pc, #680]
	bl 0x0200e4f8
	movs r0, #10
	movs r1, #2
	movs r2, #0
	bl 0x0200e570
	movs r1, #231
	ldr r2, [pc, #664]
	movs r0, #10
	lsls r1, r1, #1
	bl 0x0200e530
	movs r0, #10
	adds r1, r6, #0
	bl 0x0200c880
	movs r1, #2
	movs r0, #10
	bl 0x0200e578
	movs r0, #10
	bl 0x0200c86c
	movs r0, #9
	mov r1, r8
	bl 0x0200c880
	movs r1, #4
	movs r0, #9
	bl 0x0200e560
	movs r0, #9
	bl 0x0200c86c
	movs r1, #3
	movs r0, #8
	bl 0x0200e560
	adds r0, r5, #0
	bl 0x0200c86c
	movs r1, #129
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200e5e8
	movs r2, #40
	movs r0, #13
	movs r1, #0
	bl 0x0200e5c0
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #9
	bl 0x0200c880
	movs r1, #2
	movs r0, #9
	bl 0x0200e578
	ldr r0, [pc, #560]
	bl 0x0200c86c
	movs r0, #12
	movs r1, #0
	bl 0x0200c880
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #9
	mov r1, r8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #11
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #13
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r2, #20
	movs r0, #10
	adds r1, r6, #0
	bl 0x0200e5d0
	movs r0, #12
	movs r1, #1
	bl 0x0200e580
	ldr r0, [pc, #492]
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r2, #40
	movs r0, #8
	ldr r1, [pc, #480]
	bl 0x0200e5e8
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200c880
	movs r1, #0
	ldr r0, [pc, #468]
	bl 0x0200e5b0
	movs r0, #0
	movs r1, #0
	bl 0x0200e4e0
	cmp r0, #0
	bne .L_0200291a_5
	movs r0, #8
	movs r1, #3
	bl 0x0200e560
	ldr r0, [pc, #440]
	bl 0x0200c86c
	ldr r3, [pc, #388]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200291a_6
.L_0200291a_5:
	ldr r3, [pc, #368]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	ldr r0, [pc, #400]
	bl 0x0200c86c
.L_0200291a_6:
	movs r0, #0
	movs r1, #3
	bl 0x0200e560
	movs r1, #3
	movs r0, #8
	bl 0x0200e560
	ldr r0, [pc, #380]
	bl 0x0200c86c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl 0x0200c880
	ldr r0, [pc, #328]
	bl 0x0200c86c
	movs r2, #0
	movs r0, #2
	movs r1, #0
	bl 0x0200c8e8
	movs r0, #12
	movs r1, #3
	bl 0x0200e550
	movs r0, #11
	movs r1, #3
	bl 0x0200e550
	movs r0, #9
	movs r1, #3
	bl 0x0200e550
	movs r0, #10
	movs r1, #2
	bl 0x0200e578
	movs r1, #2
	movs r0, #13
	bl 0x0200e580
	movs r0, #20
	bl 0x0200e4c0
	ldr r5, [pc, #304]
	movs r0, #10
	adds r1, r5, #0
	bl 0x0200e500
	movs r0, #4
	bl 0x0200e4c0
	adds r1, r5, #0
	movs r0, #11
	bl 0x0200e500
	movs r0, #4
	bl 0x0200e4c0
	adds r1, r5, #0
	movs r0, #12
	bl 0x0200e500
	movs r0, #4
	bl 0x0200e4c0
	movs r0, #9
	adds r1, r5, #0
	bl 0x0200e500
	movs r0, #3
	movs r1, #2
	bl 0x0200e550
	movs r0, #2
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_0200291a_7
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200e518
.L_0200291a_7:
	movs r0, #3
	bl 0x0200e540
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #2
	movs r1, #2
	bl 0x0200e550
	movs r0, #1
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_0200291a_8
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200e518
.L_0200291a_8:
	movs r0, #2
	bl 0x0200e540
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #1
	movs r1, #2
	bl 0x0200e550
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_0200291a_9
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200e518
.L_0200291a_9:
	movs r0, #1
	bl 0x0200e540
	movs r2, #0
	movs r0, #1
	movs r1, #0
	bl 0x0200e548
	adds r1, r5, #0
	movs r0, #13
	bl 0x0200e500
	movs r1, #228
	movs r2, #162
	lsls r2, r2, #2
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200e530
	movs r1, #0
	movs r0, #8
	bl 0x0200c880
	movs r0, #232
	bl 0x0200e4b8
	ldr r0, [pc, #76]
	bl 0x0200e4a8
	bl 0x0200e4d0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00004008
	.4byte 0x00000103
	.4byte 0x0000100b
	.4byte 0x00026666
	.4byte 0x00013333
	.4byte 0x000002a2
	.4byte 0x00001009
	.4byte 0x0000100c
	.4byte 0x00000101
	.4byte 0x00001008
	.4byte 0x0200e6e4
	.4byte 0x00000925
	.section .text.x0200b51c,"ax",%progbits
	.align 2
	.global SceneState_RunFlagGatedSetupCascade
	.thumb_func
SceneState_RunFlagGatedSetupCascade:
	.global Func_0200351c
	.thumb_func
Func_0200351c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r0, [pc, #292]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200351c_0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #14
	b .L_0200351c_1
.L_0200351c_0:
	movs r0, #138
	lsls r0, r0, #4
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200351c_2
	movs r2, #222
	movs r3, #192
	lsls r2, r2, #1
	lsls r3, r3, #6
	movs r0, #8
	movs r1, #152
	bl 0x0200c890
	ldr r1, [pc, #204]
	movs r0, #8
	bl 0x0200e500
	movs r3, #240
	lsls r3, r3, #1
	mov r10, r3
	movs r3, #176
	lsls r3, r3, #8
	movs r5, #244
	mov r8, r3
	movs r0, #10
	movs r1, #184
	mov r2, r10
	lsls r5, r5, #1
	movs r6, #208
	bl 0x0200c890
	lsls r6, r6, #8
	movs r0, #12
	movs r1, #170
	adds r2, r5, #0
	mov r3, r8
	bl 0x0200c890
	movs r0, #13
	movs r1, #136
	adds r2, r5, #0
	adds r3, r6, #0
	bl 0x0200c890
	movs r0, #15
	movs r1, #120
	mov r2, r10
	adds r3, r6, #0
	bl 0x0200c890
	ldr r2, [pc, #136]
	movs r0, #14
	movs r1, #184
	mov r3, r8
	bl 0x0200c890
	movs r2, #146
	movs r3, #128
	movs r0, #11
	movs r1, #136
	lsls r2, r2, #2
	lsls r3, r3, #8
	bl 0x0200c890
	ldr r1, [pc, #112]
	movs r0, #11
	bl 0x0200e500
	b .L_0200351c_3
.L_0200351c_2:
	ldr r0, [pc, #104]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200351c_4
	bl 0x0200d004
	b .L_0200351c_3
.L_0200351c_4:
	ldr r0, [pc, #92]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200351c_5
	movs r0, #18
.L_0200351c_1:
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	b .L_0200351c_3
.L_0200351c_5:
	ldr r0, [pc, #76]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200351c_3
	ldr r0, [pc, #68]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200351c_3
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
.L_0200351c_3:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000093e
	.4byte 0x0200e958
	.4byte 0x0000020e
	.4byte 0x0200e840
	.4byte 0x00000928
	.4byte 0x00000925
	.4byte 0x00000911
	.4byte 0x00000922
	.global RunSceneSelectionChain
	.thumb_func
RunSceneSelectionChain:
	.global Func_0200366c
	.thumb_func
Func_0200366c:
	push {lr}
	movs r0, #1
	bl 0x0200e420
	bl 0x0200b7b4
	ldr r0, [pc, #284]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200366c_0
	movs r0, #4
	movs r1, #4
	movs r2, #0
	bl 0x0200c8e8
	movs r1, #206
	movs r3, #192
	lsls r1, r1, #1
	lsls r3, r3, #6
	movs r0, #8
	movs r2, #222
	bl 0x0200c890
	movs r1, #229
	movs r3, #128
	lsls r1, r1, #1
	lsls r3, r3, #8
	movs r0, #9
	movs r2, #161
	bl 0x0200c890
	b .L_0200366c_1
.L_0200366c_0:
	movs r0, #138
	lsls r0, r0, #4
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200366c_2
	movs r1, #236
	movs r2, #152
	lsls r2, r2, #16
	movs r0, #8
	lsls r1, r1, #17
	bl 0x0200e548
	movs r0, #9
	movs r1, #5
	bl 0x0200e550
	movs r0, #4
	movs r1, #4
	movs r2, #0
	bl 0x0200c8e8
	b .L_0200366c_1
.L_0200366c_2:
	ldr r0, [pc, #188]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200366c_3
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #4
	movs r1, #4
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #3
	bl 0x0200c254
	b .L_0200366c_1
.L_0200366c_3:
	ldr r0, [pc, #156]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200366c_4
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #4
	movs r1, #3
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #2
	bl 0x0200c254
	b .L_0200366c_1
.L_0200366c_4:
	ldr r0, [pc, #120]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200366c_5
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #4
	movs r1, #2
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #1
	bl 0x0200c254
	b .L_0200366c_1
.L_0200366c_5:
	ldr r0, [pc, #88]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200366c_6
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r0, #0
	bl 0x0200c254
	b .L_0200366c_1
.L_0200366c_6:
	movs r0, #9
	movs r1, #5
	bl 0x0200e550
	ldr r0, [pc, #44]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_0200366c_1
	ldr r0, [pc, #40]
	bl 0x0200e4a0
	cmp r0, #0
	bne .L_0200366c_1
	bl 0x0200b8ac
.L_0200366c_1:
	pop {r0}
	bx r0
	.4byte 0x0000093e
	.4byte 0x0000092b
	.4byte 0x0000092a
	.4byte 0x00000929
	.4byte 0x00000928
	.4byte 0x00000925
	.4byte 0x00000926
	.global SceneActor_SetFlagBit3ForActors28To35
	.thumb_func
SceneActor_SetFlagBit3ForActors28To35:
	.global Func_020037b4
	.thumb_func
Func_020037b4:
	push {r5, r6, r7, lr}
	movs r5, #28
	movs r6, #8
	movs r7, #0
.L_020037b4_0:
	adds r0, r5, #0
	bl 0x0200e4e8
	adds r0, #89
	ldrb r3, [r0]
	adds r5, #1
	orrs r3, r6
	strb r3, [r0]
	cmp r5, #35
	bls .L_020037b4_0
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .text.x0200b8ac,"ax",%progbits
	.align 2
	.global Func_020038ac
	.thumb_func
Func_020038ac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	bl 0x0200e4c8
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #24
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #220
	movs r1, #1
	movs r2, #168
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	ldr r3, [pc, #1008]
	bl 0x0200c8ac
	movs r2, #220
	lsls r2, r2, #1
	mov r8, r2
	movs r3, #160
	lsls r3, r3, #7
	movs r0, #27
	mov r1, r8
	movs r2, #164
	mov r9, r3
	bl 0x0200c890
	movs r2, #208
	lsls r2, r2, #8
	mov r10, r2
	movs r1, #214
	lsls r1, r1, #1
	movs r0, #8
	movs r2, #190
	mov r3, r10
	movs r7, #176
	bl 0x0200c890
	lsls r7, r7, #8
	movs r1, #226
	movs r2, #190
	adds r3, r7, #0
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200c890
	movs r0, #9
	movs r1, #1
	bl 0x0200e550
	movs r3, #128
	lsls r3, r3, #8
	movs r0, #0
	mov r1, r8
	movs r2, #134
	mov r11, r3
	bl 0x0200c890
	ldr r3, [pc, #920]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	movs r6, #128
	adds r3, r3, r2
	lsls r6, r6, #1
	str r6, [r3]
	bl 0x0200e630
	movs r0, #0
	ldr r1, [pc, #904]
	ldr r2, [pc, #904]
	bl 0x0200e4f8
	movs r1, #204
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #134
	bl 0x0200e530
	movs r1, #204
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #148
	bl 0x0200e530
	movs r1, #212
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #148
	bl 0x0200e530
	movs r1, #128
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #7
	bl 0x0200e5d0
	movs r1, #1
	movs r0, #27
	bl 0x0200e580
	ldr r0, [pc, #848]
	bl 0x0200e5a8
	movs r0, #27
	bl 0x0200c86c
	movs r1, #1
	movs r0, #8
	bl 0x0200e580
	movs r0, #8
	bl 0x0200c86c
	movs r1, #3
	movs r0, #27
	bl 0x0200e560
	movs r0, #27
	bl 0x0200c86c
	movs r0, #27
	mov r1, r10
	bl 0x0200c880
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_020038ac_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200e548
.L_020038ac_0:
	movs r0, #1
	ldr r1, [pc, #768]
	ldr r2, [pc, #772]
	bl 0x0200e4f8
	movs r0, #1
	mov r1, r8
	movs r2, #148
	bl 0x0200e530
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #1
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_020038ac_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200e548
.L_020038ac_1:
	movs r0, #2
	ldr r1, [pc, #716]
	ldr r2, [pc, #720]
	bl 0x0200e4f8
	movs r1, #228
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #148
	bl 0x0200e530
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #2
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_020038ac_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200e548
.L_020038ac_2:
	movs r0, #3
	ldr r1, [pc, #664]
	ldr r2, [pc, #664]
	bl 0x0200e4f8
	movs r1, #236
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #148
	bl 0x0200e530
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #20
	movs r5, #128
	bl 0x0200e5d0
	lsls r5, r5, #7
	movs r0, #0
	movs r1, #0
	movs r2, #60
	bl 0x0200c8e8
	adds r1, r5, #0
	movs r0, #1
	movs r2, #20
	bl 0x0200c8e8
	movs r0, #2
	movs r1, #1
	movs r2, #20
	bl 0x0200c8e8
	mov r1, r9
	movs r2, #20
	movs r0, #27
	bl 0x0200e5d0
	movs r0, #27
	bl 0x0200c86c
	movs r0, #9
	movs r1, #1
	bl 0x0200e578
	adds r1, r6, #0
	movs r2, #40
	movs r0, #9
	bl 0x0200e5e8
	movs r0, #9
	bl 0x0200c86c
	movs r0, #1
	movs r1, #3
	bl 0x0200e578
	movs r2, #60
	movs r0, #1
	ldr r1, [pc, #556]
	bl 0x0200e5e8
	movs r1, #3
	movs r0, #27
	bl 0x0200e560
	movs r0, #27
	bl 0x0200c86c
	movs r0, #10
	movs r1, #1
	bl 0x0200e580
	movs r1, #3
	movs r0, #10
	bl 0x0200e550
	movs r0, #10
	bl 0x0200c86c
	movs r0, #8
	movs r1, #3
	bl 0x0200e550
	movs r0, #9
	movs r1, #3
	bl 0x0200e550
	movs r0, #11
	movs r1, #3
	bl 0x0200e550
	movs r0, #12
	movs r1, #3
	bl 0x0200e550
	movs r0, #13
	movs r1, #3
	bl 0x0200e560
	movs r0, #0
	movs r1, #0
	movs r2, #40
	bl 0x0200c8e8
	movs r0, #2
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r2, #20
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200c8e8
	movs r1, #4
	movs r0, #27
	bl 0x0200e560
	movs r0, #27
	bl 0x0200c86c
	movs r1, #129
	movs r2, #60
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r1, #1
	movs r0, #8
	bl 0x0200e578
	movs r0, #8
	bl 0x0200c86c
	movs r1, #3
	movs r0, #27
	bl 0x0200e560
	movs r0, #27
	bl 0x0200c86c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	mov r1, r11
	movs r0, #9
	movs r2, #40
	bl 0x0200e5d0
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200e5e8
	movs r1, #129
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r0, #27
	movs r1, #1
	bl 0x0200e580
	movs r0, #27
	movs r1, #3
	bl 0x0200e550
	movs r2, #20
	movs r0, #27
	movs r1, #0
	bl 0x0200e5c0
	movs r0, #8
	movs r1, #3
	bl 0x0200e550
	movs r1, #3
	movs r0, #9
	bl 0x0200e560
	movs r0, #40
	bl 0x0200e4c0
	movs r2, #20
	adds r1, r6, #0
	movs r0, #9
	bl 0x0200e5e8
	adds r1, r7, #0
	movs r0, #9
	bl 0x0200c880
	movs r0, #9
	bl 0x0200c86c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #27
	bl 0x0200c880
	movs r0, #27
	ldr r1, [pc, #268]
	movs r2, #60
	bl 0x0200e5e8
	movs r0, #27
	movs r1, #0
	movs r2, #60
	bl 0x0200e5c0
	movs r1, #131
	movs r2, #20
	movs r0, #27
	lsls r1, r1, #1
	bl 0x0200e5e8
	adds r1, r7, #0
	movs r0, #27
	bl 0x0200c880
	movs r1, #3
	movs r0, #27
	bl 0x0200e560
	movs r0, #27
	bl 0x0200c86c
	movs r2, #80
	movs r0, #3
	movs r1, #2
	bl 0x0200c8e8
	mov r1, r10
	movs r0, #8
	bl 0x0200c880
	movs r1, #2
	movs r0, #8
	bl 0x0200e578
	movs r0, #8
	bl 0x0200c86c
	movs r0, #9
	movs r1, #3
	bl 0x0200e560
	movs r1, #2
	movs r0, #9
	bl 0x0200e578
	movs r0, #9
	bl 0x0200c86c
	mov r1, r9
	movs r0, #27
	bl 0x0200c880
	movs r0, #27
	movs r1, #3
	bl 0x0200e560
	movs r1, #1
	movs r0, #27
	bl 0x0200e580
	movs r0, #27
	bl 0x0200c86c
	movs r0, #27
	ldr r1, [pc, #112]
	ldr r2, [pc, #116]
	bl 0x0200e4f8
	movs r1, #204
	movs r0, #27
	lsls r1, r1, #1
	movs r2, #158
	bl 0x0200e530
	movs r1, #204
	movs r0, #27
	lsls r1, r1, #1
	movs r2, #148
	bl 0x0200e530
	movs r2, #20
	movs r0, #27
	movs r1, #0
	bl 0x0200e5d0
	movs r1, #1
	movs r0, #27
	bl 0x0200e580
	movs r0, #27
	bl 0x0200c86c
	mov r1, r11
	movs r0, #1
	movs r2, #20
	bl 0x0200c8e8
	movs r0, #2
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r1, #204
	movs r0, #27
	lsls r1, r1, #1
	movs r2, #134
	bl 0x0200e530
	mov r1, r8
	movs r2, #134
	movs r0, #27
	bl 0x0200e528
	movs r0, #40
	bl 0x0200e4c0
	b .L_020038ac_3
	.2byte 0x0000
	.4byte 0x01000001
	.4byte 0x03001ebc
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00001e27
	.4byte 0x00000103
	.4byte 0x00000101
.L_020038ac_3:
	movs r0, #9
	movs r1, #10
	movs r2, #0
	bl 0x0200c8e8
	ldr r0, [pc, #20]
	bl 0x0200e4a8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000926
	.global FieldScene_RunScene3b1_02003d10
	.thumb_func
FieldScene_RunScene3b1_02003d10:
	.global Func_02003d10
	.thumb_func
Func_02003d10:
	push {r5, lr}
	bl 0x0200e4c8
	movs r2, #1
	movs r0, #15
	movs r1, #0
	bl 0x0200c8e8
	movs r1, #1
	movs r0, #8
	bl 0x0200e580
	movs r0, #20
	bl 0x0200e4c0
	movs r0, #8
	ldr r1, [pc, #164]
	ldr r2, [pc, #168]
	bl 0x0200e4f8
	movs r1, #234
	movs r0, #8
	lsls r1, r1, #1
	ldr r2, [pc, #160]
	bl 0x0200e530
	movs r1, #236
	movs r2, #149
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl 0x0200e530
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200e5d0
	movs r2, #20
	movs r1, #4
	movs r0, #8
	bl 0x0200e570
	bl 0x0200c5d0
	adds r5, r0, #0
	movs r0, #20
	bl 0x0200e4c0
	movs r0, #214
	bl 0x0200e660
	ldr r1, [pc, #104]
	adds r0, r5, #0
	bl 0x0200e458
	movs r0, #40
	bl 0x0200e4c0
	movs r1, #3
	movs r0, #8
	bl 0x0200e560
	movs r0, #20
	bl 0x0200e4c0
	movs r1, #233
	movs r2, #156
	lsls r2, r2, #2
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200e530
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #8
	bl 0x0200c880
	movs r1, #2
	movs r0, #8
	bl 0x0200e578
	ldr r0, [pc, #48]
	bl 0x0200e5a8
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r0, #9
	movs r1, #11
	movs r2, #0
	bl 0x0200c8e8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000266
	.4byte 0x0200e738
	.4byte 0x00001e3b
	.global FieldScene_RunScene3b1_02003dec
	.thumb_func
FieldScene_RunScene3b1_02003dec:
	.global Func_02003dec
	.thumb_func
Func_02003dec:
	push {lr}
	bl 0x0200e4c8
	movs r0, #15
	movs r1, #1
	movs r2, #1
	bl 0x0200c8e8
	movs r1, #160
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #7
	bl 0x0200e5d0
	movs r1, #2
	movs r0, #8
	bl 0x0200e578
	ldr r0, [pc, #28]
	bl 0x0200e5a8
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r0, #9
	movs r1, #11
	movs r2, #0
	bl 0x0200c8e8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001e3d
	.global FieldScene_RunScene3b1_02003e34
	.thumb_func
FieldScene_RunScene3b1_02003e34:
	.global Func_02003e34
	.thumb_func
Func_02003e34:
	push {lr}
	bl 0x0200e4c8
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r1, #150
	movs r0, #16
	lsls r1, r1, #16
	ldr r2, [pc, #112]
	bl 0x0200e548
	movs r0, #156
	movs r1, #1
	movs r2, #134
	ldr r3, [pc, #104]
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200c8ac
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #16
	ldr r1, [pc, #84]
	ldr r2, [pc, #84]
	bl 0x0200e4f8
	movs r0, #16
	movs r1, #168
	ldr r2, [pc, #80]
	bl 0x0200e530
	movs r0, #16
	movs r1, #168
	ldr r2, [pc, #72]
	bl 0x0200e530
	movs r1, #128
	movs r2, #20
	movs r0, #16
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r1, #2
	movs r0, #16
	bl 0x0200e578
	ldr r0, [pc, #52]
	bl 0x0200e5a8
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r0, #9
	movs r1, #12
	movs r2, #0
	bl 0x0200c8e8
	pop {r0}
	bx r0
	.4byte 0x024a0000
	.4byte 0x01000001
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000242
	.4byte 0x0000022a
	.4byte 0x00001e3c
	.global FieldScene_RunScene3b1_02003eec
	.thumb_func
FieldScene_RunScene3b1_02003eec:
	.global Func_02003eec
	.thumb_func
Func_02003eec:
	push {lr}
	bl 0x0200e4c8
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	bl 0x0200d004
	movs r1, #150
	movs r0, #18
	lsls r1, r1, #16
	ldr r2, [pc, #112]
	bl 0x0200e548
	movs r0, #156
	movs r1, #1
	movs r2, #134
	ldr r3, [pc, #104]
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl 0x0200c8ac
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #18
	ldr r1, [pc, #84]
	ldr r2, [pc, #84]
	bl 0x0200e4f8
	movs r0, #18
	movs r1, #168
	ldr r2, [pc, #80]
	bl 0x0200e530
	movs r0, #18
	movs r1, #168
	ldr r2, [pc, #72]
	bl 0x0200e530
	movs r1, #128
	movs r2, #20
	movs r0, #18
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r1, #2
	movs r0, #18
	bl 0x0200e578
	ldr r0, [pc, #52]
	bl 0x0200e5a8
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r0, #9
	movs r1, #12
	movs r2, #0
	bl 0x0200c8e8
	pop {r0}
	bx r0
	.4byte 0x024a0000
	.4byte 0x01000001
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000242
	.4byte 0x0000022a
	.4byte 0x00001e3c
	.global FieldScene_RunFlagBranchedSetupCascade
	.thumb_func
FieldScene_RunFlagBranchedSetupCascade:
	.global Func_02003f94
	.thumb_func
Func_02003f94:
	push {lr}
	bl 0x0200e4c8
	movs r0, #9
	movs r1, #5
	bl 0x0200e550
	movs r0, #24
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl 0x0200c8e8
	movs r0, #0
	bl 0x0200c670
	movs r2, #20
	movs r0, #8
	movs r1, #1
	bl 0x0200c8e8
	ldr r0, [pc, #248]
	ldr r1, [pc, #252]
	bl 0x0200e5f8
	movs r0, #220
	movs r1, #1
	movs r2, #176
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200e600
	movs r0, #20
	bl 0x0200e4c0
	movs r1, #7
	movs r0, #9
	bl 0x0200e550
	movs r0, #30
	bl 0x0200e4c0
	movs r0, #188
	bl 0x0200e660
	movs r0, #30
	bl 0x0200e4c0
	movs r0, #16
	bl 0x0200c670
	movs r0, #80
	bl 0x0200e4c0
	movs r0, #0
	bl 0x0200c670
	movs r0, #60
	bl 0x0200e4c0
	movs r1, #7
	movs r0, #9
	bl 0x0200e550
	movs r0, #30
	bl 0x0200e4c0
	movs r0, #188
	bl 0x0200e660
	movs r0, #30
	bl 0x0200e4c0
	movs r0, #16
	bl 0x0200c670
	movs r0, #80
	bl 0x0200e4c0
	movs r0, #0
	bl 0x0200c670
	movs r0, #90
	bl 0x0200e4c0
	movs r0, #188
	bl 0x0200e660
	movs r0, #30
	bl 0x0200e4c0
	ldr r3, [pc, #112]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #67
	str r2, [r3]
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	ldr r0, [pc, #92]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02003f94_0
	movs r0, #20
	bl 0x0200e618
	b .L_02003f94_1
.L_02003f94_0:
	ldr r0, [pc, #80]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02003f94_2
	movs r0, #18
	bl 0x0200e618
	b .L_02003f94_1
.L_02003f94_2:
	ldr r0, [pc, #64]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02003f94_3
	movs r0, #17
	bl 0x0200e618
	b .L_02003f94_1
.L_02003f94_3:
	ldr r0, [pc, #52]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02003f94_4
	movs r0, #16
	bl 0x0200e618
	b .L_02003f94_1
.L_02003f94_4:
	movs r0, #13
	bl 0x0200e618
.L_02003f94_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00006666
	.4byte 0x00000ccc
	.4byte 0x03001ebc
	.4byte 0x0000092b
	.4byte 0x0000092a
	.4byte 0x00000929
	.4byte 0x00000928
	.global FieldScene_RunScene3b1_020040e8
	.thumb_func
FieldScene_RunScene3b1_020040e8:
	.global Func_020040e8
	.thumb_func
Func_020040e8:
	push {lr}
	bl 0x0200e4c8
	movs r2, #1
	movs r0, #15
	movs r1, #1
	bl 0x0200c8e8
	movs r1, #1
	movs r0, #8
	bl 0x0200e580
	movs r0, #10
	bl 0x0200e4c0
	movs r1, #192
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #6
	bl 0x0200e5d0
	movs r1, #2
	movs r0, #8
	bl 0x0200e578
	ldr r0, [pc, #28]
	bl 0x0200e5a8
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r0, #9
	movs r1, #14
	movs r2, #0
	bl 0x0200c8e8
	pop {r0}
	bx r0
	.4byte 0x00001e40
	.global FieldScene_RunScene3b1_0200413c
	.thumb_func
FieldScene_RunScene3b1_0200413c:
	.global Func_0200413c
	.thumb_func
Func_0200413c:
	push {lr}
	bl 0x0200e4c8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x0200e600
	movs r0, #1
	bl 0x0200e420
	movs r2, #1
	movs r0, #15
	movs r1, #1
	bl 0x0200c8e8
	movs r1, #1
	movs r0, #8
	bl 0x0200e580
	ldr r0, [pc, #36]
	bl 0x0200e5a8
	movs r0, #8
	bl 0x0200c86c
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200e5d0
	movs r0, #9
	movs r1, #15
	movs r2, #0
	bl 0x0200c8e8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001e43
	.global FieldScene_RunScene3b1_02004198
	.thumb_func
FieldScene_RunScene3b1_02004198:
	.global Func_02004198
	.thumb_func
Func_02004198:
	push {r5, lr}
	bl 0x0200e4c8
	movs r0, #24
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #25
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	bl 0x0200b7b4
	movs r2, #12
	movs r0, #19
	movs r1, #11
	bl 0x0200c8e8
	movs r0, #10
	movs r1, #6
	bl 0x0200e550
	ldr r1, [pc, #68]
	movs r0, #12
	bl 0x0200e500
	ldr r5, [pc, #64]
	movs r0, #36
	adds r1, r5, #0
	bl 0x0200e500
	adds r1, r5, #0
	movs r0, #37
	bl 0x0200e500
	adds r1, r5, #0
	movs r0, #38
	bl 0x0200e500
	movs r0, #36
	movs r1, #3
	bl 0x0200e598
	movs r0, #37
	movs r1, #3
	bl 0x0200e598
	movs r0, #38
	movs r1, #3
	bl 0x0200e598
	bl 0x0200d0e4
	bl 0x0200e4d0
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200e840
	.4byte 0x0200e8e4
	.global FieldScene_RunActors24And25Setup
	.thumb_func
FieldScene_RunActors24And25Setup:
	.global Func_02004218
	.thumb_func
Func_02004218:
	push {lr}
	bl 0x0200e4c8
	movs r0, #24
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r1, #11
	movs r2, #12
	movs r0, #19
	bl 0x0200c8e8
	bl 0x0200d2f4
	ldr r0, [pc, #12]
	bl 0x0200e4a8
	bl 0x0200e4d0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000928
	.section .text.x0200c670,"ax",%progbits
	.align 2
	.global FieldScene_InstallFlaggedActors10To17
	.thumb_func
FieldScene_InstallFlaggedActors10To17:
	.global Func_02004670
	.thumb_func
Func_02004670:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r0, [pc, #320]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02004670_0
	movs r1, #0
	movs r0, #0
	bl 0x0200cfa8
	movs r1, #205
	movs r2, #172
	adds r5, r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200e548
	movs r0, #7
	adds r1, r5, #0
	adds r2, r6, #0
	bl 0x0200c8e8
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	b .L_02004670_1
.L_02004670_0:
	movs r0, #5
	movs r1, #10
	adds r2, r6, #0
	bl 0x0200c8e8
.L_02004670_1:
	ldr r0, [pc, #260]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02004670_2
	movs r1, #0
	movs r0, #1
	bl 0x0200cfa8
	movs r1, #235
	movs r2, #172
	adds r5, r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200e548
	adds r0, r5, #0
	bl 0x0200e4e8
	ldr r3, [pc, #228]
	adds r1, r5, #0
	str r3, [r0, #24]
	adds r2, r6, #0
	movs r0, #7
	bl 0x0200c8e8
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	b .L_02004670_3
.L_02004670_2:
	movs r0, #6
	movs r1, #11
	adds r2, r6, #0
	bl 0x0200c8e8
.L_02004670_3:
	ldr r0, [pc, #196]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02004670_4
	movs r1, #0
	movs r0, #2
	bl 0x0200cfa8
	movs r1, #205
	movs r2, #204
	adds r5, r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200e548
	movs r0, #7
	adds r1, r5, #0
	adds r2, r6, #0
	bl 0x0200c8e8
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	b .L_02004670_5
.L_02004670_4:
	movs r0, #5
	movs r1, #12
	adds r2, r6, #0
	bl 0x0200c8e8
.L_02004670_5:
	ldr r0, [pc, #136]
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02004670_6
	movs r1, #0
	movs r0, #3
	bl 0x0200cfa8
	movs r1, #235
	movs r2, #204
	adds r5, r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200e548
	adds r0, r5, #0
	bl 0x0200e4e8
	ldr r3, [pc, #88]
	adds r1, r5, #0
	str r3, [r0, #24]
	adds r2, r6, #0
	movs r0, #7
	bl 0x0200c8e8
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	b .L_02004670_7
.L_02004670_6:
	movs r0, #6
	movs r1, #13
	adds r2, r6, #0
	bl 0x0200c8e8
.L_02004670_7:
	adds r2, r6, #0
	movs r0, #5
	movs r1, #14
	bl 0x0200c8e8
	adds r2, r6, #0
	movs r0, #6
	movs r1, #15
	bl 0x0200c8e8
	adds r2, r6, #0
	movs r0, #5
	movs r1, #16
	bl 0x0200c8e8
	movs r0, #6
	movs r1, #17
	adds r2, r6, #0
	bl 0x0200c8e8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000928
	.4byte 0x00000929
	.4byte 0xffff0000
	.4byte 0x0000092a
	.4byte 0x0000092b
	.section .text.x0200c86c,"ax",%progbits
	.align 2
	.global FieldScene_RunStepThen10
	.thumb_func
FieldScene_RunStepThen10:
	.global Func_0200486c
	.thumb_func
Func_0200486c:
	push {lr}
	movs r1, #0
	bl 0x0200e5b8
	movs r0, #10
	bl 0x0200e4c0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global FieldScene_CallPairWith10
	.thumb_func
FieldScene_CallPairWith10:
	.global Func_02004880
	.thumb_func
Func_02004880:
	push {lr}
	lsls r1, r1, #16
	lsrs r1, r1, #16
	movs r2, #10
	bl 0x0200e5d0
	pop {r0}
	bx r0
	.global OverlayObject_SetPositionAndHeading
	.thumb_func
OverlayObject_SetPositionAndHeading:
	.global Func_02004890
	.thumb_func
Func_02004890:
	push {r5, r6, lr}
	adds r5, r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	adds r6, r3, #0
	bl 0x0200e548
	adds r0, r5, #0
	bl 0x0200e4e8
	strh r6, [r0, #6]
	pop {r5, r6}
	pop {r0}
	bx r0
	.global ConfigureSceneMotionFlags
	.thumb_func
ConfigureSceneMotionFlags:
	.global Func_020048ac
	.thumb_func
Func_020048ac:
	push {r5, r6, lr}
	adds r5, r3, #0
	movs r3, #1
	bics r3, r5
	bl 0x0200e600
	movs r3, #128
	ldr r6, [pc, #40]
	lsls r3, r3, #21
	ands r3, r5
	ands r6, r5
	cmp r3, #0
	beq .L_020048ac_0
	bl 0x0200e608
.L_020048ac_0:
	movs r3, #128
	lsls r3, r3, #17
	ands r3, r5
	cmp r3, #0
	beq .L_020048ac_1
	bl 0x0200e468
.L_020048ac_1:
	adds r0, r6, #0
	bl 0x0200e4c0
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00001111
	.global FieldScene_RunSceneStep
	.thumb_func
FieldScene_RunSceneStep:
	.global Func_020048e8
	.thumb_func
Func_020048e8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r1, #0
	adds r6, r2, #0
	cmp r0, #25
	bls .L_020048e8_0
	b .L_020048e8_1
.L_020048e8_0:
	ldr r2, [pc, #952]
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
	ldmia r1!, {r2, r3, r5, r6}
	lsls r0, r0, #8
	ldmia r1!, {r2, r4, r7}
	lsls r0, r0, #8
	.2byte 0xc9be
	lsls r0, r0, #8
	ldmia r1!, {r4, r5, r6, r7}
	lsls r0, r0, #8
	ldmia r2!, {r5}
	lsls r0, r0, #8
	.2byte 0xca3c
	lsls r0, r0, #8
	.2byte 0xca4c
	lsls r0, r0, #8
	.2byte 0xca6e
	lsls r0, r0, #8
	ldmia r2!, {r4, r7}
	lsls r0, r0, #8
	ldmia r2!, {r1, r4, r5, r7}
	lsls r0, r0, #8
	ldmia r2!, {r3, r6, r7}
	lsls r0, r0, #8
	ldmia r3!, {r5}
	lsls r0, r0, #8
	ldmia r4!, {r1, r5, r6}
	lsls r0, r0, #8
	ldmia r4!, {r1, r3, r7}
	lsls r0, r0, #8
	ldmia r4!, {r2, r6, r7}
	lsls r0, r0, #8
	.2byte 0xccda
	lsls r0, r0, #8
	ldmia r5!, {r4, r6}
	lsls r0, r0, #8
	.2byte 0xcd74
	lsls r0, r0, #8
	ldmia r5!, {r1, r3, r7}
	lsls r0, r0, #8
	.2byte 0xcde0
	lsls r0, r0, #8
	ldmia r6!, {r1, r2, r7}
	lsls r0, r0, #8
	ldmia r6!, {r1, r3, r4, r7}
	lsls r0, r0, #8
	.2byte 0xced0
	lsls r0, r0, #8
	.2byte 0xceea
	lsls r0, r0, #8
	.2byte 0xcefe
	lsls r0, r0, #8
	ldmia r7!, {r1, r2, r5}
	lsls r0, r0, #8
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	b .L_020048e8_2
	.2byte 0x2000
	.2byte 0x1c39
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfe19
	.2byte 0x2001
	.2byte 0x1c39
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfe14
	.2byte 0x2002
	.2byte 0x1c39
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfe0f
	.2byte 0x2003
	.2byte 0x1c39
.L_020048e8_2:
	adds r2, r6, #0
	bl 0x0200e5d0
	b .L_020048e8_1
	.2byte 0x2000
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfdc5
	.2byte 0x2001
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfdc1
	.2byte 0x2002
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfdbd
	.2byte 0x2003
	.2byte 0x2103
	.2byte 0xf001
	.2byte 0xfdb9
	.2byte 0x2f00
	.2byte 0xd002
	.2byte 0x2003
	.2byte 0xf001
	.2byte 0xfdc0
	.2byte 0x2e00
	.2byte 0xd100
	.2byte 0xe2b6
	.2byte 0xe013
	.2byte 0x2181
	.2byte 0x2000
	.2byte 0x0049
	.2byte 0xf001
	.2byte 0xfdfb
	.2byte 0x2181
	.2byte 0x2001
	.2byte 0x0049
	.2byte 0xf001
	.2byte 0xfdf6
	.2byte 0x2181
	.2byte 0x2002
	.2byte 0x0049
	.2byte 0xf001
	.2byte 0xfdf1
	.2byte 0x2181
	.2byte 0x2003
	.2byte 0x0049
	.2byte 0xf001
	.2byte 0xfdec
	.2byte 0x1c30
	.2byte 0xf001
	.2byte 0xfd51
	.2byte 0xe29d
	.2byte 0x2500
	.2byte 0x42bd
	.2byte 0xd300
	.2byte 0xe299
	.2byte 0x1c28
	.2byte 0x300a
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0x3501
	.2byte 0xf001
	.2byte 0xfd89
	.2byte 0x42bd
	.2byte 0xd3f6
	.2byte 0xe28f
	.2byte 0x1c38
	.2byte 0xf001
	.2byte 0xfd53
	.2byte 0x23a0
	.2byte 0x01db
	.2byte 0x80c3
	.2byte 0x2105
	.2byte 0xe008
	.2byte 0x1c38
	.2byte 0xf001
	.2byte 0xfd4b
	.2byte 0x23a0
	.2byte 0x01db
	.2byte 0x80c3
	.2byte 0x4b98
	.2byte 0x2105
	.2byte 0x6183
	.2byte 0x1c38
	.2byte 0xf001
	.2byte 0xfd76
	.2byte 0x1c38
	.2byte 0x1c31
	.2byte 0xf001
	.2byte 0xfd76
	.2byte 0xe276
	.2byte 0x1c38
	.2byte 0xf001
	.2byte 0xfd3a
	.2byte 0x23a0
	.2byte 0x01db
	.2byte 0x80c3
	.2byte 0x1c38
	.2byte 0xf7ff
	.2byte 0xfea6
	.2byte 0x2e00
	.2byte 0xd000
	.2byte 0xe26a
	.2byte 0x1c38
	.2byte 0x2100
	.2byte 0xf001
	.2byte 0xfd65
	.2byte 0xe265
	.2byte 0x4b8b
	.2byte 0x22e0
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x3242
	.2byte 0x601a
	.2byte 0xf001
	.2byte 0xfdc7
	.2byte 0x2f00
	.2byte 0xd001
	.2byte 0xf001
	.2byte 0xfdcb
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfd08
	.2byte 0xe254
	.2byte 0xf001
	.2byte 0xfdc1
	.2byte 0xf001
	.2byte 0xfdc3
	.2byte 0x2f00
	.2byte 0xd100
	.2byte 0xe24d
	.2byte 0x1c38
	.2byte 0xf001
	.2byte 0xfda9
	.2byte 0xe249
	.2byte 0x2018
	.2byte 0x2101
	.2byte 0x2200
	.2byte 0xf7ff
	.2byte 0xff0b
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0x2019
	.2byte 0xf7ff
	.2byte 0xff06
	.2byte 0x2580
	.2byte 0x2000
	.2byte 0xf7fe
	.2byte 0xfe7a
	.2byte 0x01ed
	.2byte 0x21d8
	.2byte 0x0049
	.2byte 0x2000
	.2byte 0x22a8
	.2byte 0x1c2b
	.2byte 0xf7ff
	.2byte 0xfece
	.2byte 0x21e0
	.2byte 0x0049
	.2byte 0x2001
	.2byte 0x22a8
	.2byte 0x1c2b
	.2byte 0xf7ff
	.2byte 0xfec7
	.2byte 0x21d4
	.2byte 0x0049
	.2byte 0x2002
	.2byte 0x2298
	.2byte 0x1c2b
	.2byte 0xf7ff
	.2byte 0xfec0
	.2byte 0x21e5
	.2byte 0x0049
	.2byte 0x2003
	.2byte 0x2298
	.2byte 0x1c2b
	.2byte 0xf7ff
	.2byte 0xfeb9
	.2byte 0xe21d
	.2byte 0x2f00
	.2byte 0xd00f
	.2byte 0x2101
	.2byte 0x200d
	.2byte 0xf001
	.2byte 0xfd12
	.2byte 0x200d
	.2byte 0xf001
	.2byte 0xfcdb
	.2byte 0x23c0
	.2byte 0x019b
	.2byte 0x80c3
	.2byte 0x200d
	.2byte 0xf001
	.2byte 0xfcd5
	.2byte 0x2380
	.2byte 0x025b
	.2byte 0x6183
	.2byte 0x2101
	.2byte 0x200e
	.2byte 0xf001
	.2byte 0xfd02
	.2byte 0x200e
	.2byte 0xf001
	.2byte 0xfccb
	.2byte 0x23a0
	.2byte 0x01db
	.2byte 0x4698
	.2byte 0x4642
	.2byte 0x80c2
	.2byte 0x2101
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xfcf6
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xfcbf
	.2byte 0x23c0
	.2byte 0x019b
	.2byte 0x469a
	.2byte 0x4652
	.2byte 0x80c2
	.2byte 0x200f
	.2byte 0xf001
	.2byte 0xfcb7
	.2byte 0x2580
	.2byte 0x026d
	.2byte 0x2101
	.2byte 0x6185
	.2byte 0x2010
	.2byte 0xf001
	.2byte 0xfce4
	.2byte 0x2010
	.2byte 0xf001
	.2byte 0xfcad
	.2byte 0x4643
	.2byte 0x80c3
	.2byte 0x2101
	.2byte 0x2011
	.2byte 0xf001
	.2byte 0xfcdb
	.2byte 0x2011
	.2byte 0xf001
	.2byte 0xfca4
	.2byte 0x4652
	.2byte 0x80c2
	.2byte 0x2011
	.2byte 0xf001
	.2byte 0xfc9f
	.2byte 0x21cd
	.2byte 0x22ae
	.2byte 0x6185
	.2byte 0x0449
	.2byte 0x201c
	.2byte 0x0412
	.2byte 0xf001
	.2byte 0xfcc7
	.2byte 0x21eb
	.2byte 0x22ae
	.2byte 0x201d
	.2byte 0x0449
	.2byte 0x0412
	.2byte 0xf001
	.2byte 0xfcc0
	.2byte 0x21cd
	.2byte 0x22ce
	.2byte 0x201e
	.2byte 0x0449
	.2byte 0x0412
	.2byte 0xf001
	.2byte 0xfcb9
	.2byte 0x21eb
	.2byte 0x22ce
	.2byte 0x201f
	.2byte 0x0449
	.2byte 0x0412
	.2byte 0xf001
	.2byte 0xfcb2
	.2byte 0x21cd
	.2byte 0x228f
	.2byte 0x2020
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xfcab
	.2byte 0x21eb
	.2byte 0x228f
	.2byte 0x2021
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xfca4
	.2byte 0x21cd
	.2byte 0x229e
	.2byte 0x2022
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xfc9d
	.2byte 0x21eb
	.2byte 0x229e
	.2byte 0x2023
	.2byte 0x0449
	.2byte 0x0452
	.2byte 0xf001
	.2byte 0xfc96
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfbff
	.2byte 0x2f00
	.2byte 0xd005
	.2byte 0x21b0
	.2byte 0x200d
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfccf
	.2byte 0x21d0
	.2byte 0x200e
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfcc9
	.2byte 0x21b0
	.2byte 0x200f
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfcc3
	.2byte 0x21d0
	.2byte 0x2010
	.2byte 0x0209
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfcbd
	.2byte 0x21b0
	.2byte 0x0209
	.2byte 0x2011
	.2byte 0xf7ff
	.2byte 0xfe10
	.2byte 0xe17c
	.2byte 0x1c38
	.2byte 0xf001
	.2byte 0xfc40
	.2byte 0x2101
	.2byte 0x1c05
	.2byte 0x1c38
	.2byte 0xf001
	.2byte 0xfc6f
	.2byte 0x2e00
	.2byte 0xd002
	.2byte 0x23c0
	.2byte 0x019b
	.2byte 0xe001
	.2byte 0x23a0
	.2byte 0x01db
	.2byte 0x80eb
	.2byte 0x2380
	.2byte 0x025b
	.2byte 0x61ab
	.2byte 0xe168
	.2byte 0x2009
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfc5a
	.2byte 0x200c
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfc55
	.2byte 0x200b
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfc50
	.2byte 0x200d
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfc4b
	.2byte 0x200a
	.2byte 0xe00c
	.2byte 0x0000
	.4byte 0x0200c904
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x200e
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfc3d
	.2byte 0x200d
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfc38
	.2byte 0xe140
	.2byte 0x2018
	.2byte 0x2101
	.2byte 0x2200
	.2byte 0xf7ff
	.2byte 0xfe02
	.2byte 0x2009
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfc2d
	.2byte 0x200a
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfc28
	.2byte 0x21de
	.2byte 0x23d0
	.2byte 0x0049
	.2byte 0x4a9b
	.2byte 0x2008
	.2byte 0x021b
	.2byte 0xf7ff
	.2byte 0xfdc4
	.2byte 0x2000
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfc1b
	.2byte 0x2f00
	.2byte 0xd001
	.2byte 0xf7ff
	.2byte 0xfc5b
	.2byte 0x20e0
	.2byte 0x2180
	.2byte 0x229c
	.2byte 0x0440
	.2byte 0x0389
	.2byte 0x0492
	.2byte 0x4b92
	.2byte 0xf7ff
	.2byte 0xfdc0
	.2byte 0x2e00
	.2byte 0xd100
	.2byte 0xe114
	.2byte 0x4b90
	.2byte 0x22e0
	.2byte 0x681b
	.2byte 0x0052
	.2byte 0x189b
	.2byte 0x3242
	.2byte 0x601a
	.2byte 0xf001
	.2byte 0xfc76
	.2byte 0xf001
	.2byte 0xfc7c
	.2byte 0x2014
	.2byte 0xf001
	.2byte 0xfbb9
	.2byte 0xe105
	.2byte 0x2008
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfbf7
	.2byte 0x2009
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf001
	.2byte 0xfbf2
	.2byte 0x21db
	.2byte 0x2298
	.2byte 0x201b
	.2byte 0x0449
	.2byte 0x0412
	.2byte 0xf001
	.2byte 0xfbeb
	.2byte 0xe0f3
	.2byte 0x2500
	.2byte 0x1c28
	.2byte 0x301c
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0x3501
	.2byte 0xf001
	.2byte 0xfbe2
	.2byte 0x2d07
	.2byte 0xd9f6
	.2byte 0xe0e8
	.2byte 0x2285
	.2byte 0x23b0
	.2byte 0x0092
	.2byte 0x021b
	.2byte 0x200c
	.2byte 0x2198
	.2byte 0x25f5
	.2byte 0xf7ff
	.2byte 0xfd7a
	.2byte 0x006d
	.2byte 0x23c0
	.2byte 0x019b
	.2byte 0x1c2a
	.2byte 0x2008
	.2byte 0x2186
	.2byte 0x4698
	.2byte 0xf7ff
	.2byte 0xfd71
	.2byte 0x23a0
	.2byte 0x1c2a
	.2byte 0x2009
	.2byte 0x21a6
	.2byte 0x01db
	.2byte 0xf7ff
	.2byte 0xfd6a
	.2byte 0x350e
	.2byte 0x23a0
	.2byte 0x200a
	.2byte 0x21b6
	.2byte 0x1c2a
	.2byte 0x01db
	.2byte 0xf7ff
	.2byte 0xfd62
	.2byte 0x200b
	.2byte 0x2176
	.2byte 0x1c2a
	.2byte 0x4643
	.2byte 0xf7ff
	.2byte 0xfd5c
	.2byte 0x2700
	.2byte 0x200e
	.2byte 0x2600
	.2byte 0xe58a
	.2byte 0x21d0
	.2byte 0x22a4
	.2byte 0x0049
	.2byte 0x0052
	.2byte 0x2008
	.2byte 0x2300
	.2byte 0xf7ff
	.2byte 0xfd50
	.2byte 0x22e0
	.2byte 0x0052
	.2byte 0x4692
	.2byte 0x23d0
	.2byte 0x021b
	.2byte 0x3a60
	.2byte 0x2009
	.2byte 0x4651
	.2byte 0x4699
	.2byte 0xf7ff
	.2byte 0xfd45
	.2byte 0x22c0
	.2byte 0x0192
	.2byte 0x4690
	.2byte 0x21e3
	.2byte 0x0049
	.2byte 0x200a
	.2byte 0x22f8
	.2byte 0x4643
	.2byte 0x25cc
	.2byte 0xf7ff
	.2byte 0xfd3a
	.2byte 0x006d
	.2byte 0x2291
	.2byte 0x1c29
	.2byte 0x0052
	.2byte 0x1c38
	.2byte 0x2300
	.2byte 0xf7ff
	.2byte 0xfd32
	.2byte 0x22ab
	.2byte 0x1c29
	.2byte 0x0052
	.2byte 0x1c30
	.2byte 0x2300
	.2byte 0xf7ff
	.2byte 0xfd2b
	.2byte 0x21d2
	.2byte 0x22b2
	.2byte 0x0049
	.2byte 0x0052
	.2byte 0x200d
	.2byte 0x464b
	.2byte 0xf7ff
	.2byte 0xfd23
	.2byte 0x2298
	.2byte 0x1c29
	.2byte 0x0052
	.2byte 0x200e
	.2byte 0x2300
	.2byte 0xf7ff
	.2byte 0xfd1c
	.2byte 0x3d1e
	.2byte 0x21d1
	.2byte 0x0049
	.2byte 0x200f
	.2byte 0x1c2a
	.2byte 0x464b
	.2byte 0xf7ff
	.2byte 0xfd14
	.2byte 0x21dc
	.2byte 0x2283
	.2byte 0x0049
	.2byte 0x0052
	.2byte 0x2010
	.2byte 0x4643
	.2byte 0xf7ff
	.2byte 0xfd0c
	.2byte 0x2011
	.2byte 0x4651
	.2byte 0x1c2a
	.2byte 0x464b
	.2byte 0xf7ff
	.2byte 0xfd06
	.2byte 0xe06a
	.2byte 0x1c3d
	.2byte 0x42b7
	.2byte 0xd867
	.2byte 0x1c28
	.2byte 0x3501
	.2byte 0xf001
	.2byte 0xfb0e
	.2byte 0x42b5
	.2byte 0xd9f9
	.2byte 0xe060
	.2byte 0x4937
	.2byte 0x4a37
	.2byte 0x2014
	.2byte 0xf7ff
	.2byte 0xfd22
	.2byte 0x4936
	.2byte 0x4a37
	.2byte 0x2014
	.2byte 0xf7ff
	.2byte 0xfd1d
	.2byte 0x2199
	.2byte 0x0109
	.2byte 0x4a35
	.2byte 0x2014
	.2byte 0xf7ff
	.2byte 0xfd17
	.2byte 0x20c0
	.2byte 0x0080
	.2byte 0xf001
	.2byte 0xfaf7
	.2byte 0x4832
	.2byte 0xf001
	.2byte 0xfaf4
	.2byte 0x4831
	.2byte 0xf001
	.2byte 0xfaf1
	.2byte 0xe045
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfaa5
	.2byte 0x2017
	.2byte 0x2100
	.2byte 0x2200
	.2byte 0xf7ff
	.2byte 0xfd04
	.2byte 0x492c
	.2byte 0x200c
	.2byte 0xf001
	.2byte 0xfb0c
	.2byte 0xe038
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfb00
	.2byte 0x2002
	.2byte 0xf001
	.2byte 0xfafd
	.2byte 0x2003
	.2byte 0xf001
	.2byte 0xfafa
	.2byte 0xe02e
	.2byte 0x2001
	.2byte 0x2101
	.2byte 0x2201
	.2byte 0x4240
	.2byte 0x4249
	.2byte 0x4252
	.2byte 0x2300
	.2byte 0xf001
	.2byte 0xfb78
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfa85
	.2byte 0x2f00
	.2byte 0xd020
	.2byte 0xf001
	.2byte 0xfb79
	.2byte 0x2300
	.2byte 0x3055
	.2byte 0x7003
	.2byte 0xe01a
	.2byte 0x481c
	.2byte 0xf001
	.2byte 0xfad6
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfa77
	.2byte 0x2f01
	.2byte 0xd101
	.2byte 0x4819
	.2byte 0xe002
	.2byte 0x2f02
	.2byte 0xd106
	.2byte 0x4818
	.2byte 0xf001
	.2byte 0xfaca
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfa6b
	.2byte 0xe007
	.2byte 0x2f03
	.2byte 0xd105
	.2byte 0x4814
	.2byte 0xf001
	.2byte 0xfac1
	.2byte 0x2001
	.2byte 0xf001
	.2byte 0xfa62
.L_020048e8_1:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0266
	.2byte 0x0000
	.2byte 0x0001
	.2byte 0x0100
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x092c
	.2byte 0x0000
	.2byte 0x093d
	.2byte 0x0000
	.2byte 0x0917
	.2byte 0x0000
	.2byte 0x091f
	.2byte 0x0000
	.2byte 0x0998
	.2byte 0x0000
	.2byte 0x0301
	.2byte 0x0000
	.2byte 0x0302
	.2byte 0x0000
	.2byte 0xe840
	.2byte 0x0200
	.2byte 0xf2a0
	.2byte 0x0200
	.2byte 0xf300
	.2byte 0x0200
	.2byte 0xf360
	.2byte 0x0200
	.2byte 0xf3c0
	.2byte 0x0200
	.section .text.x0200d004,"ax",%progbits
	.align 2
	.global Func_02005004
	.thumb_func
Func_02005004:
	push {lr}
	ldr r1, [pc, #36]
	movs r0, #8
	bl 0x0200d038
	ldr r1, [pc, #32]
	movs r0, #8
	bl 0x0200d038
	ldr r1, [pc, #28]
	movs r0, #8
	bl 0x0200d038
	movs r1, #153
	lsls r1, r1, #4
	movs r0, #8
	bl 0x0200d038
	pop {r0}
	bx r0
	.4byte 0x0000092c
	.4byte 0x00000935
	.4byte 0x00000917
	.global Func_02005038
	.thumb_func
Func_02005038:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r5, r1, #0
	movs r7, #0
.L_02005038_2:
	adds r0, r5, #0
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_02005038_0
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	b .L_02005038_1
.L_02005038_0:
	adds r7, #1
	adds r6, #1
	adds r5, #1
	cmp r7, #8
	bls .L_02005038_2
.L_02005038_1:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global FieldScene_RunScene3b1_02005068
	.thumb_func
FieldScene_RunScene3b1_02005068:
	.global Func_02005068
	.thumb_func
Func_02005068:
	push {r5, r6, lr}
	movs r1, #0
	movs r0, #0
	bl 0x0200cfa8
	adds r6, r0, #0
	bl 0x0200e4c8
	movs r0, #24
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #25
	movs r1, #2
.L_02005086:
	movs r2, #0
	bl 0x0200c8e8
	bl 0x0200b7b4
	movs r2, #12
	adds r1, r6, #0
	movs r0, #19
	bl 0x0200c8e8
	movs r0, #10
	movs r1, #6
	bl 0x0200e550
	ldr r5, [pc, #56]
	adds r0, r6, #0
	adds r1, r5, #0
	bl 0x0200e500
	movs r0, #11
	bl 0x0200e4f0
	adds r1, r5, #0
	movs r0, #12
	bl 0x0200e500
	ldr r5, [pc, #36]
	movs r0, #36
	adds r1, r5, #0
	bl 0x0200e500
	movs r0, #37
	adds r1, r5, #0
	bl 0x0200e500
	bl 0x0200d0e4
	bl 0x0200e4d0
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200e840
	.4byte 0x0200e8e4
	.global Func_020050e4
	.thumb_func
Func_020050e4:
	push {r5, r6, r7, lr}
	movs r6, #220
	movs r7, #1
	lsls r6, r6, #17
	negs r7, r7
	movs r2, #176
	ldr r3, [pc, #472]
	lsls r2, r2, #16
	adds r0, r6, #0
	adds r1, r7, #0
	bl 0x0200c8ac
	movs r2, #134
	movs r0, #0
	adds r1, r6, #0
	lsls r2, r2, #16
	bl 0x0200e548
	bl 0x0200e630
	ldr r2, [pc, #448]
	movs r0, #0
	ldr r1, [pc, #448]
	bl 0x0200e4f8
	movs r0, #0
	movs r1, #5
	bl 0x0200e550
	movs r1, #204
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #134
	bl 0x0200e520
	movs r1, #204
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #152
	bl 0x0200e520
	movs r1, #216
	movs r2, #166
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200e520
	movs r0, #0
	movs r1, #1
	bl 0x0200e550
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_020050e4_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200e548
.L_020050e4_0:
	movs r0, #0
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_020050e4_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200e548
.L_020050e4_1:
	movs r0, #1
.L_02005180:
	bl 0x0200e4e8
	cmp r0, #0
	beq .L_02005180_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200e548
.L_02005180_0:
	movs r0, #1
	bl 0x0200e420
	movs r0, #2
	ldr r1, [pc, #312]
	ldr r2, [pc, #304]
	bl 0x0200e4f8
	movs r1, #212
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #152
	bl 0x0200e528
	movs r0, #1
	ldr r1, [pc, #288]
	ldr r2, [pc, #284]
	bl 0x0200e4f8
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #168
	bl 0x0200e528
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200e4f8
	movs r1, #229
	movs r2, #152
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200e530
	movs r0, #1
	movs r1, #1
	bl 0x0200e550
	movs r0, #2
	movs r1, #1
	bl 0x0200e550
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #40
	movs r0, #3
	bl 0x0200e5d0
	movs r0, #10
	bl 0x0200e5d8
	ldr r5, [pc, #188]
	movs r1, #1
	adds r0, r5, #0
	movs r2, #10
	bl 0x0200e490
	movs r0, #10
	bl 0x0200e4c0
	movs r0, #0
	movs r1, #0
	movs r2, #40
	bl 0x0200c8e8
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #7
	movs r0, #1
	bl 0x0200c8e8
	ldr r0, [pc, #152]
	ldr r1, [pc, #156]
	bl 0x0200e5f8
.L_02005248:
	movs r2, #160
	ldr r3, [pc, #152]
	lsls r2, r2, #17
	adds r0, r6, #0
	adds r1, r7, #0
	bl 0x0200c8ac
	movs r0, #8
	movs r1, #2
	bl 0x0200e580
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #8
	adds r5, #1
	bl 0x0200c880
	adds r0, r5, #0
	bl 0x0200e5a8
	movs r0, #8
	bl 0x0200c86c
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x0200e5d0
	movs r2, #134
	movs r3, #128
	lsls r3, r3, #21
	lsls r2, r2, #16
	adds r0, r6, #0
	adds r1, r7, #0
	bl 0x0200c8ac
	ldr r5, [pc, #84]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200e500
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200e500
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200e510
	movs r0, #40
	bl 0x0200e4c0
	ldr r0, [pc, #56]
	bl 0x0200e4a8
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	ldr r0, [pc, #44]
	bl 0x0200e4b0
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0001
	.2byte 0x0100
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0x1e46
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0003
	.2byte 0x7333
	.2byte 0x0000
	.4byte 0x10000014
	.4byte 0x0200e7c8
	.4byte 0x00000301
	.4byte 0x0000012f
	.global Func_020052f4
	.thumb_func
Func_020052f4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r6, #222
	lsls r6, r6, #1
	movs r2, #150
	adds r1, r6, #0
	lsls r2, r2, #1
	movs r0, #0
	movs r3, #0
	movs r5, #155
	bl 0x0200c890
	lsls r5, r5, #1
	movs r1, #229
	adds r2, r5, #0
	lsls r1, r1, #1
	movs r0, #1
	movs r3, #0
	bl 0x0200c890
	movs r2, #165
	adds r1, r6, #0
	lsls r2, r2, #1
	movs r0, #2
	movs r3, #0
	bl 0x0200c890
	movs r2, #216
	lsls r2, r2, #1
	mov r9, r2
	movs r0, #3
	adds r2, r5, #0
	mov r1, r9
	movs r3, #0
	bl 0x0200c890
	movs r1, #220
	movs r3, #128
.L_02005348:
	lsls r1, r1, #1
	lsls r3, r3, #8
	movs r0, #27
	movs r2, #134
	bl 0x0200c890
	movs r1, #227
	movs r3, #192
	lsls r3, r3, #6
	movs r2, #248
	lsls r1, r1, #1
	movs r0, #10
	mov r8, r3
	movs r5, #220
	bl 0x0200c890
	movs r6, #1
	movs r0, #10
	movs r1, #6
	bl 0x0200e550
	lsls r5, r5, #17
	negs r6, r6
	movs r2, #154
	ldr r3, [pc, #724]
	lsls r2, r2, #17
	adds r1, r6, #0
	adds r0, r5, #0
	bl 0x0200c8ac
	bl 0x0200e630
	bl 0x0200e640
	movs r0, #20
	bl 0x0200e4c0
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #40
	bl 0x0200e5d0
	movs r1, #1
	movs r2, #20
	movs r0, #2
	bl 0x0200c8e8
	ldr r0, [pc, #644]
	bl 0x0200e5a8
	movs r0, #27
	bl 0x0200c86c
	movs r2, #192
	lsls r2, r2, #8
	mov r10, r2
	movs r0, #1
	movs r2, #0
	mov r1, r10
	bl 0x0200c8e8
	ldr r0, [pc, #620]
	ldr r1, [pc, #624]
	bl 0x0200e5f8
	movs r2, #176
	movs r3, #1
	adds r0, r5, #0
	adds r1, r6, #0
	lsls r2, r2, #16
	bl 0x0200e600
	movs r0, #27
	ldr r1, [pc, #604]
	ldr r2, [pc, #608]
	bl 0x0200e4f8
	movs r1, #204
	movs r0, #27
	lsls r1, r1, #1
	movs r2, #134
	bl 0x0200e530
	movs r1, #204
.L_02005416:
	movs r0, #27
	lsls r1, r1, #1
	movs r2, #152
	bl 0x0200e530
	movs r1, #212
	movs r2, #164
	movs r0, #27
	lsls r1, r1, #1
	bl 0x0200e530
	ldr r0, [pc, #560]
	ldr r1, [pc, #568]
	bl 0x0200e5f8
	movs r2, #150
	movs r3, #1
	adds r0, r5, #0
	adds r1, r6, #0
	lsls r2, r2, #17
	bl 0x0200e600
	movs r1, #212
	movs r0, #27
	lsls r1, r1, #1
	movs r2, #222
	bl 0x0200e530
	movs r1, #212
	movs r2, #131
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200e530
	movs r2, #20
	movs r0, #27
	mov r1, r8
	bl 0x0200e5d0
	movs r1, #1
	movs r0, #27
	bl 0x0200e580
	movs r0, #27
	bl 0x0200c86c
	movs r2, #20
.L_02005476:
	movs r0, #2
	movs r1, #1
	bl 0x0200c8e8
	movs r0, #27
	movs r1, #3
	bl 0x0200e560
	movs r1, #1
	movs r0, #27
	bl 0x0200e580
	movs r0, #27
	bl 0x0200c86c
	movs r0, #3
	movs r1, #2
	movs r2, #60
	bl 0x0200c8e8
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #1
	movs r2, #60
	bl 0x0200c8e8
	movs r2, #40
	movs r0, #27
	movs r1, #0
	bl 0x0200e5d0
	movs r0, #27
	movs r1, #1
	bl 0x0200e580
	movs r0, #27
	movs r1, #2
	bl 0x0200e550
	movs r2, #134
	movs r0, #27
	mov r1, r9
	lsls r2, r2, #1
	bl 0x0200e520
	movs r1, #226
	movs r2, #134
	lsls r2, r2, #1
	movs r0, #27
	lsls r1, r1, #1
	bl 0x0200e520
	movs r0, #27
	movs r1, #1
	bl 0x0200e550
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #27
	bl 0x0200c880
	movs r0, #27
	movs r1, #2
	bl 0x0200e578
	movs r0, #27
	movs r1, #0
	movs r2, #20
	bl 0x0200e5c0
	movs r2, #20
.L_02005504:
	movs r0, #1
	mov r1, r10
	bl 0x0200c8e8
	movs r1, #4
	movs r0, #27
	bl 0x0200e560
	movs r0, #40
	bl 0x0200e4c0
	movs r2, #80
	movs r0, #27
	movs r1, #0
	bl 0x0200e5c0
	movs r1, #1
	movs r0, #27
	bl 0x0200e580
	movs r0, #20
	bl 0x0200e4c0
	movs r1, #3
	movs r0, #27
	bl 0x0200e560
	movs r0, #10
	bl 0x0200e4c0
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #27
	bl 0x0200c880
	movs r1, #0
	movs r0, #27
	bl 0x0200e5b0
	movs r0, #0
	movs r1, #0
	bl 0x0200e4e0
.L_0200555a:
	cmp r0, #0
	bne .L_0200555a_0
	movs r0, #27
	movs r1, #3
	bl 0x0200e560
	movs r0, #27
	bl 0x0200c86c
	b .L_0200555a_1
.L_0200555a_0:
	movs r1, #4
	movs r0, #27
	bl 0x0200e560
	ldr r3, [pc, #244]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #27
	bl 0x0200c86c
	movs r2, #40
	movs r0, #3
	movs r1, #2
	bl 0x0200c8e8
	movs r0, #27
	movs r1, #1
	bl 0x0200e580
	movs r0, #27
	movs r1, #3
	bl 0x0200e550
	movs r0, #27
	bl 0x0200c86c
.L_0200555a_1:
	movs r2, #20
	movs r0, #2
	movs r1, #1
	bl 0x0200c8e8
	ldr r5, [pc, #184]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200e500
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200e500
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200e510
	ldr r0, [pc, #160]
	ldr r1, [pc, #164]
	bl 0x0200e5f8
	movs r0, #220
	movs r1, #1
	movs r2, #176
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200e600
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #212
	movs r2, #136
	movs r0, #0
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200e530
	movs r1, #212
	lsls r1, r1, #1
	movs r2, #164
	movs r0, #0
	bl 0x0200e528
	movs r0, #60
	bl 0x0200e4c0
	ldr r3, [pc, #80]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl 0x0200c8e8
	ldr r0, [pc, #72]
	bl 0x0200e4b0
	ldr r0, [pc, #72]
	bl 0x0200e4b0
	movs r0, #4
	bl 0x0200e618
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
.L_0200564a:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0001
	.2byte 0x0100
	.2byte 0x1e6e
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0002
	.2byte 0x4ccc
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0xe7f0
	.2byte 0x0200
	.2byte 0x9999
	.2byte 0x0000
	.2byte 0x1333
	.2byte 0x0000
	.2byte 0x0301
	.2byte 0x0000
	.2byte 0x0927
	.2byte 0x0000
	.global FieldScene_RunActors24And25SetupWithValue929
	.thumb_func
FieldScene_RunActors24And25SetupWithValue929:
	.global Func_02005684
	.thumb_func
Func_02005684:
	push {r5, lr}
	movs r1, #0
	movs r0, #0
	bl 0x0200cfa8
	adds r5, r0, #0
	bl 0x0200e4c8
	movs r0, #24
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r1, #0
	movs r2, #0
	movs r0, #25
	bl 0x0200c8e8
.L_020056a8:
	movs r0, #0
	bl 0x0200b7d8
	adds r1, r5, #0
	movs r0, #19
	movs r2, #12
	bl 0x0200c8e8
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x0200e548
	bl 0x0200d2f4
	ldr r0, [pc, #16]
	bl 0x0200e4a8
	bl 0x0200e4d0
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000929
	.global FieldScene_RunScene3b1_020056dc
	.thumb_func
FieldScene_RunScene3b1_020056dc:
	.global Func_020056dc
	.thumb_func
Func_020056dc:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r1, #0
	movs r0, #0
	bl 0x0200cfa8
	movs r1, #0
	adds r6, r0, #0
	movs r0, #1
	bl 0x0200cfa8
	mov r8, r0
	bl 0x0200e4c8
	movs r0, #24
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #25
	movs r1, #3
	movs r2, #0
	bl 0x0200c8e8
	bl 0x0200b7b4
	mov r2, r8
	adds r1, r6, #0
	movs r0, #19
	bl 0x0200c8e8
	movs r0, #10
	movs r1, #6
	bl 0x0200e550
	ldr r5, [pc, #80]
	adds r0, r6, #0
	adds r1, r5, #0
	bl 0x0200e500
	movs r0, #11
	bl 0x0200e4f0
	adds r1, r5, #0
	mov r0, r8
	bl 0x0200e500
	movs r0, #12
	bl 0x0200e4f0
	ldr r5, [pc, #56]
	movs r0, #36
	adds r1, r5, #0
	bl 0x0200e500
	adds r1, r5, #0
	movs r0, #37
	bl 0x0200e500
	movs r0, #36
	movs r1, #3
	bl 0x0200e598
	movs r0, #37
	movs r1, #3
	bl 0x0200e598
	bl 0x0200d0e4
	bl 0x0200e4d0
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
.L_02005774:
	bx r0
	.2byte 0x0000
	.2byte 0xe840
	.2byte 0x0200
	.2byte 0xe8e4
	.2byte 0x0200
	.global FieldScene_RunActors24And25SetupWithValue92a
	.thumb_func
FieldScene_RunActors24And25SetupWithValue92a:
	.global Func_02005780
	.thumb_func
Func_02005780:
	push {r5, r6, lr}
	movs r1, #0
	movs r0, #0
	bl 0x0200cfa8
	movs r1, #0
	adds r6, r0, #0
	movs r0, #1
	bl 0x0200cfa8
	adds r5, r0, #0
	bl 0x0200e4c8
	movs r0, #24
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r1, #0
	movs r2, #0
	movs r0, #25
	bl 0x0200c8e8
	movs r0, #0
	bl 0x0200b7d8
	adds r1, r6, #0
	adds r2, r5, #0
	movs r0, #19
	bl 0x0200c8e8
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl 0x0200e548
	bl 0x0200d2f4
	ldr r0, [pc, #16]
	bl 0x0200e4a8
	bl 0x0200e4d0
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000092a
	.global FieldScene_RunExtendedFormationPresentation
	.thumb_func
FieldScene_RunExtendedFormationPresentation:
	.global Func_020057ec
	.thumb_func
Func_020057ec:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r1, #0
	movs r0, #0
	bl 0x0200cfa8
	movs r1, #0
	adds r6, r0, #0
	movs r0, #1
	bl 0x0200cfa8
	movs r1, #0
	mov r8, r0
	movs r0, #2
	bl 0x0200cfa8
	mov r10, r0
	bl 0x0200e4c8
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r1, #236
	movs r2, #152
	lsls r2, r2, #16
	movs r0, #8
	lsls r1, r1, #17
	bl 0x0200e548
	movs r0, #9
	movs r1, #5
	bl 0x0200e550
.L_02005840:
	movs r1, #220
	movs r2, #134
	lsls r2, r2, #16
	movs r0, #27
	lsls r1, r1, #17
	bl 0x0200e548
	movs r1, #15
	movs r0, #27
	bl 0x0200e598
	movs r0, #27
	bl 0x0200e4e8
	movs r1, #0
	bl 0x0200e480
	movs r5, #1
	movs r0, #16
	bl 0x0200c670
	negs r5, r5
	movs r0, #219
	movs r2, #174
	ldr r3, [pc, #944]
	adds r1, r5, #0
	lsls r0, r0, #17
	lsls r2, r2, #16
	bl 0x0200c8ac
	movs r1, #1
	movs r2, #20
	movs r0, #8
	bl 0x0200c8e8
	movs r0, #19
	bl 0x0200e660
	movs r0, #181
	bl 0x0200e660
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200e488
	movs r0, #10
	bl 0x0200e4c0
	adds r1, r5, #0
	ldr r2, [pc, #892]
	adds r0, r5, #0
	bl 0x0200e488
	movs r0, #80
	bl 0x0200e4c0
	movs r0, #181
	bl 0x0200e660
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200e488
	movs r0, #10
	bl 0x0200e4c0
	ldr r2, [pc, #848]
	adds r1, r5, #0
	adds r0, r5, #0
	bl 0x0200e488
	movs r0, #63
	bl 0x0200e660
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200e4a8
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #3
	movs r5, #192
	bl 0x0200e5f0
	lsls r5, r5, #7
	movs r0, #40
	bl 0x0200e4c0
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200c880
	ldr r0, [pc, #800]
	bl 0x0200e5a8
	movs r1, #0
	movs r2, #40
	movs r0, #3
	bl 0x0200e5c0
	movs r0, #27
	bl 0x0200c86c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200e5d0
	adds r1, r5, #0
	movs r0, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	adds r1, r5, #0
	movs r0, #2
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200e5d0
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r2, #60
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200e5e8
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #2
	bl 0x0200c880
	movs r1, #1
	movs r0, #2
	bl 0x0200e580
	movs r0, #2
	bl 0x0200c86c
	movs r0, #0
	movs r1, #1
	bl 0x0200e578
	movs r0, #1
	movs r1, #1
	bl 0x0200e578
	movs r1, #1
	movs r0, #3
	bl 0x0200e580
	movs r0, #10
	bl 0x0200e4c0
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #160
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r1, #0
	movs r0, #27
	bl 0x0200e598
	movs r0, #27
	bl 0x0200e4e8
	movs r1, #1
	bl 0x0200e480
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #215
	movs r2, #134
	movs r0, #27
	lsls r1, r1, #1
	bl 0x0200e530
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #27
	bl 0x0200c880
	movs r1, #2
	movs r0, #27
	bl 0x0200e578
	movs r0, #27
	bl 0x0200c86c
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200e578
	mov r0, r8
	movs r1, #1
	bl 0x0200e578
	mov r0, r10
	movs r1, #1
	bl 0x0200e578
	movs r0, #13
	movs r1, #1
	bl 0x0200e580
	movs r1, #129
	adds r0, r6, #0
	lsls r1, r1, #1
	bl 0x0200e5f0
	movs r1, #129
	mov r0, r8
	lsls r1, r1, #1
	bl 0x0200e5f0
	movs r1, #129
	mov r0, r10
	lsls r1, r1, #1
	bl 0x0200e5f0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #13
	bl 0x0200e5f0
	movs r0, #40
	bl 0x0200e4c0
	adds r1, r6, #0
	movs r0, #12
	movs r2, #0
	bl 0x0200c8e8
	mov r1, r8
	movs r0, #12
	movs r2, #1
	bl 0x0200c8e8
	mov r1, r10
	movs r0, #12
	movs r2, #0
	bl 0x0200c8e8
	movs r0, #11
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r1, #208
	adds r0, r6, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #176
	mov r0, r8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #208
	mov r0, r10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
.L_02005ae0:
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r0, #27
	movs r1, #2
	bl 0x0200e578
	movs r0, #27
	movs r1, #0
	bl 0x0200e5b8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200e5d0
	movs r1, #220
	movs r0, #27
	lsls r1, r1, #1
	movs r2, #134
	movs r5, #128
	bl 0x0200e530
	lsls r5, r5, #8
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl 0x0200e548
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200c880
	movs r1, #1
	movs r0, #1
	bl 0x0200e580
	movs r0, #1
	bl 0x0200c86c
	movs r2, #0
	movs r0, #2
	movs r1, #0
	bl 0x0200e5d0
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200c880
	movs r0, #0
	movs r1, #3
	bl 0x0200e550
	movs r0, #1
	movs r1, #3
	bl 0x0200e550
	movs r0, #2
	movs r1, #3
	bl 0x0200e550
	movs r0, #3
	movs r1, #3
	bl 0x0200e560
	movs r1, #128
	adds r2, r5, #0
	movs r0, #1
	lsls r1, r1, #9
	bl 0x0200e4f8
	movs r1, #128
	adds r2, r5, #0
	movs r0, #2
	lsls r1, r1, #9
	bl 0x0200e4f8
	movs r1, #128
	adds r2, r5, #0
	movs r0, #3
	lsls r1, r1, #9
	bl 0x0200e4f8
	ldr r5, [pc, #100]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200e500
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200e500
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200e510
	ldr r0, [pc, #80]
	bl 0x0200e4a8
	ldr r2, [pc, #76]
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, [pc, #72]
	bl 0x0200e428
	movs r1, #0
	movs r2, #0
	movs r0, #23
	bl 0x0200c8e8
	movs r0, #27
	bl 0x0200e4f0
	ldr r0, [pc, #52]
	bl 0x0200e4b0
	ldr r0, [pc, #52]
	bl 0x0200e4b0
	bl 0x0200e4d0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0001
	.2byte 0x0100
	.2byte 0xe666
	.2byte 0x0000
	.2byte 0x1ec1
	.2byte 0x0000
	.4byte 0x0200e818
	.4byte 0x00000302
	.4byte 0x0200ff84
	.4byte 0x0200dc49
	.4byte 0x0000012f
	.4byte 0x00000927
	.section .text.x0200dca4,"ax",%progbits
	.align 2
	.global RunActorsEightAndNineMapEvent
	.thumb_func
RunActorsEightAndNineMapEvent:
	.global Func_02005ca4
	.thumb_func
Func_02005ca4:
	push {lr}
	bl 0x0200e4c8
	movs r0, #15
	movs r1, #1
	movs r2, #0
	bl 0x0200c8e8
	movs r1, #234
	movs r2, #154
	movs r3, #128
	lsls r3, r3, #8
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #9
	bl 0x0200c890
	movs r2, #20
	movs r0, #8
	movs r1, #1
	bl 0x0200c8e8
	movs r1, #2
	movs r0, #9
	bl 0x0200e580
	movs r0, #20
	bl 0x0200e4c0
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #80
	bl 0x0200e5d0
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x0200e5d0
	movs r1, #3
	movs r0, #8
	bl 0x0200e560
	movs r0, #20
	bl 0x0200e4c0
	movs r0, #9
	movs r1, #21
	movs r2, #0
	bl 0x0200c8e8
	pop {r0}
	bx r0
	.section .text.x0200e110,"ax",%progbits
	.align 2
	.global FieldScene_RunScene3b1_02006110
	.thumb_func
FieldScene_RunScene3b1_02006110:
	.global Func_02006110
	.thumb_func
Func_02006110:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #0
	movs r0, #0
	bl 0x0200cfa8
	movs r1, #0
	mov r8, r0
	movs r0, #1
	bl 0x0200cfa8
	movs r1, #0
	adds r6, r0, #0
	movs r0, #2
	bl 0x0200cfa8
	movs r1, #0
	adds r5, r0, #0
	movs r0, #3
	bl 0x0200cfa8
	mov r10, r0
	bl 0x0200e4c8
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200c8e8
	movs r1, #236
	movs r3, #160
	lsls r1, r1, #1
	lsls r3, r3, #7
	movs r0, #8
	movs r2, #144
	bl 0x0200c890
	movs r3, #204
	lsls r3, r3, #1
	mov r9, r3
	movs r3, #192
	mov r1, r9
	movs r0, #27
	lsls r3, r3, #6
	movs r2, #142
	bl 0x0200c890
	ldr r3, [pc, #532]
	movs r7, #224
	ldr r2, [r3]
	mov r11, r3
	ldr r3, [pc, #528]
	lsls r7, r7, #1
	str r3, [r2, r7]
	bl 0x0200e630
	bl 0x0200e640
	movs r0, #40
	bl 0x0200e4c0
	movs r1, #1
	movs r0, #27
	bl 0x0200e580
	ldr r0, [pc, #504]
	bl 0x0200e5a8
	movs r0, #27
	bl 0x0200c86c
	mov r0, r8
	movs r1, #2
	bl 0x0200e578
	adds r0, r6, #0
	movs r1, #2
	bl 0x0200e578
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200e578
	mov r0, r10
	movs r1, #2
	bl 0x0200e580
	movs r0, #20
	bl 0x0200e4c0
	mov r0, r8
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	adds r0, r6, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #128
	mov r0, r10
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200e5d0
	movs r1, #128
	movs r2, #128
	mov r0, r8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #128
	movs r2, #128
	adds r0, r6, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #128
	movs r2, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #128
	movs r2, #128
	mov r0, r10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200e4f8
	movs r1, #235
	mov r0, r8
	lsls r1, r1, #1
	movs r2, #172
	bl 0x0200e528
	movs r1, #205
	adds r0, r6, #0
	lsls r1, r1, #1
	movs r2, #172
	bl 0x0200e528
	movs r1, #235
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #204
	bl 0x0200e528
	movs r1, #205
	movs r2, #204
	mov r0, r10
	lsls r1, r1, #1
	bl 0x0200e530
	mov r0, r8
	movs r1, #1
	bl 0x0200e550
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200e550
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200e550
	movs r1, #208
	adds r0, r6, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #176
	mov r0, r8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #208
	mov r0, r10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200e5d0
	movs r1, #176
	movs r2, #20
	adds r0, r5, #0
	lsls r1, r1, #8
	bl 0x0200e5d0
	movs r1, #1
	movs r0, #27
	bl 0x0200e580
	movs r0, #27
	bl 0x0200c86c
	mov r0, r8
	movs r1, #3
	bl 0x0200e550
	adds r0, r6, #0
	movs r1, #3
	bl 0x0200e550
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200e550
	mov r0, r10
	movs r1, #3
	bl 0x0200e560
	movs r0, #27
	bl 0x0200c86c
	mov r0, r8
	movs r1, #3
	bl 0x0200e550
	adds r0, r6, #0
	movs r1, #3
	bl 0x0200e550
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200e550
	movs r5, #128
	mov r0, r10
	movs r1, #3
	bl 0x0200e560
	lsls r5, r5, #8
.L_02006302:
	movs r2, #0
	movs r0, #27
	movs r1, #0
.L_02006308:
	bl 0x0200e5d0
	adds r1, r5, #0
	movs r0, #0
	bl 0x0200c880
	movs r0, #0
	movs r1, #3
.L_02006318:
	bl 0x0200e560
	movs r0, #27
	movs r1, #3
	bl 0x0200e560
	movs r1, #128
.L_02006326:
	adds r2, r5, #0
.L_02006328:
	movs r0, #27
	lsls r1, r1, #9
	bl 0x0200e4f8
	mov r1, r9
	movs r0, #27
	movs r2, #132
.L_02006336:
	bl 0x0200e530
	movs r1, #222
	movs r0, #27
	lsls r1, r1, #1
	movs r2, #132
	bl 0x0200e530
.L_02006346:
	movs r0, #27
.L_02006348:
	movs r1, #0
	movs r2, #0
	bl 0x0200e548
	mov r3, r11
	ldr r2, [r3]
	ldr r3, [pc, #68]
	str r3, [r2, r7]
.L_02006358:
	bl 0x0200e638
	bl 0x0200e640
	ldr r0, [pc, #60]
	ldr r1, [pc, #64]
.L_02006364:
	bl 0x0200e3ac
	movs r1, #153
	lsls r1, r1, #4
	ldr r0, [pc, #56]
	bl 0x0200e3ac
	movs r0, #138
.L_02006374:
	lsls r0, r0, #4
	bl 0x0200e4b0
	movs r0, #10
	bl 0x0200e618
	pop {r3, r5, r6, r7}
.L_02006382:
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0201
	.2byte 0x0000
	.2byte 0x1f78
	.2byte 0x0000
	.2byte 0x0202
	.2byte 0x0000
	.2byte 0x092c
	.2byte 0x0000
	.2byte 0x0935
	.2byte 0x0000
	.2byte 0x0917
	.2byte 0x0000
	.global Func_020063ac
	.thumb_func
Func_020063ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #0
	mov r8, r0
	adds r7, r1, #0
	mov r10, r3
	mov r9, r3
	movs r6, #0
	b .L_020063ac_0
.L_020063ac_2:
	movs r3, #1
	add r10, r3
	adds r6, #1
.L_020063ac_0:
	cmp r6, #8
	bhi .L_020063ac_1
	mov r3, r8
	adds r5, r3, r6
	adds r0, r5, #0
	bl 0x0200e4a0
	cmp r0, #0
	beq .L_020063ac_2
	adds r0, r5, #0
	bl 0x0200e4b0
.L_020063ac_1:
	movs r6, #0
	b .L_020063ac_3
	.2byte 0x2301
	.2byte 0x4499
	.2byte 0x3601
.L_020063ac_3:
	cmp r6, #8
	bhi 0x0200e402
.L_020063f0:
	adds r5, r7, r6
	adds r0, r5, #0
	bl 0x0200e4a0
	cmp r0, #0
	beq 0x0200e3e6
	adds r0, r5, #0
	bl 0x0200e4b0
	mov r3, r10
	adds r0, r7, r3
	bl 0x0200e4a8
	mov r0, r8
	add r0, r9
	bl 0x0200e4a8
.L_02006412:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
.L_02006418:
	mov r10, r6
.L_0200641a:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.include "games/THE BROKEN SEAL/SRC/FIELD/FUNE_HEYA/IMPORT.INC"
	.section .rodata.part1,"a",%progbits
	.global FuneHeya_TurnSteps
FuneHeya_TurnSteps:
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
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01f40000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00008000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02a20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneHeya_ProgressTableA
FuneHeya_ProgressTableA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01b20000
	.4byte 0x00000000
	.4byte 0x00a60000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01b60000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffb000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global FuneHeya_EntryActionScript
FuneHeya_EntryActionScript:
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02008059
	.global FuneHeya_ProgressTableB
FuneHeya_ProgressTableB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global FuneHeya_ProgressTableC
FuneHeya_ProgressTableC:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x00ba0000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x020080b1
	.4byte 0x00000022
	.4byte 0x020082f5
	.global FuneHeya_AnchorObject
FuneHeya_AnchorObject:
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000098
	.4byte 0xc0000248
	.4byte 0x001e0000
	.4byte 0x010e017c
	.4byte 0x0000026c
	.4byte 0xffff0002
	.4byte 0x000000a0
	.4byte 0x00000228
	.4byte 0x001e0000
	.4byte 0x010e017c
	.4byte 0x0000026c
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0x80000088
	.4byte 0x00190000
	.4byte 0x01090050
	.4byte 0x0000015e
	.4byte 0xffff0004
	.4byte 0x00000198
	.4byte 0x80000088
	.4byte 0x013b0000
	.4byte 0x022b0050
	.4byte 0x0000019a
	.4byte 0xffff0005
	.4byte 0x000001b8
	.4byte 0xc00002d8
	.4byte 0x013b0000
	.4byte 0x022b01f4
	.4byte 0x000002fd
	.4byte 0xffff000a
	.4byte 0x000001b8
	.4byte 0xc00002d8
	.4byte 0x013b0000
	.4byte 0x022b01f4
	.4byte 0x000002fd
	.4byte 0xffff000b
	.4byte 0x00000098
	.4byte 0xc0000248
	.4byte 0x001e0000
	.4byte 0x010e017c
	.4byte 0x0000026c
	.4byte 0xffff000c
	.4byte 0x000001b8
	.4byte 0xc0000156
	.4byte 0x013b0000
	.4byte 0x022b0050
	.4byte 0x0000019a
	.4byte 0xffff000d
	.4byte 0x000001b8
	.4byte 0xc00002d8
	.4byte 0x013b0000
	.4byte 0x022b01f4
	.4byte 0x000002fd
	.4byte 0xffff000e
	.4byte 0x000001b8
	.4byte 0xc00002d8
	.4byte 0x013b0000
	.4byte 0x022b01f4
	.4byte 0x000002fd
	.4byte 0xffff000f
	.4byte 0x00000198
	.4byte 0x80000088
	.4byte 0x013b0000
	.4byte 0x022b0050
	.4byte 0x0000019a
	.4byte 0xffff0010
	.4byte 0x00000198
	.4byte 0x80000088
	.4byte 0x013b0000
	.4byte 0x022b0050
	.4byte 0x0000019a
	.4byte 0xffff0011
	.4byte 0x00000198
	.4byte 0x80000088
	.4byte 0x013b0000
	.4byte 0x022b0050
	.4byte 0x0000019a
	.4byte 0xffff0012
	.4byte 0x00000198
	.4byte 0x80000088
	.4byte 0x013b0000
	.4byte 0x022b0050
	.4byte 0x0000019a
	.4byte 0xffff0013
	.4byte 0x00000198
	.4byte 0x80000088
	.4byte 0x013b0000
	.4byte 0x022b0050
	.4byte 0x0000019a
	.4byte 0xffff0014
	.4byte 0x00000198
	.4byte 0x80000088
	.4byte 0x013b0000
	.4byte 0x022b0050
	.4byte 0x0000019a
	.4byte 0xffff0015
	.4byte 0x00000198
	.4byte 0x80000088
	.4byte 0x01310000
	.4byte 0x0235003c
	.4byte 0x000001ae
	.4byte 0xffff0016
	.4byte 0x000001bc
	.4byte 0xc0000278
	.4byte 0x013b0000
	.4byte 0x022b01f4
	.4byte 0x000002fd
	.4byte 0xffff0017
	.4byte 0x00000198
	.4byte 0x80000088
	.4byte 0x013b0000
	.4byte 0x022b0050
	.4byte 0x0000019a
	.4byte 0xffff0018
	.4byte 0x00000198
	.4byte 0x80000088
	.4byte 0x013b0000
	.4byte 0x022b0050
	.4byte 0x0000019a
	.4byte 0xffff001e
	.4byte 0x000000a6
	.4byte 0xc000023a
	.4byte 0x001e0000
	.4byte 0x010e017c
	.4byte 0x0000026c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000006f
	.4byte 0x0010106d
	.4byte 0x0020406f
	.4byte 0x0030206d
	.4byte 0x0040206f
	.4byte 0x0050306d
	.4byte 0x00a0a06d
	.4byte 0x00b0b06f
	.4byte 0x00c0c06f
	.4byte 0x00d41002
	.4byte 0x00e0b06d
	.4byte 0x00f0c06d
	.4byte 0x01042002
	.4byte 0x01143002
	.4byte 0x01244002
	.4byte 0x0130f06d
	.4byte 0x01445002
	.4byte 0x0154b002
	.4byte 0x000001ff
	.global FuneHeya_StepScriptA
FuneHeya_StepScriptA:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHeya_StepScriptB
FuneHeya_StepScriptB:
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x01be0000
	.4byte 0x00000000
	.4byte 0x026c0000
	.4byte 0x00023000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02720000
	.4byte 0x00028000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x01c60000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x0002b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHeya_StepScriptC
FuneHeya_StepScriptC:
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x01e20000
	.4byte 0x00000000
	.4byte 0x025c0000
	.4byte 0x00015000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x01da0000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00005000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x02b20000
	.4byte 0x0000d000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0000d000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x02740000
	.4byte 0x00000000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02940000
	.4byte 0x0000b000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
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
	.global FuneHeya_StepScriptD
FuneHeya_StepScriptD:
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x01ce0000
	.4byte 0x00000000
	.4byte 0x02640000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHeya_StepScriptE
FuneHeya_StepScriptE:
	.4byte 0xffff00f4
	.4byte 0x0200e960
	.4byte 0x00960000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHeya_StepScriptF
FuneHeya_StepScriptF:
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x0000b000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x0000b000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x00015000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00820000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x008e0000
	.4byte 0x00000000
	.4byte 0x01be0000
	.4byte 0x00005000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x009c0000
	.4byte 0x00000000
	.4byte 0x01d20000
	.4byte 0x00008000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00018000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000d000
	.4byte 0xffff00f4
	.4byte 0x0200e960
	.4byte 0x00960000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHeya_StepScriptG
FuneHeya_StepScriptG:
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x008a0000
	.4byte 0x00000000
	.4byte 0x01be0000
	.4byte 0x00003000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02140000
	.4byte 0x0001d000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x01ec0000
	.4byte 0x00008000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01ec0000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01e00000
	.4byte 0x0000d000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x020e0000
	.4byte 0x0000b000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00ac0000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00b40000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x0000d000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x007a0000
	.4byte 0x00000000
	.4byte 0x02040000
	.4byte 0x00000000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00018000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000d000
	.4byte 0xffff00f4
	.4byte 0x0200e960
	.4byte 0x00960000
	.4byte 0x00000000
	.4byte 0x02100000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneHeya_StepScriptH
FuneHeya_StepScriptH:
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01b60000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00005000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01b60000
	.4byte 0x00000000
	.4byte 0x01760000
	.4byte 0x0000d000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x019a0000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x0000d000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x01d60000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x0000b000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x019a0000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x0000d000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x01d60000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x0000b000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x019a0000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x0000d000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x01d60000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x0000b000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x019a0000
	.4byte 0x00000000
	.4byte 0x013a0000
	.4byte 0x0000d000
	.4byte 0xffff0090
	.4byte 0x00000001
	.4byte 0x01d60000
	.4byte 0x00000000
	.4byte 0x013a0000
	.4byte 0x0000b000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bc
	.4byte 0x00000007
	.4byte 0x019a0000
	.4byte 0x00000000
	.4byte 0x00ae0000
	.4byte 0x00000000
	.4byte 0xffff00bc
	.4byte 0x00000007
	.4byte 0x01d60000
	.4byte 0x00000000
	.4byte 0x00ae0000
	.4byte 0x01008000
	.4byte 0xffff00bc
	.4byte 0x00000007
	.4byte 0x019a0000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x01000000
	.4byte 0xffff00bc
	.4byte 0x00000007
	.4byte 0x01d60000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x01008000
	.4byte 0xffff00bc
	.4byte 0x00000007
	.4byte 0x019a0000
	.4byte 0x00000000
	.4byte 0x011e0000
	.4byte 0x01000000
	.4byte 0xffff00bc
	.4byte 0x00000007
	.4byte 0x01d60000
	.4byte 0x00000000
	.4byte 0x011e0000
	.4byte 0x01008000
	.4byte 0xffff00bc
	.4byte 0x00000007
	.4byte 0x019a0000
	.4byte 0x00000000
	.4byte 0x013c0000
	.4byte 0x01000000
	.4byte 0xffff00bc
	.4byte 0x00000007
	.4byte 0x01d60000
	.4byte 0x00000000
	.4byte 0x013c0000
	.4byte 0x01008000
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
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
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
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x01da0000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00028000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x01ca0000
	.4byte 0x00000000
	.4byte 0x01360000
	.4byte 0x01028000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x01da0000
	.4byte 0x00000000
	.4byte 0x014c0000
	.4byte 0x01028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c4
	.4byte 0x00000001
	.4byte 0x01da0000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00028000
	.4byte 0xffff00c4
	.4byte 0x00000001
	.4byte 0x01da0000
	.4byte 0x00000000
	.4byte 0x014c0000
	.4byte 0x01028000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x01ca0000
	.4byte 0x00000000
	.4byte 0x01360000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x01da0000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00028000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x01da0000
	.4byte 0x00000000
	.4byte 0x014c0000
	.4byte 0x01028000
	.4byte 0xffff00c5
	.4byte 0x00000001
	.4byte 0x01ca0000
	.4byte 0x00000000
	.4byte 0x01360000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000033
	.4byte 0x0f920064
	.4byte 0x001000e5
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
	.4byte 0x0922000b
	.4byte 0x02009b35
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020087f9
	.4byte 0x00000000
	.4byte 0x09250009
	.4byte 0x00001d4f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001e18
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020088a9
	.4byte 0x00000000
	.4byte 0x0925000b
	.4byte 0x00001d51
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001e1c
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001e12
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02009895
	.4byte 0x00008d15
	.4byte 0x09250008
	.4byte 0x00001d52
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001e1d
	.4byte 0x00008d15
	.4byte 0x09250009
	.4byte 0x00001d53
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001e1e
	.4byte 0x00008d15
	.4byte 0x0925000a
	.4byte 0x00001d54
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001e1f
	.4byte 0x00008d15
	.4byte 0x0925000b
	.4byte 0x00001d55
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001e20
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001e21
	.4byte 0x00008d15
	.4byte 0x0925000f
	.4byte 0x00001f7c
	.4byte 0x00008d15
	.4byte 0x093e000f
	.4byte 0x00001f7e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001f82
	.4byte 0x00000033
	.4byte 0x0f930065
	.4byte 0x00200017
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
	.4byte 0xffff0011
	.4byte 0x02009895
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001f80
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020088ed
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020089b5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008a81
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008b85
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008cc9
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008e15
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008ee1
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008fad
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02009079
	.4byte 0x00008d15
	.4byte 0x03000008
	.4byte 0x00001e95
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001ea7
	.4byte 0x00008d15
	.4byte 0x0300000a
	.4byte 0x00001e96
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001ea8
	.4byte 0x00008d15
	.4byte 0x0300000b
	.4byte 0x00001e97
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001ea9
	.4byte 0x00008d15
	.4byte 0x0300000c
	.4byte 0x00001e98
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001eaa
	.4byte 0x00008d15
	.4byte 0x0300000d
	.4byte 0x00001e99
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001eab
	.4byte 0x00008d15
	.4byte 0x0300000e
	.4byte 0x00001e9a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001eac
	.4byte 0x00008d15
	.4byte 0x0300000f
	.4byte 0x00001e9b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001ead
	.4byte 0x00008d15
	.4byte 0x03000010
	.4byte 0x00001e9c
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001eae
	.4byte 0x00008d15
	.4byte 0x03000009
	.4byte 0x00001e9d
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001eaf
	.4byte 0x00000033
	.4byte 0x0f930065
	.4byte 0x00200017
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
	.4byte 0xffff0011
	.4byte 0x02009895
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001f52
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001f3f
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001f40
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001f41
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001f42
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001f43
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001f44
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001f45
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001f46
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x02008a81
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001f49
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001f4a
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001f4b
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001f4c
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001f4d
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001f4e
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001f4f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001f50
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001f51
	.4byte 0x00000033
	.4byte 0x0f930065
	.4byte 0x00200017
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x02008729
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008671
	.4byte 0x00000000
	.4byte 0x09210009
	.4byte 0x00001d32
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001dce
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001dcf
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001dd0
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008709
	.4byte 0x00008d15
	.4byte 0x09210008
	.4byte 0x00001d33
	.4byte 0x00008d15
	.4byte 0x09250008
	.4byte 0x00001dd5
	.4byte 0x00008d15
	.4byte 0x09280008
	.4byte 0x00001e07
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001eb6
	.4byte 0x00008d15
	.4byte 0x0921000a
	.4byte 0x00001d34
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001dda
	.4byte 0x00008d15
	.4byte 0x09210009
	.4byte 0x00001d35
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001dd6
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001dd7
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001dd8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001dd9
	.4byte 0x00000002
	.4byte 0x0920000a
	.4byte 0x02009a61
	.4byte 0x0000e814
	.4byte 0x09250008
	.4byte 0x0200a7d9
	.4byte 0x0000c423
	.4byte 0x0f940066
	.4byte 0x001000b5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001f65
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001f66
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001f67
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001f68
	.4byte 0x00000023
	.4byte 0x0f940066
	.4byte 0x001000b5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001f07
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001f08
	.4byte 0x00000023
	.4byte 0x0f940066
	.4byte 0x001000b5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x08a00008
	.4byte 0x00001d79
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001f33
	.4byte 0x00000000
	.4byte 0x08a00009
	.4byte 0x00001d7a
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001f34
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001d7b
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001d7c
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001d7d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001d7e
	.4byte 0x00000000
	.4byte 0x08a0000e
	.4byte 0x00001d7f
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001f35
	.4byte 0x00000000
	.4byte 0x08a0000f
	.4byte 0x00001d80
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001f36
	.4byte 0x00000000
	.4byte 0x08a00010
	.4byte 0x00001d81
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001f37
	.4byte 0x00000000
	.4byte 0x08a00011
	.4byte 0x00001d82
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001f38
	.4byte 0x00008d15
	.4byte 0x08a00008
	.4byte 0x00001d83
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001f39
	.4byte 0x00008d15
	.4byte 0x08a00009
	.4byte 0x00001d84
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001f3a
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001d85
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001d86
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001d87
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001d88
	.4byte 0x00008d15
	.4byte 0x08a0000e
	.4byte 0x00001d89
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001f3b
	.4byte 0x00008d15
	.4byte 0x08a0000f
	.4byte 0x00001d8a
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001f3c
	.4byte 0x00008d15
	.4byte 0x08a00010
	.4byte 0x00001d8b
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001f3d
	.4byte 0x00008d15
	.4byte 0x08a00011
	.4byte 0x00001d8c
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001f3e
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02009a09
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x02009979
	.4byte 0x00000000
	.4byte 0xffff0025
	.4byte 0x02009979
	.4byte 0x00000000
	.4byte 0xffff0026
	.4byte 0x02009979
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001e4a
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001e4b
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001e4d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001e4e
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001e4f
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001e50
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001e51
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001e52
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001e4c
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001e5c
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001e5d
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001e5e
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00001e5f
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001e60
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001e61
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001e62
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001e63
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001e64
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001e53
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001e54
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001e56
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001e57
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001e58
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001e59
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001e5a
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001e5b
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001e55
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001e65
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001e66
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001e67
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001e68
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001e69
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001e6a
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001e6b
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001e6c
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e6d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000013
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001ec7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001ec8
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001ec9
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001eca
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001ecb
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001ecc
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001ecd
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02009325
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02009379
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020093cd
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02009421
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x0200945d
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020094b1
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02009505
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x02009559
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001ed3
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001ed4
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001ed5
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001ed6
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001ed7
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001ed8
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001ed9
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001eda
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x02009595
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x020095e9
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0200963d
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x02009691
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x020096cd
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x02009721
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x02009775
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x020097c9
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001ee0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001ef3
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001ef4
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001ef5
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001ef6
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001ef7
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001ef8
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001ef9
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001efa
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001efb
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001efc
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001efd
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001efe
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.global FuneHeya_CueTimer
FuneHeya_CueTimer:
	.space 4
