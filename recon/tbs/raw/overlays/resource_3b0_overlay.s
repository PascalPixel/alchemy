.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	ldr r3, [pc, #60]
	ldr r3, [r3]
	ldr r3, [r3]
	ldr r4, [pc, #56]
	ldmia r3!, {r1}
	ldr r5, [pc, #56]
	ldr r2, [r3]
	ldr r3, [r4]
	subs r1, r1, r3
	ldr r3, [r5]
	adds r3, r3, r1
	str r3, [r0, #8]
	ldr r3, [r4, #4]
	subs r2, r2, r3
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r5, #4]
	asrs r2, r2, #1
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #80]
	movs r1, #192
	ldrh r3, [r2, #30]
	lsls r1, r1, #3
	adds r3, r3, r1
	strh r3, [r2, #30]
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001e70
	.4byte 0x02009938
	.4byte 0x02009930
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {r5, lr}
	adds r5, r0, #0
	bl 0x020091c4
	lsls r0, r0, #6
	lsrs r0, r0, #16
	cmp r0, #6
	bne .L_0200007c_0
	movs r3, #208
	b .L_0200007c_1
.L_0200007c_0:
	cmp r0, #9
	bne .L_0200007c_2
	movs r3, #176
.L_0200007c_1:
	lsls r3, r3, #8
	strh r3, [r5, #6]
.L_0200007c_2:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	push {lr}
	movs r2, #128
	ldr r3, [r0, #24]
	lsls r2, r2, #9
	cmp r3, r2
	bge .L_020000a4_0
	adds r3, #160
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, #160
	str r3, [r0, #28]
.L_020000a4_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020000c0
	.thumb_func
Func_020000c0:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #100
	movs r1, #0
	ldrsh r3, [r6, r1]
	cmp r3, #9
	bne .L_020000c0_0
	movs r3, #0
	str r3, [r5, #76]
	b .L_020000c0_1
.L_020000c0_0:
	cmp r3, #0
	beq .L_020000c0_2
	bl 0x020091c4
	ldr r3, [r5, #76]
	lsls r0, r0, #11
	lsrs r0, r0, #16
	ldr r2, [pc, #140]
	subs r3, r3, r0
	str r3, [r5, #76]
	cmp r3, r2
	bge .L_020000c0_1
	movs r3, #0
	b .L_020000c0_3
.L_020000c0_2:
	bl 0x020091c4
	ldr r3, [r5, #76]
	lsls r0, r0, #11
	lsrs r0, r0, #16
	movs r1, #192
	adds r3, r3, r0
	lsls r1, r1, #8
	str r3, [r5, #76]
	cmp r3, r1
	ble .L_020000c0_1
	movs r3, #1
.L_020000c0_3:
	strh r3, [r6]
.L_020000c0_1:
	ldr r1, [pc, #104]
	ldr r2, [r5, #8]
	adds r3, r2, r1
	ldr r1, [pc, #104]
	cmp r3, r1
	bhi .L_020000c0_4
	ldr r3, [r5, #76]
	adds r3, r2, r3
	str r3, [r5, #8]
.L_020000c0_4:
	adds r6, r5, #0
	adds r6, #102
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #9
	bne .L_020000c0_5
	movs r3, #0
	str r3, [r5, #12]
	b .L_020000c0_6
.L_020000c0_5:
	cmp r3, #0
	beq .L_020000c0_7
	bl 0x020091c4
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r2, [r5, #12]
	lsls r3, r3, #14
	lsrs r3, r3, #16
	subs r2, r2, r3
	str r2, [r5, #12]
	cmp r2, #0
	bge .L_020000c0_6
	movs r3, #0
	b .L_020000c0_8
.L_020000c0_7:
	bl 0x020091c4
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r2, [r5, #12]
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #13
	str r2, [r5, #12]
	cmp r2, r3
	ble .L_020000c0_6
	movs r3, #1
.L_020000c0_8:
	strh r3, [r6]
.L_020000c0_6:
	movs r0, #1
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0xffff4000
	.4byte 0xffd7ffff
	.4byte 0x0117fffe
	.global Func_02000180
	.thumb_func
Func_02000180:
	push {lr}
	ldr r1, [r0, #80]
	ldrb r3, [r1, #9]
	movs r2, #12
	orrs r3, r2
	strb r3, [r1, #9]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #52]
	movs r2, #128
	ldr r3, [r0, #24]
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_02000180_0
	ldr r2, [pc, #32]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	b .L_02000180_1
.L_02000180_0:
	movs r3, #0
	str r3, [r0, #8]
	str r3, [r0, #12]
	str r3, [r0, #16]
	str r3, [r0, #36]
	str r3, [r0, #40]
	str r3, [r0, #44]
.L_02000180_1:
	movs r0, #1
	pop {r1}
	bx r1
	.4byte 0xfffffc00
	.global Func_020001c8
	.thumb_func
Func_020001c8:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020095c0
	.global Func_020001d0
	.thumb_func
Func_020001d0:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009680
	.global Func_020001d8
	.thumb_func
Func_020001d8:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020096a0
	.global Func_020001e0
	.thumb_func
Func_020001e0:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020096c4
	.global Func_020001e8
	.thumb_func
Func_020001e8:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200988c
	.global Func_020001f0
	.thumb_func
Func_020001f0:
	push {lr}
	bl 0x020092dc
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020001fc
	.thumb_func
Func_020001fc:
	push {lr}
	ldr r0, [pc, #52]
	bl 0x020091fc
	cmp r0, #0
	bne .L_020001fc_0
	ldr r0, [pc, #44]
	bl 0x020091fc
	cmp r0, #0
	beq .L_020001fc_1
.L_020001fc_0:
	bl 0x02009214
	movs r0, #232
	movs r1, #3
	bl 0x02009304
	movs r1, #0
	movs r0, #232
	bl 0x0200922c
	ldr r0, [pc, #20]
	bl 0x02009204
	bl 0x0200921c
.L_020001fc_1:
	pop {r0}
	bx r0
	.4byte 0x00000923
	.4byte 0x00000922
	.4byte 0x00000924
	.global Func_02000240
	.thumb_func
Func_02000240:
	push {r5, lr}
	movs r0, #162
	lsls r0, r0, #1
	bl 0x02009204
	ldr r3, [pc, #396]
	movs r0, #224
	ldr r3, [r3]
	lsls r0, r0, #1
	ldr r2, [pc, #392]
	adds r3, r3, r0
	str r2, [r3]
	ldr r0, [pc, #388]
	bl 0x020091fc
	cmp r0, #0
	bne .L_02000240_0
	ldr r0, [pc, #384]
	bl 0x020091fc
	cmp r0, #0
	beq .L_02000240_1
.L_02000240_0:
	ldr r0, [pc, #376]
	bl 0x020091fc
	cmp r0, #0
	bne .L_02000240_1
	movs r0, #138
	lsls r0, r0, #4
	bl 0x020091fc
	cmp r0, #0
	bne .L_02000240_1
	ldr r5, [pc, #360]
	bl 0x020091c4
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5]
	ldr r5, [pc, #352]
	bl 0x020091c4
	lsls r0, r0, #16
	lsrs r0, r0, #16
	movs r1, #200
	str r0, [r5]
	lsls r1, r1, #4
	ldr r0, [pc, #340]
	bl 0x020091bc
.L_02000240_1:
	ldr r0, [pc, #336]
	bl 0x020091fc
	cmp r0, #0
	beq .L_02000240_2
	ldr r0, [pc, #312]
	bl 0x020091fc
	cmp r0, #0
	bne .L_02000240_2
	movs r1, #164
	movs r2, #164
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009264
.L_02000240_2:
	ldr r1, [pc, #308]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	subs r3, #1
	cmp r3, #13
	bhi .L_02000240_3
	ldr r2, [pc, #292]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	strh r0, [r3, #24]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r2, #30]
	lsls r0, r0, #8
	strh r0, [r5, #26]
	lsls r0, r0, #8
	strh r6, [r7, #26]
	lsls r0, r0, #8
	strh r0, [r3, #28]
	lsls r0, r0, #8
	strh r2, [r6, #28]
	lsls r0, r0, #8
	strh r4, [r1, #30]
	lsls r0, r0, #8
	ldr r0, [pc, #232]
	bl 0x020091fc
	cmp r0, #0
	bne .L_02000240_3
	movs r0, #0
	bl 0x02009234
	adds r5, r0, #0
	bl 0x02009214
	bl 0x020092e4
	movs r3, #224
	lsls r3, r3, #14
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	str r3, [r5, #12]
	negs r0, r0
	movs r3, #0
	bl 0x020092c4
	movs r0, #1
	bl 0x020091b4
	movs r0, #0
	movs r1, #0
	bl 0x020092bc
	bl 0x020091dc
	movs r0, #1
	bl 0x020091b4
	bl 0x0200921c
	b .L_02000240_3
	.2byte 0x481e
	.2byte 0xf000
	.2byte 0xff47
	.2byte 0x2800
	.2byte 0xd002
	.2byte 0xf000
	.2byte 0xf879
	.2byte 0xe02b
	.2byte 0xf000
	.2byte 0xf848
	.2byte 0xe028
	.2byte 0x20e2
	.2byte 0x4b21
	.2byte 0x0040
	.2byte 0x180a
	.2byte 0x8013
	.2byte 0x23e3
	.2byte 0x005b
	.2byte 0x18ca
	.2byte 0x231e
	.2byte 0x8013
	.2byte 0xf000
	.2byte 0xf8e7
	.2byte 0xe01b
	.2byte 0x20e2
	.2byte 0x4b1b
	.2byte 0x0040
	.2byte 0x180a
	.2byte 0x8013
	.2byte 0x23e3
	.2byte 0x005b
	.2byte 0x18ca
	.2byte 0x231e
	.2byte 0x8013
	.2byte 0xf000
	.2byte 0xfa00
	.2byte 0xe00e
	.2byte 0x20e2
	.2byte 0x4b14
	.2byte 0x0040
	.2byte 0x180a
	.2byte 0x8013
	.2byte 0x23e3
	.2byte 0x005b
	.2byte 0x18ca
	.2byte 0x231e
	.2byte 0x8013
	.2byte 0xf000
	.2byte 0xfb97
	.2byte 0xe001
	.2byte 0xf000
	.2byte 0xfd54
.L_02000240_3:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x00000209
	.4byte 0x00000927
	.4byte 0x00000928
	.4byte 0x0000093e
	.4byte 0x02009940
	.4byte 0x02009928
	.4byte 0x020090a1
	.4byte 0x00000925
	.4byte 0x02000240
	.4byte 0x020082e0
	.4byte 0x00000109
	.2byte 0x006f
	.2byte 0x0000
	.global Func_0200040c
	.thumb_func
Func_0200040c:
	push {lr}
	bl 0x02009214
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl 0x020092c4
	movs r0, #1
	bl 0x020091b4
	bl 0x020092cc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #128
	movs r0, #164
	lsls r1, r1, #15
	ldr r2, [pc, #40]
	lsls r0, r0, #16
	bl 0x020092c4
	bl 0x020091dc
	movs r0, #1
	bl 0x020091b4
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x02009264
	bl 0x020084b0
	bl 0x0200921c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x01410000
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {lr}
	bl 0x02009214
	movs r1, #164
	ldr r2, [pc, #56]
	movs r0, #0
	lsls r1, r1, #16
	bl 0x02009264
	movs r1, #15
	movs r0, #0
	bl 0x02009284
	movs r0, #0
	bl 0x02009234
	movs r1, #0
	bl 0x020091f4
	movs r0, #1
	bl 0x020091b4
	bl 0x020091dc
	movs r0, #1
	bl 0x020091b4
	bl 0x020084b0
	bl 0x0200921c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x01410000
	.global Func_020004b0
	.thumb_func
Func_020004b0:
	push {lr}
	ldr r3, [pc, #164]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x020092ec
	bl 0x020092fc
	movs r0, #20
	bl 0x0200920c
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200923c
	movs r0, #8
	movs r1, #164
	ldr r2, [pc, #120]
	bl 0x0200925c
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200929c
	movs r1, #176
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200929c
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200929c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200929c
	movs r2, #167
	movs r0, #8
	movs r1, #164
	lsls r2, r2, #1
	bl 0x0200925c
	movs r2, #40
	movs r0, #8
	movs r1, #4
	bl 0x0200926c
	movs r1, #2
	movs r0, #8
	bl 0x02009274
	ldr r0, [pc, #40]
	bl 0x0200928c
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009294
	bl 0x020092f4
	bl 0x020092fc
	movs r0, #10
	bl 0x020092d4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000141
	.4byte 0x00001e3a
	.global Func_02000564
	.thumb_func
Func_02000564:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, [pc, #508]
	ldr r3, [r1]
	mov r8, r1
	ldr r5, [r3]
	bl 0x02009214
	ldr r0, [pc, #500]
	bl 0x02009224
	movs r0, #1
	bl 0x020091b4
	movs r1, #15
	movs r0, #0
	bl 0x02009284
	movs r0, #0
	bl 0x02009234
	movs r1, #0
	bl 0x020091f4
	movs r0, #8
	ldr r1, [pc, #472]
	bl 0x02009244
	movs r2, #76
	add r8, r2
	mov r3, r8
	ldr r2, [r3]
	movs r1, #224
	ldr r3, [pc, #460]
	lsls r1, r1, #1
	str r3, [r2, r1]
	mov r10, r1
	bl 0x020092ec
	bl 0x020092fc
	movs r0, #20
	bl 0x0200920c
	ldmia r5!, {r3}
	ldr r2, [pc, #440]
	str r3, [r2]
	ldr r3, [r5]
	movs r1, #160
	str r3, [r2, #4]
	movs r2, #210
	lsls r2, r2, #16
	lsls r1, r1, #15
	movs r0, #9
	bl 0x02009264
	movs r0, #9
	bl 0x02009234
	movs r5, #160
	ldr r3, [pc, #412]
	lsls r5, r5, #15
	movs r6, #0
	adds r0, #85
	ldr r1, [pc, #408]
	strb r6, [r0]
	str r5, [r3]
	str r6, [r3, #4]
	movs r0, #9
	bl 0x02009244
	movs r0, #20
	bl 0x0200920c
	movs r0, #29
	bl 0x0200930c
	movs r0, #143
	lsls r0, r0, #4
	bl 0x02009204
	movs r0, #8
	bl 0x0200924c
	movs r0, #1
	bl 0x020091b4
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #0
	bl 0x020092ac
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200929c
	ldr r0, [pc, #344]
	bl 0x0200928c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02009294
	movs r2, #210
	adds r1, r5, #0
	movs r0, #10
	lsls r2, r2, #16
	bl 0x02009264
	movs r2, #210
	adds r1, r5, #0
	movs r0, #11
	lsls r2, r2, #16
	bl 0x02009264
	movs r2, #210
	lsls r2, r2, #16
	adds r1, r5, #0
	movs r0, #12
	bl 0x02009264
	movs r0, #10
	movs r1, #3
	bl 0x020092a4
	movs r0, #11
	movs r1, #3
	bl 0x020092a4
	movs r0, #12
	movs r1, #3
	bl 0x020092a4
	movs r0, #10
	movs r1, #3
	bl 0x02009284
	movs r0, #11
	movs r1, #3
	bl 0x02009284
	movs r1, #3
	movs r0, #12
	bl 0x02009284
	movs r0, #10
	bl 0x02009234
	movs r5, #128
	ldr r6, [pc, #240]
	lsls r5, r5, #8
	str r5, [r0, #28]
	str r5, [r0, #24]
	str r6, [r0, #108]
	movs r0, #11
	bl 0x02009234
	str r5, [r0, #28]
	str r5, [r0, #24]
	str r6, [r0, #108]
	movs r0, #12
	bl 0x02009234
	str r5, [r0, #28]
	str r5, [r0, #24]
	str r6, [r0, #108]
	movs r0, #1
	bl 0x020091b4
	movs r0, #10
	ldr r1, [pc, #200]
	ldr r2, [pc, #204]
	bl 0x0200923c
	movs r0, #11
	ldr r1, [pc, #200]
	ldr r2, [pc, #200]
	bl 0x0200923c
	movs r0, #12
	ldr r1, [pc, #196]
	ldr r2, [pc, #200]
	bl 0x0200923c
	movs r0, #10
	movs r1, #128
	ldr r2, [pc, #192]
	bl 0x02009254
	movs r2, #165
	movs r0, #11
	movs r1, #136
	lsls r2, r2, #1
	bl 0x02009254
	movs r2, #170
	lsls r2, r2, #1
	movs r1, #156
	movs r0, #12
	bl 0x02009254
	movs r0, #60
	bl 0x0200920c
	movs r0, #8
	movs r1, #2
	bl 0x0200927c
	movs r2, #172
	movs r0, #8
	movs r1, #164
	lsls r2, r2, #1
	bl 0x0200925c
	movs r0, #8
	movs r1, #4
	movs r2, #10
	bl 0x0200926c
	movs r2, #40
	movs r0, #8
	movs r1, #6
	bl 0x0200926c
	movs r0, #8
	movs r1, #3
	bl 0x02009274
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009294
	mov r3, r8
	ldr r2, [r3]
	ldr r3, [pc, #100]
	mov r1, r10
	str r3, [r2, r1]
	bl 0x020092f4
	bl 0x020092fc
	movs r0, #11
	bl 0x020092d4
	bl 0x0200921c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001e70
	.4byte 0x020096f4
	.4byte 0x0200939c
	.4byte 0x00000203
	.4byte 0x02009938
	.4byte 0x02009930
	.4byte 0x02009314
	.4byte 0x00001e3e
	.4byte 0x020080a5
	.4byte 0x0000851e
	.4byte 0x0000428f
	.4byte 0x00007333
	.4byte 0x00003999
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x00000159
	.4byte 0x00000202
	.global Func_020007b0
	.thumb_func
Func_020007b0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	bl 0x02009214
	movs r1, #15
	movs r0, #0
	bl 0x02009284
	movs r0, #0
	bl 0x02009234
	movs r1, #0
	bl 0x020091f4
	movs r0, #1
	bl 0x020091b4
	ldr r0, [pc, #660]
	bl 0x02009224
	movs r0, #1
	bl 0x020091b4
	movs r0, #9
	bl 0x02008a84
	movs r0, #10
	bl 0x02008a84
	movs r0, #11
	bl 0x02008a84
	movs r0, #12
	bl 0x02008a84
	movs r0, #13
	bl 0x02008a84
	movs r0, #14
	bl 0x02008a84
	movs r0, #15
	bl 0x02008a84
	ldr r2, [pc, #608]
	mov r10, r2
	mov r1, r10
	movs r0, #8
	bl 0x02009244
	ldr r3, [pc, #600]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #67
	str r2, [r3]
	bl 0x020092ec
	bl 0x020092fc
	movs r0, #120
	bl 0x0200920c
	movs r0, #9
	bl 0x02009234
	adds r5, r0, #0
	movs r0, #9
	bl 0x0200924c
	movs r3, #128
	movs r6, #0
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #60]
	str r3, [r5, #64]
	str r6, [r5, #36]
	str r6, [r5, #40]
	str r6, [r5, #44]
	str r6, [r5, #76]
	movs r0, #20
	mov r8, r3
	bl 0x0200920c
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #12
	lsls r2, r2, #11
	bl 0x0200923c
	movs r1, #164
	movs r2, #144
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	ldr r3, [pc, #512]
	bl 0x020091e4
	adds r0, r5, #0
	bl 0x020091ec
	movs r1, #164
	movs r2, #208
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r2, r2, #15
	ldr r3, [pc, #488]
	bl 0x020091e4
	adds r0, r5, #0
	bl 0x020091ec
	movs r1, #204
	movs r2, #248
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r2, r2, #15
	ldr r3, [pc, #468]
	bl 0x020091e4
	adds r0, r5, #0
	bl 0x020091ec
	movs r1, #144
	movs r3, #169
	lsls r3, r3, #16
	lsls r1, r1, #16
	movs r2, #0
	adds r0, r5, #0
	bl 0x020091e4
	movs r0, #8
	bl 0x0200924c
	movs r0, #1
	bl 0x020091b4
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200929c
	movs r0, #8
	ldr r1, [pc, #416]
	movs r2, #60
	bl 0x020092ac
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	movs r0, #9
	bl 0x0200923c
	movs r0, #9
	bl 0x02008a84
	movs r0, #20
	bl 0x0200920c
	mov r1, r10
	movs r0, #8
	bl 0x02009244
	movs r0, #120
	bl 0x0200920c
.L_02000908:
	movs r0, #9
	bl 0x0200924c
	mov r2, r8
	str r2, [r5, #56]
	str r2, [r5, #60]
	str r2, [r5, #64]
	str r6, [r5, #36]
	str r6, [r5, #40]
	str r6, [r5, #44]
	str r6, [r5, #76]
	movs r0, #20
	bl 0x0200920c
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #12
	lsls r2, r2, #11
	bl 0x0200923c
	movs r1, #164
	movs r2, #144
	ldr r3, [pc, #320]
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x020091e4
	adds r0, r5, #0
	bl 0x020091ec
	movs r1, #160
	movs r2, #160
	movs r0, #9
	lsls r1, r1, #11
.L_02000950:
	lsls r2, r2, #10
	bl 0x0200923c
	movs r1, #164
	movs r2, #208
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r2, r2, #15
	ldr r3, [pc, #276]
	bl 0x020091e4
	adds r0, r5, #0
	bl 0x020091ec
	movs r1, #164
	movs r2, #228
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r2, r2, #15
	ldr r3, [pc, #256]
	bl 0x020091e4
	adds r0, r5, #0
	bl 0x020091ec
	movs r1, #164
	movs r2, #208
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r2, r2, #15
	ldr r3, [pc, #232]
	bl 0x020091e4
	adds r0, r5, #0
	bl 0x020091ec
	movs r1, #204
	movs r2, #248
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r2, r2, #15
	ldr r3, [pc, #212]
	bl 0x020091e4
	adds r0, r5, #0
	bl 0x020091ec
	movs r1, #144
	movs r3, #169
	lsls r3, r3, #16
	lsls r1, r1, #16
	movs r2, #0
	adds r0, r5, #0
	bl 0x020091e4
	movs r0, #8
	bl 0x0200924c
	movs r0, #1
	bl 0x020091b4
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200929c
	movs r0, #8
	ldr r1, [pc, #160]
	movs r2, #60
	bl 0x020092ac
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #9
	bl 0x0200923c
	movs r0, #9
	bl 0x02008a84
	movs r0, #8
	movs r1, #4
	movs r2, #20
	bl 0x0200926c
	movs r1, #6
	movs r2, #40
	movs r0, #8
	bl 0x0200926c
	movs r0, #29
	bl 0x0200930c
	movs r0, #143
	lsls r0, r0, #4
	bl 0x02009204
	ldr r0, [pc, #104]
	bl 0x0200928c
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x02009294
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #0
	bl 0x020092ac
	movs r2, #172
	lsls r2, r2, #1
	movs r1, #164
	movs r0, #8
	bl 0x0200925c
	movs r0, #40
	bl 0x0200920c
	movs r1, #2
	movs r0, #8
	bl 0x0200927c
	bl 0x020092f4
	bl 0x020092fc
	movs r0, #12
	bl 0x020092d4
	bl 0x0200921c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x976c
	.2byte 0x0200
	.2byte 0x939c
	.2byte 0x0200
	.2byte 0x1ebc
	.2byte 0x0300
	.4byte 0x01410000
	.4byte 0x00000103
	.4byte 0x00001e49
	.global Func_02000a84
	.thumb_func
Func_02000a84:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl 0x02009234
	movs r1, #1
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x020092a4
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	bl 0x020091c4
	adds r3, r6, #0
	adds r3, #100
	lsrs r0, r0, #15
	strh r0, [r3]
	bl 0x020091c4
	adds r3, r6, #0
	adds r3, #102
	lsrs r0, r0, #15
	strh r0, [r3]
	bl 0x020091c4
	lsls r0, r0, #2
	lsrs r0, r0, #16
	movs r2, #192
	lsls r2, r2, #11
	lsls r0, r0, #16
	adds r0, r0, r2
	str r0, [r6, #12]
	bl 0x020091c4
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r2, [pc, #28]
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r3, r3, r2
	str r3, [r6, #76]
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r6, #24]
	str r3, [r6, #28]
	ldr r1, [pc, #16]
	adds r0, r5, #0
	bl 0x02009244
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0xffffd000
	.4byte 0x020093a4
	.global Func_02000af8
	.thumb_func
Func_02000af8:
	push {r5, r6, r7, lr}
	bl 0x02009214
	movs r1, #15
	movs r0, #0
	bl 0x02009284
	movs r0, #0
	bl 0x02009234
	movs r1, #0
	bl 0x020091f4
	movs r0, #1
	bl 0x020091b4
	ldr r0, [pc, #816]
	bl 0x02009224
	movs r0, #1
	bl 0x020091b4
	ldr r0, [pc, #808]
	bl 0x02009224
	movs r0, #1
	bl 0x020091b4
	movs r0, #9
	bl 0x02008a84
	movs r0, #10
	bl 0x02008a84
	movs r0, #11
	bl 0x02008a84
	movs r0, #12
	bl 0x02008a84
	movs r0, #13
	bl 0x02008a84
	movs r0, #14
	bl 0x02008a84
	movs r0, #15
	bl 0x02008a84
	ldr r1, [pc, #760]
	movs r0, #8
	bl 0x02009244
	ldr r3, [pc, #756]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #67
	str r2, [r3]
	bl 0x020092ec
	bl 0x020092fc
	movs r0, #150
	lsls r0, r0, #1
	bl 0x0200920c
	movs r0, #147
	bl 0x0200930c
	movs r0, #100
	bl 0x0200920c
	movs r0, #9
	bl 0x0200924c
	movs r0, #10
	bl 0x0200924c
	movs r0, #11
	bl 0x0200924c
	movs r0, #12
	bl 0x0200924c
	movs r0, #13
	bl 0x0200924c
	movs r0, #14
	bl 0x0200924c
	movs r0, #15
	bl 0x0200924c
	movs r1, #192
	movs r2, #192
	movs r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #11
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #12
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #13
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #14
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #15
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r0, #9
	movs r1, #0
	movs r2, #100
	bl 0x02009254
	movs r0, #10
	movs r1, #60
	movs r2, #100
	bl 0x02009254
	movs r0, #11
	movs r1, #120
	movs r2, #100
	bl 0x02009254
	movs r0, #12
	movs r1, #180
	movs r2, #100
	bl 0x02009254
	movs r0, #13
	movs r1, #240
	movs r2, #100
	bl 0x02009254
	movs r1, #160
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #100
	bl 0x02009254
	movs r1, #190
	lsls r1, r1, #1
	movs r2, #100
	movs r0, #15
	bl 0x02009254
	movs r0, #40
	bl 0x0200920c
	ldr r1, [pc, #496]
	movs r2, #0
	movs r0, #8
	bl 0x020092ac
	movs r0, #20
	bl 0x0200920c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02009264
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009264
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x02009264
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009264
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009264
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x02009264
	movs r1, #0
	movs r2, #0
	movs r0, #15
	bl 0x02009264
	movs r0, #100
	bl 0x0200920c
	movs r0, #18
	bl 0x02009234
	ldr r3, [pc, #404]
	adds r5, r0, #0
	movs r1, #172
	movs r2, #170
	str r3, [r5, #24]
	str r3, [r5, #28]
	lsls r2, r2, #17
	lsls r1, r1, #16
	movs r0, #18
	bl 0x02009264
	movs r0, #8
	bl 0x0200924c
	movs r0, #1
	bl 0x020091b4
	movs r0, #8
	movs r1, #1
	bl 0x0200927c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl 0x0200929c
	movs r0, #29
	bl 0x0200930c
	movs r0, #143
	lsls r0, r0, #4
	bl 0x02009204
	ldr r7, [pc, #340]
	movs r6, #0
.L_02000af8_0:
	ldr r3, [r5, #24]
	adds r3, r3, r7
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r7
	str r3, [r5, #28]
	movs r0, #1
	adds r6, #1
	bl 0x020091b4
	cmp r6, #31
	bls .L_02000af8_0
	movs r2, #60
	movs r0, #8
	ldr r1, [pc, #300]
	bl 0x020092ac
	movs r0, #8
	movs r1, #2
	bl 0x0200927c
	movs r2, #170
	movs r0, #8
	movs r1, #168
	lsls r2, r2, #1
	bl 0x0200925c
	movs r2, #170
	movs r0, #8
	movs r1, #200
	lsls r2, r2, #1
	bl 0x0200925c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl 0x0200929c
	movs r0, #17
	bl 0x02009234
	ldr r3, [pc, #256]
	adds r5, r0, #0
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #172
	lsls r3, r3, #16
	str r3, [r5, #8]
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r5, #12]
	movs r3, #170
	lsls r3, r3, #17
	str r3, [r5, #16]
	movs r3, #0
	strh r3, [r5, #6]
	ldr r3, [pc, #228]
	str r3, [r5, #68]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #72]
	movs r0, #20
	bl 0x0200920c
	movs r2, #20
	movs r1, #6
	movs r0, #8
	bl 0x0200926c
	movs r0, #147
	bl 0x0200930c
	movs r0, #20
	bl 0x0200920c
	ldr r1, [pc, #196]
	movs r0, #8
	bl 0x02009244
	movs r0, #80
	bl 0x0200920c
	movs r0, #17
	movs r1, #1
	bl 0x020092a4
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #17
	bl 0x0200923c
	ldr r3, [pc, #144]
	str r3, [r5, #68]
	ldr r3, [pc, #160]
	movs r0, #153
	str r3, [r5, #72]
	bl 0x0200930c
	movs r3, #128
	lsls r3, r3, #12
	movs r2, #180
	str r3, [r5, #40]
	movs r0, #17
	movs r1, #132
	lsls r2, r2, #1
	bl 0x02009254
	movs r2, #180
	movs r1, #132
	lsls r2, r2, #1
	movs r0, #18
	bl 0x02009254
	movs r0, #40
	bl 0x0200920c
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl 0x02009264
	movs r0, #8
	bl 0x02009234
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #160
	lsls r3, r3, #7
	movs r0, #40
	strh r3, [r5, #6]
	bl 0x0200920c
	ldr r3, [pc, #48]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x020092f4
	bl 0x020092fc
	movs r0, #13
	bl 0x020092d4
	bl 0x0200921c
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200976c
	.4byte 0x02009844
	.4byte 0x0200939c
	.4byte 0x03001ebc
	.4byte 0x00000101
	.4byte 0x00001999
	.4byte 0x00000ccc
	.4byte 0x00012666
	.4byte 0x00006666
	.4byte 0x020093ac
	.4byte 0x0000b333
	.global Func_02000e78
	.thumb_func
Func_02000e78:
	push {r5, r6, lr}
	bl 0x02009214
	movs r1, #15
	movs r0, #0
	bl 0x02009284
	movs r0, #0
	bl 0x02009234
	movs r1, #0
	bl 0x020091f4
	ldr r0, [pc, #464]
	bl 0x02009224
	movs r0, #1
	bl 0x020091b4
	movs r0, #9
	bl 0x02008a84
	movs r0, #10
	bl 0x02008a84
	movs r0, #11
	bl 0x02008a84
	movs r0, #12
	bl 0x02008a84
	movs r0, #13
	bl 0x02008a84
	movs r0, #14
	bl 0x02008a84
	movs r0, #15
	bl 0x02008a84
	ldr r1, [pc, #412]
	movs r0, #8
	bl 0x02009244
	ldr r5, [pc, #408]
	ldr r3, [pc, #412]
	ldr r2, [r5]
	movs r6, #224
	lsls r6, r6, #1
	str r3, [r2, r6]
	bl 0x020092ec
	bl 0x020092fc
	movs r0, #200
	lsls r0, r0, #1
	bl 0x0200920c
	movs r0, #9
	bl 0x0200924c
	movs r0, #10
	bl 0x0200924c
	movs r0, #11
	bl 0x0200924c
	movs r0, #12
	bl 0x0200924c
	movs r0, #13
	bl 0x0200924c
	movs r0, #14
	bl 0x0200924c
	movs r0, #15
	bl 0x0200924c
	movs r1, #192
	movs r2, #192
	movs r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #11
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #12
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #13
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	movs r0, #14
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200923c
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #9
	movs r0, #15
	lsls r1, r1, #10
	bl 0x0200923c
	ldr r1, [pc, #248]
	movs r0, #9
	bl 0x02009244
	ldr r1, [pc, #244]
	movs r0, #10
	bl 0x02009244
	ldr r1, [pc, #240]
	movs r0, #11
	bl 0x02009244
	ldr r1, [pc, #236]
	movs r0, #12
	bl 0x02009244
	ldr r1, [pc, #232]
	movs r0, #13
	bl 0x02009244
	ldr r1, [pc, #228]
	movs r0, #14
	bl 0x02009244
	ldr r1, [pc, #224]
	movs r0, #15
	bl 0x02009244
	movs r0, #40
	bl 0x0200920c
	movs r0, #8
	movs r1, #3
	bl 0x02009274
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl 0x020092b4
	movs r0, #120
	bl 0x0200920c
	movs r0, #8
	movs r1, #1
	bl 0x02009274
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #60
	bl 0x020092ac
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200923c
	movs r2, #172
	movs r0, #8
	movs r1, #164
	lsls r2, r2, #1
	bl 0x0200925c
	movs r0, #8
	movs r1, #4
	movs r2, #10
	bl 0x0200926c
	movs r1, #6
	movs r2, #20
	movs r0, #8
	bl 0x0200926c
	ldr r0, [pc, #124]
	bl 0x0200928c
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009294
	ldr r2, [r5]
	ldr r3, [pc, #112]
	str r3, [r2, r6]
	bl 0x020092f4
	bl 0x020092fc
	ldr r1, [pc, #104]
	movs r0, #226
	ldr r2, [pc, #104]
	lsls r0, r0, #1
	adds r3, r1, r0
	strh r2, [r3]
	movs r3, #227
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #2
	strh r3, [r2]
	bl 0x02009130
	cmp r0, #11
	bne .L_02000e78_0
	movs r0, #15
	bl 0x020092d4
	b .L_02000e78_1
.L_02000e78_0:
	movs r0, #14
	bl 0x020092d4
.L_02000e78_1:
	bl 0x0200921c
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200976c
	.4byte 0x0200939c
	.4byte 0x03001ebc
	.4byte 0x00000203
	.4byte 0x02009450
	.4byte 0x02009480
	.4byte 0x020094b0
	.4byte 0x020094e0
	.4byte 0x02009510
	.4byte 0x02009540
	.4byte 0x02009570
	.4byte 0x00001ee4
	.4byte 0x00000202
	.4byte 0x02000240
	.4byte 0x0000006f
	.global Func_020010a0
	.thumb_func
Func_020010a0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r3, [pc, #108]
	ldr r6, [pc, #112]
	ldr r3, [r3]
	ldr r0, [r6]
	ldr r5, [r3]
	bl 0x020091d4
	ldr r2, [pc, #104]
	mov r10, r0
	ldr r0, [r2]
	mov r8, r2
	bl 0x020091cc
	ldr r3, [r5]
	add r3, r10
	stmia r5!, {r3}
	ldr r3, [r5]
	lsls r0, r0, #2
	adds r3, r3, r0
	str r3, [r5]
	ldr r2, [pc, #80]
	ldr r3, [r2]
	add r3, r10
	str r3, [r2]
	ldr r2, [pc, #76]
	ldr r3, [r2]
	adds r3, r3, r0
	str r3, [r2]
	bl 0x020091c4
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r2, [r6]
	lsls r3, r3, #7
	lsrs r3, r3, #16
	adds r2, r2, r3
	str r2, [r6]
	bl 0x020091c4
	mov r2, r8
	ldr r3, [r2]
	lsls r0, r0, #9
	ldrh r2, [r6]
	lsrs r0, r0, #16
	ldr r1, [pc, #40]
	adds r3, r3, r0
	str r2, [r6]
	ands r3, r1
	mov r2, r8
	str r3, [r2]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e70
	.4byte 0x02009940
	.4byte 0x02009928
	.4byte 0x02009924
	.4byte 0x02009920
	.4byte 0x0000ffff
	.global Func_02001130
	.thumb_func
Func_02001130:
	push {r5, r6, lr}
	movs r0, #0
	bl 0x0200915c
	adds r6, r0, #0
	movs r0, #2
	bl 0x0200915c
	adds r6, r6, r0
	movs r0, #1
	bl 0x0200915c
	adds r5, r0, #0
	movs r0, #3
	bl 0x0200915c
	adds r5, r5, r0
	subs r6, r6, r5
	adds r0, r6, #0
	pop {r5, r6}
	pop {r1}
	bx r1
	.global Func_0200115c
	.thumb_func
Func_0200115c:
	push {r5, r6, lr}
	movs r6, #0
	cmp r0, #1
	beq .L_0200115c_0
	cmp r0, #1
	bcc .L_0200115c_1
	cmp r0, #2
	beq .L_0200115c_2
	cmp r0, #3
	beq .L_0200115c_3
	b .L_0200115c_4
.L_0200115c_1:
	ldr r6, [pc, #48]
	b .L_0200115c_4
.L_0200115c_0:
	ldr r6, [pc, #48]
	b .L_0200115c_4
.L_0200115c_2:
	ldr r6, [pc, #48]
	b .L_0200115c_4
.L_0200115c_6:
	ldr r3, [pc, #48]
	lsls r2, r5, #2
	ldr r0, [r3, r2]
	b .L_0200115c_5
.L_0200115c_3:
	movs r6, #153
	lsls r6, r6, #4
.L_0200115c_4:
	movs r5, #0
.L_0200115c_7:
	adds r0, r6, r5
	bl 0x020091fc
	cmp r0, #0
	bne .L_0200115c_6
	adds r5, #1
	cmp r5, #8
	bls .L_0200115c_7
	movs r0, #0
.L_0200115c_5:
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x0000092c
	.4byte 0x00000935
	.4byte 0x00000917
	.4byte 0x020098f8
	.include "games/THE BROKEN SEAL/SRC/FIELD/FUNE_HOBASHIRA/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000800
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffff800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x0200807d
	.4byte 0x00000022
	.4byte 0x020080c1
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
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001400
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x00f20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x00dc0000
	.4byte 0x00000000
	.4byte 0x00860000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00e40000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x011a0000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x00000003
	.4byte 0x009a0000
	.4byte 0x00000000
	.4byte 0x00870000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000022
	.4byte 0x02008181
	.4byte 0x00000010
	.4byte 0x09090000
	.4byte 0x0d70090d
	.4byte 0x0db315cb
	.4byte 0x15f5164c
	.4byte 0x1f0e1aad
	.4byte 0x1f6f227a
	.4byte 0x23f223b1
	.4byte 0x2bf727f4
	.4byte 0xffff0000
	.4byte 0x000000a5
	.4byte 0xc00000eb
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x003800b8
	.4byte 0xc0000160
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x006000a4
	.4byte 0xc0000141
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x006000a4
	.4byte 0xc0000141
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x006000a4
	.4byte 0xc0000141
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x006000a4
	.4byte 0xc0000141
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x006000a4
	.4byte 0xc0000141
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x001000b0
	.4byte 0x00c0015c
	.4byte 0x016c0020
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000006e
	.4byte 0x0010406d
	.4byte 0x00a0a06f
	.4byte 0x00b0d06f
	.4byte 0x00c0d06d
	.4byte 0x00d0e06d
	.4byte 0x00e1106d
	.4byte 0x00f1306d
	.4byte 0x000001ff
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff011e
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00a90000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x00860000
	.4byte 0x00000000
	.4byte 0x00bd0000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x00d10000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x00810000
	.4byte 0x00000000
	.4byte 0x00ae0000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x008b0000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00024000
	.4byte 0xffff009b
	.4byte 0x00000007
	.4byte 0x009a0000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x00024000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x01a40000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00c5
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004602
	.4byte 0xffff000a
	.4byte 0x020081f1
	.4byte 0x00000000
	.4byte 0x09280008
	.4byte 0x00001e23
	.4byte 0x00000000
	.4byte 0x08a00008
	.4byte 0x00001e25
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001f63
	.4byte 0x00008d15
	.4byte 0x09280008
	.4byte 0x00001e24
	.4byte 0x00008d15
	.4byte 0x08a00008
	.4byte 0x00001e26
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001f64
	.4byte 0x00000003
	.4byte 0x0924000b
	.4byte 0x020081fd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000007
	.4byte 0x00000005
	.4byte 0x00000008
	.4byte 0x00000004
	.4byte 0x00000009
	.4byte 0x0000000c
	.4byte 0x00000006
	.4byte 0x00000003
	.4byte 0x00000000
