.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/ENTRY.INC"
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
	.4byte 0x0000008c
	.4byte 0x0200b094
	.4byte 0x0000008e
	.4byte 0x0200b274
	.4byte 0x0200b034
	.global Func_02000070
	.thumb_func
Func_02000070:
	movs r0, #0
	bx lr
	.global Func_02000074
	.thumb_func
Func_02000074:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200b2bc
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	ldr r1, [pc, #380]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #372]
	cmp r2, r3
	beq .L_0200007c_0
	b .L_0200007c_1
.L_0200007c_0:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	subs r3, #5
	cmp r3, #65
	bls .L_0200007c_2
	b .L_0200007c_3
.L_0200007c_2:
	ldr r2, [pc, #348]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	strh r4, [r6, #12]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r7, #12]
	lsls r0, r0, #8
	strh r4, [r7, #12]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r0, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r4, [r7, #12]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r4, [r7, #12]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r0, [r1, #14]
	lsls r0, r0, #8
	strh r4, [r7, #12]
	lsls r0, r0, #8
	strh r4, [r7, #12]
	lsls r0, r0, #8
	strh r4, [r0, #14]
	lsls r0, r0, #8
	strh r4, [r7, #12]
	lsls r0, r0, #8
	strh r4, [r0, #14]
	lsls r0, r0, #8
	strh r4, [r6, #12]
	lsls r0, r0, #8
	strh r0, [r7, #12]
	lsls r0, r0, #8
	ldr r0, [pc, #80]
	b .L_0200007c_4
	.2byte 0x4814
	.2byte 0xe01c
	.2byte 0x4814
	.2byte 0xe01a
	.2byte 0x4814
	.2byte 0xe018
	.2byte 0x4814
	.2byte 0xe016
.L_0200007c_3:
	ldr r0, [pc, #80]
	b .L_0200007c_4
.L_0200007c_1:
	ldr r3, [pc, #80]
	cmp r2, r3
	bne .L_0200007c_5
	movs r0, #149
	lsls r0, r0, #4
	bl 0x0200abbc
	cmp r0, #0
	beq .L_0200007c_6
	ldr r0, [pc, #68]
	b .L_0200007c_4
.L_0200007c_6:
	ldr r0, [pc, #68]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_0200007c_7
	ldr r0, [pc, #60]
	b .L_0200007c_4
.L_0200007c_7:
	ldr r0, [pc, #60]
	b .L_0200007c_4
.L_0200007c_5:
	ldr r0, [pc, #60]
.L_0200007c_4:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008c
	.4byte 0x020080ac
	.4byte 0x0200b39c
	.2byte 0xb5f4
	.2byte 0x0200
	.2byte 0xb7bc
	.2byte 0x0200
	.2byte 0xb87c
	.2byte 0x0200
	.2byte 0xb99c
	.2byte 0x0200
	.4byte 0x0200b75c
	.4byte 0x0000008e
	.4byte 0x0200be1c
	.4byte 0x00000962
	.4byte 0x0200bbdc
	.4byte 0x0200ba44
	.4byte 0x0200b324
	.global Func_02000238
	.thumb_func
Func_02000238:
	push {lr}
	ldr r1, [pc, #64]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02000238_0
	ldr r0, [pc, #52]
	b .L_02000238_1
.L_02000238_0:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02000238_2
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #12
	bne .L_02000238_3
	ldr r0, [pc, #36]
	b .L_02000238_1
.L_02000238_3:
	ldr r0, [pc, #36]
	b .L_02000238_1
.L_02000238_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000238_4
	ldr r0, [pc, #36]
	b .L_02000238_1
.L_02000238_4:
	ldr r0, [pc, #36]
.L_02000238_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000008d
	.4byte 0x0200be70
	.4byte 0x0000008c
	.4byte 0x0200c110
	.4byte 0x0200be94
	.4byte 0x0000008e
	.4byte 0x0200bf60
	.4byte 0x0200be64
	.global Func_020002a0
	.thumb_func
Func_020002a0:
	push {lr}
	bl 0x0200abf4
	ldr r0, [pc, #48]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_020002a0_0
	ldr r0, [pc, #40]
	bl 0x0200ac94
	movs r0, #10
	movs r1, #0
	bl 0x0200aca4
	b .L_020002a0_1
.L_020002a0_0:
	ldr r0, [pc, #28]
	bl 0x0200ac94
	movs r0, #10
	movs r1, #0
	bl 0x0200acb4
.L_020002a0_1:
	bl 0x0200abfc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000962
	.4byte 0x00002251
	.4byte 0x00002057
	.global Func_020002e4
	.thumb_func
Func_020002e4:
	push {lr}
	bl 0x0200abf4
	ldr r0, [pc, #60]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_020002e4_0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #13
	movs r2, #40
	bl 0x0200acc4
	ldr r0, [pc, #40]
	bl 0x0200ac94
	movs r0, #13
	movs r1, #0
	bl 0x0200aca4
	b .L_020002e4_1
.L_020002e4_0:
	ldr r0, [pc, #28]
	bl 0x0200ac94
	movs r0, #13
	movs r1, #0
	bl 0x0200aca4
.L_020002e4_1:
	bl 0x0200abfc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000962
	.4byte 0x00002254
	.4byte 0x0000205c
	.global Func_02000334
	.thumb_func
Func_02000334:
	push {lr}
	bl 0x0200abf4
	ldr r0, [pc, #84]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_02000334_0
	movs r1, #2
	movs r0, #14
	bl 0x0200ac7c
	ldr r0, [pc, #68]
	bl 0x0200ac94
	movs r0, #14
	bl 0x02009c48
	movs r1, #0
	movs r2, #0
	movs r0, #14
	bl 0x0200ac84
	movs r0, #20
	bl 0x0200abec
	movs r1, #0
	movs r0, #14
	bl 0x0200acb4
	movs r0, #14
	movs r1, #0
	bl 0x02009c5c
	b .L_02000334_1
.L_02000334_0:
	ldr r0, [pc, #28]
	bl 0x0200ac94
	movs r0, #14
	movs r1, #0
	bl 0x0200aca4
.L_02000334_1:
	bl 0x0200abfc
	pop {r0}
	bx r0
	.4byte 0x00000962
	.4byte 0x00002256
	.4byte 0x0000205d
	.global Func_0200039c
	.thumb_func
Func_0200039c:
	push {r5, lr}
	bl 0x0200abf4
	ldr r0, [pc, #176]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_0200039c_0
	movs r0, #240
	lsls r0, r0, #2
	bl 0x0200abbc
	cmp r0, #0
	beq .L_0200039c_1
	ldr r0, [pc, #156]
	bl 0x0200ac94
	b .L_0200039c_2
.L_0200039c_1:
	ldr r0, [pc, #152]
	bl 0x0200ac94
	movs r1, #0
	movs r0, #16
	bl 0x0200ac9c
	movs r0, #0
	movs r1, #0
	bl 0x0200ac04
	cmp r0, #0
	bne .L_0200039c_2
	ldr r5, [pc, #132]
	movs r2, #236
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	movs r1, #128
	adds r2, #1
	strh r2, [r3]
	movs r0, #16
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200acc4
	movs r1, #0
	movs r0, #16
	bl 0x0200ac9c
	movs r0, #0
	movs r1, #0
	bl 0x0200ac04
	cmp r0, #0
	bne .L_0200039c_3
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_0200039c_3:
	movs r0, #40
	bl 0x0200abec
	movs r0, #16
	movs r1, #0
	bl 0x0200aca4
	movs r0, #240
	lsls r0, r0, #2
	bl 0x0200abc4
	b .L_0200039c_4
.L_0200039c_2:
	movs r0, #16
	movs r1, #0
	bl 0x0200aca4
	b .L_0200039c_4
.L_0200039c_0:
	ldr r0, [pc, #40]
	bl 0x0200ac94
	movs r0, #16
	movs r1, #0
	bl 0x0200acb4
.L_0200039c_4:
	bl 0x0200abfc
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000962
	.4byte 0x0000225e
	.4byte 0x0000225a
	.4byte 0x03001ebc
	.4byte 0x0000205e
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {r5, lr}
	movs r0, #13
	bl 0x0200ac0c
	adds r5, r0, #0
	bl 0x0200abf4
	movs r0, #13
	bl 0x0200ac24
	movs r2, #20
	movs r1, #0
	movs r0, #13
	bl 0x0200ac84
	ldr r0, [pc, #60]
	bl 0x0200ac94
	movs r0, #13
	bl 0x02009c48
	movs r0, #13
	movs r1, #1
	bl 0x0200ac7c
	movs r0, #13
	movs r1, #0
	bl 0x0200aca4
	adds r2, r5, #0
	movs r3, #180
	adds r2, #100
	lsls r3, r3, #2
	strh r3, [r2]
	adds r5, #102
	movs r3, #112
	strh r3, [r5]
	movs r0, #13
	movs r1, #2
	bl 0x0200ac1c
	bl 0x0200abfc
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00002114
	.global Func_020004c8
	.thumb_func
Func_020004c8:
	push {lr}
	bl 0x0200abf4
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl 0x0200accc
	movs r1, #2
	movs r0, #14
	bl 0x0200ac7c
	ldr r0, [pc, #36]
	bl 0x0200ac94
	movs r0, #14
	bl 0x02009c48
	movs r1, #129
	movs r2, #40
	movs r0, #14
	lsls r1, r1, #1
	bl 0x0200acc4
	movs r0, #14
	movs r1, #0
	bl 0x0200aca4
	bl 0x0200abfc
	pop {r0}
	bx r0
	.4byte 0x00002116
	.global Func_0200050c
	.thumb_func
Func_0200050c:
	push {lr}
	bl 0x0200abf4
	ldr r0, [pc, #68]
	bl 0x0200ac94
	movs r0, #15
	bl 0x02009c48
	movs r2, #20
	movs r1, #0
	movs r0, #15
	bl 0x0200ac84
	movs r0, #15
	bl 0x02009c48
	movs r0, #15
	movs r1, #3
	bl 0x0200ac6c
	movs r1, #0
	movs r0, #15
	bl 0x0200ac5c
	movs r0, #15
	bl 0x02009c48
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl 0x02009c5c
	bl 0x0200abfc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002118
	.global Func_0200055c
	.thumb_func
Func_0200055c:
	push {lr}
	bl 0x0200abf4
	movs r1, #2
	movs r0, #16
	bl 0x0200ac7c
	ldr r0, [pc, #120]
	bl 0x0200ac94
	movs r0, #16
	movs r1, #0
	movs r2, #20
	bl 0x0200acac
	ldr r0, [pc, #108]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_0200055c_0
	movs r0, #20
	bl 0x0200abec
	b .L_0200055c_1
.L_0200055c_0:
	movs r0, #17
	movs r1, #0
	bl 0x02009c5c
	movs r1, #1
	movs r0, #17
	bl 0x0200ac7c
	movs r0, #17
	bl 0x02009c48
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200ac84
	movs r1, #4
	movs r0, #17
	bl 0x0200ac5c
	movs r0, #17
	bl 0x02009c48
	ldr r1, [pc, #48]
	movs r2, #40
	movs r0, #17
	bl 0x0200acc4
	movs r0, #17
	bl 0x02009c48
	movs r1, #160
	movs r0, #17
	lsls r1, r1, #7
	bl 0x02009c5c
	ldr r0, [pc, #16]
	bl 0x0200abc4
.L_0200055c_1:
	bl 0x0200abfc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000211b
	.4byte 0x000003c1
	.4byte 0x00000105
	.global Func_020005f0
	.thumb_func
Func_020005f0:
	push {lr}
	bl 0x0200abf4
	movs r2, #20
	movs r1, #0
	movs r0, #17
	bl 0x0200ac84
	ldr r0, [pc, #64]
	bl 0x0200ac94
	movs r0, #17
	bl 0x02009c48
	movs r0, #0
	movs r1, #3
	bl 0x0200ac6c
	movs r1, #3
	movs r0, #17
	bl 0x0200ac6c
	movs r0, #17
	bl 0x02009c48
	movs r1, #1
	movs r0, #17
	bl 0x0200ac7c
	movs r0, #17
	bl 0x02009c48
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #17
	bl 0x02009c5c
	bl 0x0200abfc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000211f
	.global Func_02000648
	.thumb_func
Func_02000648:
	push {lr}
	bl 0x0200abf4
	movs r1, #0
	movs r2, #20
	movs r0, #18
	bl 0x0200ac84
	ldr r0, [pc, #92]
	bl 0x0200ac94
	movs r0, #18
	bl 0x02009c48
	movs r1, #208
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200acbc
	movs r1, #176
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200acbc
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200acbc
	movs r2, #20
	movs r1, #0
	movs r0, #18
	bl 0x0200ac84
	movs r0, #18
	bl 0x02009c48
	movs r1, #3
	movs r0, #18
	bl 0x0200ac6c
	movs r0, #18
	bl 0x02009c48
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #18
	bl 0x02009c5c
	bl 0x0200abfc
	pop {r0}
	bx r0
	.4byte 0x00002122
	.global Func_020006bc
	.thumb_func
Func_020006bc:
	push {lr}
	bl 0x0200abf4
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl 0x0200ac84
	ldr r0, [pc, #56]
	bl 0x0200ac94
	movs r1, #0
	movs r0, #8
	bl 0x0200ac9c
	movs r0, #0
	movs r1, #0
	bl 0x0200ac04
	cmp r0, #0
	beq .L_020006bc_0
	ldr r3, [pc, #36]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020006bc_0:
	movs r0, #8
	movs r1, #0
	bl 0x0200aca4
	bl 0x0200abfc
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002125
	.4byte 0x03001ebc
	.global Func_02000710
	.thumb_func
Func_02000710:
	push {r5, r6, r7, lr}
	ldr r0, [pc, #932]
	bl 0x0200abbc
	cmp r0, #0
	bne .L_02000710_0
	b .L_02000710_1
.L_02000710_0:
	bl 0x0200abf4
	movs r0, #17
	movs r1, #1
	bl 0x0200ac7c
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200acbc
	movs r0, #17
	movs r1, #0
	movs r2, #60
	bl 0x0200acbc
	movs r5, #192
	movs r1, #128
	lsls r5, r5, #6
	movs r2, #40
	movs r0, #17
	lsls r1, r1, #1
	bl 0x0200acc4
	adds r1, r5, #0
	movs r0, #17
	bl 0x02009c5c
	ldr r0, [pc, #864]
	bl 0x0200ac94
	movs r1, #2
	movs r0, #17
	bl 0x0200ac74
	movs r0, #17
	bl 0x02009c48
	movs r0, #18
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200acbc
	movs r0, #19
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200acbc
	movs r2, #0
	adds r1, r5, #0
	movs r0, #20
	bl 0x0200acbc
	bl 0x0200acf4
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r1, [pc, #808]
	ldr r0, [pc, #812]
	bl 0x0200acdc
	movs r0, #128
	movs r1, #1
	movs r2, #172
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl 0x0200ace4
	bl 0x0200acec
	movs r0, #20
	bl 0x0200abec
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #18
	bl 0x0200acc4
	movs r0, #18
	bl 0x02009c48
	movs r1, #3
	movs r0, #17
	bl 0x0200ac6c
	movs r0, #17
	bl 0x02009c48
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #19
	bl 0x0200accc
	movs r0, #40
	bl 0x0200abec
	movs r1, #0
	movs r0, #19
	bl 0x02009c5c
	movs r0, #19
	bl 0x02009c48
	movs r2, #40
	movs r0, #20
	ldr r1, [pc, #716]
	bl 0x0200acc4
	movs r0, #20
	movs r1, #0
	bl 0x02009c5c
	movs r0, #20
	movs r1, #2
	bl 0x0200ac74
	movs r2, #20
	movs r0, #20
	movs r1, #0
	bl 0x0200acac
	movs r1, #1
	movs r0, #17
	bl 0x0200ac7c
	movs r0, #10
	bl 0x0200abec
	movs r1, #176
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200acbc
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200acac
	movs r0, #18
	movs r1, #1
	bl 0x0200ac7c
	movs r6, #128
	movs r1, #4
	movs r0, #18
	bl 0x0200ac6c
	lsls r6, r6, #8
	movs r0, #20
	bl 0x0200abec
	movs r0, #17
	adds r1, r6, #0
	bl 0x02009c5c
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200acac
	movs r1, #1
	movs r0, #19
	bl 0x0200ac7c
	movs r0, #40
	bl 0x0200abec
	movs r0, #19
	movs r1, #4
	bl 0x0200ac6c
	movs r2, #20
	movs r0, #17
	movs r1, #0
	bl 0x0200acac
	movs r1, #2
	movs r0, #20
	bl 0x0200ac7c
	movs r0, #40
	bl 0x0200abec
	movs r1, #3
	movs r0, #20
	bl 0x0200ac6c
	movs r0, #20
	bl 0x0200abec
	movs r0, #17
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #18
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #19
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #20
	movs r1, #3
	bl 0x0200ac6c
	movs r0, #17
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200acbc
	movs r0, #18
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200acbc
	movs r2, #0
	movs r0, #19
	adds r1, r5, #0
	bl 0x0200acbc
	movs r0, #20
	adds r1, r5, #0
	bl 0x02009c5c
	movs r0, #17
	ldr r1, [pc, #476]
	ldr r2, [pc, #476]
	bl 0x0200ac14
	movs r1, #129
	movs r0, #17
	lsls r1, r1, #1
	movs r2, #172
	bl 0x0200ac4c
	movs r0, #0
	ldr r1, [pc, #460]
	ldr r2, [pc, #464]
	bl 0x0200ac14
	movs r1, #131
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #188
	bl 0x0200ac4c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r0, #0
	bl 0x0200ac0c
	cmp r0, #0
	beq .L_02000710_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200ac54
.L_02000710_2:
	movs r0, #0
	bl 0x0200ac0c
	cmp r0, #0
	beq .L_02000710_3
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x0200ac54
.L_02000710_3:
	movs r0, #0
	bl 0x0200ac0c
	cmp r0, #0
	beq .L_02000710_4
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x0200ac54
.L_02000710_4:
	movs r0, #1
	ldr r1, [pc, #368]
	ldr r2, [pc, #368]
	bl 0x0200ac14
	movs r0, #2
	ldr r1, [pc, #356]
	ldr r2, [pc, #360]
	bl 0x0200ac14
	movs r0, #3
	ldr r1, [pc, #348]
	ldr r2, [pc, #348]
	bl 0x0200ac14
	movs r0, #1
	movs r1, #246
	movs r2, #200
	bl 0x0200ac44
	movs r1, #131
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #200
	bl 0x0200ac44
	movs r1, #139
	movs r2, #200
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200ac4c
	movs r0, #2
	movs r1, #1
	bl 0x0200ac5c
	movs r0, #1
	movs r1, #1
	bl 0x0200ac5c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #192
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200acbc
	movs r1, #1
	movs r0, #17
	bl 0x0200ac7c
	movs r0, #17
	bl 0x02009c48
	movs r0, #0
	movs r1, #2
	bl 0x0200ac74
	movs r0, #1
	movs r1, #2
	bl 0x0200ac74
	movs r0, #2
	movs r1, #2
	bl 0x0200ac74
	movs r0, #3
	movs r1, #2
	bl 0x0200ac7c
	movs r0, #17
	movs r1, #3
	bl 0x0200ac6c
	movs r1, #0
	movs r0, #17
	bl 0x0200ac9c
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r0, #0
	movs r1, #0
	bl 0x0200ac04
	cmp r0, #0
	bne .L_02000710_5
	movs r1, #3
	movs r0, #17
	bl 0x0200ac6c
	movs r0, #17
	bl 0x02009c48
	ldr r5, [pc, #148]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200ac1c
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200ac1c
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200ac2c
	ldr r0, [pc, #120]
	ldr r1, [pc, #124]
	bl 0x0200acdc
	movs r0, #128
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #15
	bl 0x0200ace4
	movs r1, #128
	adds r2, r6, #0
	movs r0, #17
	lsls r1, r1, #9
	bl 0x0200ac14
	ldr r5, [pc, #92]
	movs r0, #17
	adds r1, r5, #0
	bl 0x0200ac1c
	movs r0, #10
	bl 0x0200abec
	movs r1, #128
	adds r2, r6, #0
	movs r0, #0
	lsls r1, r1, #9
	bl 0x0200ac14
	adds r1, r5, #0
	movs r0, #0
	bl 0x0200ac1c
	movs r0, #80
	bl 0x0200abec
	ldr r3, [pc, #56]
	movs r2, #228
	ldr r3, [r3]
	b .L_02000710_6
	.2byte 0x0000
	.4byte 0x00000962
	.4byte 0x00002267
	.4byte 0x00003333
	.4byte 0x00019999
	.4byte 0x00000103
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0200ad3c
	.4byte 0x00000ccc
	.4byte 0x0200ade4
	.4byte 0x03001ebc
.L_02000710_5:
	ldr r7, [pc, #704]
	movs r3, #236
	ldr r2, [r7]
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #129
	adds r3, #1
	strh r3, [r2]
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #17
	bl 0x0200acc4
	movs r0, #17
	bl 0x02009c48
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #18
	bl 0x0200acc4
	movs r0, #18
	bl 0x02009c48
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #19
	bl 0x0200acc4
	movs r0, #19
	bl 0x02009c48
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #20
	bl 0x0200acc4
	movs r0, #19
	bl 0x02009c48
	movs r1, #4
	movs r0, #17
	bl 0x0200ac6c
	movs r0, #17
	bl 0x02009c48
	movs r1, #1
	movs r0, #2
	bl 0x0200ac7c
	movs r0, #20
	bl 0x0200abec
	movs r0, #2
	bl 0x02009c48
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #3
	bl 0x0200acc4
	movs r0, #3
	bl 0x02009c48
	movs r1, #176
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r0, #19
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200acbc
	movs r0, #20
	movs r1, #0
	movs r2, #60
	bl 0x0200acbc
	movs r0, #17
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200acbc
	movs r0, #19
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200acbc
	movs r2, #20
	movs r0, #20
	adds r1, r5, #0
	bl 0x0200acbc
	movs r0, #17
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #18
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #19
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #20
	movs r1, #3
	bl 0x0200ac6c
	movs r1, #128
	movs r2, #60
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200acc4
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #3
	bl 0x02009c5c
	movs r0, #3
	bl 0x02009c48
	movs r1, #4
	movs r0, #1
	bl 0x0200ac5c
	movs r0, #20
	bl 0x0200abec
	movs r1, #0
	movs r0, #1
	bl 0x0200ac9c
	movs r0, #0
	movs r1, #0
	bl 0x0200ac04
	cmp r0, #0
	bne .L_02000710_7
	movs r1, #3
	movs r0, #17
	bl 0x0200ac6c
	movs r0, #17
	bl 0x02009c48
	ldr r5, [pc, #400]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200ac1c
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200ac1c
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200ac2c
	ldr r0, [pc, #376]
	ldr r1, [pc, #380]
	bl 0x0200acdc
	movs r0, #128
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #15
	bl 0x0200ace4
	movs r1, #128
	adds r2, r6, #0
	movs r0, #17
	lsls r1, r1, #9
	bl 0x0200ac14
	ldr r5, [pc, #348]
	movs r0, #17
	adds r1, r5, #0
	bl 0x0200ac1c
	movs r0, #10
	bl 0x0200abec
	movs r1, #128
	adds r2, r6, #0
	movs r0, #0
	lsls r1, r1, #9
	bl 0x0200ac14
	adds r1, r5, #0
	movs r0, #0
	bl 0x0200ac1c
	movs r0, #80
	bl 0x0200abec
	ldr r3, [r7]
	movs r2, #228
.L_02000710_6:
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #40
	str r2, [r3]
	bl 0x0200ad24
	bl 0x0200ad2c
	b .L_02000710_8
.L_02000710_7:
	ldr r2, [r7]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #20
	bl 0x0200abec
	movs r0, #1
	movs r1, #2
	bl 0x0200ac74
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200acac
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #2
	bl 0x0200acc4
	movs r0, #2
	bl 0x02009c48
	movs r0, #3
	adds r1, r6, #0
	bl 0x02009c5c
	movs r1, #3
	movs r0, #3
	bl 0x0200ac5c
	movs r0, #3
	bl 0x02009c48
	ldr r5, [pc, #208]
	movs r0, #2
	adds r1, r5, #0
	bl 0x0200ac1c
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200ac2c
	movs r0, #20
	bl 0x0200abec
	adds r1, r5, #0
	movs r0, #0
	bl 0x0200ac2c
	movs r1, #131
	movs r2, #188
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ac4c
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #1
	bl 0x02009c5c
	movs r1, #3
	movs r0, #1
	bl 0x0200ac6c
	movs r0, #1
	bl 0x02009c48
	ldr r0, [pc, #128]
	ldr r1, [pc, #132]
	bl 0x0200acdc
	movs r0, #128
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #15
	bl 0x0200ace4
	movs r1, #128
	adds r2, r6, #0
	movs r0, #17
	lsls r1, r1, #9
	bl 0x0200ac14
	ldr r5, [pc, #100]
	movs r0, #17
	adds r1, r5, #0
	bl 0x0200ac1c
	movs r0, #10
	bl 0x0200abec
	movs r1, #128
	adds r2, r6, #0
	movs r0, #1
	lsls r1, r1, #9
	bl 0x0200ac14
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200ac1c
	movs r0, #80
	bl 0x0200abec
	ldr r3, [r7]
	movs r2, #228
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #40
	str r2, [r3]
	bl 0x0200ad24
	bl 0x0200ad2c
.L_02000710_8:
	movs r0, #2
	bl 0x0200acfc
	ldr r0, [pc, #40]
	bl 0x0200abc4
	bl 0x0200abfc
.L_02000710_1:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0200ad3c
	.4byte 0x00006666
	.4byte 0x00000ccc
	.4byte 0x0200ade4
	.4byte 0x0200ad74
	.4byte 0x0000093f
	.global Func_02000dcc
	.thumb_func
Func_02000dcc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #141
	lsls r0, r0, #2
	bl 0x0200abbc
	cmp r0, #0
	bne .L_02000dcc_0
	b .L_02000dcc_1
.L_02000dcc_0:
	ldr r0, [pc, #904]
	bl 0x0200abc4
	bl 0x0200abf4
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ac14
	movs r1, #218
	movs r2, #120
	movs r0, #0
	lsls r1, r1, #2
	bl 0x0200ac4c
	movs r3, #160
	lsls r3, r3, #8
	mov r10, r3
	movs r0, #0
	mov r1, r10
	bl 0x02009c5c
	ldr r0, [pc, #856]
	ldr r1, [pc, #860]
	bl 0x0200acdc
	movs r0, #204
	movs r1, #1
	movs r2, #224
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #15
	bl 0x0200ace4
	bl 0x0200acec
	movs r1, #192
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #6
	bl 0x0200acbc
	movs r1, #1
	movs r0, #8
	bl 0x0200ac7c
	ldr r0, [pc, #816]
	bl 0x0200ac94
	movs r1, #0
	ldr r0, [pc, #812]
	bl 0x0200ac9c
	movs r0, #0
	movs r1, #0
	bl 0x0200ac04
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02000dcc_2
	b .L_02000dcc_3
.L_02000dcc_2:
	movs r0, #20
	bl 0x0200abec
	movs r3, #208
	lsls r3, r3, #8
	mov r8, r3
	movs r0, #12
	mov r1, r8
	bl 0x02009c5c
	movs r1, #1
	movs r0, #12
	bl 0x0200ac7c
	ldr r0, [pc, #764]
	bl 0x0200ac94
	movs r6, #128
	ldr r0, [pc, #760]
	bl 0x02009c48
	lsls r6, r6, #8
	movs r0, #17
	movs r1, #0
	bl 0x02009c5c
	movs r0, #0
	adds r1, r6, #0
	bl 0x02009c5c
	movs r0, #17
	movs r1, #3
	bl 0x0200ac6c
	movs r0, #0
	movs r1, #3
	bl 0x0200ac6c
	movs r0, #15
	movs r1, #1
	bl 0x0200ac5c
	movs r2, #20
	movs r0, #15
	mov r1, r8
	bl 0x0200acbc
	movs r1, #1
	movs r0, #15
	bl 0x0200ac7c
	movs r0, #15
	bl 0x02009c48
	movs r0, #16
	adds r1, r6, #0
	bl 0x02009c5c
	movs r0, #16
	movs r1, #3
	bl 0x0200ac6c
	movs r0, #17
	movs r1, #2
	bl 0x0200ac7c
	movs r5, #176
	mov r1, r10
	movs r0, #17
	bl 0x02009c5c
	lsls r5, r5, #8
	ldr r0, [pc, #656]
	bl 0x02009c48
	adds r1, r5, #0
	movs r0, #18
	bl 0x02009c5c
	movs r1, #2
	movs r0, #18
	bl 0x0200ac74
	ldr r0, [pc, #640]
	bl 0x02009c48
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #11
	bl 0x02009c5c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl 0x0200accc
	movs r0, #60
	bl 0x0200abec
	movs r1, #2
	movs r0, #11
	bl 0x0200ac74
	ldr r0, [pc, #604]
	bl 0x02009c48
	movs r0, #13
	bl 0x0200ac24
	movs r0, #1
	bl 0x0200ab8c
	movs r0, #13
	movs r1, #2
	bl 0x0200ac74
	movs r0, #14
	movs r1, #2
	bl 0x0200ac74
	movs r1, #2
	movs r0, #16
	bl 0x0200ac7c
	movs r0, #20
	bl 0x0200abec
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x0200acbc
	movs r0, #14
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200acbc
	movs r2, #40
	adds r1, r5, #0
	movs r0, #16
	bl 0x0200acbc
	movs r0, #13
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #14
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #16
	movs r1, #3
	bl 0x0200ac6c
	movs r0, #13
	ldr r1, [pc, #468]
	ldr r2, [pc, #500]
	bl 0x0200ac14
	movs r0, #14
	ldr r1, [pc, #456]
	ldr r2, [pc, #492]
	bl 0x0200ac14
	ldr r2, [pc, #484]
	movs r0, #16
	ldr r1, [pc, #444]
	bl 0x0200ac14
	ldr r1, [pc, #480]
	movs r0, #13
	bl 0x0200ac1c
	ldr r1, [pc, #476]
	movs r0, #16
	bl 0x0200ac1c
	movs r0, #20
	bl 0x0200abec
	movs r0, #15
	mov r1, r8
	movs r2, #0
	bl 0x0200acbc
	adds r1, r5, #0
	movs r0, #17
	movs r2, #0
	bl 0x0200acbc
	movs r0, #0
	mov r1, r10
	movs r2, #0
	bl 0x0200acbc
	movs r0, #12
	mov r1, r8
	movs r2, #0
	bl 0x0200acbc
	movs r2, #0
	adds r1, r5, #0
	movs r0, #18
	bl 0x0200acbc
	ldr r1, [pc, #416]
	movs r0, #14
	bl 0x0200ac2c
	movs r0, #20
	bl 0x0200abec
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200acbc
	movs r2, #40
	movs r0, #11
	adds r1, r6, #0
	bl 0x0200acbc
	movs r0, #8
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #11
	movs r1, #3
	bl 0x0200ac6c
	movs r0, #202
	movs r1, #1
	movs r2, #172
	movs r3, #1
	lsls r2, r2, #15
	lsls r0, r0, #18
	negs r1, r1
	bl 0x0200ace4
	bl 0x0200acec
	movs r1, #2
	movs r0, #8
	bl 0x0200ac7c
	movs r0, #8
	bl 0x02009c48
	movs r1, #128
	lsls r1, r1, #9
	adds r2, r6, #0
	movs r0, #8
	bl 0x0200ac14
	movs r0, #8
	bl 0x0200ac0c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #198
	strb r3, [r0]
	lsls r1, r1, #2
	movs r2, #72
	movs r0, #8
	bl 0x0200ac4c
	movs r0, #1
	bl 0x0200abec
	movs r0, #8
	bl 0x0200ac0c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #8
	bl 0x02009c5c
	movs r0, #13
	bl 0x0200ac24
	movs r0, #13
	bl 0x0200ac0c
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #100
	str r7, [r2, #108]
	strh r7, [r3]
	adds r3, #2
	strh r7, [r3]
	movs r3, #128
	lsls r3, r3, #24
	str r7, [r2, #36]
	str r7, [r2, #40]
	str r7, [r2, #44]
	str r3, [r2, #56]
	str r3, [r2, #60]
	str r3, [r2, #64]
	movs r0, #1
	bl 0x0200ab8c
	ldr r5, [pc, #216]
	movs r0, #15
	adds r1, r5, #0
	bl 0x0200ac1c
	movs r0, #20
	bl 0x0200abec
	adds r1, r5, #0
	movs r0, #13
	bl 0x0200ac1c
	movs r0, #20
	bl 0x0200abec
	adds r1, r5, #0
	movs r0, #17
	bl 0x0200ac1c
	movs r0, #20
	bl 0x0200abec
	adds r1, r5, #0
	movs r0, #14
	bl 0x0200ac1c
	movs r0, #20
	bl 0x0200abec
	adds r1, r5, #0
	movs r0, #16
	bl 0x0200ac1c
	movs r0, #20
	bl 0x0200abec
	adds r1, r5, #0
	movs r0, #12
	bl 0x0200ac1c
	movs r0, #20
	bl 0x0200abec
	adds r1, r5, #0
	movs r0, #18
	bl 0x0200ac1c
	movs r0, #60
	bl 0x0200abec
	movs r0, #0
	adds r1, r5, #0
	bl 0x0200ac1c
	movs r0, #80
	bl 0x0200abec
	movs r0, #66
	bl 0x0200acfc
	b .L_02000dcc_4
.L_02000dcc_3:
	ldr r3, [pc, #96]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	ldr r0, [pc, #36]
	movs r1, #0
	bl 0x0200aca4
.L_02000dcc_4:
	bl 0x0200abfc
.L_02000dcc_1:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00000235
	.4byte 0x00019999
	.4byte 0x00003333
	.4byte 0x00002125
	.4byte 0x00008008
	.4byte 0x0000212b
	.4byte 0x0000400c
	.4byte 0x00004011
	.4byte 0x00004012
	.4byte 0x0000800b
	.4byte 0x0000cccc
	.4byte 0x0200ae20
	.4byte 0x0200aed4
	.4byte 0x0200ae5c
	.4byte 0x0200af24
	.4byte 0x03001ebc
	.global Func_020011ac
	.thumb_func
Func_020011ac:
	push {lr}
	ldr r0, [pc, #16]
	bl 0x0200abcc
	movs r0, #141
	lsls r0, r0, #2
	bl 0x0200abc4
	pop {r0}
	bx r0
	.4byte 0x00000235
	.global Func_020011c4
	.thumb_func
Func_020011c4:
	push {r5, lr}
	bl 0x0200abf4
	ldr r0, [pc, #176]
	bl 0x0200abbc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020011c4_0
	movs r1, #0
	movs r2, #40
	movs r0, #17
	bl 0x0200ac84
	ldr r0, [pc, #156]
	bl 0x0200ac94
	movs r0, #17
	bl 0x02009c48
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200acbc
	b .L_020011c4_1
.L_020011c4_0:
	movs r1, #2
	movs r0, #17
	bl 0x0200ac74
	ldr r0, [pc, #128]
	bl 0x0200ac94
	movs r1, #0
	movs r0, #17
	bl 0x0200aca4
	bl 0x0200acf4
	adds r0, #85
	strb r5, [r0]
	movs r0, #1
	bl 0x0200ab8c
	ldr r0, [pc, #104]
	ldr r1, [pc, #104]
	bl 0x0200acdc
	movs r0, #135
	movs r1, #1
	movs r2, #208
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl 0x0200ace4
	bl 0x0200acec
	ldr r3, [pc, #80]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #64
	str r3, [r2]
	subs r3, #56
	adds r2, r1, r3
	movs r3, #32
	str r3, [r2]
	bl 0x0200ad24
	bl 0x0200ad2c
	ldr r0, [pc, #56]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_020011c4_2
	movs r0, #70
	bl 0x0200acfc
	b .L_020011c4_1
.L_020011c4_2:
	movs r0, #7
	bl 0x0200acfc
.L_020011c4_1:
	bl 0x0200abfc
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x000008a4
	.4byte 0x0000206f
	.4byte 0x0000206d
	.4byte 0x00066666
	.4byte 0x0000cccc
	.4byte 0x03001ebc
	.4byte 0x000008a3
	.global Func_02001298
	.thumb_func
Func_02001298:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	bl 0x0200abf4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r1, r1
	negs r0, r0
	bl 0x0200ace4
	movs r0, #247
	bl 0x0200ad34
	movs r0, #8
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #9
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #10
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #11
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #12
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #13
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #14
	movs r1, #0
	bl 0x0200ac5c
	movs r0, #15
	movs r1, #0
	bl 0x0200ac5c
	movs r0, #16
	movs r1, #0
	bl 0x0200ac5c
	movs r0, #17
	movs r1, #0
	bl 0x0200ac5c
	movs r1, #0
	movs r0, #18
	bl 0x0200ac5c
	movs r0, #21
	bl 0x0200ac0c
	movs r1, #0
	bl 0x0200aba4
	movs r0, #19
	bl 0x0200ac0c
	ldr r5, [pc, #704]
	str r5, [r0, #24]
	movs r0, #20
	bl 0x0200ac0c
	str r5, [r0, #24]
	movs r0, #1
	bl 0x0200ab8c
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x0200ac54
	movs r0, #1
	bl 0x0200ab8c
	ldr r1, [pc, #672]
	movs r3, #224
	lsls r3, r3, #1
	ldr r2, [r1]
	mov r10, r3
	mov r8, r1
	adds r3, #64
	mov r1, r10
	str r3, [r2, r1]
	subs r3, #56
	mov r9, r3
	mov r1, r9
	movs r3, #32
	str r3, [r2, r1]
	bl 0x0200ad1c
	bl 0x0200ad2c
	movs r0, #40
	bl 0x0200abec
	movs r1, #1
	movs r0, #16
	bl 0x0200ac7c
	movs r0, #20
	bl 0x0200abec
	movs r0, #16
	ldr r1, [pc, #620]
	ldr r2, [pc, #620]
	bl 0x0200ac14
	movs r2, #226
	lsls r2, r2, #2
	movs r1, #164
	movs r0, #16
	bl 0x0200ac4c
	movs r0, #20
	bl 0x0200abec
	movs r1, #9
	movs r0, #16
	bl 0x0200ac5c
	movs r0, #40
	bl 0x0200abec
	movs r1, #10
	movs r0, #16
	bl 0x0200ac5c
	movs r0, #60
	bl 0x0200abec
	movs r1, #1
	movs r0, #16
	bl 0x0200ac5c
	movs r0, #20
	bl 0x0200abec
	movs r2, #230
	movs r0, #16
	movs r1, #164
	lsls r2, r2, #2
	bl 0x0200ac4c
	movs r2, #230
	movs r0, #16
	movs r1, #185
	lsls r2, r2, #2
	bl 0x0200ac4c
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200acbc
	movs r2, #229
	lsls r2, r2, #2
	movs r0, #16
	movs r1, #185
	bl 0x0200ac4c
	movs r1, #11
	movs r0, #16
	bl 0x0200ac5c
	movs r0, #40
	bl 0x0200abec
	movs r1, #1
	movs r0, #16
	bl 0x0200ac7c
	movs r0, #60
	bl 0x0200abec
	movs r1, #3
	movs r0, #16
	bl 0x0200ac7c
	movs r0, #40
	bl 0x0200abec
	ldr r1, [pc, #468]
	movs r0, #16
	bl 0x0200ac1c
	movs r0, #80
	bl 0x0200abec
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #16
	bl 0x0200accc
	movs r0, #60
	bl 0x0200abec
	movs r1, #208
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200acbc
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200acbc
	movs r1, #128
	movs r2, #20
	movs r0, #18
	lsls r1, r1, #8
	bl 0x0200acbc
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl 0x0200accc
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl 0x0200accc
	movs r1, #129
	movs r0, #17
	lsls r1, r1, #1
	bl 0x0200accc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #18
	bl 0x0200accc
	movs r0, #60
	bl 0x0200abec
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200acbc
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200acbc
	movs r6, #192
	movs r1, #192
	movs r2, #0
	lsls r6, r6, #6
	movs r0, #17
	lsls r1, r1, #6
	bl 0x0200acbc
	adds r1, r6, #0
	movs r0, #18
	bl 0x02009c5c
	movs r0, #16
	bl 0x0200ac24
	movs r0, #16
	bl 0x0200ac0c
	movs r5, #128
	movs r3, #208
	lsls r3, r3, #8
	lsls r5, r5, #9
	strh r3, [r0, #6]
	str r5, [r0, #24]
	str r5, [r0, #28]
	movs r0, #20
	bl 0x0200abec
	movs r1, #0
	movs r0, #16
	bl 0x0200ac5c
	movs r0, #40
	bl 0x0200abec
	movs r0, #19
	movs r1, #5
	bl 0x0200ac5c
	movs r1, #5
	movs r0, #20
	bl 0x0200ac5c
	movs r0, #60
	bl 0x0200abec
	movs r2, #20
	adds r1, r6, #0
	movs r0, #16
	bl 0x0200acbc
	movs r1, #8
	movs r0, #16
	bl 0x0200ac5c
	movs r0, #20
	bl 0x0200abec
	movs r0, #14
	movs r1, #4
	bl 0x0200ac5c
	movs r0, #15
	movs r1, #4
	bl 0x0200ac5c
	movs r0, #17
	movs r1, #4
	bl 0x0200ac5c
	movs r1, #4
	movs r0, #18
	bl 0x0200ac6c
	movs r0, #40
	bl 0x0200abec
	movs r1, #4
	movs r0, #16
	bl 0x0200ac6c
	movs r0, #10
	bl 0x0200abec
	movs r1, #128
	adds r2, r5, #0
	movs r0, #16
	lsls r1, r1, #10
	bl 0x0200ac14
	movs r2, #229
	movs r0, #16
	movs r1, #162
	lsls r2, r2, #2
	bl 0x0200ac4c
	ldr r2, [pc, #136]
	movs r0, #16
	movs r1, #162
	bl 0x0200ac4c
	movs r0, #19
	movs r1, #1
	bl 0x0200ac5c
	movs r0, #20
	movs r1, #1
	bl 0x0200ac5c
	movs r0, #16
	movs r1, #184
	ldr r2, [pc, #108]
	bl 0x0200ac4c
	movs r2, #199
	movs r0, #16
	movs r1, #184
	lsls r2, r2, #2
	bl 0x0200ac4c
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200ac54
	mov r3, r8
	ldr r2, [r3]
	ldr r3, [pc, #80]
	mov r1, r10
	str r3, [r2, r1]
	mov r1, r9
	movs r3, #16
	str r3, [r2, r1]
	bl 0x0200ad24
	bl 0x0200ad2c
	mov r3, r8
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r10
	str r3, [r2, r1]
	movs r0, #69
	bl 0x0200acfc
	bl 0x0200abfc
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0xffff0000
	.4byte 0x03001ebc
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0200af88
	.4byte 0x0000037a
	.4byte 0x0000035f
	.4byte 0x00000201
	.global Func_0200160c
	.thumb_func
Func_0200160c:
	push {r5, lr}
	bl 0x0200abf4
	ldr r5, [pc, #100]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #65
	str r2, [r3]
	bl 0x0200ad1c
	bl 0x0200ad2c
	movs r0, #20
	bl 0x0200abec
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #17
	bl 0x02009c5c
	ldr r0, [pc, #64]
	bl 0x0200ac94
	ldr r0, [pc, #64]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_0200160c_0
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_0200160c_0:
	movs r0, #17
	bl 0x02009c48
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #17
	bl 0x02009c5c
	ldr r0, [pc, #28]
	bl 0x0200abc4
	bl 0x0200abfc
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0000206e
	.4byte 0x000008a4
	.4byte 0x000008a3
	.global Func_02001688
	.thumb_func
Func_02001688:
	push {r5, r6, r7, lr}
	bl 0x0200abf4
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r1, r1
	negs r0, r0
	bl 0x0200ace4
	movs r0, #247
	bl 0x0200ad34
	movs r0, #8
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #9
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #10
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #11
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #12
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #13
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #14
	movs r1, #0
	bl 0x0200ac5c
	movs r0, #15
	movs r1, #0
	bl 0x0200ac5c
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl 0x0200ac54
	movs r0, #17
	movs r1, #0
	bl 0x0200ac5c
	movs r1, #0
	movs r0, #18
	bl 0x0200ac5c
	movs r0, #21
	bl 0x0200ac0c
	movs r1, #0
	bl 0x0200aba4
	movs r0, #19
	bl 0x0200ac0c
	ldr r5, [pc, #720]
	str r5, [r0, #24]
	movs r0, #20
	bl 0x0200ac0c
	str r5, [r0, #24]
	movs r0, #1
	bl 0x0200ab8c
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x0200ac54
	movs r0, #1
	bl 0x0200ab8c
	ldr r3, [pc, #688]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #64
	str r3, [r2]
	subs r3, #56
	adds r2, r1, r3
	movs r3, #32
	str r3, [r2]
	bl 0x0200ad1c
	bl 0x0200ad2c
	movs r0, #40
	bl 0x0200abec
	movs r1, #1
	movs r0, #17
	bl 0x0200ac7c
	movs r0, #20
	bl 0x0200abec
	movs r0, #17
	ldr r1, [pc, #640]
	ldr r2, [pc, #644]
	bl 0x0200ac14
	movs r2, #226
	lsls r2, r2, #2
	movs r1, #164
	movs r0, #17
	bl 0x0200ac4c
	movs r0, #20
	bl 0x0200abec
	movs r1, #9
	movs r0, #17
	bl 0x0200ac5c
	movs r0, #40
	bl 0x0200abec
	movs r1, #10
	movs r0, #17
	bl 0x0200ac5c
	movs r0, #60
	bl 0x0200abec
	movs r1, #1
	movs r0, #17
	bl 0x0200ac5c
	movs r0, #20
	bl 0x0200abec
	movs r2, #230
	movs r0, #17
	movs r1, #164
	lsls r2, r2, #2
	bl 0x0200ac4c
	movs r2, #230
	movs r0, #17
	movs r1, #185
	lsls r2, r2, #2
	bl 0x0200ac4c
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200acbc
	movs r2, #229
	lsls r2, r2, #2
	movs r0, #17
	movs r1, #185
	bl 0x0200ac4c
	movs r1, #11
	movs r0, #17
	bl 0x0200ac5c
	movs r0, #40
	bl 0x0200abec
	movs r1, #1
	movs r0, #17
	bl 0x0200ac7c
	movs r0, #60
	bl 0x0200abec
	movs r1, #3
	movs r0, #17
	bl 0x0200ac7c
	movs r0, #40
	bl 0x0200abec
	ldr r1, [pc, #492]
	movs r0, #17
	bl 0x0200ac1c
	movs r0, #80
	bl 0x0200abec
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl 0x0200accc
	movs r0, #60
	bl 0x0200abec
	movs r1, #208
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200acbc
	movs r1, #128
	movs r2, #20
	movs r0, #18
	lsls r1, r1, #8
	bl 0x0200acbc
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl 0x0200accc
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl 0x0200accc
	movs r1, #129
	movs r0, #17
	lsls r1, r1, #1
	bl 0x0200accc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #18
	bl 0x0200accc
	movs r0, #60
	bl 0x0200abec
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200acbc
	movs r1, #192
	movs r2, #0
	movs r0, #15
	lsls r1, r1, #6
	bl 0x0200acbc
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #18
	bl 0x02009c5c
	movs r0, #17
	ldr r1, [pc, #348]
	bl 0x0200accc
	movs r0, #21
	bl 0x0200ac0c
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	movs r3, #0
	strb r3, [r7]
	movs r5, #0
.L_02001688_0:
	ldr r3, [r6, #12]
	ldr r2, [pc, #324]
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #4
	bl 0x0200ab8c
	ldr r3, [r6, #12]
	ldr r2, [pc, #316]
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #4
	adds r5, #1
	bl 0x0200ab8c
	cmp r5, #19
	bls .L_02001688_0
	movs r0, #19
	movs r1, #6
	bl 0x0200ac5c
	movs r1, #6
	movs r0, #20
	bl 0x0200ac5c
	movs r0, #60
	bl 0x0200abec
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #17
	bl 0x0200accc
	movs r0, #17
	bl 0x0200ac24
	movs r1, #1
	movs r0, #17
	bl 0x0200ac5c
	movs r0, #17
	bl 0x0200ac0c
	movs r3, #208
	lsls r3, r3, #8
	movs r5, #128
	strh r3, [r0, #6]
	lsls r5, r5, #9
	movs r3, #3
	strb r3, [r7]
	movs r0, #10
	str r5, [r6, #24]
	str r5, [r6, #28]
	bl 0x0200abec
	movs r0, #107
	bl 0x0200ad34
	adds r1, r5, #0
	adds r2, r5, #0
	adds r0, r5, #0
	bl 0x0200abac
	movs r0, #10
	bl 0x0200abec
	ldr r0, [pc, #204]
	bl 0x0200ad34
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #192]
	bl 0x0200abac
	bl 0x0200abb4
	movs r0, #17
	ldr r1, [pc, #184]
	ldr r2, [pc, #152]
	bl 0x0200ac14
	movs r2, #232
	movs r1, #208
	lsls r2, r2, #2
	movs r0, #17
	bl 0x0200ac4c
	movs r0, #92
	bl 0x0200ad34
	movs r1, #192
	movs r2, #20
	movs r0, #17
	lsls r1, r1, #6
	bl 0x0200acbc
	movs r1, #9
	movs r0, #17
	bl 0x0200ac5c
	movs r0, #20
	bl 0x0200abec
	movs r1, #10
	movs r0, #17
	bl 0x0200ac5c
	movs r0, #40
	bl 0x0200abec
	movs r1, #9
	movs r0, #17
	bl 0x0200ac5c
	movs r0, #20
	bl 0x0200abec
	movs r1, #10
	movs r0, #17
	bl 0x0200ac5c
	movs r0, #80
	bl 0x0200abec
	ldr r3, [pc, #56]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #65
	str r3, [r2]
	subs r3, #57
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	bl 0x0200ad24
	bl 0x0200ad2c
	ldr r0, [pc, #64]
	bl 0x0200abc4
	movs r0, #69
	bl 0x0200acfc
	bl 0x0200abfc
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffff0000
	.4byte 0x03001ebc
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0200af88
	.4byte 0x00000101
	.4byte 0x00009999
	.4byte 0xffffb334
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00019999
	.4byte 0x000008a4
	.global Func_02001a14
	.thumb_func
Func_02001a14:
	push {lr}
	ldr r3, [pc, #40]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #32]
	cmp r2, r3
	bne .L_02001a14_0
	bl 0x02009a4c
	b .L_02001a14_1
.L_02001a14_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_02001a14_1
	bl 0x02009c6c
.L_02001a14_1:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008c
	.4byte 0x0000008e
	.global Func_02001a4c
	.thumb_func
Func_02001a4c:
	push {lr}
	movs r0, #1
	bl 0x0200ab8c
	ldr r3, [pc, #476]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #5
	cmp r3, #65
	bls .L_02001a4c_0
	b .L_02001a4c_1
.L_02001a4c_0:
	ldr r2, [pc, #460]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	ldr r3, [sp, #480]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r3, [sp, #680]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r3, [sp, #816]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r3, [sp, #928]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #72]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r4, [sp, #184]
	lsls r0, r0, #8
	ldr r3, [sp, #728]
	lsls r0, r0, #8
	ldr r3, [sp, #768]
	lsls r0, r0, #8
	ldr r3, [sp, #792]
	lsls r0, r0, #8
	ldr r4, [sp, #24]
	lsls r0, r0, #8
	ldr r4, [sp, #48]
	lsls r0, r0, #8
	ldr r3, [sp, #552]
	lsls r0, r0, #8
	ldr r3, [sp, #704]
	lsls r0, r0, #8
	movs r0, #8
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #9
	movs r1, #2
	bl 0x0200ac5c
	b .L_02001a4c_1
	.2byte 0x2008
	.2byte 0x2102
	.2byte 0xf001
	.2byte 0xf865
	.2byte 0x2009
	.2byte 0x2102
	.2byte 0xf001
	.2byte 0xf861
	.2byte 0x4828
	.2byte 0xf001
	.2byte 0xf80e
	.2byte 0x2800
	.2byte 0xd144
	.2byte 0xf7ff
	.2byte 0xfd32
	.2byte 0xe041
	.2byte 0xf7ff
	.2byte 0xfb75
	.2byte 0xe03e
	.2byte 0xf7ff
	.2byte 0xfd6a
	.2byte 0xe03b
	.2byte 0xf000
	.2byte 0xf88d
	.2byte 0xf001
	.2byte 0xf813
	.2byte 0xe036
	.2byte 0xf000
	.2byte 0xfc0e
	.2byte 0xe033
	.2byte 0xf000
	.2byte 0xfd13
	.2byte 0xe030
	.2byte 0x20a2
	.2byte 0x0040
	.2byte 0xf000
	.2byte 0xfff8
	.2byte 0xf000
	.2byte 0xffa2
	.2byte 0x4818
	.2byte 0xf000
	.2byte 0xffef
	.2byte 0x2800
	.2byte 0xd125
	.2byte 0xf000
	.2byte 0xfc79
	.2byte 0xe022
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfff3
	.2byte 0x2002
	.2byte 0xf000
	.2byte 0xfff0
	.2byte 0x2003
	.2byte 0xf000
	.2byte 0xffed
	.2byte 0x4811
	.2byte 0xf000
	.2byte 0xffe2
	.2byte 0xf000
	.2byte 0xfd32
	.2byte 0xe013
	.2byte 0xf000
	.2byte 0xfe0b
	.2byte 0xe010
	.2byte 0xf000
	.2byte 0xfe7a
	.2byte 0xe00d
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xffde
	.2byte 0x2002
	.2byte 0xf000
	.2byte 0xffdb
	.2byte 0x2003
	.2byte 0xf000
	.2byte 0xffd8
	.2byte 0x4807
	.2byte 0xf000
	.2byte 0xffcd
	.2byte 0xf000
	.2byte 0xfe9b
.L_02001a4c_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02009a70
	.2byte 0x0109
	.2byte 0x0000
	.2byte 0x090e
	.2byte 0x0000
	.2byte 0x090f
	.2byte 0x0000
	.global Func_02001c48
	.thumb_func
Func_02001c48:
	push {lr}
	movs r1, #0
	bl 0x0200aca4
	movs r0, #10
	bl 0x0200abec
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001c5c
	.thumb_func
Func_02001c5c:
	push {lr}
	lsls r1, r1, #16
	lsrs r1, r1, #16
	movs r2, #10
	bl 0x0200acbc
	pop {r0}
	bx r0
	.global Func_02001c6c
	.thumb_func
Func_02001c6c:
	push {lr}
	movs r0, #149
	lsls r0, r0, #4
	sub sp, #8
	bl 0x0200abbc
	cmp r0, #0
	beq .L_02001c6c_0
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #48
	movs r3, #5
	bl 0x0200ab94
	movs r3, #16
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #8
	movs r2, #2
	movs r3, #1
	bl 0x0200ab9c
	b .L_02001c6c_1
.L_02001c6c_0:
	movs r0, #16
	movs r1, #2
	bl 0x0200ac8c
	ldr r0, [pc, #32]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_02001c6c_1
	movs r3, #14
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #22
	movs r2, #1
	movs r3, #2
	bl 0x0200ab9c
.L_02001c6c_1:
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000962
	.global Func_02001cd4
	.thumb_func
Func_02001cd4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl 0x0200abf4
	movs r1, #198
	movs r2, #136
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200ac54
	movs r1, #206
	movs r2, #136
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200ac54
	movs r1, #202
	movs r2, #152
	lsls r1, r1, #18
	lsls r2, r2, #16
	movs r0, #3
	bl 0x0200ac54
	movs r0, #1
	bl 0x0200ab8c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	negs r1, r1
	bl 0x0200ace4
	movs r0, #0
	movs r1, #0
	bl 0x0200ad0c
	movs r1, #0
	movs r0, #0
	bl 0x0200ad04
	movs r0, #1
	bl 0x0200ad14
	movs r0, #1
	bl 0x0200ab8c
	ldr r3, [pc, #1016]
	ldr r1, [r3]
	mov r9, r3
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #67
	str r3, [r2]
	subs r3, #59
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	bl 0x0200ad1c
	bl 0x0200ad2c
	movs r0, #0
	movs r1, #0
	bl 0x0200ad0c
	movs r1, #0
	ldr r0, [pc, #980]
	bl 0x0200ad04
	movs r0, #40
	bl 0x0200ad14
	movs r0, #80
	bl 0x0200abec
	movs r1, #1
	movs r0, #8
	bl 0x0200ac7c
	movs r0, #20
	bl 0x0200abec
	movs r1, #2
	movs r0, #2
	bl 0x0200ac7c
	movs r0, #40
	bl 0x0200abec
	movs r3, #192
	lsls r3, r3, #6
	mov r8, r3
	movs r0, #8
	mov r1, r8
	bl 0x02009c5c
	movs r1, #2
	movs r0, #8
	bl 0x0200ac7c
	movs r0, #20
	bl 0x0200abec
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl 0x0200ad04
	movs r0, #40
	bl 0x0200ad14
	movs r0, #80
	bl 0x0200abec
	movs r1, #128
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #1
	bl 0x0200acc4
	movs r1, #1
	movs r0, #2
	bl 0x0200ac7c
	movs r0, #20
	bl 0x0200abec
	ldr r0, [pc, #860]
	bl 0x0200ac94
	movs r0, #2
	bl 0x02009c48
	movs r1, #3
	movs r0, #8
	bl 0x0200ac6c
	movs r0, #8
	bl 0x02009c48
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #3
	bl 0x0200acc4
	movs r0, #3
	bl 0x02009c48
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r0, #9
	movs r1, #0
	movs r2, #40
	bl 0x0200acbc
	movs r1, #160
	movs r2, #0
	movs r0, #8
	lsls r1, r1, #7
	bl 0x0200acbc
	movs r0, #9
	mov r1, r8
	bl 0x02009c5c
	ldr r1, [pc, #780]
	movs r2, #20
	movs r0, #9
	bl 0x0200acc4
	movs r0, #9
	bl 0x02009c48
	movs r2, #20
	movs r0, #1
	ldr r1, [pc, #764]
	bl 0x0200acc4
	movs r1, #2
	movs r0, #1
	bl 0x0200ac74
	movs r0, #1
	bl 0x02009c48
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #10
	bl 0x0200acc4
	movs r0, #10
	bl 0x02009c48
	movs r1, #3
	movs r0, #11
	bl 0x0200ac6c
	movs r0, #11
	bl 0x02009c48
	movs r3, #160
	lsls r3, r3, #8
	mov r10, r3
	movs r0, #2
	mov r1, r10
	bl 0x02009c5c
	movs r1, #4
	movs r0, #2
	bl 0x0200ac6c
	movs r0, #2
	bl 0x02009c48
	movs r1, #1
	movs r0, #3
	bl 0x0200ac7c
	movs r0, #3
	bl 0x02009c48
	movs r2, #20
	movs r0, #1
	ldr r1, [pc, #664]
	bl 0x0200acc4
	movs r0, #1
	movs r1, #0
	bl 0x02009c5c
	movs r5, #192
	movs r1, #2
	movs r0, #1
	lsls r5, r5, #7
	bl 0x0200ac74
	movs r0, #1
	bl 0x02009c48
	adds r1, r5, #0
	movs r0, #0
	bl 0x02009c5c
	movs r1, #129
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200acc4
	movs r0, #2
	movs r1, #1
	bl 0x0200ac7c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #2
	bl 0x02009c5c
	movs r0, #2
	bl 0x02009c48
	movs r1, #4
	movs r0, #3
	bl 0x0200ac5c
	movs r0, #20
	bl 0x0200abec
	movs r6, #128
	movs r0, #3
	bl 0x02009c48
	lsls r6, r6, #6
	movs r0, #1
	movs r1, #1
	bl 0x0200ac7c
	adds r1, r6, #0
	movs r0, #1
	bl 0x02009c5c
	movs r0, #1
	bl 0x02009c48
	movs r0, #2
	ldr r1, [pc, #536]
	movs r2, #60
	bl 0x0200acc4
	movs r0, #2
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200acbc
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200acbc
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200acbc
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acc4
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acc4
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200acc4
	movs r1, #131
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #1
	movs r7, #192
	lsls r7, r7, #8
	bl 0x0200acc4
	movs r0, #2
	movs r1, #1
	bl 0x0200ac7c
	adds r1, r7, #0
	movs r0, #2
	bl 0x02009c5c
	movs r0, #2
	bl 0x02009c48
.L_02001f9e:
	movs r0, #0
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200acbc
	movs r0, #1
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200acbc
	movs r2, #20
	movs r0, #3
	adds r1, r7, #0
	bl 0x0200acbc
	movs r0, #8
	mov r1, r8
	bl 0x02009c5c
	movs r1, #3
	movs r0, #8
	bl 0x0200ac6c
	movs r0, #8
	bl 0x02009c48
.L_02001fd2:
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acc4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200acc4
	movs r1, #128
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #1
	bl 0x0200acc4
	movs r1, #1
	movs r0, #9
	bl 0x0200ac7c
	movs r0, #9
	bl 0x02009c48
	adds r1, r5, #0
	movs r0, #0
	movs r2, #0
	bl 0x0200acbc
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	adds r1, r5, #0
	movs r0, #2
	movs r2, #0
	bl 0x0200acbc
	movs r1, #224
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200acbc
	movs r1, #1
	movs r0, #10
	bl 0x0200ac7c
	movs r0, #10
	bl 0x02009c48
	movs r0, #0
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200acbc
	movs r0, #1
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200acbc
	movs r2, #0
	movs r0, #2
	adds r1, r7, #0
	bl 0x0200acbc
	movs r0, #3
	adds r1, r7, #0
	bl 0x02009c5c
	movs r1, #3
	movs r0, #11
	bl 0x0200ac6c
	movs r0, #11
	bl 0x02009c48
	adds r1, r5, #0
	movs r0, #0
	movs r2, #0
	bl 0x0200acbc
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	adds r1, r5, #0
	movs r0, #2
	movs r2, #0
	bl 0x0200acbc
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200acbc
	movs r0, #1
	ldr r1, [pc, #164]
	movs r2, #0
	bl 0x0200acc4
	movs r0, #2
	ldr r1, [pc, #156]
	movs r2, #0
	bl 0x0200acc4
	movs r0, #3
	ldr r1, [pc, #144]
	movs r2, #80
	bl 0x0200acc4
	movs r1, #131
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #1
	movs r5, #224
	bl 0x0200acc4
	lsls r5, r5, #8
	movs r0, #2
	movs r1, #1
	bl 0x0200ac7c
	adds r1, r5, #0
	movs r0, #2
.L_020020d8:
	bl 0x02009c5c
	movs r0, #2
	bl 0x02009c48
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200acbc
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200acbc
	movs r2, #60
	movs r0, #11
	ldr r1, [pc, #84]
	bl 0x0200acc4
	movs r1, #0
	movs r0, #2
	bl 0x0200ac9c
	movs r0, #0
	adds r1, r6, #0
	movs r2, #0
	bl 0x0200acbc
	adds r1, r5, #0
	movs r0, #1
	movs r2, #0
	bl 0x0200acbc
	movs r0, #2
	mov r1, r10
	movs r2, #0
	bl 0x0200acbc
	movs r0, #3
	adds r1, r7, #0
	movs r2, #0
	bl 0x0200acbc
	movs r0, #0
.L_02002132:
	movs r1, #0
	movs r5, #0
	bl 0x0200ac04
	b .L_02002132_0
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0002
	.2byte 0x0001
	.2byte 0x20f8
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x0103
	.2byte 0x0000
	.2byte 0x0101
	.2byte 0x0000
.L_02002132_0:
	cmp r0, #1
	bne .L_02002132_1
	movs r0, #2
	bl 0x02009c48
	movs r5, #1
	b 0x0200a188
.L_02002132_1:
	mov r3, r9
.L_02002164:
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #2
.L_02002174:
	movs r1, #3
	bl 0x0200ac6c
	movs r0, #2
	adds r1, r7, #0
	bl 0x02009c5c
	movs r0, #2
	bl 0x02009c48
	cmp r5, #0
.L_0200218a:
	beq .L_0200218a_0
	ldr r3, [pc, #568]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_0200218a_0:
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
.L_020021a8:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200acbc
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200acbc
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200acbc
	movs r0, #8
	ldr r1, [pc, #508]
	movs r2, #0
	bl 0x0200acc4
	movs r0, #9
	ldr r1, [pc, #500]
	movs r2, #0
	bl 0x0200acc4
	movs r0, #10
	ldr r1, [pc, #488]
	movs r2, #0
	bl 0x0200acc4
.L_020021e8:
	movs r5, #192
	movs r2, #60
	movs r0, #11
.L_020021ee:
	ldr r1, [pc, #476]
	bl 0x0200acc4
	lsls r5, r5, #6
	movs r0, #8
	movs r1, #1
	bl 0x0200ac7c
	adds r1, r5, #0
.L_02002200:
	movs r0, #8
	bl 0x02009c5c
	movs r0, #8
	bl 0x02009c48
	movs r1, #192
	movs r0, #0
.L_02002210:
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #192
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x0200acbc
	movs r0, #9
	movs r1, #1
	bl 0x0200ac7c
	adds r1, r5, #0
	movs r0, #9
	bl 0x02009c5c
	movs r0, #9
	bl 0x02009c48
.L_02002252:
	movs r0, #10
	movs r1, #1
	bl 0x0200ac7c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #10
	movs r6, #128
	bl 0x02009c5c
	lsls r6, r6, #8
.L_02002268:
	movs r0, #10
	bl 0x02009c48
	adds r1, r6, #0
	movs r0, #11
	bl 0x02009c5c
.L_02002276:
	movs r1, #3
	movs r0, #11
	bl 0x0200ac6c
	movs r0, #11
	bl 0x02009c48
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #160
	movs r2, #20
.L_02002294:
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200acbc
	movs r0, #1
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #2
	movs r1, #3
	bl 0x0200ac5c
	movs r0, #3
	movs r1, #3
	bl 0x0200ac6c
	movs r1, #128
	adds r2, r6, #0
	movs r0, #1
	lsls r1, r1, #9
	bl 0x0200ac14
	movs r1, #128
	adds r2, r6, #0
	movs r0, #2
	lsls r1, r1, #9
	bl 0x0200ac14
	movs r1, #128
	adds r2, r6, #0
	movs r0, #3
	lsls r1, r1, #9
	bl 0x0200ac14
	ldr r5, [pc, #244]
	movs r0, #1
	adds r1, r5, #0
.L_020022de:
	bl 0x0200ac1c
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200ac1c
	adds r1, r5, #0
.L_020022ec:
	movs r0, #3
	bl 0x0200ac2c
	movs r0, #20
	bl 0x0200abec
	movs r0, #0
	movs r1, #0
	bl 0x02009c5c
	movs r0, #0
	movs r1, #3
	bl 0x0200ac6c
	movs r0, #11
.L_0200230a:
	movs r1, #3
	bl 0x0200ac6c
	movs r1, #128
	adds r2, r6, #0
	movs r0, #11
	lsls r1, r1, #9
	bl 0x0200ac14
	movs r1, #128
	adds r2, r6, #0
	movs r0, #0
	lsls r1, r1, #9
	bl 0x0200ac14
	movs r0, #11
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #11
	ldr r1, [pc, #160]
	movs r2, #152
	bl 0x0200ac3c
	movs r1, #202
	movs r0, #11
	lsls r1, r1, #2
	movs r2, #164
	bl 0x0200ac3c
	movs r1, #202
	movs r2, #156
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #11
	bl 0x0200ac34
	movs r0, #20
	bl 0x0200abec
	ldr r0, [pc, #124]
	ldr r1, [pc, #124]
	bl 0x0200acdc
	movs r0, #202
	movs r1, #1
	movs r2, #156
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200ace4
	movs r1, #202
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #164
	bl 0x0200ac4c
	movs r1, #202
	movs r2, #156
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #0
	bl 0x0200ac44
	movs r0, #60
	bl 0x0200abec
	ldr r3, [pc, #48]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	subs r3, #192
	str r3, [r2]
	adds r3, #200
	adds r2, r1, r3
	movs r3, #40
	str r3, [r2]
	bl 0x0200ad24
	bl 0x0200ad2c
.L_020023b2:
	movs r0, #64
	bl 0x0200acfc
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0xadac
	.2byte 0x0200
	.2byte 0x033e
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0ccc
	.2byte 0x0000
	.global Func_020023e0
	.thumb_func
Func_020023e0:
	push {lr}
	movs r0, #5
	bl 0x0200abbc
	cmp r0, #0
	beq .L_020023e0_0
	ldr r0, [pc, #216]
	bl 0x0200abc4
	movs r0, #5
	bl 0x0200abdc
	movs r0, #3
	bl 0x0200abd4
.L_020023e0_0:
	bl 0x0200abf4
	movs r1, #178
	movs r2, #147
	lsls r2, r2, #18
	lsls r1, r1, #18
	movs r0, #11
	bl 0x0200ac54
.L_02002410:
	movs r0, #1
	bl 0x0200ab8c
	movs r0, #11
	movs r1, #1
	bl 0x0200acd4
.L_0200241e:
	movs r0, #11
	ldr r1, [pc, #168]
	ldr r2, [pc, #172]
	bl 0x0200ac14
	ldr r2, [pc, #164]
	ldr r1, [pc, #160]
	movs r0, #0
	bl 0x0200ac14
	movs r0, #11
	bl 0x0200ac0c
	movs r3, #0
	strh r3, [r0, #6]
.L_0200243c:
	bl 0x0200ad1c
	movs r0, #0
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #11
.L_0200244a:
	movs r1, #2
	bl 0x0200ac5c
	movs r1, #195
	movs r2, #147
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac34
	movs r1, #203
	movs r2, #147
.L_02002462:
	movs r0, #11
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac3c
	movs r1, #220
	movs r2, #147
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac34
	movs r1, #228
	movs r2, #147
	movs r0, #11
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac3c
	movs r1, #245
	movs r2, #147
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac34
	movs r1, #253
	movs r2, #147
	movs r0, #11
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac34
	bl 0x0200ad24
	bl 0x0200ad2c
	ldr r0, [pc, #36]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_02002462_0
	movs r0, #31
	bl 0x0200acfc
	b .L_02002462_1
.L_02002462_0:
	movs r0, #65
	bl 0x0200acfc
.L_02002462_1:
	pop {r0}
	bx r0
	.2byte 0x016d
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0xcccc
	.2byte 0x0000
	.4byte 0x0000090f
	.global Func_020024d8
	.thumb_func
Func_020024d8:
	push {r5, lr}
	movs r0, #13
	bl 0x0200ac0c
	adds r5, r0, #0
	bl 0x0200abf4
	bl 0x0200ad1c
	bl 0x0200ad2c
	movs r0, #40
	bl 0x0200abec
	movs r1, #2
	movs r0, #8
	bl 0x0200ac7c
	movs r0, #13
	bl 0x0200ac24
	movs r0, #1
	bl 0x0200ab8c
	movs r1, #224
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200acbc
	movs r0, #13
	movs r1, #1
	bl 0x0200ac5c
	movs r1, #208
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x0200acbc
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #208
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
.L_02002546:
	bl 0x0200acbc
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #176
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #18
	bl 0x0200acbc
	ldr r0, [pc, #124]
	bl 0x0200ac94
	movs r0, #8
	bl 0x02009c48
	movs r0, #0
	movs r1, #3
	bl 0x0200ac6c
	adds r2, r5, #0
	movs r3, #180
	adds r2, #100
	lsls r3, r3, #2
	strh r3, [r2]
	adds r5, #102
	movs r3, #112
	strh r3, [r5]
	movs r0, #13
	movs r1, #2
	bl 0x0200ac1c
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200acbc
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200acbc
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x0200acbc
	movs r1, #160
	movs r0, #17
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200acbc
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200acbc
	bl 0x0200abfc
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002112
	.global Func_020025f0
	.thumb_func
Func_020025f0:
	push {lr}
	bl 0x0200abf4
	ldr r2, [pc, #100]
	movs r0, #0
	ldr r1, [pc, #100]
	bl 0x0200ac14
	bl 0x0200ad1c
	movs r0, #0
	movs r1, #2
	bl 0x0200ac5c
	movs r1, #195
	movs r2, #214
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200ac3c
	movs r1, #220
	movs r2, #214
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200ac3c
.L_02002628:
	movs r1, #245
	movs r2, #214
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200ac34
	bl 0x0200ad24
	bl 0x0200ad2c
	ldr r0, [pc, #36]
	bl 0x0200abbc
	cmp r0, #0
	beq .L_02002628_0
	movs r0, #32
	bl 0x0200acfc
	b .L_02002628_1
.L_02002628_0:
	movs r0, #12
	bl 0x0200acfc
.L_02002628_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.4byte 0x0000090f
	.global Func_02002668
	.thumb_func
Func_02002668:
	push {r5, lr}
	bl 0x0200abf4
	movs r1, #198
	movs r2, #136
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200ac54
	movs r1, #206
	movs r2, #136
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200ac54
.L_0200268a:
	movs r1, #202
	movs r2, #152
	lsls r2, r2, #16
	lsls r1, r1, #18
	movs r0, #3
	bl 0x0200ac54
	bl 0x0200ad1c
	bl 0x0200ad2c
	movs r0, #40
	bl 0x0200abec
	movs r0, #8
	movs r1, #1
	bl 0x0200ac7c
	movs r1, #3
	movs r0, #8
	bl 0x0200ac5c
	ldr r0, [pc, #340]
	bl 0x0200ac94
	movs r0, #8
	bl 0x02009c48
	movs r1, #1
	movs r0, #9
	bl 0x0200ac7c
	movs r0, #9
	bl 0x02009c48
	movs r1, #1
	movs r0, #10
	bl 0x0200ac7c
	movs r0, #10
	bl 0x02009c48
	movs r0, #11
	movs r1, #1
	bl 0x0200ac7c
	movs r1, #3
	movs r0, #11
	bl 0x0200ac5c
	movs r0, #11
	bl 0x02009c48
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200acbc
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ac14
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ac14
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #3
	lsls r1, r1, #9
	bl 0x0200ac14
	ldr r5, [pc, #216]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200ac1c
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200ac1c
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200ac2c
	movs r0, #20
	bl 0x0200abec
	movs r0, #0
	movs r1, #0
	bl 0x02009c5c
	movs r0, #0
	movs r1, #3
	bl 0x0200ac6c
.L_02002766:
	movs r0, #11
	movs r1, #3
	bl 0x0200ac6c
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
.L_02002778:
	bl 0x0200ac14
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #0
	lsls r1, r1, #9
	bl 0x0200ac14
.L_0200278a:
	movs r0, #11
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #11
	ldr r1, [pc, #124]
	movs r2, #152
.L_02002798:
	bl 0x0200ac3c
	movs r1, #202
	movs r0, #11
	lsls r1, r1, #2
	movs r2, #164
	bl 0x0200ac3c
	movs r1, #202
	movs r2, #156
.L_020027ac:
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #11
	bl 0x0200ac34
	movs r0, #20
	bl 0x0200abec
	ldr r0, [pc, #88]
.L_020027be:
	ldr r1, [pc, #92]
	bl 0x0200acdc
	movs r0, #202
	movs r1, #1
	movs r2, #156
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200ace4
	movs r1, #202
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #164
	bl 0x0200ac4c
	movs r1, #202
	movs r2, #156
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #0
	bl 0x0200ac44
	movs r0, #60
	bl 0x0200abec
	bl 0x0200ad24
	bl 0x0200ad2c
	movs r0, #67
	bl 0x0200acfc
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x2134
	.2byte 0x0000
	.2byte 0xadac
	.2byte 0x0200
	.2byte 0x033e
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.4byte 0x00000ccc
	.global Func_02002820
	.thumb_func
Func_02002820:
	push {lr}
	movs r0, #5
	bl 0x0200abbc
	cmp r0, #0
	beq 0x0200a83e
	ldr r0, [pc, #200]
	bl 0x0200abc4
	movs r0, #5
	bl 0x0200abdc
.L_02002838:
	movs r0, #3
	bl 0x0200abd4
	bl 0x0200abf4
	movs r1, #217
	movs r2, #147
.L_02002846:
	lsls r2, r2, #18
	lsls r1, r1, #18
	movs r0, #11
	bl 0x0200ac54
	movs r0, #1
	bl 0x0200ab8c
	movs r0, #11
	movs r1, #1
	bl 0x0200acd4
	movs r0, #11
	ldr r1, [pc, #152]
	ldr r2, [pc, #156]
	bl 0x0200ac14
.L_02002868:
	ldr r2, [pc, #148]
	ldr r1, [pc, #144]
	movs r0, #0
	bl 0x0200ac14
	movs r0, #11
	bl 0x0200ac0c
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r0, #6]
	bl 0x0200ad1c
	movs r0, #0
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #11
	movs r1, #2
	bl 0x0200ac5c
	movs r1, #200
	movs r2, #147
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac34
	movs r1, #192
	movs r2, #147
	movs r0, #11
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac3c
	movs r1, #175
	movs r2, #147
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac34
	movs r1, #167
	movs r2, #147
	movs r0, #11
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac3c
	movs r1, #150
	movs r2, #147
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac34
	movs r1, #142
	movs r2, #147
	movs r0, #11
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200ac34
	bl 0x0200ad24
	bl 0x0200ad2c
	movs r0, #21
	bl 0x0200acfc
	pop {r0}
	bx r0
	.2byte 0x016d
	.2byte 0x0000
	.4byte 0x00019999
	.4byte 0x0000cccc
	.global Func_02002904
	.thumb_func
Func_02002904:
	push {lr}
	bl 0x0200abf4
	ldr r2, [pc, #80]
	movs r0, #0
	ldr r1, [pc, #80]
	bl 0x0200ac14
	bl 0x0200ad1c
	movs r0, #0
	movs r1, #2
	bl 0x0200ac5c
.L_02002920:
	movs r1, #200
	movs r2, #214
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200ac3c
	movs r1, #175
	movs r2, #214
	movs r0, #0
	lsls r1, r1, #2
.L_02002936:
	lsls r2, r2, #1
	bl 0x0200ac3c
.L_0200293c:
	movs r1, #150
	movs r2, #214
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200ac34
	bl 0x0200ad24
	bl 0x0200ad2c
	movs r0, #22
	bl 0x0200acfc
	pop {r0}
	bx r0
	.2byte 0xcccc
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.global Func_02002964
	.thumb_func
Func_02002964:
	push {r5, lr}
	bl 0x0200abf4
	movs r1, #198
	movs r2, #136
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200ac54
	movs r1, #206
.L_0200297a:
	movs r2, #136
	movs r0, #2
	lsls r1, r1, #18
.L_02002980:
	lsls r2, r2, #16
	bl 0x0200ac54
	movs r1, #202
	movs r2, #152
	lsls r2, r2, #16
	lsls r1, r1, #18
	movs r0, #3
	bl 0x0200ac54
	bl 0x0200ad1c
	bl 0x0200ad2c
	movs r0, #40
	bl 0x0200abec
	movs r0, #8
	movs r1, #1
	bl 0x0200ac7c
	movs r1, #3
	movs r0, #8
	bl 0x0200ac5c
	ldr r0, [pc, #340]
	bl 0x0200ac94
	movs r0, #8
	bl 0x02009c48
	movs r1, #1
	movs r0, #9
	bl 0x0200ac7c
	movs r0, #9
	bl 0x02009c48
	movs r1, #1
	movs r0, #10
	bl 0x0200ac7c
	movs r0, #10
	bl 0x02009c48
	movs r0, #11
	movs r1, #1
	bl 0x0200ac7c
	movs r1, #3
	movs r0, #11
	bl 0x0200ac5c
	movs r0, #11
	bl 0x02009c48
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200acbc
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200acbc
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ac14
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ac14
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #3
	lsls r1, r1, #9
	bl 0x0200ac14
	ldr r5, [pc, #216]
	movs r0, #1
	adds r1, r5, #0
	bl 0x0200ac1c
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200ac1c
	adds r1, r5, #0
	movs r0, #3
	bl 0x0200ac2c
	movs r0, #20
	bl 0x0200abec
	movs r0, #0
	movs r1, #0
	bl 0x02009c5c
	movs r0, #0
	movs r1, #3
	bl 0x0200ac6c
	movs r0, #11
	movs r1, #3
	bl 0x0200ac6c
	movs r1, #128
	movs r2, #128
.L_02002a6e:
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200ac14
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #0
	lsls r1, r1, #9
	bl 0x0200ac14
	movs r0, #11
	movs r1, #2
	bl 0x0200ac5c
	movs r0, #11
	ldr r1, [pc, #124]
	movs r2, #152
	bl 0x0200ac3c
	movs r1, #202
	movs r0, #11
	lsls r1, r1, #2
	movs r2, #164
	bl 0x0200ac3c
	movs r1, #202
	movs r2, #156
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #11
	bl 0x0200ac34
	movs r0, #20
	bl 0x0200abec
	ldr r0, [pc, #88]
	ldr r1, [pc, #92]
	bl 0x0200acdc
	movs r0, #202
	movs r1, #1
	movs r2, #156
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200ace4
.L_02002ad2:
	movs r1, #202
.L_02002ad4:
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #164
	bl 0x0200ac4c
	movs r1, #202
	movs r2, #156
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #0
	bl 0x0200ac44
	movs r0, #60
	bl 0x0200abec
.L_02002af2:
	bl 0x0200ad24
	bl 0x0200ad2c
.L_02002afa:
	movs r0, #64
	bl 0x0200acfc
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x2138
	.2byte 0x0000
	.2byte 0xadac
	.2byte 0x0200
	.2byte 0x033e
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0000
	.2byte 0x0ccc
	.2byte 0x0000
	.global Func_02002b1c
	.thumb_func
Func_02002b1c:
	push {lr}
	movs r0, #12
	movs r1, #3
	bl 0x0200ac8c
	movs r0, #13
	movs r1, #0
	bl 0x0200ac8c
	movs r0, #14
	movs r1, #4
.L_02002b32:
	bl 0x0200ac8c
	movs r0, #15
	movs r1, #1
	bl 0x0200ac8c
	movs r0, #16
.L_02002b40:
	movs r1, #5
	bl 0x0200ac8c
	movs r0, #17
	movs r1, #2
	bl 0x0200ac8c
	movs r0, #18
	movs r1, #6
	bl 0x0200ac8c
.L_02002b56:
	movs r0, #13
	movs r1, #10
	bl 0x0200ac64
	movs r0, #14
	movs r1, #20
	bl 0x0200ac64
	movs r0, #15
	movs r1, #0
	bl 0x0200ac5c
	movs r0, #16
	movs r1, #40
	bl 0x0200ac64
	movs r0, #17
	movs r1, #50
	bl 0x0200ac64
.L_02002b7e:
	movs r0, #18
	movs r1, #60
	bl 0x0200ac64
	pop {r0}
	bx r0
	.2byte 0x0000
	.include "games/THE BROKEN SEAL/SRC/FIELD/KORASHIAMU_IRIGUCHI/IMPORT.INC"
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x00920000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x037e0000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03720000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x003a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000800
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000048
	.4byte 0x40000048
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0x40000048
	.4byte 0x00100000
	.4byte 0x02100010
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000001e8
	.4byte 0x40000058
	.4byte 0x00100000
	.4byte 0x02100010
	.4byte 0x00000100
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x00000038
	.4byte 0x40000128
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000b8
	.4byte 0x40000048
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000180
	.4byte 0xffff0002
	.4byte 0x00000038
	.4byte 0x40000078
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000180
	.4byte 0xffff0003
	.4byte 0x00000078
	.4byte 0x400000b0
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000180
	.4byte 0xffff0004
	.4byte 0x00000038
	.4byte 0x40000128
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000180
	.4byte 0xffff0005
	.4byte 0x00000188
	.4byte 0x40000078
	.4byte 0x01100000
	.4byte 0x02900000
	.4byte 0x00000130
	.4byte 0xffff0006
	.4byte 0x00000208
	.4byte 0x40000048
	.4byte 0x01100000
	.4byte 0x02900000
	.4byte 0x00000130
	.4byte 0xffff0007
	.4byte 0x000000b8
	.4byte 0x40000390
	.4byte 0x00000000
	.4byte 0x017002e0
	.4byte 0x00000400
	.4byte 0xffff0008
	.4byte 0x00000328
	.4byte 0xc0000128
	.4byte 0x02900000
	.4byte 0x03c00000
	.4byte 0x00000140
	.4byte 0xffff000c
	.4byte 0x00000310
	.4byte 0x6000006c
	.4byte 0x02900000
	.4byte 0x03c00000
	.4byte 0x00000140
	.4byte 0xffff0015
	.4byte 0x00000328
	.4byte 0xc0000078
	.4byte 0x02900000
	.4byte 0x03c00000
	.4byte 0x00000140
	.4byte 0xffff001f
	.4byte 0x00000328
	.4byte 0xc0000078
	.4byte 0x02900000
	.4byte 0x03c00000
	.4byte 0x00000140
	.4byte 0xffff0040
	.4byte 0x00000328
	.4byte 0xc0000078
	.4byte 0x02900000
	.4byte 0x03c00000
	.4byte 0x00000140
	.4byte 0xffff0041
	.4byte 0x000002a8
	.4byte 0x0000024c
	.4byte 0x02080000
	.4byte 0x03fc01ea
	.4byte 0x00000280
	.4byte 0xffff0042
	.4byte 0x000002a8
	.4byte 0x000001ac
	.4byte 0x02080000
	.4byte 0x03fc014a
	.4byte 0x000001e0
	.4byte 0xffff0043
	.4byte 0x00000384
	.4byte 0x8000024c
	.4byte 0x02080000
	.4byte 0x03fc01ea
	.4byte 0x00000280
	.4byte 0xffff0044
	.4byte 0x00000384
	.4byte 0x800001ac
	.4byte 0x02080000
	.4byte 0x03fc014a
	.4byte 0x000001e0
	.4byte 0xffff0045
	.4byte 0x000001b8
	.4byte 0x20000078
	.4byte 0x01100000
	.4byte 0x02900000
	.4byte 0x00000130
	.4byte 0xffff0046
	.4byte 0x000000b8
	.4byte 0x40000390
	.4byte 0x00000000
	.4byte 0x017002e0
	.4byte 0x00000400
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000060
	.4byte 0xc0000120
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000140
	.4byte 0xffff0002
	.4byte 0x00000110
	.4byte 0x40000058
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00000140
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000008d
	.4byte 0x0010d08a
	.4byte 0x0020408c
	.4byte 0x0000008c
	.4byte 0x0010608c
	.4byte 0x0020508c
	.4byte 0x0030108c
	.4byte 0x0040208d
	.4byte 0x0050208c
	.4byte 0x0060108c
	.4byte 0x0070708c
	.4byte 0x0454508c
	.4byte 0x0464608c
	.4byte 0x0404108c
	.4byte 0x0410308f
	.4byte 0x0424208c
	.4byte 0x0434308c
	.4byte 0x00c0208f
	.4byte 0x01503090
	.4byte 0x01602090
	.4byte 0x01f03091
	.4byte 0x02002091
	.4byte 0x0000008e
	.4byte 0x0010c087
	.4byte 0x0024008c
	.4byte 0x000001ff
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0xffff00cb
	.4byte 0x00000001
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
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x01004000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00023000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00022000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00012000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x01dc0000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00012000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00012000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01c60000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00022000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00002000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01bc0000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x01002000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x01002000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01012000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01002000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x01002000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01002000
	.4byte 0xffff0077
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01002000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01ac0000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00002000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x01002000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x01002000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01002000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01002000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x00fa0000
	.4byte 0x00000000
	.4byte 0x034e0000
	.4byte 0x00024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x012a0000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x00fa0000
	.4byte 0x00000000
	.4byte 0x03ce0000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x00790000
	.4byte 0x00000000
	.4byte 0x03ce0000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x00790000
	.4byte 0x00000000
	.4byte 0x034e0000
	.4byte 0x01024000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x03840000
	.4byte 0x00023000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00720000
	.4byte 0x00000000
	.4byte 0x03740000
	.4byte 0x00023000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00023000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x03600000
	.4byte 0x00023000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x035c0000
	.4byte 0x00023000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00023000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00023000
	.4byte 0xffff00fd
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x03860000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x00000002
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00004000
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
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00025000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x030e0000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00023000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x03400000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00025000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00025000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x0001d000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x030a0000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00013000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00015000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00003000
	.4byte 0xffff003e
	.4byte 0x00000002
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00023000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0003b000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00025000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x00860000
	.4byte 0x00020000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00025000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00025000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03f80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0001d000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x0000b000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x010a0000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x0001d000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001b000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x004a0000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x0000d000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00ee0000
	.4byte 0x0000d000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x0002d000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00b40000
	.4byte 0x00005000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b40000
	.4byte 0x00013000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00300000
	.4byte 0x0002d000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0102b000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x0002d000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x0102b000
	.4byte 0xffff009f
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x0002d000
	.4byte 0xffff009f
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00400000
	.4byte 0x0102b000
	.4byte 0xffff00a9
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x0002d000
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
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00b60000
	.4byte 0x00018000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01260000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001b000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x00e40000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00015000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x0001b000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x004a0000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x0000d000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x01b60000
	.4byte 0x00000000
	.4byte 0x00ba0000
	.4byte 0x00018000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00ba0000
	.4byte 0x00020000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00b40000
	.4byte 0x00005000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b40000
	.4byte 0x00013000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01020000
	.4byte 0x00000000
	.4byte 0x00a10000
	.4byte 0x00020000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00f40000
	.4byte 0x00000000
	.4byte 0x00960000
	.4byte 0x00020000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a10000
	.4byte 0x00020000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00020000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x00340000
	.4byte 0x0002d000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x00460000
	.4byte 0x0102b000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0002d000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x0102b000
	.4byte 0xffff009f
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x0002d000
	.4byte 0xffff009f
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x008e0000
	.4byte 0x0102b000
	.4byte 0xffff00a9
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x0002b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00ee0000
	.4byte 0x00005000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x011e0000
	.4byte 0x00000000
	.4byte 0x00a40000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002069
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000206a
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000206b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0000206c
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020091c5
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002070
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002071
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002072
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002073
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002074
	.4byte 0x00000003
	.4byte 0xffff0032
	.4byte 0x004018ad
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
	.4byte 0x09620008
	.4byte 0x00002055
	.4byte 0x00000000
	.4byte 0x09500008
	.4byte 0x0000224f
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000023c1
	.4byte 0x00000000
	.4byte 0x09620009
	.4byte 0x00002056
	.4byte 0x00000000
	.4byte 0x09500009
	.4byte 0x00002250
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000023c8
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020082a1
	.4byte 0x00000000
	.4byte 0x0962000b
	.4byte 0x0000205a
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002252
	.4byte 0x00000000
	.4byte 0x0962000c
	.4byte 0x0000205b
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002253
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x020082e5
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008335
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200839d
	.4byte 0x00008d15
	.4byte 0x09620008
	.4byte 0x00002061
	.4byte 0x00008d15
	.4byte 0x09500008
	.4byte 0x0000225f
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000023c9
	.4byte 0x00008d15
	.4byte 0x09620009
	.4byte 0x00002062
	.4byte 0x00008d15
	.4byte 0x09500009
	.4byte 0x00002260
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000023cb
	.4byte 0x00008d15
	.4byte 0x0962000a
	.4byte 0x00002063
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002261
	.4byte 0x00008d15
	.4byte 0x0962000b
	.4byte 0x00002064
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002262
	.4byte 0x00008d15
	.4byte 0x0962000c
	.4byte 0x00002065
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002263
	.4byte 0x00008d15
	.4byte 0x0962000d
	.4byte 0x00002066
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002264
	.4byte 0x00008d15
	.4byte 0x0962000e
	.4byte 0x00002067
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002265
	.4byte 0x00008d15
	.4byte 0x09620010
	.4byte 0x00002068
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002266
	.4byte 0x00000002
	.4byte 0x0950000a
	.4byte 0x02008711
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002113
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008469
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020084c9
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0200850d
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200855d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020085f1
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x02008649
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020086bd
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002128
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002129
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000212a
	.4byte 0x00000002
	.4byte 0x0235000c
	.4byte 0x02008dcd
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte 0x020091ad
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
