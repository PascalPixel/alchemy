.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000678, 0x02000678
	.set sub_02000686, 0x02000686
	.set sub_020006b4, 0x020006b4
	.set sub_020006cc, 0x020006cc
	.set sub_020006de, 0x020006de
	.set sub_02000710, 0x02000710
	.set sub_02000714, 0x02000714
	.set sub_02000770, 0x02000770
	.set sub_020007b0, 0x020007b0
	.set sub_020007ba, 0x020007ba
	.set sub_020007d2, 0x020007d2
	.set sub_020007d4, 0x020007d4
	.set sub_02000890, 0x02000890
	.set sub_020008a6, 0x020008a6
	.set sub_020008a8, 0x020008a8
	.set sub_020008b6, 0x020008b6
	.set sub_020008d4, 0x020008d4
	.set sub_020008e4, 0x020008e4
	.set sub_020009b0, 0x020009b0
	.set sub_020009c4, 0x020009c4
	.set sub_020009c6, 0x020009c6
	.set sub_020009d6, 0x020009d6
	.set sub_02000a04, 0x02000a04
	.set sub_02000a20, 0x02000a20
	.set sub_02000a24, 0x02000a24
	.set sub_02000a26, 0x02000a26
	.set sub_02000a28, 0x02000a28
	.set sub_02000a2e, 0x02000a2e
	.set sub_02000a36, 0x02000a36
	.set sub_02000a38, 0x02000a38
	.set sub_02000a64, 0x02000a64
	.set sub_02000a80, 0x02000a80
	.set sub_02000a84, 0x02000a84
	.set sub_02000a86, 0x02000a86
	.set sub_02000a88, 0x02000a88
	.set sub_02000a8e, 0x02000a8e
	.set sub_02000a96, 0x02000a96
	.set sub_02000a98, 0x02000a98
	.set sub_02000acc, 0x02000acc
	.set sub_02000ae0, 0x02000ae0
	.set sub_02000aec, 0x02000aec
	.set sub_02000aee, 0x02000aee
	.set sub_02000af0, 0x02000af0
	.set sub_02000afe, 0x02000afe
	.set sub_02000b10, 0x02000b10
	.set sub_02000b2e, 0x02000b2e
	.set sub_02000b44, 0x02000b44
	.set sub_02000b4a, 0x02000b4a
	.set sub_02000b52, 0x02000b52
	.set sub_02000b5e, 0x02000b5e
	.set sub_02000b60, 0x02000b60
	.set sub_02000b62, 0x02000b62
	.set sub_02000b64, 0x02000b64
	.set sub_02000b7e, 0x02000b7e
	.set sub_02000b82, 0x02000b82
	.set sub_02000b84, 0x02000b84
	.set sub_02000b9c, 0x02000b9c
	.set sub_02000ba2, 0x02000ba2
	.set sub_02000bac, 0x02000bac
	.set sub_02000bc2, 0x02000bc2
	.set sub_02000bc6, 0x02000bc6
	.set sub_02000bc8, 0x02000bc8
	.set sub_02000bd6, 0x02000bd6
	.set sub_02000bdc, 0x02000bdc
	.set sub_02000bfc, 0x02000bfc
	.set sub_02000c3e, 0x02000c3e
	.set sub_02000c76, 0x02000c76
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/SHIAN_HEYA/ENTRY.INC"
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
	bl sub_02000686
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
	bl sub_020006b4
	adds r0, r5, #0
	movs r1, #14
	bl sub_02000714
	adds r0, r5, #0
	movs r1, #1
	bl sub_020006cc
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
	bl sub_020006de
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
	bl sub_02000710
	adds r0, r5, #0
	movs r1, #15
	bl sub_02000770
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
	bl sub_020007d2
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
	bl sub_020007b0
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
	bl sub_020007ba
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl sub_020007d4
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
	bl sub_020008a6
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
	bl sub_02000890
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
	bl sub_020008a8
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl sub_020008b6
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
	bl sub_020008d4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl sub_020008e4
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
	.4byte 0x0200876c
	.4byte 0x02008105
	.4byte 0xffff0000
	.global Func_02000314
	.thumb_func
Func_02000314:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008778
	.global Func_0200031c
	.thumb_func
Func_0200031c:
	movs r0, #0
	bx lr
	.global Func_02000320
	.thumb_func
Func_02000320:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008868
	.global Func_02000328
	.thumb_func
