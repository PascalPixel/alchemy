.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #104]
	cmp r1, #0
	beq .L_02000030_0
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
	bl 0x0200981c
	ldrh r3, [r5, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	cmp r0, #0
	beq .L_02000030_0
	movs r2, #128
	lsls r2, r2, #5
	cmp r0, r2
	ble .L_02000030_1
	adds r0, r2, #0
.L_02000030_1:
	ldr r2, [pc, #20]
	cmp r0, r2
	bge .L_02000030_2
	adds r0, r2, #0
.L_02000030_2:
	adds r3, r3, r0
	strh r3, [r5, #6]
.L_02000030_0:
	movs r0, #1
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0xfffff000
	.global Func_02000088
	.thumb_func
Func_02000088:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009fd0
	.global Func_02000090
	.thumb_func
Func_02000090:
	movs r0, #0
	bx lr
	.global Func_02000094
	.thumb_func
Func_02000094:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000094_0
	ldr r0, [pc, #12]
	b .L_02000094_1
.L_02000094_0:
	ldr r0, [pc, #12]
.L_02000094_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000834
	.4byte 0x0200a1dc
	.4byte 0x0200a198
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {r5, lr}
	ldr r3, [pc, #64]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #19
	bne .L_020000b8_0
	ldr r0, [pc, #52]
	b .L_020000b8_1
.L_020000b8_0:
	ldr r0, [pc, #52]
	bl 0x020098ec
	cmp r0, #0
	beq .L_020000b8_2
	ldr r5, [pc, #44]
	b .L_020000b8_3
.L_020000b8_2:
	ldr r0, [pc, #44]
	bl 0x020098ec
	cmp r0, #0
	beq .L_020000b8_4
	ldr r5, [pc, #40]
	b .L_020000b8_3
.L_020000b8_4:
	ldr r5, [pc, #40]
.L_020000b8_3:
	adds r0, r5, #0
	bl 0x0200991c
	adds r0, r5, #0
.L_020000b8_1:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200a518
	.4byte 0x0000087a
	.4byte 0x0200a410
	.4byte 0x00000815
	.4byte 0x0200a338
	.4byte 0x0200a218
	.global Func_02000118
	.thumb_func
Func_02000118:
	push {r5, r6, lr}
	ldr r5, [pc, #64]
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x020099c4
	movs r1, #0
	adds r0, r6, #0
	bl 0x020099cc
	movs r0, #0
	movs r1, #0
	bl 0x02009924
	cmp r0, #0
	bne .L_02000118_0
	movs r0, #10
	bl 0x02009904
	adds r0, r5, #1
	bl 0x020099c4
	b .L_02000118_1
.L_02000118_0:
	adds r0, r5, #2
	bl 0x020099c4
.L_02000118_1:
	adds r0, r6, #0
	movs r1, #0
	bl 0x020099d4
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000022b9
	.global Func_02000160
	.thumb_func
Func_02000160:
	push {lr}
	ldr r3, [pc, #84]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #19
	bne .L_02000160_0
	movs r0, #149
	lsls r0, r0, #4
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000160_1
	ldr r0, [pc, #60]
	b .L_02000160_2
.L_02000160_1:
	ldr r0, [pc, #60]
	b .L_02000160_2
.L_02000160_0:
	ldr r0, [pc, #60]
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000160_3
	ldr r0, [pc, #52]
	b .L_02000160_2
.L_02000160_3:
	ldr r0, [pc, #52]
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000160_4
	ldr r0, [pc, #48]
	b .L_02000160_2
.L_02000160_4:
	ldr r0, [pc, #48]
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000160_5
	ldr r0, [pc, #40]
	b .L_02000160_2
.L_02000160_5:
	ldr r0, [pc, #40]
.L_02000160_2:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0200ac5c
	.4byte 0x0200ab9c
	.4byte 0x00000834
	.4byte 0x0200a5a8
	.4byte 0x0000087a
	.4byte 0x0200a980
	.4byte 0x00000815
	.4byte 0x0200a800
	.4byte 0x0200a6b0
	.global Func_020001e0
	.thumb_func
Func_020001e0:
	push {lr}
	bl 0x0200990c
	movs r1, #2
	movs r0, #16
	bl 0x02009994
	movs r0, #30
	bl 0x02009904
	ldr r0, [pc, #132]
	bl 0x020099c4
	movs r0, #0
	movs r1, #16
	movs r2, #10
	bl 0x020099ac
	movs r0, #16
	movs r1, #0
	movs r2, #6
	bl 0x020099dc
	movs r1, #129
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #1
	bl 0x02009a04
	movs r1, #1
	movs r0, #16
	bl 0x02009994
	movs r0, #20
	bl 0x02009904
	movs r1, #4
	movs r0, #16
	bl 0x02009984
	movs r0, #20
	bl 0x02009904
	movs r1, #0
	movs r0, #16
	bl 0x020099cc
	movs r0, #0
	movs r1, #0
	bl 0x02009924
	cmp r0, #1
	bne .L_020001e0_0
	ldr r3, [pc, #52]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020001e0_0:
	movs r1, #1
	movs r0, #16
	bl 0x02009994
	movs r0, #20
	bl 0x02009904
	movs r0, #16
	movs r1, #0
	movs r2, #4
	bl 0x020099dc
	bl 0x02009914
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000f5b
	.4byte 0x03001ebc
	.global Func_02000284
	.thumb_func
Func_02000284:
	push {lr}
	bl 0x0200990c
	movs r1, #0
	movs r2, #10
	movs r0, #14
	bl 0x020099a4
	ldr r0, [pc, #64]
	bl 0x020099c4
	movs r1, #0
	movs r0, #14
	bl 0x020099cc
	movs r0, #0
	movs r1, #0
	bl 0x02009924
	cmp r0, #0
	bne .L_02000284_0
	movs r0, #14
	movs r1, #0
	bl 0x020099d4
	b .L_02000284_1
.L_02000284_0:
	ldr r3, [pc, #32]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #14
	movs r1, #0
	bl 0x020099e4
.L_02000284_1:
	bl 0x02009914
	pop {r0}
	bx r0
	.4byte 0x000011aa
	.4byte 0x03001ebc
	.global Func_020002e0
	.thumb_func
Func_020002e0:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, [pc, #44]
	bl 0x020098ec
	cmp r0, #0
	beq .L_020002e0_0
	bl 0x02009a44
.L_020002e0_0:
	ldr r3, [pc, #36]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	bl 0x02009a64
	bl 0x02009a6c
	adds r0, r5, #0
	bl 0x02009a34
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000834
	.4byte 0x03001ebc
	.global Func_0200031c
	.thumb_func
Func_0200031c:
	push {lr}
	movs r0, #123
	bl 0x02009aac
	movs r0, #1
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000330
	.thumb_func
Func_02000330:
	push {lr}
	movs r0, #123
	bl 0x02009aac
	movs r0, #2
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000344
	.thumb_func
Func_02000344:
	push {lr}
	movs r0, #123
	bl 0x02009aac
	movs r0, #3
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000358
	.thumb_func
Func_02000358:
	push {lr}
	movs r0, #123
	bl 0x02009aac
	movs r0, #4
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200036c
	.thumb_func
Func_0200036c:
	push {lr}
	movs r0, #128
	bl 0x02009aac
	movs r0, #5
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000380
	.thumb_func
Func_02000380:
	push {lr}
	movs r0, #123
	bl 0x02009aac
	movs r0, #6
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000394
	.thumb_func
Func_02000394:
	push {lr}
	movs r0, #128
	bl 0x02009aac
	movs r0, #7
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020003a8
	.thumb_func
Func_020003a8:
	push {lr}
	movs r0, #129
	bl 0x02009aac
	movs r0, #8
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {lr}
	movs r0, #129
	bl 0x02009aac
	movs r0, #9
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020003d0
	.thumb_func
Func_020003d0:
	push {lr}
	movs r0, #123
	bl 0x02009aac
	movs r0, #10
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020003e4
	.thumb_func
Func_020003e4:
	push {lr}
	movs r0, #123
	bl 0x02009aac
	movs r0, #11
	bl 0x020082e0
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020003f8
	.thumb_func
Func_020003f8:
	push {r5, lr}
	ldr r3, [pc, #348]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #19
	bne .L_020003f8_0
	ldr r0, [pc, #336]
	bl 0x020098fc
	ldr r3, [pc, #332]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	b .L_020003f8_1
.L_020003f8_0:
	ldr r0, [pc, #320]
	bl 0x020098ec
	cmp r0, #0
	beq .L_020003f8_2
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x02009974
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009974
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009974
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x02009974
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl 0x02009974
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl 0x02009974
	b .L_020003f8_3
.L_020003f8_2:
	bl 0x020093b4
.L_020003f8_3:
	movs r0, #13
	movs r1, #1
	bl 0x020099fc
	ldr r0, [pc, #240]
	bl 0x020098ec
	cmp r0, #0
	beq .L_020003f8_4
	movs r0, #17
	bl 0x0200992c
	movs r1, #0
	bl 0x020098a4
	ldr r3, [pc, #204]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #6
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_020003f8_1
	ldr r0, [pc, #200]
	bl 0x020098ec
	cmp r0, #0
	beq .L_020003f8_5
	ldr r0, [pc, #196]
	bl 0x020098ec
	cmp r0, #0
	beq .L_020003f8_1
	movs r0, #12
	bl 0x02009884
	b .L_020003f8_1
.L_020003f8_5:
	movs r0, #11
	bl 0x02009884
	movs r0, #8
	bl 0x0200992c
	movs r1, #0
	bl 0x020098a4
	movs r0, #8
	movs r1, #10
	bl 0x0200997c
	b .L_020003f8_1
.L_020003f8_4:
	ldr r3, [pc, #124]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #21
	bne .L_020003f8_6
	bl 0x02008a0c
	b .L_020003f8_1
.L_020003f8_6:
	cmp r3, #20
	bne .L_020003f8_7
	ldr r0, [pc, #112]
	bl 0x020098f4
	bl 0x02008578
	b .L_020003f8_1
.L_020003f8_7:
	cmp r3, #22
	bne .L_020003f8_8
	bl 0x020093e4
	b .L_020003f8_1
.L_020003f8_8:
	ldr r5, [pc, #84]
	movs r2, #224
	ldr r3, [r5]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	ldr r0, [pc, #76]
	bl 0x020098ec
	cmp r0, #0
	beq .L_020003f8_9
	bl 0x02009a3c
	ldr r3, [r5, #12]
	ldr r2, [pc, #76]
	adds r3, r3, r2
	movs r2, #1
	strh r2, [r3]
	bl 0x02009a4c
	movs r0, #30
	bl 0x02009814
	bl 0x02009a5c
	bl 0x02009a6c
	bl 0x02009a54
	b .L_020003f8_1
.L_020003f8_9:
	bl 0x0200986c
	movs r0, #1
	bl 0x02009814
.L_020003f8_1:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000012f
	.4byte 0x03001ebc
	.4byte 0x00000834
	.4byte 0x0000087a
	.4byte 0x00000109
	.4byte 0x00000203
	.4byte 0x00001f84
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r2, [pc, #188]
	mov	r9, r2
	ldr	r3, [r2, #0]
	subs	r2, #76
	ldr	r7, [r2, #0]
	movs	r2, #240
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r0, #17
	ldr	r6, [r3, #0]
	bl 0x0200992c
	ldr	r0, [r0, #80]
	mov	r8, r0
	bl 0x0200990c
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r2, #0
	movs	r1, #0
	movs	r0, #16
	bl 0x02009974
	movs	r0, #0
	bl 0x0200992c
	movs	r1, #0
	bl 0x020098a4
	movs	r1, #18
	movs	r0, #0
	bl 0x0200997c
	movs	r3, #0
	mov	sl, r3
	ldr	r3, [pc, #76]
	mov	r2, r8
	strh	r3, [r2, #30]
	movs	r0, #17
	bl 0x0200992c
	ldr	r5, [pc, #56]
	adds	r0, #85
	strb	r5, [r0, #0]
.L_02000608:
	movs	r0, #17
	bl 0x0200992c
	movs	r1, #0
	bl 0x020098a4
	movs	r1, #144
	lsls	r1, r1, #18
	ldr	r2, [pc, #44]
	movs	r0, #17
.L_0200061c:
	bl 0x02009974
	movs	r0, #7
	bl 0x02009884
	movs	r2, #172
	ldr	r1, [pc, #32]
	lsls	r2, r2, #18
	movs	r0, #8
	bl 0x02009974
	bl 0x020098b4
	movs	r0, #8
	b.n	.L_02000650
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x03001ebc
	.4byte 0x00000555
	.4byte 0x028a0000
	.2byte 0x0000
	.2byte 0x0216
.L_02000650:
	bl 0x020099f4
	ldr	r5, [pc, #880]
	movs	r1, #1
.L_02000658:
	adds	r0, r5, #0
	movs	r2, #0
	bl 0x020098dc
	movs	r0, #40
	bl 0x02009904
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
.L_0200066c:
	lsls	r1, r1, #9
	lsls	r2, r2, #9
	lsls	r0, r0, #9
	bl 0x020098ac
	movs	r0, #8
	bl 0x020099f4
	movs	r1, #1
	adds	r0, r5, #1
.L_02000680:
	movs	r2, #0
	bl 0x020098dc
	bl 0x020098bc
	movs	r0, #40
	bl 0x02009904
	adds	r2, r7, #0
	movs	r3, #164
.L_02000694:
	adds	r2, #236
	lsls	r3, r3, #17
	str	r3, [r2, #0]
	movs	r3, #150
	adds	r2, #4
	lsls	r3, r3, #18
	str	r3, [r2, #0]
	movs	r3, #156
	adds	r2, #4
	lsls	r3, r3, #18
.L_020006a8:
	str	r3, [r2, #0]
	movs	r3, #204
	adds	r2, #4
	lsls	r3, r3, #18
	str	r3, [r2, #0]
	movs	r3, #141
	lsls	r3, r3, #18
	str	r3, [r6, #8]
	mov	r3, sl
	str	r3, [r6, #12]
.L_020006bc:
	ldr	r3, [pc, #780]
	str	r3, [r6, #16]
	bl 0x0200986c
	movs	r0, #1
	bl 0x02009814
	mov	r2, r9
	ldr	r1, [r2, #0]
	movs	r3, #224
.L_020006d0:
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #73
	str	r3, [r2, #0]
	subs	r3, #65
	adds	r2, r1, r3
	movs	r3, #64
	str	r3, [r2, #0]
	bl 0x02009a3c
	mov	r2, r9
	ldr	r3, [r2, #12]
	ldr	r2, [pc, #740]
	adds	r3, r3, r2
	movs	r2, #1
	strh	r2, [r3, #0]
	bl 0x02009a4c
	movs	r0, #30
	bl 0x02009814
	adds	r5, #2
	bl 0x02009a5c
	bl 0x02009a6c
	bl 0x02009a54
	movs	r1, #4
	movs	r0, #8
	bl 0x02009984
	adds	r0, r5, #0
	bl 0x020099c4
	movs	r2, #60
	ldr	r0, [pc, #696]
	movs	r1, #0
	bl 0x020099dc
	movs	r1, #2
	movs	r0, #0
	bl 0x0200999c
	movs	r0, #40
	bl 0x02009904
	movs	r1, #1
	movs	r0, #8
	bl 0x0200999c
	movs	r0, #40
	bl 0x02009904
	movs	r2, #20
	ldr	r0, [pc, #660]
	movs	r1, #0
	bl 0x020099dc
	movs	r1, #2
	movs	r0, #0
	bl 0x0200999c
	movs	r0, #7
	bl 0x0200988c
	movs	r0, #20
	bl 0x02009904
	movs	r0, #8
	bl 0x02009884
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r0, #0
	lsls	r1, r1, #9
	bl 0x02009934
	movs	r0, #0
	movs	r1, #19
	bl 0x0200997c
	ldr	r1, [pc, #608]
	ldr	r2, [pc, #608]
	movs	r0, #0
	bl 0x0200995c
	movs	r0, #8
	bl 0x0200988c
	movs	r0, #9
	bl 0x02009884
	movs	r2, #170
	ldr	r1, [pc, #592]
	lsls	r2, r2, #2
	movs	r0, #0
	bl 0x0200995c
	movs	r0, #30
	bl 0x02009904
	movs	r1, #208
	movs	r2, #0
	lsls	r1, r1, #8
	movs	r0, #8
	bl 0x020099ec
	movs	r0, #0
	bl 0x0200992c
	movs	r1, #1
	bl 0x020098a4
	movs	r0, #0
	movs	r1, #4
	movs	r2, #0
	bl 0x0200998c
	ldr	r2, [pc, #544]
	movs	r0, #0
	ldr	r1, [pc, #544]
	bl 0x0200996c
	movs	r0, #0
	movs	r1, #3
	bl 0x020099fc
	movs	r1, #128
	movs	r2, #40
	movs	r0, #0
	lsls	r1, r1, #7
	bl 0x020099ec
	movs	r1, #4
	movs	r0, #8
	bl 0x02009984
	movs	r0, #20
	bl 0x02009904
	ldr	r0, [pc, #484]
	movs	r1, #0
	bl 0x020099d4
	bl 0x020097e4
	movs	r0, #8
	movs	r1, #2
	bl 0x02009994
	movs	r1, #0
	movs	r2, #20
	ldr	r0, [pc, #460]
	bl 0x020099dc
	movs	r0, #8
	bl 0x0200992c
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r5, #254
	adds	r3, r5, #0
	ands	r3, r2
	movs	r2, #170
	lsls	r2, r2, #2
	strb	r3, [r0, #0]
	ldr	r1, [pc, #456]
	movs	r0, #8
	bl 0x0200996c
	movs	r0, #1
	bl 0x02009904
	movs	r0, #8
	bl 0x0200992c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r6, #1
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #10
	bl 0x02009904
	movs	r1, #2
	movs	r0, #8
	bl 0x0200999c
	movs	r0, #0
	bl 0x0200992c
	movs	r1, #226
	bl 0x020098c4
	movs	r0, #33
	bl 0x020098f4
	movs	r0, #126
	bl 0x02009aac
	movs	r1, #7
	movs	r0, #0
	bl 0x020099b4
	movs	r0, #10
	bl 0x02009904
	movs	r1, #0
	movs	r0, #0
	bl 0x020099b4
	movs	r0, #20
	bl 0x02009904
	movs	r0, #8
	bl 0x0200992c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	movs	r2, #172
	ands	r5, r3
	ldr	r1, [pc, #352]
	lsls	r2, r2, #2
	strb	r5, [r0, #0]
	movs	r0, #8
	bl 0x0200996c
	movs	r0, #1
	bl 0x02009904
	movs	r0, #8
	bl 0x0200992c
	adds	r0, #90
	ldrb	r3, [r0, #0]
	orrs	r3, r6
	strb	r3, [r0, #0]
	movs	r0, #20
	bl 0x02009904
	movs	r1, #192
	movs	r2, #192
	movs	r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009934
	movs	r1, #192
	movs	r2, #192
	lsls	r2, r2, #8
	movs	r0, #0
	lsls	r1, r1, #9
	bl 0x02009934
	movs	r1, #1
	movs	r0, #8
	bl 0x02009a14
	movs	r0, #0
	bl 0x0200992c
	adds	r0, #35
	ldrb	r3, [r0, #0]
	ldr	r5, [pc, #272]
	orrs	r6, r3
	adds	r1, r5, #0
	strb	r6, [r0, #0]
	movs	r0, #8
	bl 0x0200993c
	movs	r0, #20
	bl 0x02009904
	adds	r1, r5, #0
	movs	r0, #0
	bl 0x0200993c
	movs	r0, #8
	bl 0x02009944
	movs	r0, #8
	ldr	r1, [pc, #240]
	ldr	r2, [pc, #240]
	bl 0x0200996c
	movs	r1, #204
	ldr	r2, [pc, #232]
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x0200996c
	movs	r0, #8
	movs	r1, #1
	bl 0x0200997c
	movs	r0, #0
	movs	r1, #1
	bl 0x0200997c
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #7
	movs	r2, #10
	bl 0x020099ec
	movs	r1, #0
	ldr	r0, [pc, #196]
	bl 0x020099cc
	movs	r0, #0
	movs	r1, #0
	bl 0x02009924
	cmp	r0, #0
	bne.n	.L_0200095a
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #236
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_0200095a:
	movs	r0, #20
	bl 0x02009904
	movs	r2, #20
	ldr	r0, [pc, #156]
	movs	r1, #0
	bl 0x020099dc
	movs	r0, #0
	movs	r1, #3
	bl 0x0200997c
	movs	r1, #3
	movs	r0, #8
	bl 0x02009984
	movs	r0, #20
	bl 0x02009904
	ldr	r1, [pc, #128]
	movs	r0, #8
	bl 0x0200993c
	ldr	r1, [pc, #124]
	movs	r0, #0
	bl 0x0200993c
	movs	r0, #20
	bl 0x02009904
	mov	r2, r9
	ldr	r1, [r2, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r2, r1, r3
	adds	r3, #65
	str	r3, [r2, #0]
	subs	r3, #57
	adds	r2, r1, r3
	movs	r3, #16
	str	r3, [r2, #0]
	bl 0x02009a64
	bl 0x02009a6c
	movs	r0, #20
	bl 0x02009a34
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000e52
	.4byte 0x02b30000
	.4byte 0x00001f84
	.4byte 0x00009008
	.4byte 0x0000022d
	.4byte 0x000002a7
	.4byte 0x0000022b
	.4byte 0x000002a2
	.4byte 0x0000021f
	.4byte 0x0000021e
	.4byte 0x00000216
	.4byte 0x02009ab4
	.4byte 0x000001a3
	.4byte 0x00000295
	.4byte 0x00008008
	.4byte 0x02009b04
	.2byte 0x9b34
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #10
	sub	sp, #8
	bl 0x0200992c
	adds	r5, r0, #0
	ldr	r6, [r5, #80]
	bl 0x0200990c
	movs	r0, #11
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #12
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #13
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #14
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #15
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #16
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r0, #8
	ldr	r1, [pc, #868]
	ldr	r2, [pc, #868]
	bl 0x02009974
	movs	r2, #202
	lsls	r2, r2, #17
	ldr	r1, [pc, #864]
	movs	r0, #10
.L_02000a72:
	bl 0x02009974
	movs	r0, #10
	bl 0x0200992c
	movs	r1, #0
	bl 0x020098a4
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	adds	r5, #85
	movs	r2, #0
	strb	r3, [r1, #0]
	strb	r2, [r5, #0]
	movs	r3, #13
	ldrb	r2, [r6, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r6, #9]
	ldr	r1, [pc, #816]
	movs	r0, #10
	bl 0x0200993c
	ldr	r2, [pc, #812]
	ldr	r3, [r2, #0]
	mov	sl, r2
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r2, #65
	str	r2, [r3, #0]
	movs	r3, #4
	str	r3, [sp, #4]
	mov	r8, r3
	movs	r5, #5
	movs	r0, #83
	movs	r1, #15
	movs	r2, #83
	movs	r3, #19
	str	r5, [sp, #0]
	bl 0x0200987c
	mov	r2, r8
	str	r2, [sp, #4]
	movs	r0, #90
	movs	r1, #16
	movs	r2, #90
	movs	r3, #20
	str	r5, [sp, #0]
	bl 0x0200987c
	movs	r3, #7
	str	r3, [sp, #4]
	movs	r0, #77
	movs	r1, #23
	movs	r2, #82
	movs	r3, #23
	str	r5, [sp, #0]
	bl 0x0200987c
	movs	r5, #2
	movs	r0, #83
	movs	r1, #33
	movs	r2, #85
	movs	r3, #33
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200987c
	movs	r6, #1
	movs	r0, #91
	movs	r1, #28
	movs	r2, #90
	movs	r3, #28
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200987c
	movs	r0, #91
	movs	r1, #28
	movs	r2, #88
	movs	r3, #30
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	bl 0x0200987c
	movs	r3, #6
	str	r3, [sp, #0]
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r0, #94
	movs	r1, #27
	movs	r2, #94
	movs	r3, #23
	bl 0x0200987c
	mov	r2, r8
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #92
	movs	r1, #28
	movs	r2, #87
	movs	r3, #23
	bl 0x0200987c
	movs	r0, #65
	movs	r1, #53
	movs	r2, #88
	movs	r3, #24
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200987c
	bl 0x02009894
	ldr	r2, [pc, #632]
	ldr	r3, [pc, #632]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #632]
	ldr	r5, [pc, #636]
	strh	r3, [r5, #0]
	bl 0x02009a3c
	mov	r2, sl
	ldr	r3, [r2, #12]
	ldr	r2, [pc, #628]
	adds	r3, r3, r2
	strh	r6, [r3, #0]
	bl 0x02009a4c
	movs	r0, #30
	bl 0x02009814
	movs	r0, #8
	movs	r1, #1
	bl 0x02009a14
	movs	r1, #192
	movs	r2, #192
	movs	r0, #8
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009934
	movs	r1, #192
	movs	r2, #192
	movs	r0, #0
	lsls	r1, r1, #9
	lsls	r2, r2, #8
	bl 0x02009934
	movs	r1, #192
	movs	r2, #192
	lsls	r2, r2, #8
	movs	r0, #9
	lsls	r1, r1, #9
	bl 0x02009934
	ldr	r1, [pc, #564]
	movs	r0, #0
	bl 0x0200993c
	ldr	r1, [pc, #560]
	movs	r0, #8
	bl 0x0200993c
	bl 0x02009a5c
	movs	r0, #8
	bl 0x02009944
	movs	r0, #158
	bl 0x02009aac
	movs	r1, #128
	movs	r2, #0
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x02009a04
	movs	r0, #8
	movs	r1, #2
	bl 0x0200999c
	movs	r1, #128
	movs	r2, #10
	movs	r0, #8
	lsls	r1, r1, #7
	bl 0x020099ec
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #8
	bl 0x02009a1c
	movs	r0, #207
	movs	r1, #1
	movs	r3, #1
	lsls	r0, r0, #17
	negs	r1, r1
	ldr	r2, [pc, #488]
	bl 0x02009a24
	movs	r1, #207
	movs	r0, #9
	lsls	r1, r1, #17
	ldr	r2, [pc, #476]
	bl 0x02009974
	ldr	r1, [pc, #472]
	ldr	r2, [pc, #476]
	movs	r0, #9
	bl 0x0200996c
	bl 0x02009a2c
	ldr	r0, [pc, #468]
	bl 0x020099c4
	movs	r2, #10
	ldr	r0, [pc, #464]
	movs	r1, #0
	bl 0x020099dc
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x02009a1c
	movs	r0, #240
	movs	r1, #1
	movs	r2, #222
	movs	r3, #1
	negs	r1, r1
	lsls	r2, r2, #17
	lsls	r0, r0, #17
	bl 0x02009a24
	bl 0x02009a2c
	movs	r0, #20
	bl 0x02009904
	movs	r1, #128
	movs	r2, #20
	movs	r0, #8
	lsls	r1, r1, #8
	bl 0x020099ec
	movs	r0, #8
	movs	r1, #3
	bl 0x0200997c
	movs	r0, #0
	movs	r1, #3
	bl 0x02009984
	movs	r0, #9
	movs	r1, #3
	bl 0x02009984
	ldr	r2, [pc, #384]
	ldr	r1, [pc, #384]
	movs	r0, #9
	bl 0x02009964
	movs	r0, #10
	bl 0x02009904
	ldr	r1, [pc, #376]
	movs	r0, #8
	bl 0x0200993c
	ldr	r1, [pc, #372]
	movs	r0, #0
	bl 0x0200993c
	movs	r0, #234
	bl 0x02009aac
	movs	r0, #20
	bl 0x02009904
	ldr	r1, [pc, #356]
	movs	r0, #10
	bl 0x0200993c
	movs	r6, #0
.L_02000cc0:
	ldr	r2, [pc, #348]
	adds	r3, r6, r2
	strh	r3, [r5, #0]
	movs	r0, #1
	adds	r6, #1
	bl 0x02009814
	cmp	r6, #3
	bls.n	.L_02000cc0
	movs	r0, #202
	bl 0x02009aac
	movs	r0, #10
	bl 0x02009814
	ldr	r7, [pc, #324]
	ldr	r5, [pc, #260]
	movs	r6, #0
.L_02000ce4:
	subs	r3, r7, r6
	strh	r3, [r5, #0]
	movs	r0, #1
	adds	r6, #1
	bl 0x02009814
	cmp	r6, #15
	bls.n	.L_02000ce4
	movs	r0, #0
	bl 0x02009944
	movs	r0, #8
	movs	r1, #1
	bl 0x0200997c
	movs	r0, #8
	movs	r1, #2
	bl 0x02009994
	movs	r1, #2
	movs	r0, #0
	bl 0x0200999c
	movs	r0, #10
	bl 0x02009904
	movs	r1, #192
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020099ec
	movs	r1, #192
	movs	r2, #20
	movs	r0, #0
	lsls	r1, r1, #8
	bl 0x020099ec
	movs	r1, #129
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x02009a0c
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #0
	bl 0x02009a0c
	movs	r0, #80
	bl 0x02009904
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x02009974
	movs	r2, #20
	movs	r0, #8
	movs	r1, #0
	bl 0x020099ac
	movs	r0, #8
	movs	r1, #3
	bl 0x0200997c
	movs	r1, #3
	movs	r0, #0
	bl 0x02009984
	movs	r0, #40
	bl 0x02009904
	ldr	r0, [pc, #176]
	ldr	r1, [pc, #180]
	bl 0x02009a1c
	movs	r0, #8
	movs	r1, #1
	bl 0x02009a14
	ldr	r5, [pc, #168]
	movs	r0, #8
	adds	r1, r5, #0
	bl 0x0200993c
	movs	r0, #0
	adds	r1, r5, #0
	bl 0x0200994c
	ldr	r3, [pc, #64]
	ldr	r1, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r2, r1, r3
	subs	r3, #192
	str	r3, [r2, #0]
	adds	r3, #200
	adds	r2, r1, r3
	movs	r3, #32
	str	r3, [r2, #0]
	bl 0x02009a64
	bl 0x02009a6c
	movs	r0, #21
	bl 0x02009a34
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x01af0000
	.4byte 0x01870000
	.4byte 0x01cf0000
	.4byte 0x02009cec
	.4byte 0x03001ebc
	.4byte 0x00003f42
	.4byte 0x04000050
	.4byte 0x0000100c
	.4byte 0x04000052
	.4byte 0x00001f84
	.4byte 0x02009bb4
	.4byte 0x02009b78
	.4byte 0x02120000
	.4byte 0x000001ab
	.4byte 0x000001e3
	.4byte 0x00000e5b
	.4byte 0x00008009
	.4byte 0x0000024d
	.4byte 0x0000019f
	.4byte 0x02009c04
	.4byte 0x02009c54
	.4byte 0x02009d38
	.4byte 0x0000100e
	.4byte 0x0000100f
	.4byte 0x0000cccc
	.4byte 0x00001999
	.2byte 0x9ca4
	.2byte 0x0200
	.global Func_02000e34
	.thumb_func
Func_02000e34:
	push {r5, lr}
	movs r0, #0
	bl 0x0200992c
	ldr r2, [pc, #156]
	ldrh r3, [r0, #6]
	movs r5, #144
	adds r3, r3, r2
	lsls r5, r5, #8
	cmp r3, r5
	bls .L_02000e34_0
	movs r0, #0
	movs r1, #13
	bl 0x02009aa4
	b .L_02000e34_1
.L_02000e34_0:
	bl 0x0200990c
	ldr r0, [pc, #132]
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000e34_2
	movs r0, #13
	movs r1, #2
	bl 0x0200999c
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl 0x020099a4
	movs r0, #192
	lsls r0, r0, #2
	bl 0x020098ec
	cmp r0, #0
	bne .L_02000e34_3
	ldr r0, [pc, #96]
	bl 0x020099c4
	movs r0, #13
	movs r1, #0
	bl 0x020099d4
	movs r0, #192
	lsls r0, r0, #2
	bl 0x020098f4
.L_02000e34_3:
	ldr r0, [pc, #80]
	bl 0x020099c4
	movs r1, #0
	movs r0, #13
	bl 0x020099e4
	movs r0, #13
	adds r1, r5, #0
	movs r2, #10
	bl 0x020099ec
	b .L_02000e34_4
.L_02000e34_2:
	ldr r0, [pc, #56]
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000e34_5
	ldr r0, [pc, #52]
	bl 0x020099c4
	b .L_02000e34_6
.L_02000e34_5:
	ldr r0, [pc, #48]
	bl 0x020099c4
.L_02000e34_6:
	movs r0, #13
	movs r1, #0
	bl 0x020099d4
.L_02000e34_4:
	bl 0x02009914
.L_02000e34_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffffe000
	.4byte 0x0000087a
	.4byte 0x00001c14
	.4byte 0x00001c15
	.4byte 0x00000815
	.4byte 0x000011a9
	.4byte 0x00000f58
	.global Func_02000ef8
	.thumb_func
Func_02000ef8:
	push {lr}
	bl 0x0200990c
	movs r2, #10
	movs r1, #0
	movs r0, #16
	bl 0x020099a4
	ldr r0, [pc, #36]
	bl 0x020099c4
	movs r0, #16
	movs r1, #0
	bl 0x020099d4
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #10
	movs r0, #16
	bl 0x020099ec
	ldr r0, [pc, #16]
	bl 0x020098f4
	bl 0x02009914
	pop {r0}
	bx r0
	.4byte 0x00001c13
	.4byte 0x00000301
	.global Func_02000f38
	.thumb_func
Func_02000f38:
	push {lr}
	bl 0x0200990c
	ldr r0, [pc, #28]
	bl 0x020099c4
	movs r1, #0
	movs r0, #13
	bl 0x020099d4
	ldr r0, [pc, #16]
	bl 0x020098f4
	bl 0x02009914
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001c1b
	.4byte 0x0000081c
	.global Func_02000f64
	.thumb_func
Func_02000f64:
	push {lr}
	bl 0x0200990c
	ldr r0, [pc, #28]
	bl 0x020099c4
	movs r1, #0
	movs r0, #16
	bl 0x020099d4
	ldr r0, [pc, #16]
	bl 0x020098f4
	bl 0x02009914
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001c1a
	.4byte 0x0000081c
	.global Func_02000f90
	.thumb_func
Func_02000f90:
	push {r5, lr}
	bl 0x0200990c
	movs r0, #0
	ldr r1, [pc, #848]
	ldr r2, [pc, #852]
	bl 0x02009934
	movs r0, #0
	ldr r1, [pc, #848]
	ldr r2, [pc, #848]
	bl 0x0200996c
	movs r1, #128
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #7
	bl 0x020099ec
	movs r1, #2
	movs r0, #8
	bl 0x02009994
	ldr r0, [pc, #828]
	bl 0x020099c4
	movs r0, #8
	movs r1, #0
	movs r2, #80
	bl 0x020099dc
	movs r2, #60
	movs r0, #8
	ldr r1, [pc, #812]
	bl 0x02009a04
	movs r0, #8
	movs r1, #1
	bl 0x02009994
	movs r2, #60
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #2
	movs r0, #8
	bl 0x0200999c
	movs r0, #80
	bl 0x02009904
	movs r0, #8
	ldr r1, [pc, #776]
	ldr r2, [pc, #776]
	bl 0x02009934
	movs r1, #146
	movs r2, #203
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #8
	bl 0x02009954
	movs r0, #11
	bl 0x0200988c
	movs r0, #12
	bl 0x02009884
	movs r1, #12
	movs r0, #8
	bl 0x0200997c
	movs r0, #80
	bl 0x02009904
	movs r1, #2
	movs r0, #8
	bl 0x0200999c
	movs r0, #40
	bl 0x02009904
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x020099dc
	movs r1, #132
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #40
	bl 0x02009a04
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x020099dc
	movs r2, #60
	movs r0, #0
	ldr r1, [pc, #684]
	bl 0x02009a04
	movs r0, #8
	movs r1, #13
	bl 0x02009984
	movs r2, #0
	movs r0, #8
	ldr r1, [pc, #672]
	bl 0x02009a04
	movs r1, #11
	movs r0, #8
	bl 0x0200997c
	movs r0, #40
	bl 0x02009904
	movs r2, #40
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #1
	movs r0, #8
	bl 0x0200999c
	movs r0, #20
	bl 0x02009904
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #12
	movs r0, #8
	bl 0x02009984
	movs r0, #20
	bl 0x02009904
	movs r1, #129
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #1
	bl 0x02009a04
	movs r0, #8
	movs r1, #13
	bl 0x0200997c
	movs r1, #0
	movs r0, #8
	bl 0x020099cc
	movs r0, #0
	movs r1, #0
	bl 0x02009924
	cmp r0, #1
	bne .L_02000f90_0
	ldr r3, [pc, #568]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000f90_0:
	ldr r0, [pc, #556]
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000f90_1
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02009a04
.L_02000f90_1:
	movs r0, #20
	bl 0x02009904
	movs r0, #8
	movs r1, #0
	bl 0x020099d4
	ldr r1, [pc, #524]
	movs r2, #60
	movs r0, #8
	bl 0x02009a04
	ldr r5, [pc, #516]
	adds r0, r5, #0
	bl 0x020099c4
	movs r1, #0
	movs r0, #8
	bl 0x020099cc
	movs r0, #0
	movs r1, #0
	bl 0x02009924
	cmp r0, #1
	bne .L_02000f90_2
	ldr r3, [pc, #476]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000f90_2:
	ldr r0, [pc, #464]
	bl 0x020098ec
	cmp r0, #0
	beq .L_02000f90_3
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02009a04
.L_02000f90_3:
	movs r0, #20
	bl 0x02009904
	movs r0, #8
	movs r1, #0
	bl 0x020099d4
	movs r2, #60
	ldr r1, [pc, #432]
	movs r0, #8
	bl 0x02009a04
	adds r0, r5, #3
	bl 0x020099c4
	movs r0, #8
	movs r1, #0
	bl 0x020099d4
	movs r1, #1
	movs r0, #8
	bl 0x0200999c
	movs r0, #20
	bl 0x02009904
	movs r0, #8
	movs r1, #13
	bl 0x02009984
	movs r0, #8
	movs r1, #2
	bl 0x02009994
	movs r2, #40
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #1
	movs r0, #8
	bl 0x0200999c
	movs r0, #20
	bl 0x02009904
	movs r2, #40
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #2
	movs r0, #8
	bl 0x0200999c
	movs r0, #40
	bl 0x02009904
	movs r0, #0
	bl 0x0200992c
	movs r3, #0
	strh r3, [r0, #6]
	movs r0, #1
	bl 0x02009814
	movs r0, #0
	bl 0x0200992c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r2, #194
	strb r3, [r0]
	ldr r1, [pc, #304]
	movs r0, #0
	lsls r2, r2, #1
	bl 0x02009954
	ldr r2, [pc, #240]
	movs r0, #8
	ldr r1, [pc, #296]
	bl 0x02009934
	movs r0, #8
	movs r1, #14
	bl 0x0200997c
	movs r2, #200
	ldr r1, [pc, #284]
	lsls r2, r2, #1
	movs r0, #8
	bl 0x0200995c
	movs r0, #40
	bl 0x02009904
	movs r1, #145
	movs r2, #191
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200996c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #40
	movs r0, #8
	bl 0x020099ec
	movs r0, #0
	bl 0x0200992c
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #192
	strb r3, [r0]
	lsls r1, r1, #8
	movs r0, #8
	movs r2, #8
	bl 0x020099ec
	movs r0, #8
	movs r1, #0
	movs r2, #8
	bl 0x020099ec
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #8
	bl 0x020099ec
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #10
	bl 0x020099ec
	movs r0, #8
	movs r1, #4
	movs r2, #20
	bl 0x0200998c
	movs r0, #8
	movs r1, #6
	movs r2, #40
	bl 0x0200998c
	movs r0, #8
	movs r1, #4
	movs r2, #20
	bl 0x0200998c
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x020099dc
	movs r0, #8
	ldr r1, [pc, #100]
	ldr r2, [pc, #140]
	bl 0x02009934
	movs r1, #143
	movs r2, #192
	movs r0, #8
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl 0x0200996c
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x020099dc
	movs r1, #3
	movs r0, #0
	bl 0x02009984
	movs r0, #20
	bl 0x02009904
	movs r1, #3
	movs r0, #8
	bl 0x02009984
	ldr r0, [pc, #92]
	bl 0x020098f4
	ldr r0, [pc, #88]
	bl 0x020098f4
	bl 0x02009914
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00009999
	.4byte 0x00004ccc
	.4byte 0x00000239
	.4byte 0x00000189
	.4byte 0x00001c66
	.4byte 0x00000101
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x00000105
	.4byte 0x00000103
	.4byte 0x03001ebc
	.4byte 0x0000081c
	.4byte 0x00000107
	.4byte 0x00001c6f
	.4byte 0x0000022e
	.4byte 0x00013333
	.4byte 0x0000024a
	.4byte 0x00003333
	.4byte 0x0000081e
	.4byte 0x00000203
	.global Func_0200133c
	.thumb_func
Func_0200133c:
	push {r5, lr}
	bl 0x0200990c
	ldr r0, [pc, #96]
	bl 0x020098ec
	cmp r0, #0
	beq .L_0200133c_0
	movs r1, #128
	lsls r1, r1, #9
	ldr r2, [pc, #84]
	movs r0, #8
	bl 0x020099bc
	movs r0, #20
	bl 0x02009904
	ldr r0, [pc, #76]
	bl 0x020099c4
	movs r0, #8
	movs r1, #0
	bl 0x020099d4
	b .L_0200133c_1
.L_0200133c_0:
	movs r1, #2
	movs r0, #8
	bl 0x0200999c
	movs r0, #40
	bl 0x02009904
	ldr r5, [pc, #48]
	adds r0, r5, #0
	bl 0x020099c4
	adds r5, #1
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x020099dc
	adds r0, r5, #0
	movs r1, #1
	bl 0x020098d4
.L_0200133c_1:
	bl 0x02009914
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000203
	.4byte 0x02009e6c
	.4byte 0x00001c77
	.4byte 0x00001c79
	.global Func_020013b4
	.thumb_func
Func_020013b4:
	push {lr}
	sub sp, #8
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #22
	movs r1, #85
	movs r2, #25
	movs r3, #85
	bl 0x0200987c
	movs r3, #25
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #25
	movs r1, #15
	movs r2, #2
	movs r3, #2
	bl 0x0200989c
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020013e4
	.thumb_func
Func_020013e4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl 0x0200990c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl 0x02009a24
	movs r0, #1
	bl 0x02009814
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl 0x02009974
	movs r1, #240
	movs r2, #202
	lsls r2, r2, #16
	lsls r1, r1, #17
	movs r0, #18
	bl 0x02009974
	movs r0, #1
	bl 0x02009814
	movs r0, #18
	movs r1, #1
	bl 0x02009a14
	movs r1, #164
	movs r2, #128
	movs r3, #195
	lsls r1, r1, #17
	lsls r2, r2, #10
	lsls r3, r3, #16
	movs r0, #22
	bl 0x0200985c
	mov r8, r0
	mov r3, r8
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	mov r2, r8
	ldr r6, [r2, #80]
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r2, #12]
	adds r3, r6, #0
	adds r3, #39
	strb r5, [r3]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r1, #193
	strb r3, [r6, #9]
	lsls r1, r1, #3
	movs r0, #17
	bl 0x0200982c
	adds r5, r0, #0
	movs r0, #224
	bl 0x020098e4
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	movs r1, #128
	adds r2, r5, #0
	ldrb r0, [r6, #28]
	bl 0x02009844
	movs r0, #17
	bl 0x02009834
	ldr r3, [pc, #164]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #66
	str r2, [r3]
	bl 0x02009a5c
	movs r1, #128
	movs r2, #128
	movs r0, #18
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009934
	movs r1, #240
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #176
	bl 0x0200996c
	movs r1, #210
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #164
	bl 0x0200996c
	movs r1, #163
	movs r0, #18
	lsls r1, r1, #1
	movs r2, #185
	bl 0x0200996c
	movs r1, #128
	movs r2, #10
	movs r0, #18
	lsls r1, r1, #7
	bl 0x020099ec
	ldr r1, [pc, #88]
	mov r0, r8
	bl 0x02009854
	mov r0, r8
	bl 0x02009874
	ldr r1, [pc, #76]
	mov r0, r8
	bl 0x02009854
	mov r0, r8
	bl 0x02009874
	movs r0, #20
	bl 0x02009904
	mov r0, r8
	bl 0x02009864
	movs r0, #18
	movs r1, #2
	movs r2, #20
	bl 0x0200998c
	movs r0, #18
	movs r1, #0
	movs r2, #40
	bl 0x020099ec
	bl 0x02009a64
	bl 0x02009a6c
	movs r0, #22
	bl 0x02009a34
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02009eac
	.4byte 0x02009ecc
	.global Func_02001544
	.thumb_func
Func_02001544:
	push {r5, lr}
	ldr r3, [pc, #52]
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	adds r5, r0, #0
	cmp r3, #0
	beq .L_02001544_0
	movs r1, #7
	bl 0x020098cc
	b .L_02001544_1
.L_02001544_0:
	adds r0, r5, #0
	movs r1, #0
	bl 0x020098cc
.L_02001544_1:
	ldr r3, [pc, #20]
	ldr r3, [r3]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02001544_2
	adds r0, r5, #0
	bl 0x0200968c
.L_02001544_2:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_02001580
	.thumb_func
Func_02001580:
	push {r5, r6, lr}
	ldr r5, [pc, #52]
	ldr r3, [r5]
	movs r2, #1
	ands r3, r2
	adds r6, r0, #0
	cmp r3, #0
	beq .L_02001580_0
	ldr r0, [r5]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200980c
	adds r1, r0, #0
	adds r0, r6, #0
	bl 0x020098cc
.L_02001580_0:
	ldr r3, [r5]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02001580_1
	adds r0, r6, #0
	bl 0x0200968c
.L_02001580_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_020015bc
	.thumb_func
Func_020015bc:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, [pc, #32]
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020015bc_0
	ldr r0, [r0]
	movs r1, #6
	lsrs r0, r0, #1
	bl 0x0200980c
	adds r1, r0, #0
	adds r0, r5, #0
	bl 0x020098cc
.L_020015bc_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.global Func_020015e8
	.thumb_func
Func_020015e8:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	ldr r6, [r5, #104]
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_020015e8_0
	adds r0, r5, #0
	bl 0x02009864
	b .L_020015e8_1
.L_020015e8_0:
	lsls r0, r0, #10
	bl 0x02009824
	str r0, [r5, #24]
	str r0, [r5, #28]
	ldr r3, [r6, #8]
	movs r1, #128
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #16]
.L_020015e8_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02001638
	.thumb_func
Func_02001638:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	ldr r6, [r5, #104]
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_02001638_0
	adds r0, r5, #0
	bl 0x02009864
	b .L_02001638_1
.L_02001638_0:
	lsls r0, r0, #10
	bl 0x02009824
	negs r3, r0
	str r0, [r5, #24]
	str r3, [r5, #28]
	ldr r3, [r6, #8]
	movs r1, #128
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #16]
.L_02001638_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_0200168c
	.thumb_func
Func_0200168c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #72]
	ldr r3, [r3]
	sub sp, #8
	movs r1, #63
	adds r6, r0, #0
	mov r11, r3
	movs r7, #0
	mov r10, sp
	mov r9, r1
.L_0200168c_2:
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	ldr r1, [r6, #8]
	movs r0, #26
	bl 0x0200985c
	lsls r3, r7, #2
	mov r2, r10
	str r0, [r3, r2]
	cmp r0, #0
	beq .L_0200168c_0
	ldr r3, [r6, #20]
	str r3, [r0, #20]
	adds r3, r0, #0
	ldr r5, [r0, #80]
	adds r3, #85
	movs r2, #0
	ldr r1, [pc, #16]
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	mov r8, r1
	str r6, [r0, #104]
	cmp r5, #0
	beq .L_0200168c_0
	b .L_0200168c_1
	.4byte 0x00000000
	.4byte 0x03001f30
.L_0200168c_1:
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200984c
	adds r3, r5, #0
	adds r3, #38
	mov r2, r8
	strb r2, [r3]
	ldrb r0, [r5, #28]
	bl 0x0200983c
	mov r3, r11
	adds r3, #70
	ldrh r3, [r3]
	strb r3, [r5, #28]
	ldrb r3, [r5, #29]
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #29]
	ldrb r3, [r5, #28]
	ldr r2, [pc, #64]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r1, [r3, #2]
	ldr r2, [pc, #52]
	ldrh r3, [r5, #8]
	lsls r1, r1, #17
	lsrs r1, r1, #22
	ands r3, r2
	orrs r3, r1
	movs r1, #33
	negs r1, r1
	strh r3, [r5, #8]
	ldrb r3, [r5, #5]
	adds r2, r1, #0
	ands r3, r2
	mov r2, r9
	ands r3, r2
	movs r2, #64
	orrs r3, r2
	ldrb r2, [r5, #7]
	strb r3, [r5, #5]
	mov r3, r9
	ands r3, r2
	movs r2, #128
	orrs r3, r2
	strb r3, [r5, #7]
	ldr r3, [r5, #40]
	mov r1, r8
	strb r1, [r3, #22]
	b .L_0200168c_0
	.2byte 0x0000
	.4byte 0xfffffc00
	.4byte 0x03001b10
.L_0200168c_0:
	adds r7, #1
	cmp r7, #1
	ble .L_0200168c_2
	ldr r2, [sp, #0]
	ldr r3, [pc, #60]
	ldr r0, [r2, #80]
	str r3, [r2, #108]
	movs r2, #13
	ldrb r1, [r0, #9]
	negs r2, r2
	adds r3, r2, #0
	movs r4, #8
	ands r3, r1
	orrs r3, r4
	strb r3, [r0, #9]
	mov r3, r10
	ldr r1, [r3, #4]
	ldr r0, [r1, #80]
	ldrb r3, [r0, #9]
	ands r2, r3
	ldr r3, [pc, #32]
	orrs r2, r4
	str r3, [r1, #108]
	adds r1, #35
	movs r3, #2
	strb r2, [r0, #9]
	strb r3, [r1]
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02009639
	.4byte 0x020095e9
	.global Func_020017a8
	.thumb_func
Func_020017a8:
	push {lr}
	movs r0, #140
	movs r1, #0
	bl 0x02009a7c
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020017b8
	.thumb_func
Func_020017b8:
	push {lr}
	bl 0x02009a94
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020017c4
	.thumb_func
Func_020017c4:
	push {lr}
	movs r0, #8
	bl 0x0200992c
	bl 0x02009580
	pop {r0}
	bx r0
	.global Func_020017d4
	.thumb_func
Func_020017d4:
	push {lr}
	movs r0, #17
	bl 0x0200992c
	bl 0x020095bc
	pop {r0}
	bx r0
	.global Func_020017e4
	.thumb_func
Func_020017e4:
	push {lr}
	movs r0, #148
	movs r1, #1
	bl 0x02009a7c
	movs r1, #17
	movs r0, #8
	bl 0x02009a84
	bl 0x02009a9c
	movs r0, #1
	bl 0x02009a74
	bl 0x02009a8c
	bl 0x02009a94
	pop {r0}
	bx r0
	.include "games/THE BROKEN SEAL/SRC/FIELD/COMMON/HAIDIA_BABI/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x020a0000
	.4byte 0x00000000
	.4byte 0x02ec0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01a30000
	.4byte 0x00000000
	.4byte 0x02ec0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01a30000
	.4byte 0x00000000
	.4byte 0x02b30000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01830000
	.4byte 0x00000000
	.4byte 0x02950000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02950000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01830000
	.4byte 0x00000000
	.4byte 0x02950000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x01860000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01fc0000
	.4byte 0x00000000
	.4byte 0x01920000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01af0000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x01860000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01e60000
	.4byte 0x00000000
	.4byte 0x01890000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x020d0000
	.4byte 0x00000000
	.4byte 0x019e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01fd0000
	.4byte 0x00000000
	.4byte 0x01ba0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01c70000
	.4byte 0x00000000
	.4byte 0x01c20000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x020d0000
	.4byte 0x00000000
	.4byte 0x019e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01fd0000
	.4byte 0x00000000
	.4byte 0x01ba0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01e10000
	.4byte 0x00000000
	.4byte 0x01bb0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01ac0000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x019f0000
	.4byte 0x00000000
	.4byte 0x02010000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x019f0000
	.4byte 0x00000000
	.4byte 0x024d0000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e666
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00013333
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000051e
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000051e
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000008
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffffe667
	.4byte 0x00000016
	.4byte 0x00000012
	.4byte 0xffffe667
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffd71
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000a3d
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000000e
	.4byte 0xc0020000
	.4byte 0x00000015
	.4byte 0x00000006
	.4byte 0x00080000
	.4byte 0x00000015
	.4byte 0x00000012
	.4byte 0x00080000
	.4byte 0x80030000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00007333
	.4byte 0x00000016
	.4byte 0x00000012
	.4byte 0x00007333
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000041
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffae2
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000028
	.4byte 0xc0030000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x02370000
	.4byte 0x00000000
	.4byte 0x02b20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x021f0000
	.4byte 0x00000000
	.4byte 0x02a20000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000a0
	.4byte 0xc00000e4
	.4byte 0x00180000
	.4byte 0x00f80000
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000001e0
	.4byte 0xc00000b0
	.4byte 0x01080000
	.4byte 0x02300008
	.4byte 0x000000f0
	.4byte 0xffff0003
	.4byte 0x000002d1
	.4byte 0xc00000c0
	.4byte 0x02480000
	.4byte 0x03500000
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000090
	.4byte 0xc00001ee
	.4byte 0x00080000
	.4byte 0x01400100
	.4byte 0x00000210
	.4byte 0xffff0005
	.4byte 0x00000077
	.4byte 0x40000154
	.4byte 0x00080000
	.4byte 0x01400100
	.4byte 0x00000210
	.4byte 0xffff0006
	.4byte 0x000001a0
	.4byte 0xc0000210
	.4byte 0x01580000
	.4byte 0x02800110
	.4byte 0x00000230
	.4byte 0xffff0007
	.4byte 0x00000198
	.4byte 0x40000173
	.4byte 0x01580000
	.4byte 0x02800110
	.4byte 0x00000230
	.4byte 0xffff0008
	.4byte 0x000000b8
	.4byte 0x00000279
	.4byte 0x00080000
	.4byte 0x01400240
	.4byte 0x000002e0
	.4byte 0xffff0009
	.4byte 0x00000198
	.4byte 0x00000299
	.4byte 0x01480000
	.4byte 0x02700258
	.4byte 0x00000330
	.4byte 0xffff000a
	.4byte 0x00000210
	.4byte 0x400002ab
	.4byte 0x01480000
	.4byte 0x02700258
	.4byte 0x00000330
	.4byte 0xffff000b
	.4byte 0x000002c1
	.4byte 0xc0000331
	.4byte 0x02800000
	.4byte 0x03b80228
	.4byte 0x00000360
	.4byte 0xffff000c
	.4byte 0x0000036f
	.4byte 0xc00002fd
	.4byte 0x02800000
	.4byte 0x03b80228
	.4byte 0x00000360
	.4byte 0xffff0010
	.4byte 0x0000021a
	.4byte 0x400002ab
	.4byte 0x01480000
	.4byte 0x02700258
	.4byte 0x00000330
	.4byte 0xffff0013
	.4byte 0x000002f8
	.4byte 0x40000148
	.4byte 0x02a00000
	.4byte 0x03c00110
	.4byte 0x000001e8
	.4byte 0xffff0014
	.4byte 0x00000237
	.4byte 0x000002a4
	.4byte 0x04000000
	.4byte 0x04000240
	.4byte 0x00000240
	.4byte 0xffff0015
	.4byte 0x00000198
	.4byte 0x40000173
	.4byte 0x01580000
	.4byte 0x02800110
	.4byte 0x00000230
	.4byte 0xffff0016
	.4byte 0x000001e0
	.4byte 0xc00000b0
	.4byte 0x01080000
	.4byte 0x02300008
	.4byte 0x000000f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000008
	.4byte 0x00106005
	.4byte 0x00203006
	.4byte 0x00302005
	.4byte 0x00407005
	.4byte 0x00508008
	.4byte 0x00608004
	.4byte 0x00709008
	.4byte 0x00805008
	.4byte 0x00907008
	.4byte 0x00a0c003
	.4byte 0x00b0d003
	.4byte 0x0130b08a
	.4byte 0x01415008
	.4byte 0x0150f003
	.4byte 0x0160a006
	.4byte 0x000001ff
	.4byte 0x00000008
	.4byte 0x00106005
	.4byte 0x00203006
	.4byte 0x00302005
	.4byte 0x00407005
	.4byte 0x00508008
	.4byte 0x00608003
	.4byte 0x00709008
	.4byte 0x00805008
	.4byte 0x00907008
	.4byte 0x00a0c003
	.4byte 0x00b0d003
	.4byte 0x01415008
	.4byte 0x0150f003
	.4byte 0x000001ff
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff00d0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00970000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x00000078
	.4byte 0x00000002
	.4byte 0x00570000
	.4byte 0x00000000
	.4byte 0x00430000
	.4byte 0x00008000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00018000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x009f0000
	.4byte 0x00000000
	.4byte 0x01c40000
	.4byte 0x0000b000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x00011000
	.4byte 0x00000022
	.4byte 0x00000001
	.4byte 0x00360000
	.4byte 0x00000000
	.4byte 0x029d0000
	.4byte 0x00018000
	.4byte 0xffff00e2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002e000
	.4byte 0xffff001e
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
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x000000d0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00970000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x00000078
	.4byte 0x00000002
	.4byte 0x00570000
	.4byte 0x00000000
	.4byte 0x00430000
	.4byte 0x00008000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00008000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x009f0000
	.4byte 0x00000000
	.4byte 0x01c40000
	.4byte 0x0000b000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x00011000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x024a0000
	.4byte 0x00000000
	.4byte 0x01960000
	.4byte 0x00020000
	.4byte 0x00000035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x000000d0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00970000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x00000078
	.4byte 0x00000002
	.4byte 0x00570000
	.4byte 0x00000000
	.4byte 0x00430000
	.4byte 0x00008000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00008000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x009f0000
	.4byte 0x00000000
	.4byte 0x01c40000
	.4byte 0x0000b000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x00011000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x0001b000
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x017a0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x0000c000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00010000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x03400000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200831d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008359
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200836d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008381
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008395
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020083a9
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x020083bd
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020083d1
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020083e5
	.4byte 0x000000d3
	.4byte 0xffff0064
	.4byte 0x00400955
	.4byte 0x00000023
	.4byte 0xffff0065
	.4byte 0x0040094a
	.4byte 0x00000033
	.4byte 0xffff0066
	.4byte 0x0040094b
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00000f56
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00000f57
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00000f59
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00000f5a
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x020081e1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008e35
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200831d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008359
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200836d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008381
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008395
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020083a9
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x020083bd
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020083d1
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020083e5
	.4byte 0x000000d3
	.4byte 0x0f470064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f480065
	.4byte 0x00200005
	.4byte 0x00000033
	.4byte 0x0f490066
	.4byte 0x00200001
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000011a7
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000011a8
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008285
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000011af
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008e35
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000011de
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000011df
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000011e1
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000011e2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000011e0
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200831d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008359
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200836d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008381
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008395
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020083a9
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x020083bd
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020083d1
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020083e5
	.4byte 0x000000d3
	.4byte 0x0f470064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f480065
	.4byte 0x00200005
	.4byte 0x00000033
	.4byte 0x0f490066
	.4byte 0x00200001
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001c0b
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001c0c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001c18
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001c19
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008e35
	.4byte 0x00000000
	.4byte 0x03000010
	.4byte 0x02008ef9
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001c13
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001c10
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001c11
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001c1c
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001c1d
	.4byte 0x00008d15
	.4byte 0x0300040d
	.4byte 0x02008e35
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x02008f39
	.4byte 0x00008d15
	.4byte 0x03010410
	.4byte 0x02008ef9
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x02008f65
	.4byte 0x00000000
	.4byte 0x081e0008
	.4byte 0x02008f91
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200933d
	.4byte 0x00008d15
	.4byte 0x081e0408
	.4byte 0x02008f91
	.4byte 0x00008d15
	.4byte 0x02030008
	.4byte 0x00001c7b
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001c78
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x0200831d
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte 0x02008331
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte 0x02008345
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02008359
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x0200836d
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte 0x02008381
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte 0x02008395
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte 0x020083a9
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x020083bd
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x020083d1
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020083e5
	.4byte 0x000000d3
	.4byte 0x0f470064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f480065
	.4byte 0x00200005
	.4byte 0x00000033
	.4byte 0x0f490066
	.4byte 0x00200001
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0x00000023
	.4byte 0x08ad006c
	.4byte 0x001000b6
	.4byte 0x00000033
	.4byte 0x08ae006b
	.4byte 0x0020007b
	.4byte 0x00000003
	.4byte 0x08af006d
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002017
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002018
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002019
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000201a
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000201b
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000201c
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000201d
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000201e
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000201f
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002020
	.4byte 0x00000021
	.4byte 0xffff0014
	.4byte 0x00000013
	.4byte 0x00000023
	.4byte 0x0f9e0067
	.4byte 0x001000c2
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029b7
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029b8
	.4byte 0x000000f3
	.4byte 0xffff00d1
	.4byte 0x004029b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000022b8
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008119
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000022bc
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000022bd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000022be
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000022bf
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000022c0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000022c1
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000022c2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000022c3
	.4byte 0x00000021
	.4byte 0xffff0014
	.4byte 0x00000013
	.4byte 0x00000023
	.4byte 0x0f9e0067
	.4byte 0x001000c2
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029b7
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029b8
	.4byte 0x000000f3
	.4byte 0xffff00d1
	.4byte 0x004029b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
