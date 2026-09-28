.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [r0, #80]
	movs r3, #3
	ldrb r2, [r0, #9]
	ands r1, r3
	movs r3, #13
	negs r3, r3
	lsls r1, r1, #2
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
	bx lr
	.2byte 0x0000
	.global Func_02000048
	.thumb_func
Func_02000048:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x0200c8c4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000048_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x0200c8fc
	adds r0, r5, #0
	movs r1, #14
	bl 0x0200ca4c
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c904
	adds r0, r5, #0
	b .L_02000048_1
.L_02000048_0:
	movs r0, #0
.L_02000048_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x0200c8c4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020000a0_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x0200c8fc
	adds r0, r5, #0
	movs r1, #15
	bl 0x0200ca4c
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_020000a0_1
.L_020000a0_0:
	movs r0, #0
.L_020000a0_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.global Func_02000104
	.thumb_func
Func_02000104:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	ldr r1, [r0, #80]
	adds r0, #100
	ldrh r3, [r1, #30]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #30]
	bx lr
	.2byte 0x0000
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r1, #0
	ldr r1, [sp, #48]
	adds r5, r0, #0
	movs r0, #0
	mov r8, r2
	str r3, [sp, #4]
	mov r10, r1
	ldr r7, [sp, #52]
	bl 0x0200c9bc
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_0200013c_0
	cmp r7, #0
	beq .L_0200013c_0
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_0200013c_1
.L_0200013c_0:
	adds r2, r6, #0
	movs r0, #222
.L_0200013c_1:
	adds r1, r5, #0
	mov r3, r8
	bl 0x0200c8c4
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200013c_2
	b .L_0200013c_3
.L_0200013c_2:
	ldr r1, [r6, #80]
	mov r8, r1
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl 0x0200c8b4
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200c8bc
	adds r3, r6, #0
	movs r0, #0
	adds r3, #85
	strb r0, [r3]
	mov r3, r8
	adds r3, #38
	strb r0, [r3]
	ldr r3, [pc, #328]
	str r3, [r6, #108]
	ldr r3, [sp, #4]
	str r3, [r6, #68]
	ldr r3, [sp, #40]
	str r3, [r6, #72]
	ldr r3, [sp, #44]
	mov r1, r9
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	mov r9, r3
	ands r3, r1
	orrs r3, r2
	adds r2, r6, #0
	mov r1, r8
	adds r2, #100
	strb r3, [r1, #9]
	adds r3, r2, #0
	str r0, [r6, #48]
	str r0, [r6, #52]
	str r2, [sp, #0]
	strh r0, [r3]
	ldr r3, [pc, #276]
	mov r1, r10
	ands r3, r1
	movs r5, #3
	cmp r3, #0
	beq .L_0200013c_3
	cmp r7, #0
	beq .L_0200013c_3
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl 0x0200ca4c
.L_0200013c_4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_5
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	mov r3, r8
	ldrb r2, [r7]
	ldrb r1, [r3, #9]
	ands r2, r5
	mov r3, r9
	ands r3, r1
	lsls r2, r2, #2
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #9]
.L_0200013c_5:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_0200013c_6
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200013c_6:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_7
	ldr r3, [pc, #156]
	mov r1, r11
	ldr r5, [r3, r1]
	cmp r2, #0
	beq .L_0200013c_8
	ldr r0, [r7, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl 0x0200c854
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200013c_9
.L_0200013c_8:
	ldr r0, [r7, #16]
	ldr r2, [pc, #128]
	ldr r1, [r5, #12]
	adds r0, r0, r2
	bl 0x0200c854
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x0200c854
	str r0, [r6, #52]
.L_0200013c_7:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_10
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200c8b4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200c8bc
.L_0200013c_10:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_11
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #30]
.L_0200013c_11:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_12
	ldrh r3, [r7, #34]
	ldr r1, [sp, #0]
	strh r3, [r1]
.L_0200013c_12:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_3
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200013c_3:
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200cbc4
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #104]
	cmp r1, #0
	beq .L_02000314_0
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r1, #16]
	ldr r3, [r5, #16]
	ldr r1, [r1, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl 0x0200c87c
	ldrh r3, [r5, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r0, #0
	beq .L_02000314_0
	movs r2, #128
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_02000314_1
	adds r0, r2, #0
.L_02000314_1:
	ldr r2, [pc, #20]
	cmp r0, r2
	bge .L_02000314_2
	adds r0, r2, #0
.L_02000314_2:
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_02000314_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xfffff000
	.global Func_0200036c
	.thumb_func
Func_0200036c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200cd88
	.global Func_02000374
	.thumb_func
Func_02000374:
	movs r0, #0
	bx lr
	.global Func_02000378
	.thumb_func
Func_02000378:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200cdb8
	.global Func_02000380
	.thumb_func
Func_02000380:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200cdc4
	.global Func_02000388
	.thumb_func
Func_02000388:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200cfa4
	.global Func_02000390
	.thumb_func
Func_02000390:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	sub sp, #8
	bl 0x0200c99c
	movs r0, #141
	bl 0x0200cb14
	movs r5, #0
.L_02000390_1:
	movs r1, #1
	ldr r0, [pc, #776]
	bl 0x0200cacc
	movs r0, #8
	bl 0x0200cadc
	movs r0, #8
	bl 0x0200c994
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl 0x0200cacc
	movs r0, #8
	bl 0x0200cadc
	movs r0, #8
	bl 0x0200c994
	cmp r5, #1
	bne .L_02000390_0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200c90c
.L_02000390_0:
	adds r5, #1
	cmp r5, #6
	bne .L_02000390_1
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #30
	bl 0x0200c994
	ldr r0, [pc, #692]
	ldr r1, [pc, #696]
	bl 0x0200caa4
	movs r0, #166
	movs r1, #1
	movs r3, #1
	negs r1, r1
	ldr r2, [pc, #684]
	lsls r0, r0, #18
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #20
	bl 0x0200c994
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #144
	bl 0x0200cb14
	ldr r2, [pc, #648]
	mov r8, r2
	mov r0, r8
	movs r1, #96
	movs r2, #29
	bl 0x0200c8e4
	movs r3, #41
	movs r2, #29
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r9, r3
	mov r10, r2
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	movs r5, #1
	movs r3, #31
	movs r6, #2
	movs r1, #42
	movs r2, #41
	movs r0, #87
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200c8ec
	movs r0, #40
	bl 0x0200c994
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200c90c
	ldr r0, [pc, #580]
	ldr r1, [pc, #580]
	bl 0x0200caa4
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #576]
	negs r1, r1
	ldr r2, [pc, #556]
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #20
	bl 0x0200c994
	movs r0, #144
	bl 0x0200cb14
	mov r0, r8
	movs r1, #74
	movs r2, #29
	bl 0x0200c8e4
	movs r3, #19
	str r3, [sp, #0]
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	movs r3, #31
	movs r1, #42
	movs r2, #19
	movs r0, #87
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200c8ec
	movs r0, #40
	bl 0x0200c994
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c90c
	movs r1, #1
	movs r2, #192
	movs r3, #1
	ldr r0, [pc, #468]
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #20
	bl 0x0200c994
	movs r0, #144
	bl 0x0200cb14
	mov r0, r8
	movs r1, #96
	movs r2, #10
	bl 0x0200c8e4
	mov r2, r9
	movs r3, #10
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	movs r1, #42
	movs r2, #41
	movs r3, #12
	movs r0, #87
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200c8ec
	movs r0, #40
	bl 0x0200c994
	ldr r3, [pc, #380]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x0200caec
	bl 0x0200caf4
	movs r1, #1
	movs r2, #237
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #17
	ldr r0, [pc, #352]
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200c90c
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #40
	bl 0x0200c994
	ldr r0, [pc, #308]
	bl 0x0200cb14
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #300]
	negs r0, r0
	bl 0x0200c90c
	bl 0x0200c914
	movs r0, #20
	bl 0x0200c994
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #40
	movs r2, #43
	movs r3, #66
	movs r0, #0
	bl 0x0200c8ec
	movs r0, #20
	bl 0x0200c994
	movs r1, #178
	movs r2, #128
	movs r3, #232
	lsls r3, r3, #17
	lsls r2, r2, #13
	lsls r1, r1, #18
	movs r0, #220
	bl 0x0200c260
	adds r5, r0, #0
	movs r0, #40
	bl 0x0200c994
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c924
	ldr r5, [pc, #228]
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200c91c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #20
	bl 0x0200ca7c
	bl 0x0200caec
	bl 0x0200caf4
	movs r0, #231
	movs r1, #1
	movs r2, #175
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #40
	bl 0x0200c994
	subs r5, #1
	movs r2, #30
	movs r1, #4
	movs r0, #9
	bl 0x0200ca1c
	adds r0, r5, #0
	bl 0x0200ca54
	movs r0, #9
	movs r1, #20
	bl 0x0200c248
	bl 0x0200caec
	bl 0x0200caf4
	movs r1, #1
	movs r2, #237
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #0
	ldr r0, [pc, #88]
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	bl 0x0200cae4
	bl 0x0200caf4
	ldr r0, [pc, #80]
	bl 0x0200c96c
	bl 0x0200c9a4
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00403a52
	.4byte 0x00026666
	.4byte 0x00004ccc
	.4byte 0x01f10000
	.4byte 0x0200d088
	.4byte 0x00066666
	.4byte 0x0000cccc
	.4byte 0x01370000
	.4byte 0x02970000
	.4byte 0x03001ebc
	.4byte 0x02c60000
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00001075
	.4byte 0x0000083c
	.global Func_020006f4
	.thumb_func
Func_020006f4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	sub sp, #8
	bl 0x0200c99c
	movs r0, #141
	bl 0x0200cb14
	movs r5, #0
.L_020006f4_1:
	movs r1, #1
	ldr r0, [pc, #792]
	bl 0x0200cacc
	movs r0, #8
	bl 0x0200cadc
	movs r0, #8
	bl 0x0200c994
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl 0x0200cacc
	movs r0, #8
	bl 0x0200cadc
	movs r0, #8
	bl 0x0200c994
	cmp r5, #1
	bne .L_020006f4_0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200c90c
.L_020006f4_0:
	adds r5, #1
	cmp r5, #6
	bne .L_020006f4_1
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #30
	bl 0x0200c994
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200c90c
	ldr r0, [pc, #700]
	ldr r1, [pc, #700]
	bl 0x0200caa4
	movs r0, #236
	movs r1, #1
	movs r2, #196
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #15
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #20
	bl 0x0200c994
	movs r0, #144
	bl 0x0200cb14
	ldr r2, [pc, #648]
	mov r10, r2
	mov r0, r10
	movs r1, #84
	movs r2, #4
	bl 0x0200c8e4
	movs r3, #29
	mov r9, r3
	mov r2, r9
	movs r3, #4
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	movs r3, #2
	str r3, [sp, #4]
	movs r6, #1
	mov r8, r3
	movs r1, #42
	movs r3, #6
	movs r2, #29
	movs r0, #87
	str r6, [sp, #0]
	bl 0x0200c8ec
	movs r0, #40
	bl 0x0200c994
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200c90c
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #572]
	negs r1, r1
	ldr r2, [pc, #572]
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #20
	bl 0x0200c994
	movs r0, #144
	bl 0x0200cb14
	mov r0, r10
	movs r1, #76
	movs r2, #21
	bl 0x0200c8e4
	movs r5, #21
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c8f4
	mov r2, r8
	str r2, [sp, #4]
	movs r3, #23
	movs r1, #42
	movs r2, #21
	movs r0, #87
	str r6, [sp, #0]
	bl 0x0200c8ec
	movs r0, #40
	bl 0x0200c994
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200c90c
	ldr r0, [pc, #476]
	ldr r1, [pc, #480]
	bl 0x0200caa4
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #456]
	negs r1, r1
	ldr r2, [pc, #468]
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #20
	bl 0x0200c994
	movs r0, #144
	bl 0x0200cb14
	mov r0, r10
	movs r1, #76
	movs r2, #29
	bl 0x0200c8e4
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200c8f4
	mov r2, r8
	str r2, [sp, #4]
	movs r1, #42
	movs r2, #21
	movs r3, #31
	movs r0, #87
	str r6, [sp, #0]
	bl 0x0200c8ec
	movs r0, #40
	bl 0x0200c994
	ldr r3, [pc, #384]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x0200caec
	bl 0x0200caf4
	movs r0, #178
	movs r1, #1
	movs r2, #152
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #18
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200c90c
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #40
	bl 0x0200c994
	ldr r0, [pc, #308]
	bl 0x0200cb14
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #300]
	negs r0, r0
	bl 0x0200c90c
	bl 0x0200c914
	movs r0, #20
	bl 0x0200c994
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #40
	movs r2, #43
	movs r3, #46
	movs r0, #0
	bl 0x0200c8ec
	movs r6, #178
	movs r0, #20
	bl 0x0200c994
	lsls r6, r6, #18
	movs r2, #128
	movs r3, #144
	lsls r3, r3, #16
	lsls r2, r2, #13
	adds r1, r6, #0
	movs r0, #221
	bl 0x0200c260
	adds r5, r0, #0
	movs r0, #40
	bl 0x0200c994
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c924
	ldr r5, [pc, #224]
	movs r1, #1
	adds r0, r5, #0
	bl 0x0200c91c
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200ca7c
	bl 0x0200caec
	bl 0x0200caf4
	movs r0, #231
	movs r1, #1
	movs r2, #175
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #40
	bl 0x0200c994
	subs r5, #2
	movs r2, #30
	movs r1, #4
	movs r0, #9
	bl 0x0200ca1c
	adds r0, r5, #0
	bl 0x0200ca54
	movs r0, #9
	movs r1, #20
	bl 0x0200c248
	bl 0x0200caec
	bl 0x0200caf4
	movs r1, #1
	movs r2, #152
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #0
	adds r0, r6, #0
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	bl 0x0200cae4
	bl 0x0200caf4
	ldr r0, [pc, #76]
	bl 0x0200c96c
	bl 0x0200c9a4
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00404a4e
	.4byte 0x00059999
	.4byte 0x0000b333
	.4byte 0x0200d088
	.4byte 0x01570000
	.4byte 0x01710000
	.4byte 0x00033333
	.4byte 0x00006666
	.4byte 0x01f10000
	.4byte 0x03001ebc
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00001076
	.4byte 0x0000083d
	.global Func_02000a64
	.thumb_func
Func_02000a64:
	push {lr}
	bl 0x0200c99c
	bl 0x02008a98
	bl 0x02008d5c
	bl 0x02008f8c
	bl 0x02009450
	bl 0x0200978c
	bl 0x02009d04
	ldr r0, [pc, #16]
	bl 0x0200c96c
	bl 0x0200c9a4
	bl 0x0200a27c
	pop {r0}
	bx r0
	.4byte 0x0000083e
	.global Func_02000a98
	.thumb_func
Func_02000a98:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r0, #141
	sub sp, #8
	bl 0x0200cb14
	movs r5, #0
.L_02000a98_1:
	movs r1, #1
	ldr r0, [pc, #628]
	bl 0x0200cacc
	movs r0, #8
	bl 0x0200cadc
	movs r0, #8
	bl 0x0200c994
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl 0x0200cacc
	movs r0, #8
	bl 0x0200cadc
	movs r0, #8
	bl 0x0200c994
	cmp r5, #1
	bne .L_02000a98_0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200c90c
.L_02000a98_0:
	adds r5, #1
	cmp r5, #6
	bne .L_02000a98_1
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #10
	bl 0x0200c90c
	movs r0, #30
	bl 0x0200c994
	ldr r0, [pc, #544]
	ldr r1, [pc, #548]
	bl 0x0200caa4
	movs r0, #167
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	ldr r2, [pc, #536]
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #20
	bl 0x0200c994
	movs r0, #144
	bl 0x0200cb14
	ldr r2, [pc, #500]
	mov r8, r2
	mov r0, r8
	movs r1, #65
	movs r2, #31
	bl 0x0200c8e4
	movs r3, #10
	mov r10, r3
	mov r2, r10
	movs r3, #31
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	movs r5, #1
	movs r3, #33
	movs r6, #2
	movs r1, #42
	movs r2, #10
	movs r0, #87
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200c8ec
	movs r0, #40
	bl 0x0200c994
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200c90c
	ldr r0, [pc, #432]
	ldr r1, [pc, #432]
	bl 0x0200caa4
	movs r1, #1
	movs r2, #177
	movs r3, #1
	ldr r0, [pc, #424]
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #20
	bl 0x0200c994
	movs r0, #144
	bl 0x0200cb14
	mov r0, r8
	movs r1, #79
	movs r2, #9
	bl 0x0200c8e4
	movs r3, #24
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	movs r3, #11
	movs r1, #42
	movs r2, #24
	movs r0, #87
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200c8ec
	movs r0, #40
	bl 0x0200c994
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl 0x0200c90c
	ldr r0, [pc, #296]
	ldr r1, [pc, #300]
	bl 0x0200caa4
	movs r1, #1
	movs r2, #193
	movs r3, #1
	ldr r0, [pc, #312]
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #20
	bl 0x0200c994
	movs r0, #144
	bl 0x0200cb14
	mov r0, r8
	movs r1, #91
	movs r2, #10
	bl 0x0200c8e4
	movs r3, #36
	str r3, [sp, #0]
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	movs r1, #42
	movs r2, #36
	movs r3, #12
	movs r0, #87
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200c8ec
	movs r0, #40
	bl 0x0200c994
	ldr r3, [pc, #220]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x0200caec
	bl 0x0200caf4
	movs r0, #232
	movs r1, #1
	movs r3, #0
	negs r1, r1
	ldr r2, [pc, #196]
	lsls r0, r0, #16
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200c90c
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #40
	bl 0x0200c994
	ldr r0, [pc, #152]
	bl 0x0200cb14
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #144]
	negs r0, r0
	bl 0x0200c90c
	bl 0x0200c914
	movs r0, #20
	bl 0x0200c994
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #40
	movs r2, #13
	movs r3, #66
	movs r0, #0
	bl 0x0200c8ec
	movs r0, #20
	bl 0x0200c994
	movs r1, #232
	movs r2, #128
	movs r3, #232
	lsls r2, r2, #13
	lsls r3, r3, #17
	lsls r1, r1, #16
	movs r0, #223
	bl 0x0200c260
	adds r5, r0, #0
	movs r0, #40
	bl 0x0200c994
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c924
	ldr r0, [pc, #72]
	movs r1, #1
	bl 0x0200c91c
	sub sp, #-8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x004049d2
	.4byte 0x00026666
	.4byte 0x00004ccc
	.4byte 0x02110000
	.4byte 0x0200d088
	.4byte 0x00066666
	.4byte 0x0000cccc
	.4byte 0x01870000
	.4byte 0x02470000
	.4byte 0x03001ebc
	.4byte 0x01dd0000
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00001077
	.global Func_02000d5c
	.thumb_func
Func_02000d5c:
	push {lr}
	movs r0, #17
	bl 0x0200cb14
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c9c4
	movs r2, #245
	movs r0, #0
	movs r1, #231
	lsls r2, r2, #1
	bl 0x0200c9f4
	movs r1, #192
	movs r2, #30
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #3
	movs r0, #0
	bl 0x0200ca14
	movs r0, #180
	bl 0x0200c994
	movs r1, #2
	movs r0, #0
	bl 0x0200ca2c
	movs r0, #80
	bl 0x0200c994
	movs r0, #0
	ldr r1, [pc, #416]
	movs r2, #60
	bl 0x0200ca8c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c9c4
	movs r0, #0
	movs r1, #246
	ldr r2, [pc, #396]
	bl 0x0200c9f4
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200ca7c
	movs r0, #0
	bl 0x0200c9bc
	cmp r0, #0
	beq .L_02000d5c_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200ca04
.L_02000d5c_0:
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c9c4
	movs r0, #1
	ldr r1, [pc, #336]
	ldr r2, [pc, #344]
	bl 0x0200c9f4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca24
	movs r1, #2
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r0, #0
	ldr r1, [pc, #280]
	movs r2, #0
	bl 0x0200ca8c
	movs r2, #80
	movs r0, #1
	ldr r1, [pc, #268]
	bl 0x0200ca8c
	movs r0, #0
	movs r1, #3
	bl 0x0200ca0c
	movs r0, #1
	movs r1, #4
	bl 0x0200ca14
	movs r0, #0
	ldr r1, [pc, #256]
	ldr r2, [pc, #260]
	bl 0x0200c9c4
	movs r0, #1
	ldr r1, [pc, #248]
	ldr r2, [pc, #248]
	bl 0x0200c9c4
	movs r0, #0
	ldr r1, [pc, #244]
	ldr r2, [pc, #248]
	bl 0x0200c9ec
	movs r1, #141
	ldr r2, [pc, #244]
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200c9f4
	movs r0, #0
	movs r1, #1
	bl 0x0200ca0c
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200ca1c
	movs r0, #1
	movs r1, #6
	movs r2, #60
	bl 0x0200ca1c
	movs r2, #166
	movs r0, #5
	ldr r1, [pc, #156]
	lsls r2, r2, #17
	bl 0x0200ca04
	movs r2, #166
	movs r0, #9
	ldr r1, [pc, #148]
	lsls r2, r2, #17
	bl 0x0200ca04
	movs r2, #174
	movs r0, #11
	ldr r1, [pc, #140]
	lsls r2, r2, #17
	bl 0x0200ca04
	movs r2, #174
	lsls r2, r2, #17
	movs r0, #10
	ldr r1, [pc, #132]
	bl 0x0200ca04
	ldr r0, [pc, #128]
	ldr r1, [pc, #132]
	bl 0x0200caa4
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #124]
	negs r1, r1
	ldr r2, [pc, #124]
	bl 0x0200caac
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	bl 0x0200cab4
	movs r0, #40
	bl 0x0200c994
	pop {r0}
	bx r0
	.4byte 0x00000101
	.4byte 0x000001df
	.4byte 0x000001eb
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x00000109
	.4byte 0x000001c5
	.4byte 0x000001d5
	.4byte 0x01db0000
	.4byte 0x01eb0000
	.4byte 0x01cb0000
	.4byte 0x01fb0000
	.4byte 0x00073333
	.4byte 0x0000e666
	.4byte 0x01e50000
	.4byte 0x01590000
	.global Func_02000f8c
	.thumb_func
Func_02000f8c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #61
	bl 0x0200cb14
	movs r1, #4
	movs r0, #10
	bl 0x0200ca0c
	ldr r0, [pc, #1004]
	bl 0x0200ca54
	movs r0, #10
	movs r1, #10
	bl 0x0200c248
	movs r0, #11
	movs r1, #4
	bl 0x0200ca0c
	movs r0, #11
	movs r1, #30
	bl 0x0200c248
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200ca8c
	movs r0, #9
	movs r1, #4
	movs r2, #10
	bl 0x0200ca1c
	movs r2, #30
	movs r0, #9
	movs r1, #6
	bl 0x0200ca1c
	movs r0, #9
	movs r1, #10
	bl 0x0200c248
	movs r0, #10
	movs r1, #1
	bl 0x0200ca2c
	movs r1, #176
	movs r2, #10
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #10
	movs r1, #20
	bl 0x0200c248
	movs r0, #11
	movs r1, #1
	bl 0x0200ca2c
	movs r1, #208
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #11
	movs r1, #30
	bl 0x0200c248
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200ca8c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #80
	bl 0x0200ca7c
	movs r1, #129
	movs r2, #40
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200ca8c
	movs r0, #5
	movs r1, #20
	bl 0x0200c248
	movs r0, #9
	movs r1, #2
	bl 0x0200ca2c
	movs r0, #9
	movs r1, #4
	bl 0x0200ca0c
	movs r1, #10
	movs r0, #9
	bl 0x0200c248
	movs r0, #12
	bl 0x0200c9bc
	adds r6, r0, #0
	movs r0, #8
	bl 0x0200c9bc
	ldr r3, [r6, #80]
	movs r5, #0
	adds r3, #38
	strb r5, [r3]
	mov r9, r3
	ldr r3, [pc, #780]
	movs r1, #128
	str r3, [r6, #24]
	str r3, [r6, #28]
	lsls r1, r1, #1
	str r3, [r0, #24]
	str r3, [r0, #28]
	mov r8, r0
	movs r0, #12
	bl 0x0200ca44
	movs r2, #145
	ldr r1, [pc, #760]
	movs r0, #12
	lsls r2, r2, #17
	bl 0x0200ca04
	movs r2, #85
	movs r3, #160
	lsls r3, r3, #14
	adds r2, r2, r6
	strb r5, [r2]
	movs r0, #1
	str r3, [r6, #12]
	mov r10, r2
	bl 0x0200c994
	movs r0, #12
	movs r1, #10
	bl 0x0200c248
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #30
	bl 0x0200ca8c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200ca7c
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #176
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200caa4
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #636]
	negs r1, r1
	ldr r2, [pc, #636]
	bl 0x0200caac
	bl 0x0200cab4
	movs r2, #145
	ldr r1, [pc, #620]
	lsls r2, r2, #17
	movs r0, #8
	bl 0x0200ca04
	movs r0, #190
	bl 0x0200cb14
	movs r0, #12
	movs r1, #2
	bl 0x0200ca84
	ldr r7, [pc, #604]
.L_02000f8c_0:
	ldr r3, [r6, #12]
	ldr r2, [pc, #604]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #24]
	adds r3, r3, r7
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	adds r3, r3, r7
	str r3, [r6, #28]
	mov r2, r8
	ldr r3, [r2, #24]
	adds r3, r3, r7
	str r3, [r2, #24]
	ldr r3, [r2, #28]
	adds r3, r3, r7
	str r3, [r2, #28]
	movs r0, #1
	adds r5, #1
	bl 0x0200c994
	cmp r5, #90
	bne .L_02000f8c_0
	movs r3, #5
	mov r2, r10
	strb r3, [r2]
	movs r0, #80
	bl 0x0200c994
	movs r5, #0
.L_02000f8c_1:
	ldr r3, [r6, #12]
	ldr r2, [pc, #548]
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #1
	adds r5, #1
	bl 0x0200c994
	cmp r5, #60
	bne .L_02000f8c_1
	movs r3, #3
	mov r2, r10
	strb r3, [r2]
	movs r0, #30
	bl 0x0200c994
	movs r3, #1
	mov r2, r9
	strb r3, [r2]
	movs r0, #8
	movs r2, #0
	movs r1, #0
	bl 0x0200ca04
	movs r1, #1
	movs r0, #12
	bl 0x0200ca84
	movs r0, #12
	bl 0x0200c9bc
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r5, r3
	strb r5, [r0]
	movs r1, #0
	movs r0, #12
	bl 0x0200ca44
	movs r1, #128
	movs r2, #128
	movs r0, #12
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c9c4
	movs r2, #153
	lsls r2, r2, #1
	ldr r1, [pc, #456]
	movs r0, #12
	bl 0x0200c9f4
	movs r0, #40
	bl 0x0200c994
	movs r0, #12
	movs r1, #2
	bl 0x0200ca2c
	ldr r0, [pc, #436]
	movs r1, #20
	bl 0x0200c248
	movs r2, #0
	movs r1, #9
	movs r0, #5
	bl 0x0200ca3c
	movs r0, #20
	bl 0x0200c994
	movs r0, #5
	movs r1, #2
	bl 0x0200ca24
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #40
	bl 0x0200c994
	movs r0, #10
	movs r1, #1
	bl 0x0200ca24
	movs r1, #1
	movs r0, #11
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r0, #10
	movs r1, #4
	bl 0x0200ca14
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r2, #10
	movs r0, #11
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r0, #10
	movs r1, #30
	bl 0x0200c248
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200ca94
	movs r0, #60
	bl 0x0200c994
	movs r1, #1
	movs r0, #11
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	movs r0, #11
	movs r1, #30
	bl 0x0200c248
	movs r1, #208
	movs r2, #30
	movs r0, #11
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #3
	movs r0, #11
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r0, #11
	movs r1, #30
	bl 0x0200c248
	movs r1, #3
	movs r0, #12
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	movs r2, #20
	movs r0, #5
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r1, #3
	movs r0, #10
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	bl 0x0200caec
	bl 0x0200caf4
	movs r0, #132
	movs r1, #1
	movs r2, #230
	lsls r2, r2, #17
	movs r3, #0
	negs r1, r1
	lsls r0, r0, #17
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #40
	bl 0x0200c994
	movs r0, #10
	movs r1, #40
	bl 0x0200c248
	movs r0, #0
	movs r1, #3
	bl 0x0200ca24
	movs r1, #3
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #80
	bl 0x0200c994
	movs r1, #0
	movs r0, #11
	bl 0x0200ca5c
	movs r0, #0
	movs r1, #0
	bl 0x0200c9b4
	cmp r0, #0
	beq .L_02000f8c_2
	ldr r3, [pc, #96]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000f8c_2:
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x0200ca6c
	bl 0x0200caec
	bl 0x0200caf4
	movs r1, #1
	movs r2, #167
	movs r3, #0
	lsls r2, r2, #17
	negs r1, r1
	ldr r0, [pc, #56]
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	b .L_02000f8c_3
	.2byte 0x0000
	.4byte 0x0000107d
	.4byte 0x00001999
	.4byte 0x01d70000
	.4byte 0x01350000
	.4byte 0x0000028f
	.4byte 0xffffe667
	.4byte 0xffff8000
	.4byte 0x000001d7
	.4byte 0x0000400c
	.4byte 0x03001ebc
	.4byte 0x01dd0000
.L_02000f8c_3:
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #20
	bl 0x0200c994
	movs r0, #10
	movs r1, #2
	bl 0x0200ca2c
	movs r1, #176
	movs r2, #10
	lsls r1, r1, #8
	movs r0, #10
	bl 0x0200ca7c
	ldr r0, [pc, #100]
	bl 0x0200ca54
	movs r0, #10
	movs r1, #20
	bl 0x0200c248
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #6
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #4
	bl 0x0200ca0c
	ldr r0, [pc, #56]
	movs r1, #40
	bl 0x0200c248
	movs r0, #11
	movs r1, #1
	bl 0x0200ca2c
	movs r0, #11
	movs r1, #10
	bl 0x0200c248
	movs r0, #5
	movs r1, #2
	bl 0x0200ca24
	movs r0, #9
	movs r1, #2
	bl 0x0200ca2c
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000108d
	.4byte 0x00005009
	.global Func_02001450
	.thumb_func
Func_02001450:
	push {lr}
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200ca7c
	movs r1, #208
	movs r2, #40
	movs r0, #11
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #11
	movs r1, #2
	bl 0x0200ca2c
	movs r0, #11
	movs r1, #20
	bl 0x0200c248
	movs r0, #12
	movs r1, #2
	bl 0x0200ca2c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl 0x0200ca94
	movs r0, #60
	bl 0x0200c994
	movs r0, #12
	movs r1, #10
	bl 0x0200c248
	movs r0, #10
	movs r1, #12
	movs r2, #0
	bl 0x0200ca34
	movs r0, #5
	movs r1, #12
	movs r2, #0
	bl 0x0200ca34
	movs r2, #0
	movs r1, #12
	movs r0, #9
	bl 0x0200ca34
	movs r0, #40
	bl 0x0200c994
	movs r0, #10
	movs r1, #1
	bl 0x0200ca2c
	movs r1, #128
	movs r2, #10
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #10
	movs r1, #3
	bl 0x0200ca0c
	movs r0, #10
	movs r1, #10
	bl 0x0200c248
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl 0x0200ca7c
	movs r1, #3
	movs r0, #11
	bl 0x0200ca14
	movs r0, #10
	bl 0x0200c994
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #5
	movs r1, #2
	bl 0x0200ca24
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200ca7c
	movs r1, #176
	movs r2, #40
	movs r0, #10
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #10
	movs r1, #3
	bl 0x0200ca14
	movs r0, #10
	movs r1, #10
	bl 0x0200c248
	movs r1, #128
	movs r2, #20
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200ca8c
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	bl 0x0200ca94
	movs r1, #3
	movs r0, #12
	bl 0x0200ca2c
	movs r0, #40
	bl 0x0200c994
	movs r1, #173
	movs r2, #220
	lsls r2, r2, #17
	lsls r1, r1, #17
	movs r0, #1
	bl 0x0200ca04
	movs r0, #1
	bl 0x0200c864
	movs r0, #1
	movs r1, #0
	bl 0x0200ca64
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200ca7c
	bl 0x0200caec
	bl 0x0200caf4
	movs r1, #140
	movs r2, #235
	movs r0, #1
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200ca04
	movs r1, #1
	movs r2, #233
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #17
	ldr r0, [pc, #392]
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #20
	bl 0x0200c994
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #10
	bl 0x0200ca7c
	movs r1, #129
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ca8c
	movs r0, #1
	movs r1, #3
	bl 0x0200ca2c
	movs r1, #0
	movs r0, #1
	bl 0x0200ca74
	movs r0, #10
	bl 0x0200c994
	movs r1, #4
	movs r0, #1
	bl 0x0200ca14
	movs r0, #10
	bl 0x0200c994
	movs r1, #0
	movs r0, #1
	bl 0x0200ca74
	movs r0, #10
	bl 0x0200c994
	ldr r0, [pc, #292]
	bl 0x0200ca54
	movs r0, #11
	movs r1, #0
	bl 0x0200ca64
	movs r1, #208
	movs r2, #10
	movs r0, #11
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca24
	movs r1, #2
	movs r0, #0
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200ca7c
	bl 0x0200caec
	bl 0x0200caf4
	movs r1, #1
	movs r2, #167
	movs r3, #0
	lsls r2, r2, #17
	negs r1, r1
	ldr r0, [pc, #204]
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #20
	bl 0x0200c994
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	movs r1, #128
	movs r2, #30
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #20
	bl 0x0200c248
	movs r2, #40
	movs r0, #5
	movs r1, #0
	bl 0x0200ca7c
	movs r1, #4
	movs r0, #5
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r0, #5
	movs r1, #10
	bl 0x0200c248
	movs r1, #2
	movs r0, #12
	bl 0x0200ca2c
	movs r0, #80
	bl 0x0200c994
	movs r0, #12
	movs r1, #20
	bl 0x0200c248
	movs r0, #5
	movs r1, #2
	bl 0x0200ca2c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #176
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #12
	movs r1, #3
	bl 0x0200ca14
	movs r0, #12
	movs r1, #20
	bl 0x0200c248
	movs r0, #5
	movs r1, #2
	bl 0x0200ca24
	movs r0, #9
	movs r1, #2
	bl 0x0200ca2c
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200ca7c
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x01050000
	.4byte 0x0000109b
	.4byte 0x01dd0000
	.global Func_0200178c
	.thumb_func
Func_0200178c:
	push {r5, r6, r7, lr}
	movs r0, #161
	bl 0x0200cb14
	movs r0, #12
	movs r1, #3
	bl 0x0200ca2c
	movs r0, #40
	bl 0x0200c994
	movs r0, #12
	bl 0x0200c9bc
	cmp r0, #0
	beq .L_0200178c_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #13
	bl 0x0200ca04
.L_0200178c_0:
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl 0x0200ca04
	movs r0, #20
	bl 0x0200c994
	movs r1, #192
	movs r2, #40
	movs r0, #13
.L_020017cc:
	lsls r1, r1, #6
	bl 0x0200ca7c
	movs r0, #5
	movs r1, #3
	bl 0x0200ca2c
	movs r1, #3
	movs r0, #5
	bl 0x0200ca2c
	movs r0, #40
	bl 0x0200c994
	movs r0, #5
	movs r1, #2
	bl 0x0200ca2c
	movs r0, #5
	movs r1, #20
	bl 0x0200c248
	movs r1, #3
	movs r0, #13
	bl 0x0200ca14
	movs r0, #10
	bl 0x0200c994
	movs r0, #5
	movs r1, #3
	bl 0x0200ca24
	movs r1, #128
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #2
	bl 0x0200ca2c
	movs r0, #9
	movs r1, #40
	bl 0x0200c248
	movs r1, #3
	movs r0, #5
	bl 0x0200ca14
	movs r0, #40
	bl 0x0200c994
	movs r1, #176
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #13
	movs r1, #2
	bl 0x0200ca2c
	movs r0, #13
	movs r1, #20
	bl 0x0200c248
	movs r1, #1
	movs r0, #5
	bl 0x0200ca2c
	movs r0, #40
	bl 0x0200c994
	movs r1, #3
	movs r0, #13
	bl 0x0200ca14
	movs r0, #10
	bl 0x0200c994
	movs r0, #13
	movs r1, #40
	bl 0x0200c248
	movs r0, #10
	movs r1, #1
	bl 0x0200ca24
	movs r1, #3
	movs r0, #10
	bl 0x0200ca14
	movs r0, #10
	bl 0x0200c994
	movs r0, #10
	movs r1, #10
	bl 0x0200c248
	movs r0, #11
	movs r1, #1
	bl 0x0200ca2c
	movs r0, #11
	movs r1, #3
	bl 0x0200ca0c
	movs r0, #11
	movs r1, #80
	bl 0x0200c248
	movs r0, #13
	movs r1, #2
	bl 0x0200ca2c
	movs r0, #13
	movs r1, #40
	bl 0x0200c248
	movs r1, #2
	movs r0, #5
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	movs r0, #5
	movs r1, #10
	bl 0x0200c248
	movs r0, #13
	movs r1, #2
	bl 0x0200ca24
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #80
	bl 0x0200c994
	movs r1, #4
	movs r0, #5
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r0, #5
	movs r1, #80
	bl 0x0200c248
	movs r0, #13
	movs r1, #4
	bl 0x0200ca14
	movs r0, #13
	movs r1, #80
	bl 0x0200c248
	movs r1, #2
	movs r0, #5
	bl 0x0200ca2c
	movs r0, #4
	bl 0x0200c994
	movs r0, #5
	movs r1, #20
	bl 0x0200c248
	movs r0, #10
	movs r1, #1
	bl 0x0200ca2c
	movs r0, #10
	movs r1, #3
	bl 0x0200ca0c
	movs r0, #10
	movs r1, #10
	bl 0x0200c248
	movs r0, #11
	movs r1, #1
	bl 0x0200ca2c
	movs r0, #11
	movs r1, #10
	bl 0x0200c248
	movs r0, #10
	movs r1, #1
	bl 0x0200ca2c
	movs r0, #10
	movs r1, #10
	bl 0x0200c248
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #80
	bl 0x0200ca7c
	movs r2, #80
	movs r0, #9
	ldr r1, [pc, #884]
	bl 0x0200ca8c
	movs r0, #11
	movs r1, #1
	bl 0x0200ca2c
	movs r1, #160
	movs r2, #40
	movs r0, #11
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r0, #11
	movs r1, #2
	bl 0x0200ca24
	movs r0, #11
	movs r1, #20
	bl 0x0200c248
	bl 0x0200caec
	bl 0x0200caf4
	movs r1, #1
	movs r2, #233
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #17
	ldr r0, [pc, #828]
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #20
	bl 0x0200c994
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #176
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca24
	movs r0, #1
	movs r1, #2
	bl 0x0200ca2c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c9c4
	movs r1, #128
.L_02001a00:
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c9c4
	movs r2, #239
	movs r0, #0
	movs r1, #244
	lsls r2, r2, #1
	bl 0x0200c9ec
	movs r1, #130
	movs r2, #245
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200c9f4
	movs r0, #0
	bl 0x0200c9fc
	movs r0, #0
	movs r1, #1
	bl 0x0200ca0c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #176
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca2c
	movs r1, #4
	movs r0, #1
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r0, #1
	bl 0x0200c93c
	ldr r4, [pc, #640]
	movs r5, #0
	adds r0, #216
	movs r1, #14
.L_02001a00_2:
	ldrh r3, [r0]
	adds r2, r4, #0
	ands r2, r3
	adds r3, r2, #0
	subs r3, #220
	adds r0, #2
	cmp r3, #1
	bls .L_02001a00_0
	cmp r2, #223
	bne .L_02001a00_1
.L_02001a00_0:
	adds r5, #1
.L_02001a00_1:
	subs r1, #1
	cmp r1, #0
	bge .L_02001a00_2
	movs r1, #0
	movs r0, #1
	bl 0x0200ca5c
	movs r0, #0
	movs r1, #0
	bl 0x0200c9b4
	cmp r0, #0
	bne .L_02001a00_3
	ldr r6, [pc, #588]
	adds r0, r6, #0
	bl 0x0200ca54
	movs r0, #1
	movs r1, #3
	bl 0x0200ca14
	movs r0, #10
	bl 0x0200c994
	cmp r5, #2
	bgt .L_02001a00_4
	movs r0, #1
	movs r1, #30
	bl 0x0200c248
	movs r2, #243
	lsls r2, r2, #1
	movs r0, #1
	movs r1, #252
	bl 0x0200c9f4
	movs r1, #2
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	adds r0, r6, #1
	movs r1, #1
	movs r2, #0
	bl 0x0200c92c
	b .L_02001a00_5
.L_02001a00_4:
	ldr r0, [pc, #520]
	bl 0x0200ca54
	movs r0, #1
	movs r1, #30
	bl 0x0200c248
	b .L_02001a00_5
.L_02001a00_3:
	cmp r5, #2
	bgt .L_02001a00_6
	ldr r6, [pc, #504]
	adds r0, r6, #0
	bl 0x0200ca54
	movs r0, #1
	movs r1, #3
	bl 0x0200ca2c
	movs r0, #1
	movs r1, #4
	bl 0x0200ca14
	movs r0, #1
	movs r1, #10
	bl 0x0200c248
	movs r0, #1
	movs r1, #4
	bl 0x0200ca14
	movs r0, #1
	movs r1, #1
	bl 0x0200ca2c
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #1
	bl 0x0200c9c4
	movs r0, #0
	bl 0x0200c9bc
	adds r7, r0, #0
	adds r5, r7, #0
	adds r5, #90
	ldrb r2, [r5]
	movs r3, #254
	ands r3, r2
	movs r2, #239
	strb r3, [r5]
	movs r0, #1
	movs r1, #244
	lsls r2, r2, #1
	bl 0x0200c9f4
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c9c4
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200ca1c
	movs r1, #218
	ldr r2, [pc, #388]
	movs r0, #0
	bl 0x0200c9dc
	adds r6, #1
	movs r0, #0
	bl 0x0200c9fc
	movs r2, #0
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200c92c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca2c
	movs r2, #30
	movs r0, #0
	movs r1, #0
	bl 0x0200ca7c
	ldrb r2, [r5]
	movs r3, #1
	orrs r3, r2
	strb r3, [r5]
	b .L_02001a00_5
.L_02001a00_6:
	ldr r0, [pc, #340]
	bl 0x0200ca54
	movs r0, #1
	movs r1, #3
	bl 0x0200ca2c
	movs r0, #1
	movs r1, #4
	bl 0x0200ca14
	movs r0, #1
	movs r1, #10
	bl 0x0200c248
	movs r0, #1
	movs r1, #4
	bl 0x0200ca14
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200ca7c
.L_02001a00_5:
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200caa4
	movs r0, #1
	movs r1, #1
	bl 0x0200ca9c
	bl 0x0200cab4
	movs r1, #128
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #1
	movs r1, #2
	bl 0x0200ca2c
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #1
	bl 0x0200c9c4
	movs r0, #1
	bl 0x0200c9bc
	adds r7, r0, #0
	adds r5, r7, #0
	adds r5, #90
	ldrb r2, [r5]
	movs r3, #254
	ands r3, r2
	movs r1, #132
	movs r2, #241
	strb r3, [r5]
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200c9f4
	ldrb r2, [r5]
	movs r3, #1
	orrs r3, r2
	movs r1, #139
	movs r2, #240
	lsls r2, r2, #1
	strb r3, [r5]
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200c9f4
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #10
	movs r5, #192
	str r3, [r7, #52]
	lsls r5, r5, #11
	movs r0, #153
	bl 0x0200cb14
	str r5, [r7, #40]
	movs r0, #1
	movs r1, #7
	bl 0x0200ca0c
	movs r1, #156
	movs r2, #235
	lsls r2, r2, #1
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200c9e4
	movs r1, #1
	movs r0, #1
	bl 0x0200ca0c
	movs r0, #30
	bl 0x0200c994
	movs r0, #153
	bl 0x0200cb14
	str r5, [r7, #40]
	movs r0, #1
	movs r1, #7
	bl 0x0200ca0c
	movs r1, #171
	movs r2, #235
	lsls r2, r2, #1
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200c9e4
	movs r1, #1
	movs r0, #1
	bl 0x0200ca0c
	movs r0, #30
	bl 0x0200c994
	movs r0, #153
	bl 0x0200cb14
	str r5, [r7, #40]
	movs r0, #1
	movs r1, #7
	bl 0x0200ca0c
	movs r1, #188
	movs r2, #235
	movs r0, #1
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl 0x0200c9e4
	movs r0, #1
	movs r1, #1
	bl 0x0200ca0c
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0105
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0105
	.4byte 0x000001ff
	.4byte 0x000010b0
	.4byte 0x000010b4
	.4byte 0x000010b2
	.4byte 0x000001d7
	.4byte 0x000010b5
	.global Func_02001d04
	.thumb_func
Func_02001d04:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r1, #3
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #1
	bl 0x0200ca7c
	movs r0, #14
	bl 0x0200c9bc
	movs r1, #0
	bl 0x0200c8fc
	movs r0, #14
	movs r1, #15
	bl 0x0200ca44
	movs r1, #196
	movs r2, #227
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200ca04
	bl 0x0200c3bc
	movs r1, #208
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #1
	movs r1, #2
	bl 0x0200ca24
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200ca8c
	movs r1, #160
	movs r2, #10
	movs r0, #14
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r1, #2
	movs r0, #14
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	ldr r6, [pc, #656]
	adds r0, r6, #0
	bl 0x0200ca54
	movs r0, #14
	movs r1, #0
	bl 0x0200ca64
	movs r2, #174
	lsls r2, r2, #17
	ldr r1, [pc, #640]
	ldr r5, [pc, #644]
	movs r0, #10
	bl 0x0200ca04
	movs r0, #20
	bl 0x0200c994
	adds r0, r5, #0
	movs r1, #10
	bl 0x0200c248
	adds r0, r5, #0
	movs r1, #40
	bl 0x0200c248
	movs r2, #174
	lsls r2, r2, #17
	movs r0, #10
	ldr r1, [pc, #612]
	bl 0x0200ca04
	movs r1, #2
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #40
	bl 0x0200c994
	movs r1, #3
	movs r0, #1
	bl 0x0200ca14
	movs r0, #40
	bl 0x0200c994
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c9c4
	movs r2, #234
	movs r0, #1
	ldr r1, [pc, #564]
	lsls r2, r2, #1
	bl 0x0200c9f4
	movs r1, #208
	movs r2, #60
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #1
	movs r1, #20
	bl 0x0200c248
	adds r0, r6, #4
	movs r1, #1
	movs r2, #10
	bl 0x0200c92c
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #1
	bl 0x0200c9c4
	movs r0, #1
	bl 0x0200c9bc
	adds r7, r0, #0
	adds r5, r7, #0
	adds r5, #90
	ldrb r2, [r5]
	movs r3, #254
	ands r3, r2
	movs r2, #0
	mov r8, r2
	movs r1, #188
	movs r2, #235
	strb r3, [r5]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #1
	bl 0x0200c9f4
	movs r0, #30
	bl 0x0200c994
	ldrb r2, [r5]
	movs r3, #1
	orrs r3, r2
	strb r3, [r5]
	movs r1, #4
	movs r0, #14
	bl 0x0200ca14
	adds r6, #5
	movs r0, #10
	bl 0x0200c994
	adds r0, r6, #0
	bl 0x0200ca54
	movs r0, #14
	movs r1, #20
	bl 0x0200c248
	movs r2, #60
	movs r0, #1
	ldr r1, [pc, #428]
	bl 0x0200ca8c
	movs r0, #14
	movs r1, #3
	bl 0x0200ca14
	movs r0, #14
	movs r1, #20
	bl 0x0200c248
	movs r1, #129
	movs r2, #60
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ca8c
	movs r1, #3
	movs r0, #14
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r1, #192
	movs r2, #20
	movs r0, #14
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #14
	bl 0x0200ca44
	movs r0, #14
	bl 0x0200c9bc
.L_02001ec8:
	movs r1, #0
	bl 0x0200c8fc
	movs r0, #14
	bl 0x0200c9bc
	adds r7, r0, #0
	adds r6, r7, #0
	mov r3, r8
	adds r6, #85
	strb r3, [r6]
	movs r0, #220
	bl 0x0200cb14
	movs r5, #0
.L_02001ec8_0:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	adds r5, #1
	bl 0x0200c994
	cmp r5, #30
	bne .L_02001ec8_0
	movs r3, #5
	strb r3, [r6]
	movs r0, #1
	movs r1, #2
	bl 0x0200ca24
	movs r0, #1
	movs r1, #10
	bl 0x0200c248
	movs r0, #14
	ldr r1, [pc, #280]
	movs r2, #60
	bl 0x0200ca8c
	movs r1, #160
	movs r2, #10
	movs r0, #14
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r0, #1
	movs r1, #20
	bl 0x0200c248
	movs r0, #14
	movs r1, #1
	bl 0x0200ca2c
	movs r0, #14
	movs r1, #20
	bl 0x0200c248
	movs r2, #20
	movs r0, #1
	ldr r1, [pc, #236]
	bl 0x0200ca8c
	movs r0, #1
	movs r1, #30
	bl 0x0200c248
	movs r0, #14
	ldr r1, [pc, #224]
	movs r2, #80
	bl 0x0200ca8c
	movs r1, #208
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200ca7c
	bl 0x0200caec
	bl 0x0200caf4
	movs r1, #1
	movs r2, #167
	lsls r2, r2, #17
	movs r3, #0
	negs r1, r1
	ldr r0, [pc, #176]
	bl 0x0200caac
	bl 0x0200c8d4
	movs r0, #1
	bl 0x0200c864
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #20
	bl 0x0200c994
	movs r0, #10
	movs r1, #4
	bl 0x0200ca14
	movs r0, #10
	movs r1, #10
	bl 0x0200c248
	movs r1, #0
	movs r0, #11
	bl 0x0200ca5c
	ldr r0, [pc, #128]
	ldr r1, [pc, #128]
	bl 0x0200caa4
	movs r0, #187
	movs r1, #1
	movs r2, #235
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200caac
	bl 0x0200cab4
	movs r1, #160
	movs r0, #14
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200ca7c
	movs r0, #1
	movs r1, #2
	bl 0x0200ca2c
	movs r0, #1
	movs r1, #0
	bl 0x0200c9b4
	cmp r0, #0
	beq .L_02001ec8_1
	movs r0, #10
	bl 0x0200c994
	movs r1, #4
	movs r0, #14
	bl 0x0200ca14
	ldr r0, [pc, #48]
	b .L_02001ec8_2
	.2byte 0x0000
	.2byte 0x10b6
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x01d5
	.2byte 0x200a
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x01fb
	.2byte 0x0185
	.2byte 0x0000
	.4byte 0x00000101
	.4byte 0x00000103
	.4byte 0x00000105
	.4byte 0x01dd0000
	.4byte 0x00066666
	.4byte 0x0000cccc
	.4byte 0x000010c3
.L_02001ec8_3:
	movs r0, #20
	bl 0x0200c994
	movs r1, #4
	movs r0, #14
	bl 0x0200ca14
	movs r0, #10
	bl 0x0200c994
	ldr r0, [pc, #512]
.L_02001ec8_2:
	bl 0x0200ca54
	movs r1, #0
	movs r0, #14
	bl 0x0200ca5c
	movs r0, #1
	movs r1, #0
	bl 0x0200c9b4
	cmp r0, #0
	beq .L_02001ec8_3
.L_02001ec8_1:
	movs r0, #30
	bl 0x0200c994
	movs r1, #3
	movs r0, #14
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	ldr r0, [pc, #472]
	bl 0x0200ca54
	movs r0, #14
	movs r1, #30
	bl 0x0200c248
	movs r1, #3
	movs r0, #14
	bl 0x0200ca14
	movs r0, #10
	bl 0x0200c994
	movs r0, #14
	movs r1, #30
	bl 0x0200c248
	movs r3, #0
	strb r3, [r6]
	movs r0, #14
	ldr r1, [pc, #432]
	ldr r2, [pc, #436]
	bl 0x0200c9c4
	movs r1, #230
	movs r3, #180
	lsls r3, r3, #17
	movs r2, #0
	adds r0, r7, #0
	lsls r1, r1, #17
	bl 0x0200c8dc
	movs r0, #14
	bl 0x0200c9fc
	movs r1, #0
	movs r0, #14
	bl 0x0200ca44
	movs r0, #14
	bl 0x0200c9bc
	movs r1, #1
	bl 0x0200c8fc
	movs r0, #30
	bl 0x0200c994
	movs r1, #1
	movs r0, #1
	bl 0x0200ca9c
	bl 0x0200cab4
	movs r0, #40
	bl 0x0200c994
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #364]
	bl 0x0200ca8c
	movs r1, #3
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r0, #1
	bl 0x0200c9bc
	adds r7, r0, #0
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #10
	movs r5, #192
	str r3, [r7, #52]
	lsls r5, r5, #11
	movs r0, #153
	bl 0x0200cb14
	movs r0, #1
	movs r1, #7
	str r5, [r7, #40]
	bl 0x0200ca0c
	movs r1, #171
	movs r2, #235
	lsls r2, r2, #1
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200c9e4
	movs r1, #1
	movs r0, #1
	bl 0x0200ca0c
	movs r0, #30
	bl 0x0200c994
	movs r0, #153
	bl 0x0200cb14
	movs r0, #1
	movs r1, #7
	str r5, [r7, #40]
	bl 0x0200ca0c
	movs r1, #156
	movs r2, #235
	lsls r2, r2, #1
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200c9e4
	movs r1, #1
	movs r0, #1
	bl 0x0200ca0c
	movs r0, #30
	bl 0x0200c994
	movs r0, #153
	bl 0x0200cb14
	movs r0, #1
	movs r1, #7
	str r5, [r7, #40]
	bl 0x0200ca0c
	movs r1, #139
	movs r2, #240
	lsls r2, r2, #1
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200c9e4
	movs r1, #1
	movs r0, #1
	bl 0x0200ca0c
	movs r0, #30
	bl 0x0200c994
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200caa4
	movs r0, #0
	movs r1, #1
	bl 0x0200ca9c
	movs r0, #1
	ldr r1, [pc, #156]
	ldr r2, [pc, #160]
	bl 0x0200c9c4
	movs r2, #0
	movs r1, #1
	movs r0, #0
	bl 0x0200ca3c
	movs r0, #30
	bl 0x0200c994
	movs r0, #1
	movs r1, #3
	bl 0x0200ca14
	movs r0, #0
	movs r1, #4
	bl 0x0200ca14
	movs r0, #1
	movs r1, #2
	bl 0x0200ca2c
	movs r1, #3
	movs r0, #0
.L_02002208:
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r0, #1
	movs r1, #2
	bl 0x0200ca0c
	movs r0, #0
	bl 0x0200c9bc
	cmp r0, #0
	beq .L_02002208_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200c9dc
.L_02002208_0:
	movs r0, #1
	bl 0x0200c9fc
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200ca04
	movs r0, #220
	bl 0x0200c98c
	movs r0, #221
	bl 0x0200c98c
	movs r0, #223
	bl 0x0200c98c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x10c6
	.2byte 0x0000
	.2byte 0x10c4
	.2byte 0x0000
	.2byte 0x6666
	.2byte 0x0002
	.2byte 0x3333
	.2byte 0x0001
	.2byte 0x0103
	.2byte 0x0000
	.2byte 0x9999
	.2byte 0x0001
	.2byte 0xcccc
	.2byte 0x0000
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	movs	r0, #0
	bl 0x0200c9bc
	mov	sl, r0
	bl 0x0200c99c
	movs	r0, #5
	movs	r1, #1
	bl 0x0200c9cc
	movs	r0, #9
	movs	r1, #1
	bl 0x0200c9cc
	movs	r0, #11
	movs	r1, #1
	bl 0x0200c9cc
	movs	r0, #10
	movs	r1, #1
	bl 0x0200c9cc
	movs	r0, #14
	movs	r1, #1
	bl 0x0200c9cc
	movs	r0, #13
	movs	r1, #1
	bl 0x0200c9cc
	movs	r2, #166
	movs	r0, #5
	ldr	r1, [pc, #284]
	lsls	r2, r2, #17
	bl 0x0200ca04
	movs	r2, #166
	movs	r0, #9
	ldr	r1, [pc, #276]
	lsls	r2, r2, #17
	bl 0x0200ca04
	movs	r2, #174
	movs	r0, #11
	ldr	r1, [pc, #268]
	lsls	r2, r2, #17
	bl 0x0200ca04
	movs	r2, #174
	movs	r0, #10
	ldr	r1, [pc, #260]
	lsls	r2, r2, #17
	bl 0x0200ca04
	movs	r1, #230
	movs	r2, #180
	movs	r0, #14
	lsls	r1, r1, #17
	lsls	r2, r2, #17
	bl 0x0200ca04
	movs	r2, #153
	ldr	r1, [pc, #240]
	lsls	r2, r2, #17
	movs	r0, #13
	bl 0x0200ca04
	movs	r0, #5
	bl 0x0200c9bc
	mov	r1, sl
	str	r1, [r0, #104]
	adds	r2, r0, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r2, #0]
	ldr	r3, [pc, #212]
	movs	r1, #0
	mov	r8, r3
	mov	r9, r1
	mov	r1, r8
	bl 0x0200c8bc
	movs	r0, #9
	bl 0x0200c9bc
	mov	r1, sl
	str	r1, [r0, #104]
	adds	r2, r0, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	orrs	r3, r6
	strb	r3, [r2, #0]
	mov	r1, r8
	bl 0x0200c8bc
	movs	r0, #11
	bl 0x0200c9bc
	mov	r3, sl
	str	r3, [r0, #104]
	adds	r2, r0, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	orrs	r3, r6
	strb	r3, [r2, #0]
	mov	r1, r8
	bl 0x0200c8bc
	movs	r0, #10
	bl 0x0200c9bc
	mov	r1, sl
	str	r1, [r0, #104]
	adds	r2, r0, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	orrs	r3, r6
	mov	r1, r8
	strb	r3, [r2, #0]
	bl 0x0200c8bc
	movs	r0, #14
	bl 0x0200c9bc
	mov	r3, sl
	adds	r5, r0, #0
	str	r3, [r5, #104]
	adds	r2, r5, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	orrs	r3, r6
	strb	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	movs	r0, #11
	bl 0x0200c9bc
	adds	r0, #85
	ldrb	r3, [r0, #0]
	adds	r2, r5, #0
	adds	r2, #85
	mov	r1, r9
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	str	r1, [r5, #12]
	mov	r1, r8
	bl 0x0200c8bc
	movs	r0, #13
	bl 0x0200c9bc
	mov	r3, sl
	str	r3, [r0, #104]
	adds	r2, r0, #0
	adds	r2, #90
	ldrb	r3, [r2, #0]
	orrs	r6, r3
	strb	r6, [r2, #0]
	mov	r1, r8
	bl 0x0200c8bc
	bl 0x0200c9a4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x01db0000
	.4byte 0x01eb0000
	.4byte 0x01cb0000
	.4byte 0x01fb0000
	.4byte 0x01d70000
	.2byte 0xcbd0
	.2byte 0x0200
	.global Func_02002400
	.thumb_func
Func_02002400:
	push {r5, lr}
	sub sp, #8
	bl 0x0200c99c
	movs r0, #141
	bl 0x0200cb14
	movs r5, #0
.L_02002400_1:
	movs r1, #1
	ldr r0, [pc, #572]
	bl 0x0200cacc
	movs r0, #8
	bl 0x0200cadc
	movs r0, #8
	bl 0x0200c994
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl 0x0200cacc
	movs r0, #8
	bl 0x0200cadc
	movs r0, #8
	bl 0x0200c994
	cmp r5, #1
	bne .L_02002400_0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200c90c
.L_02002400_0:
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	cmp r5, #6
	bne .L_02002400_1
	ldr r0, [pc, #504]
	bl 0x0200cb14
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #496]
	bl 0x0200c90c
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #40
	movs r2, #13
	movs r3, #46
	movs r0, #0
	bl 0x0200c8ec
	movs r0, #20
	bl 0x0200c994
	movs r1, #232
	movs r2, #128
	movs r3, #144
	lsls r3, r3, #16
	lsls r2, r2, #13
	lsls r1, r1, #16
	movs r0, #222
	bl 0x0200c260
	adds r5, r0, #0
	movs r0, #40
	bl 0x0200c994
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200c924
	ldr r0, [pc, #436]
	movs r1, #1
	bl 0x0200c91c
	movs r0, #5
	ldr r1, [pc, #428]
	ldr r2, [pc, #432]
	bl 0x0200ca04
	movs r0, #9
	ldr r1, [pc, #420]
	ldr r2, [pc, #420]
	bl 0x0200ca04
	movs r0, #11
	ldr r1, [pc, #408]
	ldr r2, [pc, #412]
	bl 0x0200ca04
	movs r0, #10
	ldr r1, [pc, #400]
	ldr r2, [pc, #400]
	bl 0x0200ca04
	movs r0, #14
	ldr r1, [pc, #388]
	ldr r2, [pc, #392]
	bl 0x0200ca04
	movs r0, #0
	ldr r1, [pc, #388]
	ldr r2, [pc, #388]
	bl 0x0200c9c4
	movs r0, #0
	movs r1, #232
	movs r2, #156
	bl 0x0200c9f4
	movs r0, #10
	bl 0x0200c994
	movs r0, #0
	bl 0x0200c9bc
	cmp r0, #0
	beq .L_02002400_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200ca04
.L_02002400_2:
	movs r0, #1
	ldr r1, [pc, #340]
	ldr r2, [pc, #344]
	bl 0x0200c9c4
	movs r0, #1
	movs r1, #218
	movs r2, #172
	bl 0x0200c9f4
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200ca3c
	movs r0, #20
	bl 0x0200c994
	movs r0, #145
	bl 0x0200cb14
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200c90c
	movs r0, #20
	bl 0x0200c994
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl 0x0200c90c
	movs r0, #40
	bl 0x0200c994
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #50
	movs r0, #1
	bl 0x0200ca7c
	movs r0, #144
	bl 0x0200cb14
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200c90c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r0, #1
	movs r1, #0
	movs r2, #50
	bl 0x0200ca7c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl 0x0200c90c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #50
	bl 0x0200ca7c
	movs r1, #176
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #1
	bl 0x0200ca7c
	movs r0, #144
	bl 0x0200cb14
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl 0x0200c90c
	movs r0, #30
	bl 0x0200c994
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ca1c
	movs r0, #1
	movs r1, #2
	movs r2, #20
	bl 0x0200ca1c
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200ca1c
	movs r1, #6
	movs r0, #1
	movs r2, #40
	bl 0x0200ca1c
	ldr r3, [pc, #68]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	bl 0x0200caec
	bl 0x0200caf4
	movs r0, #2
	bl 0x0200cabc
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x004039d2
	.4byte 0x00000121
	.4byte 0x0000e666
	.4byte 0x00001078
	.4byte 0x01330000
	.4byte 0x01150000
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x03001ebc
	.global Func_02002674
	.thumb_func
Func_02002674:
	push {lr}
	bl 0x0200c99c
	ldr r0, [pc, #80]
	bl 0x0200c964
	cmp r0, #0
	beq .L_02002674_0
	ldr r0, [pc, #72]
	bl 0x0200ca54
	movs r0, #9
	movs r1, #0
	bl 0x0200ca64
	b .L_02002674_1
.L_02002674_0:
	ldr r0, [pc, #60]
	bl 0x0200c964
	cmp r0, #0
	bne .L_02002674_2
	ldr r0, [pc, #56]
	bl 0x0200ca54
	b .L_02002674_3
.L_02002674_2:
	ldr r0, [pc, #52]
	bl 0x0200ca54
.L_02002674_3:
	movs r1, #0
	movs r0, #9
	movs r2, #0
	bl 0x0200ca3c
	movs r0, #10
	bl 0x0200c994
	movs r0, #9
	movs r1, #0
	bl 0x0200ca64
.L_02002674_1:
	bl 0x0200c9a4
	pop {r0}
	bx r0
	.4byte 0x0000083e
	.4byte 0x000010cb
	.4byte 0x0000083c
	.4byte 0x00001079
	.4byte 0x0000107b
	.global Func_020026e0
	.thumb_func
Func_020026e0:
	push {lr}
	bl 0x0200c99c
	ldr r0, [pc, #80]
	bl 0x0200c964
	cmp r0, #0
	beq .L_020026e0_0
	ldr r0, [pc, #72]
	bl 0x0200ca54
	movs r0, #5
	movs r1, #0
	bl 0x0200ca64
	b .L_020026e0_1
.L_020026e0_0:
	ldr r0, [pc, #60]
	bl 0x0200c964
	cmp r0, #0
	bne .L_020026e0_2
	ldr r0, [pc, #56]
	bl 0x0200ca54
	b .L_020026e0_3
.L_020026e0_2:
	ldr r0, [pc, #52]
	bl 0x0200ca54
.L_020026e0_3:
	movs r1, #0
	movs r0, #5
	movs r2, #0
	bl 0x0200ca3c
	movs r0, #10
	bl 0x0200c994
	movs r0, #5
	movs r1, #0
	bl 0x0200ca64
.L_020026e0_1:
	bl 0x0200c9a4
	pop {r0}
	bx r0
	.4byte 0x0000083e
	.4byte 0x000010c9
	.4byte 0x0000083c
	.4byte 0x0000107a
	.4byte 0x0000107c
	.global Func_0200274c
	.thumb_func
Func_0200274c:
	push {lr}
	bl 0x0200c99c
	ldr r0, [pc, #20]
	bl 0x0200ca54
	movs r0, #10
	movs r1, #0
	bl 0x0200ca64
	bl 0x0200c9a4
	pop {r0}
	bx r0
	.4byte 0x000010ca
	.global Func_0200276c
	.thumb_func
Func_0200276c:
	push {lr}
	bl 0x0200c99c
	ldr r0, [pc, #20]
	bl 0x0200ca54
	movs r0, #11
	movs r1, #0
	bl 0x0200ca64
	bl 0x0200c9a4
.L_02002784:
	pop {r0}
	bx r0
	.2byte 0x10c7
	.2byte 0x0000
	.global Func_0200278c
	.thumb_func
Func_0200278c:
	push {lr}
	bl 0x0200c99c
	ldr r0, [pc, #20]
	bl 0x0200ca54
	movs r0, #13
	movs r1, #0
	bl 0x0200ca64
	bl 0x0200c9a4
	pop {r0}
	bx r0
	.4byte 0x000010c8
	.global Func_020027ac
	.thumb_func
Func_020027ac:
	push {lr}
	bl 0x0200c99c
	ldr r0, [pc, #20]
	bl 0x0200ca54
	movs r0, #14
	movs r1, #0
	bl 0x0200ca64
	bl 0x0200c9a4
	pop {r0}
	bx r0
	.4byte 0x000010cc
	.global Func_020027cc
	.thumb_func
Func_020027cc:
	push {lr}
	bl 0x0200c99c
	ldr r0, [pc, #20]
	bl 0x0200ca54
	movs r0, #9
	movs r1, #10
	bl 0x0200c248
	bl 0x0200c9a4
	pop {r0}
	bx r0
	.4byte 0x00001072
	.global Func_020027ec
	.thumb_func
Func_020027ec:
	push {lr}
	bl 0x0200cac4
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020027f8
	.thumb_func
Func_020027f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	bl 0x0200c99c
	movs r3, #27
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #27
	movs r1, #16
	movs r2, #5
	movs r3, #1
	bl 0x0200c8f4
	movs r1, #1
	ldr r2, [pc, #1004]
	negs r1, r1
	movs r3, #0
	ldr r0, [pc, #1000]
	bl 0x0200caac
	bl 0x0200cab4
	bl 0x0200c8d4
	movs r0, #8
	bl 0x0200c9bc
	ldr r5, [pc, #984]
	mov r8, r0
	str r5, [r0, #24]
	str r5, [r0, #28]
	movs r0, #0
	bl 0x0200c9bc
	adds r7, r0, #0
	ldr r3, [r7, #80]
	movs r6, #0
	adds r3, #38
	movs r1, #128
	strb r6, [r3]
	movs r0, #0
	str r5, [r7, #24]
	str r5, [r7, #28]
	lsls r1, r1, #1
	mov r9, r3
	bl 0x0200ca44
	movs r2, #145
	movs r0, #0
	ldr r1, [pc, #936]
	lsls r2, r2, #17
	bl 0x0200ca04
	movs r2, #85
	movs r3, #160
	adds r2, r2, r7
	lsls r3, r3, #14
	strb r6, [r2]
	str r3, [r7, #12]
	ldr r3, [pc, #924]
	ldr r1, [r3]
	movs r3, #224
	lsls r3, r3, #1
	mov r10, r2
	adds r2, r1, r3
	adds r3, #67
	str r3, [r2]
	subs r3, #59
	adds r2, r1, r3
	movs r3, #32
	str r3, [r2]
	bl 0x0200cae4
	bl 0x0200caf4
	movs r0, #20
	bl 0x0200c994
	movs r2, #145
	ldr r1, [pc, #876]
	lsls r2, r2, #17
	movs r0, #8
	bl 0x0200ca04
	movs r0, #190
	bl 0x0200cb14
	movs r0, #0
	movs r1, #2
	bl 0x0200ca84
	ldr r6, [pc, #864]
	movs r5, #0
.L_020027f8_0:
	ldr r3, [r7, #12]
	ldr r2, [pc, #864]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r7, #24]
	adds r3, r3, r6
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	adds r3, r3, r6
	str r3, [r7, #28]
	mov r2, r8
	ldr r3, [r2, #24]
	adds r3, r3, r6
	str r3, [r2, #24]
	ldr r3, [r2, #28]
	adds r3, r3, r6
	str r3, [r2, #28]
	movs r0, #1
	bl 0x0200c994
	movs r3, #128
	lsls r3, r3, #9
	adds r5, r5, r3
	lsrs r3, r5, #16
	cmp r3, #90
	bne .L_020027f8_0
	movs r3, #5
	mov r2, r10
	strb r3, [r2]
	movs r0, #80
	bl 0x0200c994
	ldr r0, [pc, #804]
	ldr r1, [pc, #808]
	bl 0x0200caa4
	movs r1, #1
	movs r2, #145
	ldr r0, [pc, #772]
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl 0x0200caac
	movs r5, #0
.L_020027f8_1:
	ldr r3, [r7, #12]
	ldr r2, [pc, #784]
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	bl 0x0200c994
	movs r3, #128
	lsls r3, r3, #9
	adds r5, r5, r3
	lsrs r3, r5, #16
	cmp r3, #60
	bne .L_020027f8_1
	movs r3, #3
	mov r2, r10
	strb r3, [r2]
	movs r0, #20
	bl 0x0200c994
	movs r1, #1
	movs r0, #0
	bl 0x0200ca84
	movs r0, #0
	bl 0x0200c9bc
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #0
	bl 0x0200ca44
	movs r3, #1
	mov r2, r9
	strb r3, [r2]
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl 0x0200ca04
	bl 0x0200cab4
	movs r0, #20
	bl 0x0200c994
	movs r0, #0
	movs r1, #1
	bl 0x0200ca9c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c9c4
	movs r2, #155
	lsls r2, r2, #1
	ldr r1, [pc, #668]
	movs r0, #0
	bl 0x0200c9f4
	movs r0, #20
	bl 0x0200c994
	movs r0, #0
	movs r1, #2
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r0, #0
	movs r5, #0
	bl 0x0200c9bc
	cmp r0, #0
	beq .L_020027f8_2
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #5
	bl 0x0200ca04
.L_020027f8_2:
	movs r0, #0
	bl 0x0200c9bc
	cmp r0, #0
	beq .L_020027f8_3
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x0200ca04
.L_020027f8_3:
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c9c4
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c9c4
	movs r2, #151
	movs r0, #5
	ldr r1, [pc, #572]
	lsls r2, r2, #1
	bl 0x0200c9ec
	movs r2, #151
	lsls r2, r2, #1
	movs r0, #1
	ldr r1, [pc, #560]
	bl 0x0200c9f4
	movs r0, #5
	movs r1, #1
	bl 0x0200ca0c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x0200ca1c
	movs r0, #5
	movs r1, #2
	movs r2, #0
	bl 0x0200ca1c
	movs r2, #40
	movs r0, #1
	movs r1, #2
	bl 0x0200ca1c
	movs r0, #0
	movs r1, #3
	bl 0x0200ca24
	movs r0, #5
	movs r1, #3
	bl 0x0200ca24
	movs r1, #3
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #40
	bl 0x0200c994
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200ca94
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200ca94
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200ca94
	movs r0, #60
	bl 0x0200c994
	movs r0, #0
	ldr r1, [pc, #392]
	ldr r2, [pc, #396]
	bl 0x0200c9c4
	movs r0, #5
	ldr r1, [pc, #384]
	ldr r2, [pc, #384]
	bl 0x0200c9c4
	movs r0, #1
	ldr r1, [pc, #372]
	ldr r2, [pc, #376]
	bl 0x0200c9c4
	movs r2, #173
	movs r0, #0
	ldr r1, [pc, #348]
	lsls r2, r2, #1
	bl 0x0200c9ec
	movs r2, #169
	movs r0, #5
	ldr r1, [pc, #356]
	lsls r2, r2, #1
	bl 0x0200c9ec
	movs r2, #169
	lsls r2, r2, #1
	ldr r1, [pc, #348]
	movs r0, #1
	bl 0x0200c9ec
	movs r0, #0
	bl 0x0200c9fc
	movs r1, #1
	movs r0, #0
	bl 0x0200ca0c
	movs r0, #5
	bl 0x0200c9fc
	movs r1, #1
	movs r0, #5
	bl 0x0200ca0c
	movs r0, #1
	bl 0x0200c9fc
	movs r0, #1
	movs r1, #1
	bl 0x0200ca0c
	movs r0, #0
	ldr r1, [pc, #260]
	ldr r2, [pc, #300]
	bl 0x0200c9c4
	movs r0, #5
	ldr r1, [pc, #252]
	ldr r2, [pc, #288]
	bl 0x0200c9c4
	movs r0, #1
	ldr r1, [pc, #240]
	ldr r2, [pc, #280]
	bl 0x0200c9c4
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #60
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #60
	bl 0x0200ca7c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200ca7c
	movs r2, #40
	movs r0, #0
	movs r1, #0
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #3
	bl 0x0200ca14
	movs r1, #2
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r1, #4
	movs r0, #1
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r1, #128
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #3
	bl 0x0200ca14
	movs r1, #2
	movs r0, #5
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r0, #5
	ldr r1, [pc, #104]
	movs r2, #60
	bl 0x0200ca8c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	b .L_020027f8_4
	.2byte 0x0000
	.4byte 0x01050000
	.4byte 0x01d70000
	.4byte 0x00001999
	.4byte 0x03001ebc
	.4byte 0x0000028f
	.4byte 0xffffe667
	.4byte 0x00004ccc
	.4byte 0x00000999
	.4byte 0xffff8000
	.4byte 0x000001d7
	.4byte 0x000001c5
	.4byte 0x000001e9
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x000001af
	.4byte 0x000001ff
	.4byte 0x00002666
	.4byte 0x00000101
.L_020027f8_4:
	lsls r1, r1, #7
	movs r2, #60
	movs r0, #5
	bl 0x0200ca7c
	movs r0, #9
	bl 0x0200c9bc
	adds r7, r0, #0
	ldr r3, [r7, #80]
	adds r3, #38
	strb r5, [r3]
	mov r9, r3
	ldr r3, [pc, #1008]
	mov r2, r8
	movs r1, #128
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r0, #9
	str r3, [r2, #24]
	str r3, [r2, #28]
	lsls r1, r1, #1
	bl 0x0200ca44
	movs r2, #145
	lsls r2, r2, #17
	ldr r1, [pc, #984]
	movs r0, #9
	bl 0x0200ca04
	movs r3, #85
	adds r3, r3, r7
	strb r5, [r3]
	mov r10, r3
	movs r3, #160
	lsls r3, r3, #14
	str r3, [r7, #12]
	movs r0, #1
	bl 0x0200c994
	ldr r0, [pc, #960]
	bl 0x0200ca54
	movs r0, #9
	movs r1, #0
	bl 0x0200ca64
	movs r0, #0
	movs r1, #4
	movs r2, #0
	bl 0x0200ca1c
	movs r0, #5
	movs r1, #4
	movs r2, #0
	bl 0x0200ca1c
	movs r0, #1
	movs r1, #4
	movs r2, #40
	bl 0x0200ca1c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #176
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200caa4
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #860]
.L_02002d0a:
	negs r1, r1
	ldr r2, [pc, #864]
	bl 0x0200caac
	bl 0x0200cab4
	movs r2, #145
	ldr r1, [pc, #844]
	lsls r2, r2, #17
	movs r0, #8
	bl 0x0200ca04
	movs r0, #190
	bl 0x0200cb14
	movs r0, #9
	movs r1, #2
	bl 0x0200ca84
	ldr r6, [pc, #832]
	movs r5, #0
.L_02002d0a_0:
	ldr r3, [r7, #12]
	ldr r2, [pc, #832]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r7, #24]
	adds r3, r3, r6
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	adds r3, r3, r6
	str r3, [r7, #28]
	mov r2, r8
	ldr r3, [r2, #24]
	adds r3, r3, r6
	str r3, [r2, #24]
	ldr r3, [r2, #28]
	adds r3, r3, r6
	str r3, [r2, #28]
	movs r0, #1
	bl 0x0200c994
	movs r3, #128
	lsls r3, r3, #9
	adds r5, r5, r3
	lsrs r3, r5, #16
	cmp r3, #90
	bne .L_02002d0a_0
	movs r3, #5
	mov r2, r10
	strb r3, [r2]
	movs r0, #80
	bl 0x0200c994
	movs r5, #0
.L_02002d0a_1:
	ldr r3, [r7, #12]
	ldr r2, [pc, #768]
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	bl 0x0200c994
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r5, r2
	adds r5, r3, #0
	lsrs r3, r5, #16
	cmp r3, #60
	bne .L_02002d0a_1
	movs r3, #3
	mov r2, r10
	strb r3, [r2]
	movs r0, #30
	bl 0x0200c994
	movs r1, #1
	movs r0, #9
	bl 0x0200ca84
	movs r0, #9
	bl 0x0200c9bc
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #9
	bl 0x0200ca44
	movs r3, #1
	mov r2, r9
	strb r3, [r2]
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl 0x0200ca04
	movs r0, #30
	bl 0x0200c994
	movs r0, #9
	ldr r1, [pc, #680]
	ldr r2, [pc, #680]
	bl 0x0200c9c4
	movs r2, #153
	ldr r1, [pc, #676]
	lsls r2, r2, #1
	movs r0, #9
	bl 0x0200c9f4
	movs r0, #20
	bl 0x0200c994
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r2, #80
	movs r0, #9
	movs r1, #2
	bl 0x0200ca1c
	movs r1, #3
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #30
	bl 0x0200c994
	movs r1, #128
	movs r2, #30
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r2, #30
	movs r0, #9
	movs r1, #0
	bl 0x0200ca7c
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r1, #128
	movs r2, #30
	movs r0, #9
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r1, #3
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #30
	bl 0x0200c994
	movs r0, #9
	movs r1, #20
	bl 0x0200c248
	movs r0, #9
	ldr r1, [pc, #544]
	ldr r2, [pc, #528]
	bl 0x0200c9c4
	movs r2, #153
	movs r0, #9
	ldr r1, [pc, #536]
	lsls r2, r2, #1
	bl 0x0200c9f4
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #176
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #176
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #30
	bl 0x0200c994
	movs r1, #3
	movs r0, #9
	bl 0x0200ca14
	movs r0, #40
	bl 0x0200c994
	movs r2, #153
	movs r0, #9
	ldr r1, [pc, #452]
	lsls r2, r2, #1
	bl 0x0200c9f4
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #208
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #30
	bl 0x0200c994
	ldr r5, [pc, #384]
	movs r1, #4
	movs r0, #9
	bl 0x0200ca14
	movs r0, #40
	bl 0x0200c994
	adds r0, r5, #0
	movs r1, #30
	bl 0x0200c248
	movs r2, #0
	movs r1, #5
	movs r0, #0
	bl 0x0200ca3c
	movs r0, #40
	bl 0x0200c994
	movs r0, #0
	movs r1, #2
	bl 0x0200ca24
	movs r0, #5
	movs r1, #2
	bl 0x0200ca2c
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #328]
	bl 0x0200ca8c
	movs r0, #1
	movs r1, #40
	bl 0x0200c248
	movs r1, #160
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #7
	bl 0x0200ca7c
	adds r0, r5, #0
	movs r1, #20
	bl 0x0200c248
	movs r1, #208
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200ca7c
	movs r0, #0
	ldr r1, [pc, #268]
	movs r2, #0
	bl 0x0200ca8c
	movs r2, #40
	movs r0, #5
	ldr r1, [pc, #256]
	bl 0x0200ca8c
	movs r0, #9
	movs r1, #4
	bl 0x0200ca14
	adds r0, r5, #0
	movs r1, #10
	bl 0x0200c248
	movs r0, #1
	movs r1, #3
	bl 0x0200ca0c
	movs r0, #5
	movs r1, #3
	bl 0x0200ca0c
	movs r1, #3
	movs r0, #0
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r1, #3
	movs r0, #9
	bl 0x0200ca14
	movs r0, #40
	bl 0x0200c994
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x0200caa4
	movs r1, #1
	movs r3, #1
	ldr r0, [pc, #180]
	negs r1, r1
	ldr r2, [pc, #180]
	bl 0x0200caac
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x0200c9c4
	movs r0, #9
	ldr r1, [pc, #160]
	ldr r2, [pc, #164]
	bl 0x0200c9f4
	bl 0x0200cab4
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r0, #1
	movs r1, #0
	movs r2, #20
	bl 0x0200ca7c
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #0
	movs r2, #40
	bl 0x0200ca7c
	movs r1, #192
	movs r2, #60
	movs r0, #9
	lsls r1, r1, #6
	bl 0x0200ca7c
	movs r1, #3
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #30
	bl 0x0200c994
	movs r0, #9
	movs r1, #1
	b .L_02002d0a_2
	.2byte 0x1999
	.2byte 0x0000
	.4byte 0x01d70000
	.2byte 0x103c
	.2byte 0x0000
	.4byte 0x01350000
	.4byte 0x0000028f
	.4byte 0xffffe667
	.4byte 0xffff8000
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x000001d7
	.4byte 0x00026666
	.4byte 0x000001a7
	.4byte 0x00000207
	.4byte 0x00004009
	.4byte 0x00000101
	.4byte 0x02150000
	.4byte 0x01530000
	.4byte 0x00000215
	.4byte 0x00000153
.L_02002d0a_2:
	bl 0x0200ca9c
	movs r0, #9
	ldr r1, [pc, #1008]
	ldr r2, [pc, #1008]
	bl 0x0200c9c4
	movs r2, #180
	ldr r1, [pc, #1004]
	lsls r2, r2, #1
	movs r0, #9
	bl 0x0200c9ec
	movs r0, #20
	bl 0x0200c994
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r2, #180
	ldr r1, [pc, #952]
	lsls r2, r2, #1
	movs r0, #9
	bl 0x0200c9f4
	movs r0, #20
	bl 0x0200c994
	movs r1, #160
	movs r2, #30
	movs r0, #9
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r1, #3
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #30
	bl 0x0200c994
	movs r2, #180
	ldr r1, [pc, #912]
	lsls r2, r2, #1
	movs r0, #9
	bl 0x0200c9f4
	movs r0, #20
	bl 0x0200c994
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #30
	bl 0x0200ca7c
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200ca8c
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #30
	bl 0x0200c994
	movs r0, #9
	movs r1, #30
	bl 0x0200c248
	movs r1, #3
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r0, #9
	movs r1, #30
	bl 0x0200c248
	movs r0, #5
	ldr r1, [pc, #816]
	ldr r2, [pc, #828]
	bl 0x0200c9c4
	movs r1, #220
	movs r2, #173
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #5
	bl 0x0200c9f4
	movs r0, #10
	bl 0x0200c994
	movs r1, #128
	movs r2, #10
	movs r0, #5
	lsls r1, r1, #6
	bl 0x0200ca7c
	movs r0, #5
	movs r1, #20
	bl 0x0200c248
	movs r0, #1
	ldr r1, [pc, #768]
	ldr r2, [pc, #776]
	bl 0x0200c9c4
	movs r2, #173
	ldr r1, [pc, #772]
	lsls r2, r2, #1
	movs r0, #1
	bl 0x0200c9f4
	bl 0x0200cab4
	movs r0, #10
	bl 0x0200c994
	movs r1, #192
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r1, #0
	movs r0, #1
	bl 0x0200ca5c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #0
	bl 0x0200c9b4
	cmp r0, #1
	bne .L_02002d0a_3
	ldr r3, [pc, #704]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02002d0a_3:
	movs r1, #192
	movs r2, #30
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #30
	movs r0, #9
	bl 0x0200c248
	ldr r0, [pc, #672]
	bl 0x0200ca54
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #2
	bl 0x0200ca2c
	movs r0, #9
	movs r1, #10
	bl 0x0200c248
	movs r0, #9
	movs r1, #3
	bl 0x0200ca14
	movs r0, #9
	movs r1, #20
	bl 0x0200c248
	movs r0, #9
	movs r1, #2
	bl 0x0200ca2c
	movs r0, #9
	movs r1, #10
	bl 0x0200c248
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200ca94
	movs r0, #40
	bl 0x0200c994
	movs r0, #9
	movs r1, #10
	bl 0x0200c248
	movs r1, #128
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #6
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca24
	movs r0, #5
	movs r1, #2
	bl 0x0200ca24
	movs r1, #2
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #6
	movs r0, #1
	bl 0x0200ca7c
	movs r0, #9
	bl 0x0200c9d4
	movs r0, #0
	bl 0x0200c9d4
	movs r0, #5
	bl 0x0200c9d4
	movs r0, #1
	bl 0x0200c9d4
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #12
	lsls r1, r1, #9
	bl 0x0200caa4
	movs r1, #1
	movs r2, #232
	movs r3, #1
	ldr r0, [pc, #452]
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200caac
	bl 0x0200cab4
	ldr r2, [pc, #444]
	ldr r1, [pc, #444]
	movs r0, #9
	bl 0x0200ca04
	movs r0, #40
	bl 0x0200c994
	ldr r0, [pc, #436]
	movs r1, #0
	bl 0x0200ca64
	movs r2, #180
	ldr r1, [pc, #428]
	lsls r2, r2, #17
	movs r0, #9
	bl 0x0200ca04
	movs r0, #80
	bl 0x0200c994
	movs r1, #1
	movs r2, #185
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	ldr r0, [pc, #404]
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #10
	bl 0x0200c994
	movs r1, #3
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #224
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200ca7c
	movs r1, #1
	movs r2, #147
	movs r3, #1
	ldr r0, [pc, #304]
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200caac
	bl 0x0200cab4
	movs r1, #149
	movs r2, #238
	lsls r2, r2, #16
	lsls r1, r1, #18
	movs r0, #9
	bl 0x0200ca04
	movs r0, #40
	bl 0x0200c994
	ldr r0, [pc, #280]
	movs r1, #0
	bl 0x0200ca64
	movs r2, #180
	ldr r1, [pc, #276]
	lsls r2, r2, #17
	movs r0, #9
	bl 0x0200ca04
	movs r0, #80
	bl 0x0200c994
	movs r1, #1
	movs r2, #185
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	ldr r0, [pc, #248]
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #10
	bl 0x0200c994
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl 0x0200ca1c
	movs r1, #176
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #1
	bl 0x0200ca24
	movs r0, #5
	movs r1, #1
	bl 0x0200ca24
	movs r0, #1
	movs r1, #1
	bl 0x0200ca2c
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200ca7c
	movs r0, #231
	movs r1, #1
	movs r2, #147
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200caac
	bl 0x0200cab4
	movs r1, #154
	movs r2, #250
	lsls r2, r2, #16
	lsls r1, r1, #17
	movs r0, #9
	bl 0x0200ca04
	movs r0, #40
	bl 0x0200c994
	ldr r0, [pc, #100]
	movs r1, #0
	bl 0x0200ca64
	movs r2, #180
	ldr r1, [pc, #84]
	lsls r2, r2, #17
	movs r0, #9
	bl 0x0200ca04
	movs r0, #80
	bl 0x0200c994
	movs r1, #1
	movs r2, #185
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	ldr r0, [pc, #60]
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #10
	b .L_02002d0a_4
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x000001c7
	.4byte 0x000001d7
	.4byte 0x00006666
	.4byte 0x000001ef
	.4byte 0x03001ebc
	.4byte 0x00001048
	.4byte 0x02c70000
	.4byte 0x01610000
	.4byte 0x024d0000
	.4byte 0x00001009
	.4byte 0x01d70000
	.4byte 0x00002009
.L_02002d0a_4:
	bl 0x0200c994
	movs r1, #3
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #10
	bl 0x0200c994
	movs r0, #9
	movs r1, #6
	movs r2, #0
	bl 0x0200ca1c
	movs r1, #160
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca24
	movs r0, #5
	movs r1, #2
	bl 0x0200ca24
	movs r0, #1
	movs r1, #2
	bl 0x0200ca2c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200ca7c
	movs r0, #231
	movs r1, #1
	movs r2, #232
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl 0x0200caac
	bl 0x0200cab4
	movs r1, #153
	movs r2, #181
	lsls r2, r2, #17
	lsls r1, r1, #17
	movs r0, #9
	bl 0x0200ca04
	movs r0, #40
	bl 0x0200c994
	ldr r0, [pc, #1004]
	movs r1, #0
	bl 0x0200ca64
	movs r2, #180
	ldr r1, [pc, #996]
	lsls r2, r2, #17
	movs r0, #9
	bl 0x0200ca04
	movs r0, #80
	bl 0x0200c994
	movs r1, #1
	movs r2, #185
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	ldr r0, [pc, #972]
	bl 0x0200caac
	bl 0x0200cab4
	movs r0, #30
	bl 0x0200c994
	movs r1, #130
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200ca8c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200ca7c
	movs r1, #129
	movs r2, #40
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200ca8c
	movs r0, #5
	movs r1, #1
	bl 0x0200ca2c
	movs r0, #5
	movs r1, #20
	bl 0x0200c248
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl 0x0200ca1c
	movs r1, #176
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #2
	bl 0x0200ca2c
	ldr r0, [pc, #852]
	movs r1, #10
	bl 0x0200c248
	movs r0, #9
	movs r1, #3
	bl 0x0200ca24
	ldr r0, [pc, #836]
	movs r1, #20
	bl 0x0200c248
	movs r0, #0
	ldr r1, [pc, #832]
	movs r2, #0
	bl 0x0200ca8c
	movs r0, #5
	ldr r1, [pc, #820]
	movs r2, #0
	bl 0x0200ca8c
	movs r0, #1
	ldr r1, [pc, #812]
	movs r2, #60
	bl 0x0200ca8c
	movs r1, #192
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #4
	bl 0x0200ca0c
	movs r1, #0
	ldr r0, [pc, #784]
	bl 0x0200ca5c
	movs r0, #0
	movs r1, #0
	bl 0x0200c9b4
	cmp r0, #1
	bne .L_02002d0a_5
	ldr r3, [pc, #772]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02002d0a_5:
	movs r0, #20
	bl 0x0200c994
	movs r1, #128
	movs r2, #10
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #10
	movs r0, #1
	bl 0x0200c248
	ldr r0, [pc, #732]
	bl 0x0200ca54
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #4
	movs r2, #40
	bl 0x0200ca1c
	ldr r2, [pc, #696]
	mov r11, r2
	mov r0, r11
	movs r1, #10
	bl 0x0200c248
	movs r0, #0
	ldr r1, [pc, #680]
	movs r2, #0
	bl 0x0200ca8c
	movs r0, #5
	ldr r1, [pc, #668]
	movs r2, #0
	bl 0x0200ca8c
	movs r2, #80
	movs r0, #1
	ldr r1, [pc, #656]
	bl 0x0200ca8c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200ca94
	movs r0, #40
	bl 0x0200c994
	mov r0, r11
	movs r1, #40
	bl 0x0200c248
	movs r1, #131
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r1, #131
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r1, #131
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200ca8c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200ca7c
	movs r1, #0
	mov r0, r11
	bl 0x0200ca5c
	movs r0, #0
	movs r1, #0
	bl 0x0200c9b4
	cmp r0, #1
	bne .L_02002d0a_6
	ldr r3, [pc, #556]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02002d0a_6:
	movs r1, #1
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200ca7c
	mov r0, r11
	movs r1, #40
	bl 0x0200c248
	ldr r0, [pc, #504]
	bl 0x0200ca54
	movs r0, #5
	movs r1, #4
	bl 0x0200ca0c
	movs r0, #5
	movs r1, #10
	bl 0x0200c248
	movs r1, #176
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #1
	bl 0x0200ca2c
	movs r2, #40
	movs r0, #9
	movs r1, #4
	bl 0x0200ca1c
	ldr r0, [pc, #432]
	movs r1, #10
	bl 0x0200c248
	movs r0, #0
	movs r1, #1
	bl 0x0200ca24
	movs r0, #5
	movs r1, #1
	bl 0x0200ca24
	movs r1, #1
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #40
	bl 0x0200c994
	movs r1, #192
	movs r2, #10
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #1
	bl 0x0200ca2c
	mov r0, r11
	movs r1, #40
	bl 0x0200c248
	movs r0, #0
	ldr r1, [pc, #388]
	movs r2, #0
	bl 0x0200ca8c
	movs r0, #5
	ldr r1, [pc, #376]
	movs r2, #0
	bl 0x0200ca8c
	movs r0, #1
	ldr r1, [pc, #368]
	movs r2, #120
	bl 0x0200ca8c
	movs r0, #1
	ldr r1, [pc, #360]
	movs r2, #0
	bl 0x0200ca8c
	movs r2, #40
	movs r0, #1
	movs r1, #4
	bl 0x0200ca1c
	movs r0, #1
	movs r1, #10
	bl 0x0200c248
	movs r1, #1
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #40
	bl 0x0200c994
	movs r1, #128
	movs r2, #80
	movs r0, #9
	lsls r1, r1, #7
	bl 0x0200ca7c
	mov r0, r11
	movs r1, #10
	bl 0x0200c248
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r2, #30
	movs r0, #0
	movs r1, #0
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca24
	movs r1, #2
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r2, #30
	movs r0, #0
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca24
	movs r1, #2
	movs r0, #5
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200ca94
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl 0x0200ca94
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	bl 0x0200ca94
	movs r0, #80
	bl 0x0200c994
	movs r1, #3
	movs r0, #9
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r1, #40
	mov r0, r11
	bl 0x0200c248
	movs r0, #9
	bl 0x0200c9bc
	adds r7, r0, #0
	movs r0, #6
	bl 0x0200c864
	movs r2, #192
	movs r3, #128
	lsls r3, r3, #10
	lsls r2, r2, #10
	movs r6, #192
	str r3, [r7, #52]
	str r2, [r7, #48]
	lsls r6, r6, #11
	movs r0, #153
	mov r8, r3
	mov r10, r2
	bl 0x0200cb14
	ldr r1, [pc, #84]
	ldr r2, [pc, #84]
	str r6, [r7, #40]
	movs r0, #9
	bl 0x0200c9e4
	movs r0, #6
	bl 0x0200c864
	movs r0, #9
	ldr r1, [pc, #72]
	ldr r2, [pc, #72]
	bl 0x0200c9c4
	movs r2, #90
	adds r2, r2, r7
	mov r9, r2
	movs r5, #254
	ldrb r2, [r2]
	adds r3, r5, #0
	ands r3, r2
	mov r2, r9
	b .L_02002d0a_7
	.4byte 0x00002009
	.4byte 0x01d70000
	.4byte 0x0000a009
	.4byte 0x00000101
	.4byte 0x00008009
	.4byte 0x03001ebc
	.4byte 0x00001056
	.4byte 0x0000105b
	.4byte 0x00000105
	.4byte 0x00000107
	.4byte 0x000001d7
	.4byte 0x0000018b
	.4byte 0x00004ccc
	.4byte 0x00002666
.L_02002d0a_7:
	strb r3, [r2]
	ldr r1, [pc, #676]
	ldr r2, [pc, #676]
	movs r0, #9
	bl 0x0200c9dc
	movs r0, #9
	bl 0x0200c9fc
	movs r0, #9
	movs r1, #2
	bl 0x0200ca2c
	ldr r2, [pc, #656]
	ldr r1, [pc, #656]
	movs r0, #9
	bl 0x0200c9dc
	movs r0, #9
	bl 0x0200c9fc
	movs r0, #9
	movs r1, #2
	bl 0x0200ca2c
	ldr r2, [pc, #632]
	ldr r1, [pc, #636]
	movs r0, #9
	bl 0x0200c9dc
	movs r0, #9
	bl 0x0200c9fc
	movs r0, #9
	movs r1, #10
	bl 0x0200c248
	movs r0, #9
	ldr r1, [pc, #616]
	ldr r2, [pc, #620]
	bl 0x0200c9c4
	movs r0, #9
	ldr r1, [pc, #604]
	ldr r2, [pc, #612]
	bl 0x0200c9f4
	movs r0, #9
	ldr r1, [pc, #608]
	ldr r2, [pc, #612]
	bl 0x0200c9c4
	mov r2, r9
	ldrb r3, [r2]
	movs r1, #237
	ands r5, r3
	strb r5, [r2]
	lsls r1, r1, #1
	ldr r2, [pc, #584]
	movs r0, #9
	bl 0x0200c9dc
	movs r0, #9
	bl 0x0200c9fc
	movs r0, #9
	movs r1, #3
	bl 0x0200ca2c
	movs r1, #234
	ldr r2, [pc, #560]
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200c9dc
	movs r0, #9
	bl 0x0200c9fc
	movs r0, #9
	movs r1, #3
	bl 0x0200ca2c
	ldr r2, [pc, #536]
	ldr r1, [pc, #524]
	movs r0, #9
	bl 0x0200c9dc
	movs r0, #9
	bl 0x0200c9fc
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200ca94
	movs r0, #9
	movs r1, #3
	bl 0x0200ca24
	movs r0, #9
	movs r1, #10
	bl 0x0200c248
	movs r0, #9
	ldr r1, [pc, #504]
	ldr r2, [pc, #508]
	bl 0x0200c9c4
	ldr r2, [pc, #464]
	movs r0, #9
	ldr r1, [pc, #468]
	bl 0x0200c9f4
	movs r1, #1
	movs r0, #9
	bl 0x0200c9cc
	movs r0, #30
	bl 0x0200c994
	mov r3, r9
	ldrb r2, [r3]
	movs r3, #1
	orrs r3, r2
	movs r1, #192
	mov r2, r9
	strb r3, [r2]
	lsls r1, r1, #8
	movs r2, #60
	movs r0, #9
	bl 0x0200ca7c
	movs r0, #6
	bl 0x0200c864
	mov r3, r10
	mov r2, r8
	str r3, [r7, #48]
	str r2, [r7, #52]
	movs r0, #153
	bl 0x0200cb14
	movs r2, #180
	ldr r1, [pc, #404]
	lsls r2, r2, #1
	str r6, [r7, #40]
	movs r0, #9
	bl 0x0200c9e4
	movs r0, #6
	bl 0x0200c864
	movs r0, #40
	bl 0x0200c994
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #80
	bl 0x0200ca8c
	movs r1, #129
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200ca8c
	movs r1, #129
	movs r2, #80
	movs r0, #1
	lsls r1, r1, #1
	bl 0x0200ca8c
	movs r0, #5
	movs r1, #1
	bl 0x0200ca2c
	movs r2, #40
	movs r0, #5
	ldr r1, [pc, #352]
	bl 0x0200ca8c
	movs r0, #5
	movs r1, #20
	bl 0x0200c248
	movs r1, #176
	movs r2, #40
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #4
	bl 0x0200ca14
	ldr r0, [pc, #320]
	movs r1, #30
	bl 0x0200c248
	movs r0, #5
	movs r1, #0
	movs r2, #30
	bl 0x0200ca7c
	movs r1, #131
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200ca8c
	movs r1, #128
	movs r2, #30
	movs r0, #5
	lsls r1, r1, #6
	bl 0x0200ca7c
	movs r0, #5
	movs r1, #20
	bl 0x0200c248
	movs r0, #0
	movs r1, #4
	movs r2, #0
	bl 0x0200ca1c
	movs r0, #1
	movs r1, #4
	movs r2, #0
	bl 0x0200ca1c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r2, #40
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #1
	bl 0x0200ca24
	movs r1, #1
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #40
	bl 0x0200c994
	movs r2, #30
	movs r0, #0
	movs r1, #0
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca24
	movs r1, #2
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #30
	bl 0x0200c994
	movs r1, #208
	movs r2, #30
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #1
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	mov r0, r11
	movs r1, #20
	bl 0x0200c248
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r0, #0
	movs r1, #2
	bl 0x0200ca24
	movs r1, #2
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	movs r1, #3
	movs r0, #5
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r2, #30
	movs r0, #5
	movs r1, #0
	bl 0x0200ca7c
	movs r1, #0
	movs r0, #5
	bl 0x0200ca5c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200ca7c
	b .L_02002d0a_8
	.2byte 0x0000
	.4byte 0x000001d9
	.4byte 0x0000018b
	.4byte 0x000001d5
	.4byte 0x000001d7
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0000019b
	.4byte 0x00004ccc
	.4byte 0x00002666
	.4byte 0x00003333
	.4byte 0x00001999
	.4byte 0x00000107
	.4byte 0x0000a009
.L_02002d0a_9:
	ldr r0, [pc, #640]
	bl 0x0200ca54
	movs r0, #5
	ldr r1, [pc, #636]
	movs r2, #0
	bl 0x0200ca8c
	movs r0, #5
	movs r1, #4
	movs r2, #60
	bl 0x0200ca1c
	movs r0, #5
	movs r1, #0
	bl 0x0200ca5c
.L_02002d0a_8:
	movs r0, #0
	movs r1, #0
	bl 0x0200c9b4
	cmp r0, #0
	bne .L_02002d0a_9
	ldr r0, [pc, #604]
	bl 0x0200ca54
	movs r0, #20
	bl 0x0200c994
	movs r1, #3
	movs r0, #5
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r1, #128
	movs r2, #30
	movs r0, #5
	lsls r1, r1, #6
	bl 0x0200ca7c
	movs r0, #5
	movs r1, #10
	bl 0x0200c248
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200ca7c
	movs r1, #176
	movs r2, #30
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #3
	movs r0, #9
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200ca7c
	movs r0, #9
	ldr r1, [pc, #508]
	ldr r2, [pc, #512]
	bl 0x0200c9c4
	movs r2, #176
	lsls r2, r2, #1
	ldr r1, [pc, #504]
	movs r0, #9
	bl 0x0200c9f4
	movs r0, #20
	bl 0x0200c994
	ldr r5, [pc, #496]
	movs r1, #2
	movs r0, #9
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	adds r0, r5, #0
	movs r1, #60
	bl 0x0200c248
	mov r3, r9
	ldrb r2, [r3]
	movs r3, #254
	ands r3, r2
	mov r2, r9
	strb r3, [r2]
	movs r1, #228
	movs r2, #180
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #9
	bl 0x0200c9f4
	movs r0, #20
	bl 0x0200c994
	movs r1, #2
	movs r0, #5
	bl 0x0200ca2c
	movs r0, #20
	bl 0x0200c994
	ldr r0, [pc, #428]
	bl 0x0200ca54
	movs r0, #5
	movs r1, #30
	bl 0x0200c248
	movs r1, #176
	movs r2, #30
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #3
	movs r0, #9
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r2, #60
	movs r0, #1
	ldr r1, [pc, #388]
	bl 0x0200ca8c
	movs r0, #1
	movs r1, #10
	bl 0x0200c248
	movs r1, #208
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r0, #9
	movs r1, #3
	bl 0x0200ca14
	adds r0, r5, #0
	movs r1, #20
	bl 0x0200c248
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200ca7c
	movs r0, #0
	ldr r1, [pc, #324]
	movs r2, #0
	bl 0x0200ca8c
	movs r2, #40
	movs r0, #1
	ldr r1, [pc, #312]
	bl 0x0200ca8c
	movs r0, #9
	movs r1, #2
	bl 0x0200ca2c
	adds r0, r5, #0
	movs r1, #10
	bl 0x0200c248
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #160
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #7
	bl 0x0200ca7c
	movs r1, #3
	movs r0, #9
	bl 0x0200ca14
	movs r0, #10
	bl 0x0200c994
	adds r0, r5, #0
	movs r1, #30
	bl 0x0200c248
	movs r0, #1
	movs r1, #3
	bl 0x0200ca0c
	movs r1, #3
	movs r0, #0
	bl 0x0200ca14
	movs r0, #20
	bl 0x0200c994
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x0200ca7c
	movs r1, #128
	movs r2, #30
	movs r0, #1
	lsls r1, r1, #8
	bl 0x0200ca7c
	movs r1, #2
	movs r0, #1
	bl 0x0200ca2c
	movs r0, #30
	bl 0x0200c994
	movs r1, #3
	movs r0, #0
	bl 0x0200ca14
	movs r0, #10
	bl 0x0200c994
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200c9c4
	movs r0, #1
	movs r1, #2
	bl 0x0200ca0c
	movs r0, #0
	bl 0x0200c9bc
	cmp r0, #0
	beq .L_02002d0a_10
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200c9dc
.L_02002d0a_10:
	movs r0, #1
	bl 0x0200c9fc
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x0200ca04
	ldr r0, [pc, #112]
	bl 0x0200c96c
	movs r0, #5
	bl 0x0200c9ac
	bl 0x0200c328
	movs r3, #27
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r0, #8
	movs r2, #5
	movs r3, #1
	bl 0x0200c8f4
	ldr r3, [pc, #80]
	movs r2, #228
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	ldr r0, [pc, #72]
	bl 0x0200c974
	bl 0x0200c9a4
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001068
	.4byte 0x00000107
	.4byte 0x00001069
	.4byte 0x00003333
	.4byte 0x00001999
	.4byte 0x000001d7
	.4byte 0x00008009
	.4byte 0x0000106d
	.4byte 0x00000101
	.4byte 0x0000083b
	.4byte 0x03001ebc
	.4byte 0x0000012f
	.global Func_02003f24
	.thumb_func
Func_02003f24:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	sub sp, #8
	bl 0x0200cad4
	movs r0, #162
	lsls r0, r0, #1
	bl 0x0200c96c
	movs r5, #15
	movs r6, #0
.L_02003f24_0:
	adds r0, r5, #0
	bl 0x0200c9bc
	adds r0, #89
	strb r6, [r0]
	movs r1, #1
	adds r0, r5, #0
	adds r5, #1
	bl 0x0200ca84
	cmp r5, #24
	bls .L_02003f24_0
	movs r0, #15
	movs r1, #16
	bl 0x0200c78c
	ldr r0, [pc, #712]
	bl 0x0200c964
	cmp r0, #0
	beq .L_02003f24_1
	movs r1, #228
	movs r2, #180
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200ca04
	movs r1, #220
	movs r2, #173
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200ca04
.L_02003f24_1:
	ldr r0, [pc, #676]
	bl 0x0200c964
	cmp r0, #0
	beq .L_02003f24_2
	movs r5, #3
	movs r0, #0
	movs r1, #40
	movs r2, #43
	movs r3, #66
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c8ec
	movs r2, #4
	str r2, [sp, #4]
	mov r10, r2
	movs r0, #83
	movs r1, #40
	movs r2, #96
	movs r3, #29
	str r5, [sp, #0]
	bl 0x0200c8ec
	movs r3, #41
	movs r2, #29
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r11, r3
	mov r9, r2
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	movs r3, #2
	str r3, [sp, #4]
	movs r6, #1
	mov r8, r3
	movs r0, #87
	movs r1, #42
	movs r2, #41
	movs r3, #31
	str r6, [sp, #0]
	bl 0x0200c8ec
	mov r2, r10
	str r2, [sp, #4]
	movs r0, #83
	movs r1, #40
	movs r2, #74
	movs r3, #29
	str r5, [sp, #0]
	bl 0x0200c8ec
	movs r3, #19
	str r3, [sp, #0]
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	mov r2, r8
	str r2, [sp, #4]
	movs r0, #87
	movs r1, #42
	movs r2, #19
	movs r3, #31
	str r6, [sp, #0]
	bl 0x0200c8ec
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #83
	movs r1, #40
	movs r2, #96
	movs r3, #10
	str r5, [sp, #0]
	bl 0x0200c8ec
	movs r3, #10
	mov r2, r11
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #87
	movs r1, #42
	movs r2, #41
	movs r3, #12
	str r6, [sp, #0]
	bl 0x0200c8ec
.L_02003f24_2:
	ldr r0, [pc, #476]
	bl 0x0200c964
	cmp r0, #0
	beq .L_02003f24_3
	movs r6, #3
	movs r0, #0
	movs r1, #40
	movs r2, #43
	movs r3, #46
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl 0x0200c8ec
	movs r2, #4
	str r2, [sp, #4]
	mov r8, r2
	movs r0, #83
	movs r1, #40
	movs r2, #84
	movs r3, #4
	str r6, [sp, #0]
	bl 0x0200c8ec
	movs r3, #29
	mov r2, r8
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r11, r3
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r10, r3
	mov r9, r2
	movs r0, #87
	movs r1, #42
	movs r2, #29
	movs r3, #6
	bl 0x0200c8ec
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #83
	movs r1, #40
	movs r2, #76
	movs r3, #21
	str r6, [sp, #0]
	bl 0x0200c8ec
	movs r5, #21
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c8f4
	mov r2, r10
	mov r3, r9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #87
	movs r1, #42
	movs r2, #21
	movs r3, #23
	bl 0x0200c8ec
	mov r2, r8
	str r2, [sp, #4]
	movs r0, #83
	movs r1, #40
	movs r2, #76
	movs r3, #29
	str r6, [sp, #0]
	bl 0x0200c8ec
	mov r3, r11
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200c8f4
	mov r2, r10
	mov r3, r9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #87
	movs r1, #42
	movs r2, #21
	movs r3, #31
	bl 0x0200c8ec
.L_02003f24_3:
	ldr r0, [pc, #272]
	bl 0x0200c964
	cmp r0, #0
	beq .L_02003f24_4
	movs r5, #3
	movs r0, #0
	movs r1, #40
	movs r2, #13
	movs r3, #66
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200c8ec
	movs r2, #4
	str r2, [sp, #4]
	mov r10, r2
	movs r0, #83
	movs r1, #40
	movs r2, #65
	movs r3, #31
	str r5, [sp, #0]
	bl 0x0200c8ec
	movs r3, #10
	mov r9, r3
	mov r2, r9
	movs r3, #31
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	movs r3, #2
	str r3, [sp, #4]
	movs r6, #1
	mov r8, r3
	movs r0, #87
	movs r1, #42
	movs r2, #10
	movs r3, #33
	str r6, [sp, #0]
	bl 0x0200c8ec
	mov r2, r10
	str r2, [sp, #4]
	movs r0, #83
	movs r1, #40
	movs r2, #79
	movs r3, #9
	str r5, [sp, #0]
	bl 0x0200c8ec
	movs r3, #24
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #87
	movs r1, #42
	movs r2, #24
	movs r3, #11
	str r6, [sp, #0]
	bl 0x0200c8ec
	mov r2, r10
	str r2, [sp, #4]
	movs r0, #83
	movs r1, #40
	movs r2, #91
	movs r3, #10
	str r5, [sp, #0]
	bl 0x0200c8ec
	movs r3, #36
	str r3, [sp, #0]
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl 0x0200c8f4
	mov r2, r8
	str r2, [sp, #4]
	movs r0, #87
	movs r1, #42
	movs r2, #36
	movs r3, #12
	str r6, [sp, #0]
	bl 0x0200c8ec
	bl 0x0200a27c
.L_02003f24_4:
	ldr r0, [pc, #52]
	bl 0x0200c964
	cmp r0, #0
	bne .L_02003f24_5
	ldr r3, [pc, #56]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #10
	bne .L_02003f24_5
	bl 0x0200a7f8
.L_02003f24_5:
	movs r0, #0
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000083b
	.4byte 0x0000083c
	.4byte 0x0000083d
	.4byte 0x0000083e
	.4byte 0x02000240
	.global Func_02004248
	.thumb_func
Func_02004248:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #0
	bl 0x0200ca64
	adds r0, r5, #0
	bl 0x0200c994
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r0
	movs	r0, #0
	mov	r8, r0
	movs	r0, #22
	bl 0x0200c8c4
	adds	r6, r0, #0
	movs	r0, #224
	bl 0x0200c95c
	movs	r1, #224
	adds	r7, r0, #0
	bl 0x0200c954
	mov	r9, r0
	adds	r0, r7, #0
	cmp	r6, #0
	beq.n	.L_02004316
	ldr	r1, [pc, #148]
	adds	r0, r6, #0
	bl 0x0200c8bc
	ldr	r5, [r6, #80]
	adds	r3, r5, #0
	mov	r2, r8
	adds	r3, #38
	strb	r2, [r3, #0]
	adds	r3, #1
	strb	r2, [r3, #0]
	movs	r3, #33
	ldrb	r2, [r5, #5]
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r3, #160
	lsls	r3, r3, #10
	str	r3, [r6, #40]
	movs	r3, #128
	lsls	r3, r3, #7
	movs	r1, #193
	str	r3, [r6, #72]
	lsls	r1, r1, #3
	movs	r0, #17
	bl 0x0200c894
	mov	r8, r0
	mov	r0, sl
	bl 0x0200c934
	movs	r2, #128
	lsls	r2, r2, #3
	add	r2, r8
	movs	r1, #128
	ldrb	r0, [r5, #28]
	bl 0x0200c8ac
	movs	r0, #17
	bl 0x0200c8a4
	movs	r0, #83
	bl 0x0200cb14
	adds	r0, r6, #0
	movs	r1, #3
	bl 0x0200cafc
	mov	r1, r9
	adds	r0, r7, #0
	bl 0x0200c984
	mov	r1, sl
	adds	r0, r7, #0
	bl 0x0200c944
	adds	r0, r6, #0
	bl 0x0200c8cc
	movs	r0, #0
	movs	r1, #1
	bl 0x0200ca0c
	adds	r0, r7, #0
.L_02004316:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.2byte 0xcbe4
	.2byte 0x0200
	.global Func_02004328
	.thumb_func
Func_02004328:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #132]
	movs r2, #236
	ldr r6, [r3]
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r0, #83
	sub sp, #8
	movs r2, #0
	ldrsh r7, [r3, r2]
	bl 0x0200cb14
	movs r0, #224
	movs r1, #3
	bl 0x0200cb04
	ldr r0, [pc, #104]
	movs r1, #1
	bl 0x0200c91c
.L_02004328_1:
	movs r0, #0
	bl 0x0200c97c
	movs r5, #30
	subs r5, r5, r0
	movs r0, #1
	bl 0x0200c97c
	subs r5, r5, r0
	cmp r5, #3
	bgt .L_02004328_0
	ldr r0, [pc, #80]
	movs r1, #1
	bl 0x0200c91c
	add r0, sp, #4
	mov r1, sp
	bl 0x0200cb0c
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_02004328_1
	ldr r0, [sp, #4]
	ldr r1, [sp, #0]
	bl 0x0200c984
	b .L_02004328_1
.L_02004328_0:
	movs r0, #224
	bl 0x0200c94c
	movs r0, #224
	bl 0x0200c94c
	movs r0, #224
	bl 0x0200c94c
	movs r0, #224
	bl 0x0200c94c
	movs r2, #236
	lsls r2, r2, #1
	adds r3, r6, r2
	strh r7, [r3]
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x0000111b
	.4byte 0x0000111c
	.global Func_020043bc
	.thumb_func
Func_020043bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #14
	sub sp, #56
	bl 0x0200c9bc
	mov r10, r0
	movs r0, #190
	bl 0x0200cb14
	movs r0, #14
	bl 0x0200c9bc
	movs r1, #0
	bl 0x0200c8fc
	movs r3, #1
	add r7, sp, #16
	str r3, [r7]
	movs r3, #5
	str r3, [r7, #4]
	movs r3, #142
	lsls r3, r3, #1
	strh r3, [r7, #24]
	ldr r3, [pc, #156]
	str r3, [r7, #8]
	movs r3, #192
	lsls r3, r3, #10
	movs r2, #0
	str r3, [r7, #12]
	mov r8, r2
.L_020043bc_2:
	movs r0, #1
	bl 0x0200c994
	movs r6, #1
	mov r3, r8
	ands r6, r3
	cmp r6, #0
	bne .L_020043bc_0
	bl 0x0200c874
	lsls r3, r0, #1
	adds r3, r3, r0
	mov r2, r10
	lsls r3, r3, #3
	ldr r5, [r2, #8]
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, r5, r3
	ldr r3, [pc, #112]
	adds r5, r5, r3
	bl 0x0200c874
	mov r2, r10
	lsls r0, r0, #5
	ldr r1, [r2, #12]
	lsrs r0, r0, #16
	lsls r0, r0, #16
	movs r3, #128
	adds r1, r1, r0
	lsls r3, r3, #14
	adds r1, r1, r3
	ldr r3, [pc, #88]
	ldr r2, [r2, #16]
	str r3, [sp, #0]
	movs r3, #216
	lsls r3, r3, #13
	str r3, [sp, #8]
	adds r0, r5, #0
	movs r3, #0
	str r6, [sp, #4]
	str r7, [sp, #12]
	bl 0x0200813c
.L_020043bc_0:
	mov r2, r8
	cmp r2, #20
	bne .L_020043bc_1
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #1
	bl 0x0200ca44
.L_020043bc_1:
	movs r3, #1
	add r8, r3
	mov r2, r8
	cmp r2, #31
	bls .L_020043bc_2
	movs r1, #0
	movs r0, #14
	bl 0x0200ca44
	movs r0, #14
	bl 0x0200c9bc
	movs r1, #1
	bl 0x0200c8fc
	sub sp, #-56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x00006666
	.4byte 0xfff40000
	.4byte 0xfffc0000
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #202
	lsls	r1, r1, #1
	movs	r0, #33
	sub	sp, #68
	bl 0x0200c89c
	str	r0, [sp, #64]
	str	r0, [sp, #60]
	ldr	r1, [sp, #64]
	movs	r0, #0
	movs	r2, #200
	str	r0, [sp, #56]
	lsls	r2, r2, #1
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_020044ce
	b.n	.L_02004762
.L_020044ce:
	adds	r1, #8
	ldr	r3, [sp, #64]
	ldr	r4, [sp, #64]
	str	r0, [sp, #8]
	ldr	r0, [pc, #668]
	mov	sl, r1
	ldr	r1, [pc, #668]
	adds	r3, #36
	adds	r4, #37
	adds	r0, #1
	str	r3, [sp, #16]
	str	r4, [sp, #12]
	str	r0, [sp, #4]
	str	r1, [sp, #0]
.L_020044ea:
	mov	r3, sl
	ldr	r3, [r3, #8]
	ldr	r2, [sp, #60]
	ldr	r5, [r2, #0]
	str	r3, [sp, #52]
	mov	r4, sl
	ldr	r4, [r4, #12]
	str	r4, [sp, #48]
	mov	r0, sl
	ldr	r0, [r0, #16]
	str	r0, [sp, #44]
	mov	r1, sl
	ldr	r1, [r1, #20]
	str	r1, [sp, #40]
	mov	r2, sl
	ldr	r2, [r2, #24]
	ldr	r4, [sp, #60]
	str	r2, [sp, #36]
	ldr	r3, [sp, #12]
	ldr	r4, [r4, #4]
	ldrb	r3, [r3, #0]
	ldr	r0, [sp, #60]
	str	r4, [sp, #28]
	ldr	r0, [r0, #8]
	ldr	r2, [sp, #60]
	str	r0, [sp, #24]
	ldr	r2, [r2, #12]
	mov	fp, r3
	str	r2, [sp, #20]
	ldr	r3, [sp, #16]
	ldrb	r3, [r3, #0]
	str	r3, [sp, #32]
	adds	r3, #255
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r1, fp
	str	r3, [sp, #32]
	cmp	r3, #0
	beq.n	.L_0200453a
	b.n	.L_020046ee
.L_0200453a:
	movs	r4, #3
	str	r4, [sp, #32]
	cmp	r1, #0
	bne.n	.L_0200458e
	ldr	r0, [sp, #40]
	ldr	r2, [sp, #36]
	ldr	r4, [sp, #56]
	adds	r0, r0, r2
	str	r0, [sp, #40]
	ldr	r3, [pc, #556]
	lsls	r2, r4, #2
	ldr	r3, [r3, r2]
	cmp	r0, r3
	blt.n	.L_02004560
	ldr	r3, [pc, #552]
	ldr	r3, [r3, r2]
	negs	r3, r3
	str	r3, [sp, #36]
	b.n	.L_02004588
.L_02004560:
	ldr	r0, [sp, #40]
	ldr	r3, [pc, #544]
	cmp	r0, r3
	bgt.n	.L_02004588
	ldr	r3, [pc, #532]
	ldr	r4, [pc, #536]
	ldr	r3, [r3, r2]
	str	r4, [sp, #40]
	str	r3, [sp, #36]
	ldr	r2, [r5, #8]
	str	r2, [sp, #28]
	ldr	r3, [r5, #12]
	str	r3, [sp, #24]
	ldr	r4, [r5, #16]
	movs	r0, #24
	str	r4, [sp, #20]
	str	r1, [r5, #8]
	str	r1, [r5, #12]
	str	r1, [r5, #16]
	mov	fp, r0
.L_02004588:
	ldr	r0, [sp, #40]
	str	r0, [r5, #24]
	str	r0, [r5, #28]
.L_0200458e:
	bl 0x0200c874
	ldr	r2, [pc, #480]
	ldr	r1, [sp, #8]
	ldrb	r3, [r1, r2]
	muls	r3, r0
	lsrs	r6, r3, #16
	bl 0x0200c874
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #0]
	muls	r3, r0
	lsrs	r7, r3, #16
	bl 0x0200c874
	ldr	r1, [sp, #4]
	ldrb	r3, [r1, #1]
	muls	r3, r0
	lsrs	r3, r3, #16
	mov	r8, r3
	cmp	r6, #0
	beq.n	.L_020045c8
	movs	r1, #250
	lsls	r0, r6, #16
	lsls	r1, r1, #2
	bl 0x0200c85c
	adds	r6, r0, #0
	b.n	.L_020045ca
.L_020045c8:
	movs	r6, #0
.L_020045ca:
	cmp	r7, #0
	beq.n	.L_020045dc
	movs	r1, #250
	lsls	r0, r7, #16
	lsls	r1, r1, #2
	bl 0x0200c85c
	mov	r9, r0
	b.n	.L_020045e0
.L_020045dc:
	movs	r2, #0
	mov	r9, r2
.L_020045e0:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_020045f2
	movs	r1, #250
	lsls	r0, r3, #16
	lsls	r1, r1, #2
	bl 0x0200c85c
	b.n	.L_020045f4
.L_020045f2:
	movs	r0, #0
.L_020045f4:
	ldr	r2, [pc, #400]
	ldr	r4, [sp, #8]
	ldrsb	r3, [r2, r4]
	cmp	r3, #1
	bne.n	.L_02004606
	ldr	r1, [sp, #52]
	adds	r1, r1, r6
	str	r1, [sp, #52]
	b.n	.L_02004618
.L_02004606:
	ldr	r4, [sp, #52]
	movs	r1, #1
	subs	r4, r4, r6
	negs	r1, r1
	str	r4, [sp, #52]
	cmp	r3, r1
	beq.n	.L_02004618
	movs	r3, #0
	str	r3, [sp, #52]
.L_02004618:
	ldr	r3, [sp, #8]
	adds	r3, #1
	ldrsb	r3, [r2, r3]
	cmp	r3, #1
	bne.n	.L_0200462a
	ldr	r4, [sp, #48]
	add	r4, r9
	str	r4, [sp, #48]
	b.n	.L_0200463e
.L_0200462a:
	ldr	r1, [sp, #48]
	mov	r4, r9
	subs	r1, r1, r4
	str	r1, [sp, #48]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_0200463e
	movs	r3, #0
	str	r3, [sp, #48]
.L_0200463e:
	ldr	r3, [sp, #8]
	adds	r3, #2
	ldrsb	r3, [r2, r3]
	cmp	r3, #1
	bne.n	.L_02004650
	ldr	r4, [sp, #44]
	adds	r4, r4, r0
	str	r4, [sp, #44]
	b.n	.L_02004662
.L_02004650:
	ldr	r1, [sp, #44]
	movs	r2, #1
	subs	r1, r1, r0
	negs	r2, r2
	str	r1, [sp, #44]
	cmp	r3, r2
	beq.n	.L_02004662
	movs	r3, #0
	str	r3, [sp, #44]
.L_02004662:
	ldr	r4, [sp, #0]
	ldr	r1, [sp, #52]
	ldrb	r3, [r4, #0]
	adds	r0, r3, #0
	muls	r0, r1
	bl 0x0200c884
	ldr	r2, [sp, #0]
	ldr	r4, [sp, #48]
	ldrb	r3, [r2, #1]
	lsls	r6, r0, #1
	adds	r0, r3, #0
	muls	r0, r4
	bl 0x0200c884
	lsls	r7, r0, #1
	ldr	r0, [sp, #0]
	ldr	r1, [sp, #44]
	ldrb	r3, [r0, #2]
	adds	r0, r3, #0
	muls	r0, r1
	bl 0x0200c88c
	mov	r2, fp
	lsls	r0, r0, #1
	cmp	r2, #0
	beq.n	.L_020046d0
	ldr	r3, [sp, #28]
	adds	r3, r3, r6
	str	r3, [sp, #28]
	mov	r3, fp
	ldr	r4, [sp, #24]
	ldr	r1, [sp, #20]
	adds	r3, #255
	lsls	r3, r3, #24
	adds	r4, r4, r7
	adds	r1, r1, r0
	lsrs	r3, r3, #24
	str	r4, [sp, #24]
	str	r1, [sp, #20]
	mov	fp, r3
	cmp	r3, #0
	bne.n	.L_020046ee
	ldr	r2, [sp, #28]
	mov	r3, r9
	str	r2, [r5, #8]
	str	r2, [r5, #56]
	cmp	r3, #0
	beq.n	.L_020046c8
	str	r4, [r5, #12]
	str	r4, [r5, #60]
.L_020046c8:
	ldr	r4, [sp, #20]
	str	r4, [r5, #16]
	str	r4, [r5, #64]
	b.n	.L_020046ee
.L_020046d0:
	ldr	r3, [r5, #8]
	mov	r1, r9
	adds	r3, r3, r6
	str	r3, [r5, #8]
	str	r3, [r5, #56]
	cmp	r1, #0
	beq.n	.L_020046e6
	ldr	r3, [r5, #12]
	adds	r3, r3, r7
	str	r3, [r5, #12]
	str	r3, [r5, #60]
.L_020046e6:
	ldr	r3, [r5, #16]
	adds	r3, r3, r0
	str	r3, [r5, #16]
	str	r3, [r5, #64]
.L_020046ee:
	ldr	r2, [sp, #52]
	mov	r3, sl
	str	r2, [r3, #8]
	ldr	r4, [sp, #48]
	str	r4, [r3, #12]
	ldr	r0, [sp, #44]
	str	r0, [r3, #16]
	ldr	r1, [sp, #40]
	str	r1, [r3, #20]
	ldr	r2, [sp, #36]
	str	r2, [r3, #24]
	ldr	r4, [sp, #12]
	mov	r3, fp
	strb	r3, [r4, #0]
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #60]
	str	r0, [r1, #4]
	ldr	r2, [sp, #24]
	mov	r3, sl
	str	r2, [r3, #0]
	ldr	r4, [sp, #20]
	add	r0, sp, #32
	str	r4, [r1, #12]
	ldrb	r0, [r0, #0]
	ldr	r1, [sp, #16]
	strb	r0, [r1, #0]
	ldr	r1, [sp, #0]
	ldr	r2, [sp, #8]
	adds	r1, #3
	adds	r2, #3
	ldr	r3, [sp, #4]
	ldr	r4, [sp, #56]
	str	r1, [sp, #0]
	str	r2, [sp, #8]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	adds	r3, #3
	adds	r4, #1
	adds	r1, #40
	adds	r2, #40
	str	r3, [sp, #4]
	str	r4, [sp, #56]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	ldr	r3, [sp, #60]
	movs	r0, #40
	adds	r3, #40
	add	sl, r0
	ldr	r4, [sp, #64]
	movs	r0, #200
	str	r3, [sp, #60]
	lsls	r0, r0, #1
	adds	r3, r4, r0
	ldrh	r3, [r3, #0]
	ldr	r1, [sp, #56]
	cmp	r1, r3
	beq.n	.L_02004762
	b.n	.L_020044ea
.L_02004762:
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0200d0e4
	.4byte 0x0200d102
	.4byte 0x0200d140
	.4byte 0x0200d168
	.4byte 0x00001999
	.2byte 0xd120
	.2byte 0x0200
	.global Func_0200478c
	.thumb_func
Func_0200478c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	movs r1, #202
	adds r6, r0, #0
	lsls r1, r1, #1
	movs r0, #33
	sub sp, #4
	bl 0x0200c89c
	movs r3, #0
	mov r9, r0
	mov r0, sp
	str r3, [r0]
	mov r5, r9
	ldr r3, [pc, #136]
	mov r1, r9
	ldr r2, [pc, #136]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r2, r8
	cmp r2, #10
	bls .L_0200478c_0
	movs r3, #10
	mov r8, r3
.L_0200478c_0:
	movs r2, #0
	mov r3, r8
	mov r10, r2
	cmp r3, #0
	beq .L_0200478c_1
	mov r11, r2
	movs r7, #0
.L_0200478c_2:
	adds r0, r6, #0
	bl 0x0200c9bc
	ldr r3, [r0, #80]
	mov r2, r11
	adds r3, #38
	str r0, [r5]
	adds r0, #85
	strb r2, [r3]
	strb r2, [r0]
	adds r0, r6, #0
	bl 0x0200c9bc
	movs r1, #1
	bl 0x0200c904
	ldr r2, [pc, #80]
	ldr r3, [r7, r2]
	str r3, [r5, #28]
	ldr r3, [pc, #76]
	ldr r3, [r3, r7]
	adds r2, r5, #0
	negs r3, r3
	str r3, [r5, #32]
	adds r2, #36
	movs r3, #3
	strb r3, [r2]
	movs r3, #1
	add r10, r3
	adds r7, #4
	adds r5, #40
	adds r6, #1
	cmp r10, r8
	bne .L_0200478c_2
.L_0200478c_1:
	movs r3, #200
	lsls r3, r3, #1
	add r3, r9
	mov r2, r8
	movs r1, #200
	strh r2, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #36]
	bl 0x0200c86c
	sub sp, #-4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x040000d4
	.4byte 0x85000065
	.4byte 0x0200d140
	.4byte 0x0200d168
	.4byte 0x0200c49d
	.include "games/THE BROKEN SEAL/SRC/FIELD/SORU_STAR/IMPORT.INC"
	.section .rodata,"a",%progbits
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
	.4byte 0x0200cb1c
	.4byte 0x0200cb54
	.4byte 0x0200cb8c
	.4byte 0x00000022
	.4byte 0x02008315
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000c000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000f000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000f000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
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
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x000001d8
	.4byte 0x40000142
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x0020a012
	.4byte 0x000001ff
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00034000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00034000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0022
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02690000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01ad0000
	.4byte 0x00000000
	.4byte 0x00dd0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00710000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x00aa0000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01340000
	.4byte 0x00000000
	.4byte 0x01ff0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x018d0000
	.4byte 0x00000000
	.4byte 0x01a20000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x021e0000
	.4byte 0x00000000
	.4byte 0x010d0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01a50000
	.4byte 0x00000000
	.4byte 0x00750000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01270000
	.4byte 0x00000000
	.4byte 0x00d20000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02e60000
	.4byte 0x00000000
	.4byte 0x01360000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0200a675
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte 0x0200a6e1
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0200a74d
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0200a76d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200a78d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0200a7ad
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000010cb
	.4byte 0x00008d15
	.4byte 0xffff0005
	.4byte 0x0200a6e1
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000010ca
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0200a76d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000010cc
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x0200a7cd
	.4byte 0x00000602
	.4byte 0xffff000a
	.4byte 0x0200a7ed
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte 0x0200a7ed
	.4byte 0x00000003
	.4byte 0x083c0002
	.4byte 0x02008391
	.4byte 0x00000003
	.4byte 0x083d0003
	.4byte 0x020086f5
	.4byte 0x00000003
	.4byte 0x083e0004
	.4byte 0x02008a65
	.4byte 0x00000003
	.4byte 0x083f0005
	.4byte 0x0200a401
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0028003b
	.4byte 0x00040003
	.4byte 0x003e0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280041
	.4byte 0x00040003
	.4byte 0x00440006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280047
	.4byte 0x00040003
	.4byte 0x004a0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x0028004d
	.4byte 0x00040003
	.4byte 0x00500006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280053
	.4byte 0x00040003
	.4byte 0xffff0000
	.4byte 0x04040404
	.4byte 0x00040300
	.4byte 0x04000404
	.4byte 0x04040003
	.4byte 0x00060406
	.4byte 0x03000404
	.4byte 0x02010002
	.4byte 0x01010200
	.4byte 0x02000102
	.4byte 0x01020001
	.4byte 0x00010100
	.4byte 0x02010102
	.4byte 0x01020001
	.4byte 0x00010200
	.4byte 0x02000102
	.4byte 0x01010101
	.4byte 0x00010100
	.4byte 0x0100ff01
	.4byte 0x01ff00ff
	.4byte 0x00ff0101
	.4byte 0xff00ffff
	.4byte 0xffff00ff
	.4byte 0x0000ff00
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x00009999
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x00009999
	.4byte 0x0000b333
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x0000028f
	.4byte 0x0000020c
	.4byte 0x0000028f