Func_02000328:
	push {r5, lr}
	ldr r3, [pc, #36]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #8
	bne .L_02000328_0
	ldr r0, [pc, #24]
	b .L_02000328_1
.L_02000328_0:
	ldr r5, [pc, #24]
	adds r0, r5, #0
	bl sub_020009b0
	adds r0, r5, #0
.L_02000328_1:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x020089c8
	.4byte 0x02008890
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	push {r5, lr}
	movs r0, #0
	bl sub_020009d6
	ldrh r5, [r0, #6]
	bl sub_020009c4
	ldr r3, [pc, #60]
	adds r5, r5, r3
	ldr r3, [pc, #60]
	cmp r5, r3
	bhi .L_0200035c_0
	movs r0, #16
	movs r1, #14
	bl sub_02000a2e
	b .L_0200035c_1
.L_0200035c_0:
	ldr r0, [pc, #48]
	bl sub_020009c6
	cmp r0, #0
	bne .L_0200035c_2
	ldr r0, [pc, #40]
	bl sub_02000a20
	b .L_0200035c_3
.L_0200035c_2:
	ldr r0, [pc, #36]
	bl sub_02000a28
.L_0200035c_3:
	movs r0, #14
	movs r1, #0
	bl sub_02000a38
.L_0200035c_1:
	bl sub_02000a04
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00000895
	.4byte 0x00001817
	.4byte 0x00001a46
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {r5, lr}
	movs r0, #0
	bl sub_02000a36
	ldrh r5, [r0, #6]
	bl sub_02000a24
	ldr r3, [pc, #60]
	adds r5, r5, r3
	ldr r3, [pc, #60]
	cmp r5, r3
	bhi .L_020003bc_0
	movs r0, #17
	movs r1, #15
	bl sub_02000a8e
	b .L_020003bc_1
.L_020003bc_0:
	ldr r0, [pc, #48]
	bl sub_02000a26
	cmp r0, #0
	bne .L_020003bc_2
	ldr r0, [pc, #40]
	bl sub_02000a80
	b .L_020003bc_3
.L_020003bc_2:
	ldr r0, [pc, #36]
	bl sub_02000a88
.L_020003bc_3:
	movs r0, #15
	movs r1, #0
	bl sub_02000a98
.L_020003bc_1:
	bl sub_02000a64
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00000895
	.4byte 0x00001819
	.4byte 0x00001a48
	.global Func_0200041c
	.thumb_func
Func_0200041c:
	push {r5, lr}
	movs r0, #0
	bl sub_02000a96
	ldrh r5, [r0, #6]
	bl sub_02000a84
	ldr r3, [pc, #68]
	adds r5, r5, r3
	ldr r3, [pc, #68]
	cmp r5, r3
	bhi .L_0200041c_0
	movs r0, #18
	movs r1, #16
	bl sub_02000aee
	b .L_0200041c_1
.L_0200041c_0:
	ldr r0, [pc, #56]
	bl sub_02000a86
	cmp r0, #0
	bne .L_0200041c_2
	ldr r0, [pc, #48]
	bl sub_02000ae0
	movs r0, #16
	movs r1, #0
	bl sub_02000af0
	b .L_0200041c_1
.L_0200041c_2:
	ldr r0, [pc, #36]
	bl sub_02000af0
	movs r0, #16
	movs r1, #0
	bl sub_02000b10
.L_0200041c_1:
	bl sub_02000acc
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00000895
	.4byte 0x0000181b
	.4byte 0x00001a4a
	.global Func_02000484
	.thumb_func
Func_02000484:
	push {r5, lr}
	movs r0, #0
	bl sub_02000afe
	ldrh r5, [r0, #6]
	bl sub_02000aec
	ldr r3, [pc, #64]
	adds r5, r5, r3
	movs r3, #192
	lsls r3, r3, #8
	cmp r5, r3
	bls .L_02000484_0
	movs r0, #5
	movs r1, #17
	bl sub_02000b60
	b .L_02000484_1
.L_02000484_0:
	ldr r0, [pc, #44]
	bl sub_02000af0
	cmp r0, #0
	bne .L_02000484_2
	ldr r0, [pc, #40]
	bl sub_02000b4a
	b .L_02000484_3
.L_02000484_2:
	ldr r0, [pc, #36]
	bl sub_02000b52
.L_02000484_3:
	movs r0, #17
	movs r1, #0
	bl sub_02000b62
.L_02000484_1:
	bl sub_02000b2e
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffffe000
	.4byte 0x00000895
	.4byte 0x0000181d
	.4byte 0x00001a4e
	.global Func_020004e4
	.thumb_func
Func_020004e4:
	push {lr}
	bl sub_02000b44
	ldr r0, [pc, #20]
	bl sub_02000b82
	movs r1, #0
	movs r0, #10
	bl sub_02000ba2
	bl sub_02000b5e
	pop {r0}
	bx r0
	.4byte 0x00001a3a
	.global Func_02000504
	.thumb_func
Func_02000504:
	push {lr}
	bl sub_02000b64
	ldr r0, [pc, #20]
	bl sub_02000ba2
	movs r1, #0
	movs r0, #12
	bl sub_02000bc2
	bl sub_02000b7e
	pop {r0}
	bx r0
	.4byte 0x00001a40
	.global Func_02000524
	.thumb_func
Func_02000524:
	push {lr}
	bl sub_02000b84
	ldr r0, [pc, #64]
	bl sub_02000bc2
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl sub_02000bdc
	movs r1, #10
	movs r2, #0
	movs r0, #9
	bl sub_02000bc6
	movs r0, #60
	bl sub_02000b9c
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl sub_02000bd6
	movs r0, #20
	bl sub_02000bac
	movs r0, #9
	movs r1, #0
	bl sub_02000bfc
	bl sub_02000bc8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001a64
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {lr}
	ldr r3, [pc, #24]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #8
	bne .L_02000570_0
	ldr r0, [pc, #12]
	b .L_02000570_1
.L_02000570_0:
	ldr r0, [pc, #12]
.L_02000570_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x02008d4c
	.4byte 0x02008a28
	.global Func_02000598
	.thumb_func
Func_02000598:
	push {r5, lr}
	ldr r3, [pc, #104]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	ldr r3, [pc, #92]
	subs r2, #71
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	beq .L_02000598_0
	cmp r3, #7
	bne .L_02000598_1
.L_02000598_0:
	movs r0, #248
	lsls r0, r0, #16
	ldr r2, [pc, #76]
	movs r1, #0
	b .L_02000598_2
.L_02000598_1:
	cmp r3, #6
	bne .L_02000598_3
	movs r5, #142
	lsls r5, r5, #18
	movs r0, #230
	movs r1, #0
	adds r2, r5, #0
	movs r3, #20
	lsls r0, r0, #17
	bl sub_02000678
	movs r0, #242
	lsls r0, r0, #17
	movs r1, #0
	adds r2, r5, #0
.L_02000598_2:
	movs r3, #20
	bl sub_02000686
	b .L_02000598_4
.L_02000598_3:
	cmp r3, #8
	bne .L_02000598_4
	ldr r0, [pc, #32]
	bl sub_02000c3e
	movs r0, #10
	movs r1, #6
	bl sub_02000c76
.L_02000598_4:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x01a10000
	.4byte 0x0000012f
	.include "games/THE BROKEN SEAL/SRC/FIELD/SHIAN_HEYA/IMPORT.INC"
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
	.4byte 0x020086c4
	.4byte 0x020086fc
	.4byte 0x02008734
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000080
	.4byte 0xc00000b8
	.4byte 0x00180000
	.4byte 0x00f80008
	.4byte 0x000000d0
	.4byte 0xffff0002
	.4byte 0x000001c0
	.4byte 0xc00000c8
	.4byte 0x01200000
	.4byte 0x02500008
	.4byte 0x000000e0
	.4byte 0xffff0003
	.4byte 0x000002e0
	.4byte 0xc00000c8
	.4byte 0x02600000
	.4byte 0x03500008
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x000000a0
	.4byte 0xc00001d8
	.4byte 0x00300000
	.4byte 0x01500120
	.4byte 0x000001f0
	.4byte 0xffff0005
	.4byte 0x000001d8
	.4byte 0x00000168
	.4byte 0x01800000
	.4byte 0x02a00120
	.4byte 0x000001f0
	.4byte 0xffff0006
	.4byte 0x00000200
	.4byte 0xc00002b8
	.4byte 0x01600000
	.4byte 0x029001f0
	.4byte 0x000002d0
	.4byte 0xffff0007
	.4byte 0x00000078
	.4byte 0x00000168
	.4byte 0x00300000
	.4byte 0x01500120
	.4byte 0x000001f0
	.4byte 0xffff0008
	.4byte 0x00000078
	.4byte 0xc00002b8
	.4byte 0x00400000
	.4byte 0x01300220
	.4byte 0x000002e8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000049
	.4byte 0x00105048
	.4byte 0x00206048
	.4byte 0x00307048
	.4byte 0x00408048
	.4byte 0x00507049
	.4byte 0x00609048
	.4byte 0x00705049
	.4byte 0x0080403d
	.4byte 0x000001ff
	.4byte 0x000000a0
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x000000a2
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x0001c000
	.4byte 0x0000009c
	.4byte 0x00000001
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0x0000009f
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00004000
	.4byte 0x000000a4
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x000000a6
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x0001c000
	.4byte 0x0000007c
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01c00000
	.4byte 0x00018000
	.4byte 0x00000080
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00004000
	.4byte 0x0000007b
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x18950028
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x02900000
	.4byte 0x00014000
	.4byte 0x18950027
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00010000
	.4byte 0x18950029
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
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
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x08950008
	.4byte 0x0000180b
	.4byte 0x00000000
	.4byte 0x08950009
	.4byte 0x0000180c
	.4byte 0x00000000
	.4byte 0x0895000a
	.4byte 0x0000180f
	.4byte 0x00000000
	.4byte 0x0895000b
	.4byte 0x00001810
	.4byte 0x00000000
	.4byte 0x0895000c
	.4byte 0x00001813
	.4byte 0x00000000
	.4byte 0x0895000d
	.4byte 0x00001814
	.4byte 0x00000000
	.4byte 0x0895000e
	.4byte 0x0200835d
	.4byte 0x00000000
	.4byte 0x0895000f
	.4byte 0x020083bd
	.4byte 0x00000000
	.4byte 0x08950010
	.4byte 0x0200841d
	.4byte 0x00000000
	.4byte 0x08950011
	.4byte 0x02008485
	.4byte 0x00000000
	.4byte 0x08950012
	.4byte 0x0000181e
	.4byte 0x00000000
	.4byte 0x08950013
	.4byte 0x0000181f
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a36
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a37
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020084e5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001a3d
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008505
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001a43
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0200835d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x020083bd
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200841d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008485
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001a4f
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001a50
	.4byte 0x00008d15
	.4byte 0x08950008
	.4byte 0x0000180d
	.4byte 0x00008d15
	.4byte 0x08950009
	.4byte 0x0000180e
	.4byte 0x00008d15
	.4byte 0x0895000a
	.4byte 0x00001811
	.4byte 0x00008d15
	.4byte 0x0895000b
	.4byte 0x00001812
	.4byte 0x00008d15
	.4byte 0x0895000c
	.4byte 0x00001815
	.4byte 0x00008d15
	.4byte 0x0895000d
	.4byte 0x00001816
	.4byte 0x00008d15
	.4byte 0x0895000e
	.4byte 0x00001818
	.4byte 0x00008d15
	.4byte 0x0895000f
	.4byte 0x0000181a
	.4byte 0x00008d15
	.4byte 0x08950010
	.4byte 0x0000181c
	.4byte 0x00008d15
	.4byte 0x08950011
	.4byte 0x00001820
	.4byte 0x00008d15
	.4byte 0x08950012
	.4byte 0x00001821
	.4byte 0x00008d15
	.4byte 0x08950013
	.4byte 0x00001822
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a38
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a39
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a3e
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a3f
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a44
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a45
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001a47
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001a49
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001a4d
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001a51
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001a52
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001a53
	.4byte 0x00000033
	.4byte 0x0f6f0064
	.4byte 0x001000bc
	.4byte 0x00000023
	.4byte 0x0f700065
	.4byte 0x001000e3
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x0040299b
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x0040299c
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x0040299d
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x0040299e
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x0040299f
	.4byte 0x0000c5b3
	.4byte 0xffff00cd
	.4byte 0x004029a0
	.4byte 0x000001b3
	.4byte 0xffff00ce
	.4byte 0x004029a1
	.4byte 0x000001b3
	.4byte 0xffff00cf
	.4byte 0x004029a2
	.4byte 0x000001b3
	.4byte 0xffff00d0
	.4byte 0x004029a3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x08b30008
	.4byte 0x00001a67
	.4byte 0x00000000
	.4byte 0x08b30009
	.4byte 0x02008525
	.4byte 0x00000000
	.4byte 0x08b3000a
	.4byte 0x00001a66
	.4byte 0x00008d15
	.4byte 0x08b30008
	.4byte 0x00001a6a
	.4byte 0x00008d15
	.4byte 0x08b30009
	.4byte 0x00001a68
	.4byte 0x00008d15
	.4byte 0x08b3000a
	.4byte 0x00001a69
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a6d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a6b
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a6c
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a70
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a6e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a6f
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
