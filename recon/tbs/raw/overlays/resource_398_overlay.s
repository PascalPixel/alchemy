.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	movs r1, #0
	bl 0x02008940
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
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
	.4byte 0x00000031
	.4byte 0x020089ec
	.4byte 0x00000030
	.4byte 0x02008a64
	.4byte 0x0000002f
	.4byte 0x02008b24
	.4byte 0x020089bc
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
	.4byte 0x02008bcc
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020000a0_0
	ldr r0, [pc, #36]
	b .L_020000a0_1
.L_020000a0_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020000a0_2
	ldr r0, [pc, #36]
	b .L_020000a0_1
.L_020000a0_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020000a0_3
	ldr r0, [pc, #32]
	b .L_020000a0_1
.L_020000a0_3:
	ldr r0, [pc, #32]
.L_020000a0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000031
	.4byte 0x02008c2c
	.4byte 0x00000030
	.4byte 0x02008c5c
	.4byte 0x0000002f
	.4byte 0x02008cbc
	.4byte 0x02008c14
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020000f4_0
	ldr r0, [pc, #36]
	b .L_020000f4_1
.L_020000f4_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020000f4_2
	ldr r0, [pc, #36]
	b .L_020000f4_1
.L_020000f4_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020000f4_3
	ldr r0, [pc, #32]
	b .L_020000f4_1
.L_020000f4_3:
	ldr r0, [pc, #32]
.L_020000f4_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000031
	.4byte 0x02008ea8
	.4byte 0x00000030
	.4byte 0x02008efc
	.4byte 0x0000002f
	.4byte 0x02008f80
	.4byte 0x02008e9c
	.global Func_02000148
	.thumb_func
Func_02000148:
	push {lr}
	sub sp, #8
	movs r3, #21
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x02008930
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000168
	.thumb_func
Func_02000168:
	push {lr}
	sub sp, #8
	movs r3, #21
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x02008930
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000188
	.thumb_func
Func_02000188:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #111
	movs r1, #37
	movs r2, #97
	movs r3, #21
	bl 0x02008928
	movs r3, #32
	movs r2, #24
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #38
	movs r2, #3
	movs r3, #2
	bl 0x02008930
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020001bc
	.thumb_func
Func_020001bc:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #95
	movs r1, #21
	movs r2, #97
	movs r3, #21
	bl 0x02008928
	movs r3, #32
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #38
	movs r2, #3
	movs r3, #1
	bl 0x02008930
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020001f0
	.thumb_func
Func_020001f0:
	push {lr}
	bl 0x02008968
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl 0x02008980
	ldr r0, [pc, #12]
	bl 0x02008950
	bl 0x02008970
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000882
	.global Func_02000214
	.thumb_func
Func_02000214:
	push {lr}
	sub sp, #8
	bl 0x02008968
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl 0x02008980
	ldr r0, [pc, #84]
	bl 0x02008950
	movs r0, #40
	bl 0x02008960
	movs r1, #2
	movs r0, #15
	bl 0x02008990
	movs r0, #15
	bl 0x02008978
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #15
	bl 0x02008978
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #15
	bl 0x020089a0
	movs r3, #18
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x02008930
	bl 0x02008970
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000883
	.global Func_02000280
	.thumb_func
Func_02000280:
	push {lr}
	bl 0x02008968
	movs r1, #0
	movs r0, #15
	bl 0x02008998
	movs r0, #40
	bl 0x02008960
	movs r0, #210
	bl 0x020089a8
	movs r0, #15
	movs r1, #6
	bl 0x02008990
	bl 0x02008970
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020002ac
	.thumb_func
Func_020002ac:
	push {lr}
	bl 0x02008968
	movs r1, #0
	movs r0, #16
	bl 0x02008998
	movs r0, #40
	bl 0x02008960
	movs r0, #210
	bl 0x020089a8
	movs r0, #16
	movs r1, #6
	bl 0x02008990
	bl 0x02008970
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020002d8
	.thumb_func
Func_020002d8:
	push {lr}
	bl 0x02008968
	movs r1, #0
	movs r0, #17
	bl 0x02008998
	movs r0, #40
	bl 0x02008960
	movs r0, #210
	bl 0x020089a8
	movs r0, #17
	movs r1, #6
	bl 0x02008990
	bl 0x02008970
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000304
	.thumb_func
Func_02000304:
	push {r5, r6, lr}
	movs r0, #11
	sub sp, #8
	bl 0x02008978
	adds r5, r0, #0
	movs r0, #12
	bl 0x02008978
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	adds r6, r0, #0
	cmp r3, #35
	bne .L_02000304_0
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_02000304_0
	ldr r0, [pc, #240]
	bl 0x02008950
	b .L_02000304_1
.L_02000304_0:
	ldr r0, [pc, #232]
	bl 0x02008958
.L_02000304_1:
	ldr r3, [r6, #8]
	asrs r3, r3, #20
	cmp r3, #35
	bne .L_02000304_2
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_02000304_2
	movs r0, #193
	lsls r0, r0, #2
	bl 0x02008950
	b .L_02000304_3
.L_02000304_2:
	movs r0, #193
	lsls r0, r0, #2
	bl 0x02008958
.L_02000304_3:
	ldr r0, [pc, #192]
	bl 0x02008948
	cmp r0, #0
	bne .L_02000304_4
	movs r0, #193
	lsls r0, r0, #2
	bl 0x02008948
	cmp r0, #0
	beq .L_02000304_5
.L_02000304_4:
	ldr r0, [pc, #176]
	bl 0x02008948
	cmp r0, #0
	bne .L_02000304_6
	bl 0x02008968
	movs r0, #40
	bl 0x02008960
	movs r0, #210
	bl 0x020089a8
	movs r0, #17
	movs r1, #6
	bl 0x02008990
	movs r3, #22
	str r3, [sp, #4]
	movs r5, #36
	movs r0, #0
	movs r1, #1
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02008930
	movs r3, #24
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #2
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02008930
	bl 0x02008970
.L_02000304_6:
	ldr r0, [pc, #100]
	bl 0x02008950
	b .L_02000304_7
.L_02000304_5:
	ldr r0, [pc, #92]
	bl 0x02008948
	cmp r0, #0
	beq .L_02000304_8
	bl 0x02008968
	movs r0, #40
	bl 0x02008960
	movs r0, #220
	bl 0x020089a8
	movs r0, #17
	movs r1, #2
	bl 0x02008990
	movs r3, #22
	str r3, [sp, #4]
	movs r5, #36
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02008930
	movs r3, #24
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #2
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02008930
	bl 0x02008970
.L_02000304_8:
	ldr r0, [pc, #16]
	bl 0x02008958
.L_02000304_7:
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000303
	.4byte 0x00000302
	.global Func_02000424
	.thumb_func
Func_02000424:
	push {lr}
	sub sp, #8
	movs r3, #8
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #31
	bl 0x02008930
	ldr r0, [pc, #8]
	bl 0x02008950
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000305
	.global Func_0200044c
	.thumb_func
Func_0200044c:
	ldr r3, [pc, #8]
	ldr r2, [r3]
	movs r3, #1
	strb r3, [r2, #23]
	bx lr
	.2byte 0x0000
	.4byte 0x03001e70
	.global Func_0200045c
	.thumb_func
Func_0200045c:
	ldr r3, [pc, #8]
	ldr r2, [r3]
	movs r3, #0
	strb r3, [r2, #23]
	bx lr
	.2byte 0x0000
	.4byte 0x03001e70
	.global Func_0200046c
	.thumb_func
Func_0200046c:
	push {lr}
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_0200046c_0
	bl 0x020084b4
	b .L_0200046c_1
.L_0200046c_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200046c_2
	bl 0x020084e8
	b .L_0200046c_1
.L_0200046c_2:
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_0200046c_1
	bl 0x02008538
.L_0200046c_1:
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000031
	.4byte 0x00000030
	.4byte 0x0000002f
	.global Func_020004b4
	.thumb_func
Func_020004b4:
	push {lr}
	ldr r0, [pc, #44]
	sub sp, #8
	bl 0x02008948
	cmp r0, #0
	beq .L_020004b4_0
	movs r3, #8
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #31
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x02008930
	movs r0, #8
	movs r1, #0
	bl 0x02008988
.L_020004b4_0:
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000305
	.global Func_020004e8
	.thumb_func
Func_020004e8:
	push {lr}
	ldr r3, [pc, #68]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	movs r0, #8
	movs r1, #1
	bl 0x02008988
	movs r0, #10
	movs r1, #2
	bl 0x02008988
	ldr r0, [pc, #40]
	bl 0x02008948
	cmp r0, #0
	beq .L_020004e8_0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x02008980
	b .L_020004e8_1
.L_020004e8_0:
	movs r0, #9
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
.L_020004e8_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000882
	.global Func_02000538
	.thumb_func
Func_02000538:
	push {r5, lr}
	ldr r3, [pc, #624]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #68
	str r2, [r3]
	movs r0, #18
	sub sp, #8
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
	movs r0, #19
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
	movs r0, #20
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
	movs r0, #21
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
	movs r0, #22
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
	movs r0, #23
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
	movs r0, #24
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
	movs r0, #25
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
	movs r0, #26
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
	movs r0, #18
	movs r1, #5
	bl 0x02008988
	movs r0, #19
	movs r1, #5
	bl 0x02008988
	movs r0, #20
	movs r1, #5
	bl 0x02008988
	movs r0, #21
	movs r1, #5
	bl 0x02008988
	movs r0, #22
	movs r1, #5
	bl 0x02008988
	movs r0, #23
	movs r1, #3
	bl 0x02008988
	movs r0, #24
	movs r1, #3
	bl 0x02008988
	movs r0, #25
	movs r1, #3
	bl 0x02008988
	movs r0, #26
	movs r1, #3
	bl 0x02008988
	movs r0, #9
	movs r1, #2
	bl 0x02008988
	movs r0, #10
	movs r1, #2
	bl 0x02008988
	movs r0, #11
	movs r1, #2
	bl 0x02008988
	movs r0, #12
	movs r1, #2
	bl 0x02008988
	movs r0, #13
	movs r1, #2
	bl 0x02008988
	movs r1, #2
	movs r0, #14
	bl 0x02008988
	movs r0, #18
	bl 0x02008904
	movs r0, #19
	bl 0x02008904
	movs r0, #20
	bl 0x02008904
	movs r0, #21
	bl 0x02008904
	movs r0, #22
	bl 0x02008904
	movs r0, #23
	bl 0x02008904
	movs r0, #24
	bl 0x02008904
	movs r0, #25
	bl 0x02008904
	movs r0, #26
	bl 0x02008904
	movs r0, #9
	bl 0x02008904
	movs r0, #10
	bl 0x02008904
	movs r0, #11
	bl 0x02008904
	movs r0, #12
	bl 0x02008904
	movs r0, #13
	bl 0x02008904
	movs r0, #14
	bl 0x02008904
	ldr r0, [pc, #292]
	bl 0x02008948
	cmp r0, #0
	beq .L_02000538_0
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl 0x02008980
	movs r1, #5
	movs r0, #15
	bl 0x02008988
	movs r0, #15
	bl 0x02008978
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #15
	bl 0x02008978
	ldr r3, [pc, #252]
	str r3, [r0, #12]
	movs r0, #15
	bl 0x02008978
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #15
	bl 0x020089a0
	movs r3, #18
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x02008930
	b .L_02000538_1
.L_02000538_0:
	movs r1, #2
	movs r0, #8
	bl 0x02008988
	movs r0, #8
	bl 0x02008978
	movs r1, #0
	bl 0x02008940
	movs r0, #15
	movs r1, #1
	bl 0x02008988
.L_02000538_1:
	movs r0, #16
	movs r1, #1
	bl 0x02008988
	ldr r0, [pc, #168]
	bl 0x02008948
	cmp r0, #0
	beq .L_02000538_2
	movs r0, #17
	movs r1, #1
	bl 0x02008988
	movs r3, #22
	movs r5, #36
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #1
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02008930
	movs r3, #24
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #2
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02008930
	b .L_02000538_3
.L_02000538_2:
	movs r0, #17
	movs r1, #5
	bl 0x02008988
	movs r3, #22
	movs r5, #36
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02008930
	movs r3, #24
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #2
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02008930
.L_02000538_3:
	ldr r0, [pc, #68]
	bl 0x02008948
	cmp r0, #0
	beq .L_02000538_4
	movs r2, #188
	movs r0, #11
	ldr r1, [pc, #60]
	lsls r2, r2, #17
	bl 0x02008980
.L_02000538_4:
	movs r0, #193
	lsls r0, r0, #2
	bl 0x02008948
	cmp r0, #0
	beq .L_02000538_5
	movs r2, #188
	movs r0, #12
	ldr r1, [pc, #36]
	lsls r2, r2, #17
	bl 0x02008980
.L_02000538_5:
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000883
	.4byte 0xfffc0000
	.4byte 0x00000302
	.4byte 0x00000303
	.4byte 0x023a0000
	.global Func_020007c4
	.thumb_func
Func_020007c4:
	push {r5, lr}
	ldr r3, [pc, #44]
	ldr r3, [r3]
	adds r2, r3, #0
	adds r5, r0, #0
	movs r4, #8
	adds r2, #52
.L_020007c4_2:
	ldmia r2!, {r0}
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r5, r3
	bne .L_020007c4_0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r1, r3
	beq .L_020007c4_1
.L_020007c4_0:
	adds r4, #1
	cmp r4, #65
	bls .L_020007c4_2
	movs r0, #0
.L_020007c4_1:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_020007f8
	.thumb_func
Func_020007f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #0
	sub sp, #12
	bl 0x02008978
	ldrh r3, [r0, #6]
	ldr r4, [pc, #232]
	lsrs r3, r3, #12
	lsls r5, r3, #2
	ldr r3, [r4, r5]
	movs r2, #10
	ldrsh r1, [r0, r2]
	mov r8, r0
	asrs r2, r3, #16
	adds r1, r1, r2
	mov r9, r4
	mov r4, r8
	asrs r0, r1, #4
	lsls r3, r3, #16
	movs r1, #18
	ldrsh r2, [r4, r1]
	asrs r3, r3, #16
	adds r2, r2, r3
	asrs r1, r2, #4
	bl 0x020087c4
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020007f8_0
	mov r1, r9
	ldr r2, [r1, r5]
	movs r0, #10
	ldrsh r3, [r6, r0]
	asrs r1, r2, #16
	adds r3, r3, r1
	asrs r0, r3, #4
	lsls r2, r2, #16
	movs r4, #18
	ldrsh r3, [r6, r4]
	asrs r2, r2, #16
	adds r3, r3, r2
	asrs r1, r3, #4
	bl 0x020087c4
	mov r10, r0
	cmp r0, #0
	bne .L_020007f8_0
	adds r2, r6, #0
	adds r2, #34
	movs r3, #2
	strb r3, [r2]
	mov r0, r9
	ldr r1, [r0, r5]
	ldr r2, [pc, #144]
	ldr r3, [r6, #8]
	ands r2, r1
	mov r7, sp
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r0, r6, #0
	adds r1, r7, #0
	bl 0x02008938
	cmp r0, #0
	bgt .L_020007f8_0
	movs r1, #8
	mov r0, r8
	bl 0x02008910
	ldr r5, [pc, #104]
	movs r0, #15
	bl 0x02008908
	movs r0, #185
	bl 0x020089a8
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x02008918
	mov r1, r8
	str r5, [r1, #48]
	str r5, [r1, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	mov r0, r8
	bl 0x02008918
	adds r0, r6, #0
	bl 0x02008920
	ldr r3, [r7]
	str r3, [r6, #8]
	ldr r3, [r7, #8]
	mov r2, r10
	str r3, [r6, #16]
	str r2, [r6, #36]
	str r2, [r6, #44]
	mov r0, r8
	movs r1, #1
	bl 0x02008910
	bl 0x02008304
.L_020007f8_0:
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02009064
	.4byte 0xffff0000
	.4byte 0x00003333
	.global Func_02000904
	.thumb_func
Func_02000904:
	bx lr
	.2byte 0x0000
	.include "games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_DOU/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x00000048
	.4byte 0xc00001e8
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
	.4byte 0x00000174
	.4byte 0xc0000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000063
	.4byte 0xc00001bf
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000156
	.4byte 0x400000fe
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000001b8
	.4byte 0x40000088
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
	.4byte 0x00000360
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0xc00002f4
	.4byte 0x00280000
	.4byte 0x0276003c
	.4byte 0x00000348
	.4byte 0xffff0002
	.4byte 0x000001d8
	.4byte 0xc00002a8
	.4byte 0x00280000
	.4byte 0x0276003c
	.4byte 0x00000348
	.4byte 0xffff0003
	.4byte 0x00000188
	.4byte 0x400000c8
	.4byte 0x00280000
	.4byte 0x0276003c
	.4byte 0x00000348
	.4byte 0xffff0004
	.4byte 0x00000218
	.4byte 0x400001aa
	.4byte 0x00280000
	.4byte 0x0276003c
	.4byte 0x00000348
	.4byte 0xffff0005
	.4byte 0x00000358
	.4byte 0x4000007c
	.4byte 0x02f80000
	.4byte 0x039d0041
	.4byte 0x0000014a
	.4byte 0xffff0006
	.4byte 0x00000328
	.4byte 0xc00000f8
	.4byte 0x02f80000
	.4byte 0x039d0041
	.4byte 0x0000014a
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x4000012c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc00001d8
	.4byte 0x003c0000
	.4byte 0x0316003c
	.4byte 0x00000258
	.4byte 0xffff0002
	.4byte 0x00000138
	.4byte 0xc000020c
	.4byte 0x003c0000
	.4byte 0x0316003c
	.4byte 0x00000258
	.4byte 0xffff0003
	.4byte 0x0000021a
	.4byte 0xc0000224
	.4byte 0x003c0000
	.4byte 0x0316003c
	.4byte 0x00000258
	.4byte 0xffff0004
	.4byte 0x00000158
	.4byte 0x400000b0
	.4byte 0x003c0000
	.4byte 0x0316003c
	.4byte 0x00000258
	.4byte 0xffff0005
	.4byte 0x00000358
	.4byte 0xc0000278
	.4byte 0x02e40000
	.4byte 0x03d401fe
	.4byte 0x000002b2
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000031
	.4byte 0x00123002
	.4byte 0x00201030
	.4byte 0x00302030
	.4byte 0x00000030
	.4byte 0x00102031
	.4byte 0x00203031
	.4byte 0x0030102f
	.4byte 0x00406030
	.4byte 0x0050202f
	.4byte 0x00604030
	.4byte 0x0000002f
	.4byte 0x00103030
	.4byte 0x00205030
	.4byte 0x00330002
	.4byte 0x0040502f
	.4byte 0x0050402f
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff00f7
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00f7
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x01024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x020089b0
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x020089b0
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x020089b0
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
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
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008149
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x02008169
	.4byte 0x00000c15
	.4byte 0x03050008
	.4byte 0x02008425
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
	.4byte 0x00000013
	.4byte 0x0f5d0064
	.4byte 0x00100016
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02008189
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020081bd
	.4byte 0x00002115
	.4byte 0x08820009
	.4byte 0x020081f1
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
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte 0x0200844d
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte 0x0200845d
	.4byte 0x00002115
	.4byte 0x08830008
	.4byte 0x02008215
	.4byte 0x00001815
	.4byte 0x0883000f
	.4byte 0x02008281
	.4byte 0x00001815
	.4byte 0xffff0010
	.4byte 0x020082ad
	.4byte 0x00001815
	.4byte 0x13020011
	.4byte 0x020082d9
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x02008305
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x02008305
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x020087f9
	.4byte 0x00008602
	.4byte 0xffff000b
	.4byte 0x020087f9
	.4byte 0x00000013
	.4byte 0x0f5e0064
	.4byte 0x001000b6
	.4byte 0x00000013
	.4byte 0x0fc60065
	.4byte 0x001000ba
	.4byte 0x00000013
	.4byte 0x0fc70066
	.4byte 0x001000bd
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
