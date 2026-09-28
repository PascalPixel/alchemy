.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, [r0, #80]
	ldrb r3, [r2, #9]
	movs r1, #12
	orrs r3, r1
	strb r3, [r2, #9]
	ldr r2, [r0, #80]
	ldrb r3, [r2, #21]
	orrs r3, r1
	strb r3, [r2, #21]
	movs r0, #0
	bx lr
	.2byte 0x0000
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #10
	bl 0x02009508
	adds r2, r0, #0
	ldr r3, [r5, #16]
	ldr r0, [r2, #16]
	ldr r1, [r2, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl 0x020094a0
	strh r0, [r5, #6]
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_0200007c_0
	ldr r0, [pc, #16]
	b .L_0200007c_1
.L_0200007c_0:
	ldr r0, [pc, #16]
.L_0200007c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004a
	.4byte 0x02009844
	.4byte 0x020097b4
	.global Func_020000ac
	.thumb_func
Func_020000ac:
	movs r0, #0
	bx lr
	.global Func_020000b0
	.thumb_func
Func_020000b0:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020098ec
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_020000b8_0
	ldr r0, [pc, #16]
	b .L_020000b8_1
.L_020000b8_0:
	ldr r0, [pc, #16]
.L_020000b8_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004a
	.4byte 0x02009a38
	.4byte 0x02009918
	.global Func_020000e8
	.thumb_func
Func_020000e8:
	push {lr}
	bl 0x020094f0
	ldr r0, [pc, #72]
	bl 0x02009580
	movs r1, #0
	movs r0, #10
	bl 0x02009588
	movs r0, #0
	movs r1, #0
	bl 0x02009500
	cmp r0, #1
	bne .L_020000e8_0
	movs r0, #20
	bl 0x020094e8
	movs r0, #10
	movs r1, #0
	bl 0x02009590
	b .L_020000e8_1
.L_020000e8_0:
	ldr r3, [pc, #32]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #10
	movs r1, #0
	bl 0x020095a0
.L_020000e8_1:
	bl 0x020094f8
	pop {r0}
	bx r0
	.4byte 0x00001958
	.4byte 0x03001ebc
	.global Func_02000140
	.thumb_func
Func_02000140:
	push {lr}
	bl 0x020094f0
	ldr r0, [pc, #20]
	bl 0x02009580
	movs r1, #0
	movs r0, #11
	bl 0x020095a0
	bl 0x020094f8
	pop {r0}
	bx r0
	.4byte 0x0000195d
	.global Func_02000160
	.thumb_func
Func_02000160:
	push {lr}
	bl 0x020094f0
	ldr r0, [pc, #20]
	bl 0x02009580
	movs r1, #0
	movs r0, #13
	bl 0x020095a0
	bl 0x020094f8
	pop {r0}
	bx r0
	.4byte 0x00001961
	.global Func_02000180
	.thumb_func
Func_02000180:
	push {lr}
	movs r0, #188
	bl 0x02009618
	movs r1, #67
	movs r2, #6
	ldr r0, [pc, #76]
	bl 0x020094a8
	movs r0, #0
	bl 0x02009508
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r1, [pc, #64]
	movs r0, #0
	ldr r2, [pc, #64]
	bl 0x02009510
	ldr r3, [pc, #60]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	movs r0, #0
	movs r1, #2
	bl 0x02009558
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #0
	bl 0x02009540
	movs r0, #16
	bl 0x020094e8
	movs r0, #2
	bl 0x020095e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02009788
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x03001ebc
	.global Func_020001ec
	.thumb_func
Func_020001ec:
	push {r5, lr}
	bl 0x020094f0
	movs r1, #136
	movs r2, #168
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x02009550
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl 0x020095a8
	movs r1, #144
	movs r2, #200
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x02009550
	movs r1, #160
	movs r2, #192
	movs r0, #1
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x02009550
	movs r1, #128
	movs r2, #200
	movs r0, #2
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x02009550
	movs r1, #224
	movs r2, #192
	movs r0, #3
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl 0x02009550
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #3
	movs r2, #0
	bl 0x020095a8
	ldr r5, [pc, #380]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #65
	str r2, [r3]
	bl 0x020095e8
	bl 0x020095f0
	movs r0, #60
	bl 0x020094e8
	movs r1, #3
	movs r0, #8
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	ldr r0, [pc, #344]
	bl 0x02009580
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r0, #0
	movs r1, #3
	bl 0x02009558
	movs r0, #1
	movs r1, #3
	bl 0x02009558
	movs r0, #2
	movs r1, #3
	bl 0x02009558
	movs r1, #3
	movs r0, #3
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r1, #4
	movs r0, #8
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r1, #4
	movs r0, #3
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x02009598
	movs r1, #2
	movs r0, #2
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x02009598
	movs r0, #8
	movs r1, #2
	bl 0x02009578
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #20
	bl 0x020095a8
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #2
	bl 0x020095b8
	movs r0, #120
	bl 0x020094e8
	movs r1, #2
	movs r0, #1
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #20
	bl 0x020095a8
	movs r1, #0
	movs r0, #1
	bl 0x02009588
	movs r0, #0
	movs r1, #0
	bl 0x02009500
	cmp r0, #0
	bne .L_020001ec_0
	movs r0, #20
	bl 0x020094e8
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x020095a8
	movs r0, #3
	ldr r1, [pc, #108]
	movs r2, #60
	bl 0x020095b8
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r1, #224
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl 0x020095a8
	movs r0, #1
	movs r1, #2
	bl 0x02009570
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x020095c0
	movs r0, #60
	bl 0x020094e8
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x020095a8
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl 0x02009598
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_020001ec_1
	.4byte 0x03001ebc
	.4byte 0x000019e9
	.4byte 0x00000101
.L_020001ec_0:
	ldr r2, [r5]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x020095a8
	movs r1, #3
	movs r0, #3
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x020095b8
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x02009598
.L_020001ec_1:
	movs r1, #2
	movs r0, #8
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #3
	bl 0x020095a8
	movs r0, #20
	bl 0x020094e8
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #20
	bl 0x020095a8
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r1, #2
	movs r0, #1
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r1, #3
	movs r0, #1
	bl 0x02009560
	movs r0, #30
	bl 0x020094e8
	movs r1, #3
	movs r0, #8
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r1, #2
	movs r0, #8
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r0, #0
	movs r1, #3
	bl 0x02009558
	movs r0, #1
	movs r1, #3
	bl 0x02009558
	movs r0, #2
	movs r1, #3
	bl 0x02009558
	movs r1, #3
	movs r0, #3
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r1, #3
	movs r0, #8
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #30
	movs r0, #8
	bl 0x020095a8
	movs r0, #188
	bl 0x02009618
	ldr r0, [pc, #744]
	movs r1, #67
	movs r2, #6
	bl 0x020094a8
	movs r0, #8
	ldr r1, [pc, #736]
	ldr r2, [pc, #740]
	bl 0x02009510
	movs r0, #8
	movs r1, #136
	movs r2, #136
	bl 0x02009538
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl 0x02009550
	movs r0, #188
	bl 0x02009618
	movs r2, #6
	movs r1, #67
	ldr r0, [pc, #708]
	bl 0x020094a8
	movs r0, #60
	bl 0x020094e8
	bl 0x02009608
	movs r1, #2
	movs r0, #1
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #20
	bl 0x020095a8
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #128
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x020095a8
	movs r1, #2
	movs r0, #2
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #6
	movs r2, #20
	bl 0x020095a8
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl 0x02009598
	movs r0, #1
	movs r1, #1
	bl 0x02009578
	movs r1, #224
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl 0x020095a8
	movs r1, #0
	movs r0, #1
	bl 0x020095a0
	movs r0, #20
	bl 0x020094e8
	movs r1, #2
	movs r0, #2
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x020095a8
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x02009598
	movs r0, #0
	movs r1, #1
	bl 0x02009570
	movs r0, #1
	movs r1, #1
	bl 0x02009570
	movs r1, #1
	movs r0, #3
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r1, #4
	movs r0, #2
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x02009598
	movs r1, #2
	movs r0, #3
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #176
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x020095a8
	movs r1, #3
	movs r0, #2
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r1, #3
	movs r0, #2
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x020095b8
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #224
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x020095a8
	movs r1, #4
	movs r0, #2
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r0, #2
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x020095b8
	movs r1, #129
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #60
	bl 0x020095b8
	movs r0, #3
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #160
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl 0x020095a8
	movs r1, #3
	movs r0, #2
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x02009598
	movs r0, #0
	movs r1, #1
	bl 0x02009570
	movs r0, #1
	movs r1, #1
	bl 0x02009570
	movs r1, #1
	movs r0, #3
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02009510
	movs r0, #2
	movs r1, #128
	movs r2, #184
	bl 0x02009538
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #20
	bl 0x020095a8
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x020095a8
	movs r1, #224
	movs r2, #20
	movs r0, #3
	lsls r1, r1, #8
	bl 0x020095a8
	movs r1, #3
	movs r0, #2
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #2
	movs r1, #0
	bl 0x02009598
	movs r0, #0
	movs r1, #3
	bl 0x02009558
	movs r0, #1
	movs r1, #3
	bl 0x02009558
	movs r1, #3
	movs r0, #3
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02009510
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02009510
	movs r0, #1
	movs r1, #144
	movs r2, #200
	bl 0x02009530
	movs r0, #2
	movs r1, #144
	movs r2, #200
	bl 0x02009530
	movs r1, #144
	movs r2, #200
	movs r0, #3
	bl 0x02009530
	movs r0, #1
	bl 0x02009548
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x02009550
	movs r0, #2
	bl 0x02009548
	movs r1, #0
	movs r2, #0
	movs r0, #2
	bl 0x02009550
	movs r0, #3
	bl 0x02009548
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x02009550
	bl 0x020094f8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02009788
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0200979e
	.global Func_0200084c
	.thumb_func
Func_0200084c:
	push {lr}
	bl 0x020094f0
	movs r0, #0
	movs r1, #1
	bl 0x02009558
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x020094c8
	bl 0x020094f8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001956
	.global Func_02000870
	.thumb_func
Func_02000870:
	push {lr}
	sub sp, #8
	movs r3, #21
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #85
	movs r1, #9
	movs r2, #1
	bl 0x020094b8
	movs r0, #100
	movs r1, #0
	movs r2, #0
	bl 0x020095f8
	movs r1, #172
	movs r2, #152
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x02009550
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020008a8
	.thumb_func
Func_020008a8:
	push {lr}
	sub sp, #8
	movs r3, #21
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #21
	movs r1, #73
	movs r2, #1
	bl 0x020094b8
	movs r1, #1
	movs r2, #1
	movs r0, #100
	negs r1, r1
	negs r2, r2
	bl 0x020095f8
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x02009550
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020008e0
	.thumb_func
Func_020008e0:
	push {r5, lr}
	movs r0, #0
	bl 0x02009508
	ldrh r5, [r0, #6]
	bl 0x020094f0
	ldr r3, [pc, #40]
	adds r5, r5, r3
	ldr r3, [pc, #40]
	cmp r5, r3
	bhi .L_020008e0_0
	movs r0, #15
	bl 0x02009610
	b .L_020008e0_1
.L_020008e0_0:
	ldr r0, [pc, #28]
	bl 0x02009580
	movs r0, #15
	movs r1, #0
	bl 0x02009590
.L_020008e0_1:
	bl 0x020094f8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00001a1e
	.global Func_02000924
	.thumb_func
Func_02000924:
	push {r5, lr}
	ldr r0, [pc, #380]
	bl 0x020094d0
	cmp r0, #0
	bne .L_02000924_0
	b .L_02000924_1
.L_02000924_0:
	bl 0x020094f0
	movs r1, #134
	movs r2, #216
	lsls r1, r1, #18
	lsls r2, r2, #16
	movs r0, #10
	bl 0x02009550
	ldr r0, [pc, #352]
	bl 0x02009580
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x02009598
	movs r1, #2
	movs r0, #0
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r0, #0
	bl 0x02009508
	ldr r3, [pc, #320]
	str r3, [r0, #108]
	movs r0, #0
	bl 0x02009508
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #13
	bne .L_02000924_2
	movs r1, #220
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #200
	bl 0x02009538
.L_02000924_2:
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #10
	lsls r1, r1, #10
	bl 0x02009510
	movs r0, #10
	movs r1, #2
	bl 0x020095b0
	movs r1, #204
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #10
	bl 0x02009538
	movs r0, #10
	bl 0x02009508
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl 0x020094e8
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x020095a8
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x02009598
	movs r0, #10
	movs r1, #2
	bl 0x02009570
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl 0x020095c0
	movs r0, #60
	bl 0x020094e8
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x02009598
	ldr r1, [pc, #184]
	movs r0, #10
	bl 0x02009518
	movs r0, #148
	movs r1, #1
	movs r2, #172
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x020095c8
	movs r0, #139
	lsls r0, r0, #4
	bl 0x020094d8
	movs r0, #10
	bl 0x02009520
	bl 0x020095d0
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #0
	lsls r1, r1, #9
	bl 0x02009510
	ldr r1, [pc, #128]
	movs r0, #0
	bl 0x02009518
	movs r0, #0
	bl 0x02009520
	movs r0, #10
	bl 0x020094e8
	movs r0, #0
	bl 0x02009508
	movs r5, #0
	str r5, [r0, #108]
	movs r0, #30
	bl 0x020094e8
	movs r1, #2
	movs r0, #10
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #120
	bl 0x020095a8
	movs r0, #10
	ldr r1, [pc, #68]
	movs r2, #60
	bl 0x020095b8
	movs r2, #60
	movs r0, #0
	ldr r1, [pc, #60]
	bl 0x020095b8
	movs r1, #4
	movs r0, #10
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	bl 0x020094f8
.L_02000924_1:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0000089a
	.4byte 0x000018b5
	.4byte 0x02008055
	.4byte 0x0200962c
	.4byte 0x020096b8
	.4byte 0x00000105
	.4byte 0x00000101
	.global Func_02000ac0
	.thumb_func
Func_02000ac0:
	push {lr}
	bl 0x020094f0
	ldr r0, [pc, #88]
	bl 0x02009580
	movs r0, #10
	ldr r1, [pc, #84]
	movs r2, #60
	bl 0x020095b8
	movs r1, #0
	movs r0, #10
	bl 0x02009588
	movs r0, #0
	movs r1, #0
	bl 0x02009500
	cmp r0, #1
	bne .L_02000ac0_0
	ldr r3, [pc, #60]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000ac0_0:
	movs r0, #20
	bl 0x020094e8
	movs r1, #4
	movs r0, #10
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r0, #10
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	bl 0x020094f8
	pop {r0}
	bx r0
	.4byte 0x000018b9
	.4byte 0x00000105
	.4byte 0x03001ebc
	.global Func_02000b2c
	.thumb_func
Func_02000b2c:
	push {lr}
	bl 0x020094f0
	bl 0x020095e8
	bl 0x020095f0
	movs r0, #30
	bl 0x020094e8
	movs r1, #192
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #7
	bl 0x020095a8
	movs r0, #9
	movs r1, #1
	bl 0x020095d8
	bl 0x020095d0
	movs r1, #2
	movs r0, #8
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	ldr r0, [pc, #192]
	bl 0x02009580
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r1, #1
	movs r0, #9
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x02009598
	movs r1, #3
	movs r0, #8
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x020095a8
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x02009598
	movs r1, #3
	movs r0, #8
	bl 0x02009560
	movs r0, #60
	bl 0x020094e8
	movs r1, #129
	movs r2, #60
	movs r0, #8
	lsls r1, r1, #1
	bl 0x020095b8
	movs r1, #4
	movs r0, #8
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r0, #10
	movs r1, #2
	bl 0x02009570
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl 0x020095c0
	movs r0, #60
	bl 0x020094e8
	movs r1, #176
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #8
	bl 0x020095a8
	movs r0, #9
	movs r1, #5
	bl 0x02009558
	bl 0x020094f8
	ldr r0, [pc, #12]
	bl 0x020094d8
	pop {r0}
	bx r0
	.4byte 0x000019cf
	.4byte 0x000008b1
	.global Func_02000c30
	.thumb_func
Func_02000c30:
	push {r5, lr}
	bl 0x020094f0
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl 0x020095c0
	movs r1, #2
	movs r0, #8
	bl 0x02009570
	movs r0, #60
	bl 0x020094e8
	ldr r0, [pc, #864]
	bl 0x02009580
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r1, #129
	movs r0, #10
	lsls r1, r1, #1
	bl 0x020095c0
	movs r1, #4
	movs r2, #0
	movs r0, #10
	bl 0x02009568
	movs r0, #60
	bl 0x020094e8
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x02009598
	movs r1, #1
	movs r0, #8
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl 0x02009598
	movs r1, #208
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #8
	bl 0x020095a8
	movs r1, #3
	movs r0, #10
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #138
	movs r0, #8
	movs r1, #178
	lsls r2, r2, #1
	bl 0x02009530
	movs r2, #142
	movs r1, #172
	lsls r2, r2, #1
	movs r0, #10
	bl 0x02009538
	movs r0, #8
	bl 0x02009548
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl 0x020095a8
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl 0x020095a8
	movs r0, #20
	bl 0x020094e8
	movs r1, #2
	movs r0, #8
	bl 0x02009578
	movs r0, #20
	bl 0x020094e8
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl 0x02009598
	movs r0, #8
	bl 0x02009508
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #10
	bl 0x02009508
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	ldr r1, [pc, #648]
	movs r0, #8
	ldr r2, [pc, #648]
	bl 0x02009510
	ldr r2, [pc, #644]
	movs r0, #10
	ldr r1, [pc, #636]
	bl 0x02009510
	movs r0, #8
	movs r1, #5
	bl 0x02009558
	movs r1, #6
	movs r0, #10
	bl 0x02009558
	movs r0, #20
	bl 0x020094e8
	movs r0, #125
	bl 0x02009618
	movs r0, #8
	movs r1, #2
	movs r2, #0
	bl 0x02009540
	movs r0, #9
	movs r1, #2
	movs r2, #0
	bl 0x02009540
	movs r2, #0
	movs r1, #2
	movs r0, #10
	bl 0x02009540
	movs r0, #10
	bl 0x02009548
	movs r0, #30
	bl 0x020094e8
	movs r0, #8
	movs r1, #5
	bl 0x02009558
	movs r1, #6
	movs r0, #10
	bl 0x02009558
	movs r0, #20
	bl 0x020094e8
	movs r0, #125
	bl 0x02009618
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl 0x02009540
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl 0x02009540
	movs r2, #0
	movs r1, #4
	movs r0, #10
	bl 0x02009540
	movs r0, #10
	bl 0x02009548
	movs r0, #9
	bl 0x02009528
	movs r0, #8
	movs r1, #1
	bl 0x02009558
	movs r1, #1
	movs r0, #10
	bl 0x02009558
	movs r0, #50
	bl 0x020094e8
	movs r1, #2
	movs r2, #0
	movs r0, #10
	bl 0x02009568
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x02009598
	movs r1, #3
	movs r0, #8
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r1, #0
	movs r0, #8
	bl 0x020095a0
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r0, #8
	movs r1, #5
	bl 0x02009558
	movs r1, #6
	movs r0, #10
	bl 0x02009558
	movs r0, #20
	bl 0x020094e8
	movs r0, #125
	bl 0x02009618
	movs r0, #8
	movs r1, #2
	movs r2, #0
	bl 0x02009540
	movs r0, #9
	movs r1, #2
	movs r2, #0
	bl 0x02009540
	movs r2, #0
	movs r1, #2
	movs r0, #10
	bl 0x02009540
	movs r0, #10
	bl 0x02009548
	movs r0, #30
	bl 0x020094e8
	movs r0, #8
	movs r1, #5
	bl 0x02009558
	movs r1, #6
	movs r0, #10
	bl 0x02009558
	movs r0, #20
	bl 0x020094e8
	movs r0, #125
	bl 0x02009618
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl 0x02009540
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl 0x02009540
	movs r2, #0
	movs r1, #4
	movs r0, #10
	bl 0x02009540
	movs r0, #10
	bl 0x02009548
	movs r0, #40
	bl 0x020094e8
	movs r0, #8
	movs r1, #1
	bl 0x02009558
	movs r0, #10
	movs r1, #1
	bl 0x02009558
	movs r1, #2
	movs r2, #0
	movs r0, #10
	bl 0x02009568
	movs r0, #20
	bl 0x020094e8
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #20
	bl 0x020095a8
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x02009598
	movs r1, #3
	movs r0, #8
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r0, #8
	movs r1, #0
	movs r2, #30
	bl 0x02009598
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl 0x02009598
	movs r0, #10
	bl 0x02009508
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r1, [pc, #172]
	movs r0, #10
	ldr r2, [pc, #172]
	bl 0x02009510
	movs r2, #148
	movs r0, #10
	movs r1, #168
	lsls r2, r2, #1
	bl 0x02009538
	movs r1, #208
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #8
	bl 0x020095a8
	movs r0, #10
	movs r1, #5
	bl 0x02009558
	movs r1, #3
	movs r0, #8
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r1, #3
	movs r0, #10
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #10
	movs r1, #0
	bl 0x02009598
	movs r1, #3
	movs r0, #8
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02009598
	movs r1, #3
	movs r0, #0
	bl 0x02009560
	movs r0, #20
	bl 0x020094e8
	bl 0x020094f8
	ldr r0, [pc, #48]
	bl 0x020094d8
	ldr r3, [pc, #48]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #65
	str r2, [r3]
	movs r0, #6
	bl 0x020095e0
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x000019da
	.4byte 0x00003333
	.4byte 0x00001999
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x000008b2
	.4byte 0x03001ebc
	.global Func_02000fcc
	.thumb_func
Func_02000fcc:
	push {lr}
	bl 0x020094f0
	movs r0, #0
	movs r1, #1
	bl 0x02009558
	ldr r0, [pc, #16]
	movs r1, #1
	bl 0x020094c8
	bl 0x020094f8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001a12
	.global Func_02000ff0
	.thumb_func
Func_02000ff0:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #5
	movs r1, #28
	movs r2, #5
	movs r3, #13
	bl 0x020094b0
	movs r3, #5
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #28
	movs r2, #1
	movs r3, #2
	movs r0, #5
	bl 0x020094b8
	movs r0, #1
	bl 0x020094e8
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_02001028
	.thumb_func
Func_02001028:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #28
	movs r2, #5
	movs r3, #13
	bl 0x020094b0
	movs r3, #5
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #28
	movs r2, #1
	movs r3, #2
	movs r0, #6
	bl 0x020094b8
	movs r0, #1
	bl 0x020094e8
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_02001060
	.thumb_func
Func_02001060:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x02009508
	ldr r3, [r5, #12]
	ldr r2, [r0, #12]
	cmp r2, r3
	ble .L_02001060_0
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	b .L_02001060_1
.L_02001060_0:
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #253
	ands r3, r2
.L_02001060_1:
	strb r3, [r1]
	pop {r5}
	pop {r1}
	bx r1
	.global Func_02001090
	.thumb_func
Func_02001090:
	push {r5, lr}
	movs r0, #0
	bl 0x02009508
	adds r5, r0, #0
	movs r0, #14
	bl 0x02009508
	ldr r2, [r5, #16]
	ldr r3, [r0, #16]
	cmp r2, r3
	bgt .L_02001090_0
	movs r0, #14
	movs r1, #1
	bl 0x020095b0
.L_02001090_0:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020010b8
	.thumb_func
Func_020010b8:
	push {r5, r6, lr}
	sub sp, #8
	bl 0x020094f0
	movs r0, #14
	bl 0x02009508
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #253
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #14
	bl 0x02009508
	adds r0, #89
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r0, #14
	bl 0x02009508
	movs r6, #0
	adds r0, #85
	strb r6, [r0]
	movs r0, #14
	bl 0x02009508
	ldr r3, [pc, #76]
	str r3, [r0, #108]
	movs r3, #56
	str r3, [sp, #0]
	movs r5, #18
	movs r0, #55
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x020094b8
	movs r3, #20
	str r3, [sp, #0]
	movs r2, #1
	movs r3, #1
	movs r1, #16
	movs r0, #55
	str r5, [sp, #4]
	bl 0x020094b8
	movs r0, #1
	bl 0x02009498
	movs r0, #128
	lsls r0, r0, #2
	bl 0x020094d8
	movs r0, #14
	movs r1, #2
	bl 0x020095b0
	bl 0x020094f8
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02009061
	.global Func_02001144
	.thumb_func
Func_02001144:
	push {lr}
	sub sp, #8
	bl 0x020094f0
	movs r3, #21
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #6
	movs r2, #1
	movs r3, #2
	movs r0, #14
	bl 0x020094b8
	movs r0, #15
	bl 0x02009508
	movs r3, #254
	adds r0, #89
	strb r3, [r0]
	ldr r0, [pc, #12]
	bl 0x020094d8
	bl 0x020094f8
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000201
	.global Func_02001180
	.thumb_func
Func_02001180:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02001180_0
	ldr r0, [pc, #16]
	b .L_02001180_1
.L_02001180_0:
	ldr r0, [pc, #16]
.L_02001180_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000004a
	.4byte 0x02009c9c
	.4byte 0x02009b10
	.global Func_020011b0
	.thumb_func
Func_020011b0:
	push {r5, r6, lr}
	ldr r3, [pc, #684]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #128
	lsls r3, r3, #1
	lsls r2, r2, #1
	str r3, [r1, r2]
	ldr r5, [pc, #672]
	ldrsh r2, [r5, r2]
	ldr r3, [pc, #672]
	sub sp, #8
	cmp r2, r3
	bne .L_020011b0_0
	movs r0, #169
	bl 0x02009600
	movs r0, #11
	movs r1, #5
	bl 0x02009558
	movs r0, #12
	movs r1, #5
	bl 0x02009558
	movs r0, #14
	movs r1, #2
	bl 0x02009558
	movs r3, #21
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	movs r1, #9
	movs r2, #1
	movs r3, #1
	bl 0x020094b8
	bl 0x02008870
	ldr r0, [pc, #616]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_1
	movs r1, #136
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009550
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x020095a8
.L_020011b0_1:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_020011b0_2
	ldr r0, [pc, #572]
	bl 0x020094e0
	b .L_020011b0_3
.L_020011b0_2:
	cmp r3, #3
	beq .L_020011b0_4
	b .L_020011b0_3
.L_020011b0_4:
	ldr r0, [pc, #560]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_5
	b .L_020011b0_3
.L_020011b0_5:
	bl 0x020081ec
	b .L_020011b0_3
.L_020011b0_0:
	ldr r3, [pc, #548]
	cmp r2, r3
	beq .L_020011b0_7
	b .L_020011b0_3
.L_020011b0_7:
	movs r0, #14
	bl 0x02009508
	movs r1, #0
	bl 0x020094c0
	movs r0, #14
	bl 0x02009508
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #2
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_8
	movs r0, #14
	movs r1, #5
	bl 0x02009558
	bl 0x020090b8
.L_020011b0_8:
	ldr r0, [pc, #492]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_9
	movs r0, #15
	movs r1, #4
	bl 0x02009558
	bl 0x02009144
.L_020011b0_9:
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #4
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_020011b0_10
	ldr r0, [pc, #436]
	bl 0x020094e0
.L_020011b0_10:
	ldr r0, [pc, #448]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_11
	ldr r0, [pc, #440]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_11
	ldr r0, [pc, #408]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_11
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009550
.L_020011b0_11:
	ldr r0, [pc, #388]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_12
	ldr r0, [pc, #400]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_12
	ldr r3, [pc, #360]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_020011b0_12
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl 0x02009550
	ldr r0, [pc, #340]
	bl 0x020094d8
	ldr r0, [pc, #364]
	bl 0x020094d8
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x02009550
.L_020011b0_12:
	ldr r0, [pc, #320]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_13
	movs r3, #2
	str r3, [sp, #4]
	movs r5, #1
	movs r0, #54
	movs r1, #21
	movs r2, #53
	movs r3, #21
	str r5, [sp, #0]
	bl 0x020094b0
	movs r3, #21
	str r3, [sp, #4]
	movs r6, #17
	movs r0, #18
	movs r1, #20
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	bl 0x020094b8
	movs r0, #44
	movs r1, #18
	movs r2, #43
	movs r3, #17
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x020094b0
	movs r3, #7
	str r3, [sp, #0]
	movs r0, #8
	movs r1, #17
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl 0x020094b8
.L_020011b0_13:
	ldr r0, [pc, #260]
	bl 0x020094d0
	cmp r0, #0
	beq .L_020011b0_14
	ldr r0, [pc, #224]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_14
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009550
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009550
	movs r1, #192
	movs r2, #132
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009550
	movs r1, #164
	movs r2, #140
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009550
	movs r1, #184
	movs r2, #152
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009550
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl 0x020095a8
	movs r1, #176
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #8
	bl 0x020095a8
	ldr r1, [pc, #160]
	movs r0, #9
	bl 0x02009518
	movs r0, #9
	bl 0x02009508
	ldr r3, [pc, #152]
	str r3, [r0, #24]
.L_020011b0_14:
	ldr r0, [pc, #112]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_15
	movs r1, #164
	movs r2, #140
	lsls r2, r2, #17
	movs r0, #9
	lsls r1, r1, #16
	bl 0x02009550
	ldr r1, [pc, #120]
	movs r0, #9
	bl 0x02009518
	movs r0, #9
	bl 0x02009508
	ldr r3, [pc, #108]
	str r3, [r0, #24]
.L_020011b0_15:
	ldr r3, [pc, #60]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_020011b0_3
	ldr r0, [pc, #92]
	bl 0x020094d0
.L_020011b0_6:
	cmp r0, #0
	bne .L_020011b0_3
	ldr r0, [pc, #52]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_3
	ldr r0, [pc, #32]
	bl 0x020094d0
	cmp r0, #0
	bne .L_020011b0_3
	bl 0x02008b2c
.L_020011b0_3:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000058
	.4byte 0x000008b2
	.4byte 0x0000012f
	.4byte 0x00000109
	.4byte 0x0000004a
	.4byte 0x00000201
	.4byte 0x0000089a
	.4byte 0x00000895
	.4byte 0x000008b3
	.4byte 0x02009730
	.4byte 0xffff0000
	.4byte 0x000008b1
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/YAMA_RAMA/IMPORT.INC"
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x000000f0
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000012c
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x002e0042
	.4byte 0x00020003
	.4byte 0x003f0005
	.4byte 0x0003002e
	.4byte 0x00050002
	.4byte 0x0042ffff
	.4byte 0x0003002e
	.4byte 0x00050002
	.4byte 0x002e0045
	.4byte 0x00020003
	.4byte 0xffff0005
	.4byte 0xffff0000
	.4byte 0x000001f0
	.4byte 0x800000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0xc0000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000088
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000090
	.4byte 0xc00000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000000e8
	.4byte 0x40000160
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
	.4byte 0x000001f0
	.4byte 0x800000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001f0
	.4byte 0x800000d8
	.4byte 0x00100000
	.4byte 0x02080020
	.4byte 0x000001a0
	.4byte 0xffff0002
	.4byte 0x00000180
	.4byte 0x40000048
	.4byte 0x00100000
	.4byte 0x02080020
	.4byte 0x000001a0
	.4byte 0xffff0003
	.4byte 0x00000028
	.4byte 0x00000118
	.4byte 0x00100000
	.4byte 0x02080020
	.4byte 0x000001a0
	.4byte 0xffff0004
	.4byte 0x00000058
	.4byte 0x400000f8
	.4byte 0x00100000
	.4byte 0x02080020
	.4byte 0x000001a0
	.4byte 0xffff0005
	.4byte 0x000000d8
	.4byte 0x400000f8
	.4byte 0x00100000
	.4byte 0x02080020
	.4byte 0x000001a0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000058
	.4byte 0x00111002
	.4byte 0x0020303d
	.4byte 0x0000004a
	.4byte 0x0010f002
	.4byte 0x00232002
	.4byte 0x00333002
	.4byte 0x00406050
	.4byte 0x00507050
	.4byte 0x00603058
	.4byte 0x000001ff
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
	.4byte 0xffff003d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0001c000
	.4byte 0xffff009e
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0xffff009e
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00004000
	.4byte 0xffff009e
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00024000
	.4byte 0xffff009e
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00024000
	.4byte 0xffff009e
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00014000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff003d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0001c000
	.4byte 0xffff0029
	.4byte 0x02009730
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x08b20027
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00018000
	.4byte 0x08b20127
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01140000
	.4byte 0x00008000
	.4byte 0x18950082
	.4byte 0x02009620
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0x18950083
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008181
	.4byte 0x00000000
	.4byte 0x08b20009
	.4byte 0x00001957
	.4byte 0x00000000
	.4byte 0x08b2000a
	.4byte 0x020080e9
	.4byte 0x00000000
	.4byte 0x08b2000b
	.4byte 0x02008141
	.4byte 0x00000000
	.4byte 0x08b2000c
	.4byte 0x00001960
	.4byte 0x00000000
	.4byte 0x08b2000d
	.4byte 0x02008161
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a04
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a05
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001a06
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a07
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001a08
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020088e1
	.4byte 0x00008d15
	.4byte 0x08b20009
	.4byte 0x00001964
	.4byte 0x00008d15
	.4byte 0x08b2000a
	.4byte 0x00001965
	.4byte 0x00008d15
	.4byte 0x08b2000b
	.4byte 0x00001966
	.4byte 0x00008d15
	.4byte 0x08b2000c
	.4byte 0x00001967
	.4byte 0x00008d15
	.4byte 0x08b2000d
	.4byte 0x00001968
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a0b
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a0c
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a0d
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a0e
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a0f
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001a1f
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x020088a9
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02008871
	.4byte 0x00000003
	.4byte 0xffff0005
	.4byte 0x0200884d
	.4byte 0x0000c403
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x0000e403
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x0000a403
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x00008403
	.4byte 0x0350006e
	.4byte 0x00300000
	.4byte 0x00000013
	.4byte 0x0f7a0064
	.4byte 0x001000bd
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
	.4byte 0xffff0005
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0x08b0000a
	.4byte 0x02008925
	.4byte 0x00000000
	.4byte 0x0895000a
	.4byte 0x02008ac1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000019d4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000019d6
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000019d5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a20
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001a21
	.4byte 0x00008f15
	.4byte 0x08b2000b
	.4byte 0x02008c31
	.4byte 0x00008d15
	.4byte 0x0895000a
	.4byte 0x000018bc
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000019d7
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000019d9
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000019d8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a22
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a23
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008ff1
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x02009029
	.4byte 0x10001815
	.4byte 0x0200000e
	.4byte 0x02009091
	.4byte 0x00001815
	.4byte 0x0200000e
	.4byte 0x020090b9
	.4byte 0x00000c15
	.4byte 0x0201000f
	.4byte 0x02009145
	.4byte 0x00000003
	.4byte 0xffff0007
	.4byte 0x02008fcd
	.4byte 0x00000013
	.4byte 0x0f710064
	.4byte 0x001000bf
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
