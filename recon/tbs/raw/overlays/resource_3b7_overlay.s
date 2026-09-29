.syntax unified
	.thumb
	.section .text.x020081d8,"ax",%progbits
	.balign 4
	.global Func_020001d8
	.thumb_func
Func_020001d8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r6, [pc, #652]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #644]
	cmp r2, r3
	bne .L_020001d8_0
	ldr r3, [pc, #644]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	str r2, [r3]
	ldr r3, [pc, #632]
	ldr r1, [pc, #636]
	mov r10, r3
	mov r8, r1
	mov r2, r10
	mov r3, r8
	strh r2, [r3]
	ldr r3, [pc, #628]
	ldr r7, [pc, #628]
	strh r3, [r7]
	movs r0, #24
	movs r1, #2
	bl 0x02009954
	movs r1, #2
	movs r0, #25
	bl 0x02009954
	movs r0, #24
	bl 0x0200993c
	ldr r5, [pc, #608]
	str r5, [r0, #24]
	movs r0, #25
	bl 0x0200993c
	str r5, [r0, #24]
	movs r0, #24
	bl 0x0200993c
	movs r5, #2
	adds r0, #35
	strb r5, [r0]
	movs r0, #25
	bl 0x0200993c
	adds r0, #35
	strb r5, [r0]
	bl 0x02009994
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_020001d8_1
	b .L_020001d8_2
.L_020001d8_1:
	bl 0x020096a8
	movs r0, #128
	lsls r0, r0, #2
	bl 0x020098e4
	cmp r0, #0
	bne .L_020001d8_3
	b .L_020001d8_2
.L_020001d8_3:
	mov r3, r10
	mov r1, r8
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r7]
	b .L_020001d8_2
.L_020001d8_0:
	movs r0, #149
	lsls r0, r0, #4
	bl 0x020098e4
	cmp r0, #0
	beq .L_020001d8_4
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl 0x0200994c
.L_020001d8_4:
	ldr r2, [pc, #504]
	movs r3, #1
	strb r3, [r2]
	ldr r3, [pc, #472]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	movs r3, #225
	lsls r3, r3, #1
	adds r5, r6, r3
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #10
	bne .L_020001d8_5
	movs r0, #8
	movs r1, #1
	bl 0x0200995c
	movs r0, #9
	movs r1, #2
	bl 0x0200995c
	ldrh r2, [r5]
.L_020001d8_5:
	lsls r3, r2, #16
	movs r2, #208
	lsls r2, r2, #12
	cmp r3, r2
	bne .L_020001d8_6
	ldr r0, [pc, #444]
	bl 0x020098e4
	cmp r0, #0
	bne .L_020001d8_6
	bl 0x0200991c
	movs r0, #8
	movs r1, #1
	bl 0x0200995c
	movs r1, #2
	movs r0, #9
	bl 0x0200995c
	bl 0x02009994
	bl 0x0200999c
	movs r0, #10
	bl 0x02009914
	movs r2, #112
	movs r0, #0
	movs r1, #120
	bl 0x02009944
	movs r0, #20
	bl 0x02009914
	ldr r3, [pc, #388]
	ldr r2, [r6, #16]
	ldr r3, [r3]
	subs r5, r2, r3
	cmp r5, #0
	ble .L_020001d8_7
	ldr r3, [pc, #380]
	cmp r5, r3
	ble .L_020001d8_8
	movs r0, #93
	bl 0x020099c4
	b .L_020001d8_9
.L_020001d8_8:
	ldr r1, [pc, #368]
	cmp r5, r1
	ble .L_020001d8_10
	movs r0, #92
	bl 0x020099c4
	b .L_020001d8_9
.L_020001d8_10:
	movs r0, #91
	bl 0x020099c4
.L_020001d8_9:
	movs r0, #20
	bl 0x02009914
	ldr r0, [pc, #348]
	bl 0x02009964
	adds r0, r5, #0
	movs r1, #5
	bl 0x020098c4
	movs r0, #9
	movs r1, #0
	bl 0x02009974
	bl 0x020099bc
	b .L_020001d8_11
.L_020001d8_7:
	cmp r5, #0
	bge .L_020001d8_11
	ldr r0, [pc, #320]
	bl 0x02009964
	negs r0, r5
	movs r1, #5
	bl 0x020098c4
	movs r0, #9
	movs r1, #0
	bl 0x02009974
.L_020001d8_11:
	bl 0x02009924
.L_020001d8_6:
	ldr r5, [pc, #236]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #12
	bne .L_020001d8_2
	ldr r0, [pc, #256]
	bl 0x020098e4
	cmp r0, #0
	bne .L_020001d8_2
	movs r2, #150
	lsls r2, r2, #1
	adds r7, r5, r2
	bl 0x0200991c
	bl 0x02009994
	bl 0x0200999c
	movs r0, #10
	bl 0x02009914
	movs r5, #1
	movs r3, #0
	ldrsb r3, [r7, r3]
	negs r5, r5
	cmp r3, r5
	bne .L_020001d8_12
	movs r0, #1
	bl 0x020088f8
	b .L_020001d8_13
.L_020001d8_12:
	movs r1, #2
	negs r1, r1
	cmp r3, r1
	beq .L_020001d8_13
	ldr r0, [pc, #220]
	bl 0x02009964
	movs r0, #8
	movs r1, #0
	bl 0x02009974
	movs r3, #0
	ldrsb r3, [r7, r3]
	cmp r3, r5
	beq .L_020001d8_14
	mov r8, r5
	adds r6, r7, #0
.L_020001d8_17:
	cmp r6, r7
	bne .L_020001d8_15
	ldr r0, [pc, #192]
	bl 0x02009964
	b .L_020001d8_16
.L_020001d8_15:
	ldr r0, [pc, #188]
	bl 0x02009964
.L_020001d8_16:
	movs r0, #0
	ldrsb r0, [r6, r0]
	bl 0x02008d70
	movs r1, #2
	adds r5, r0, #0
	bl 0x020098c4
	movs r0, #8
	movs r1, #0
	bl 0x02009974
	adds r0, r5, #0
	movs r1, #3
	bl 0x020099a4
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200992c
	movs r0, #10
	bl 0x02009914
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200997c
	adds r6, #1
	movs r0, #30
	bl 0x02009914
	movs r3, #0
	ldrsb r3, [r6, r3]
	cmp r3, r8
	bne .L_020001d8_17
.L_020001d8_14:
	ldr r3, [pc, #40]
	movs r2, #150
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #254
	ldr r0, [pc, #104]
	strb r2, [r3]
	bl 0x02009964
	movs r0, #8
	movs r1, #0
	bl 0x02009974
.L_020001d8_13:
	bl 0x02009924
.L_020001d8_2:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x000000bd
	.4byte 0x03001ebc
	.4byte 0x00003f42
	.4byte 0x04000050
	.4byte 0x0000080c
	.4byte 0x04000052
	.4byte 0xffff0000
	.4byte 0x03001d18
	.4byte 0x00000109
	.4byte 0x02001000
	.4byte 0x00004e1f
	.4byte 0x00001387
	.4byte 0x00000e13
	.4byte 0x00000e14
	.4byte 0x00000e2e
	.4byte 0x00000e2f
	.4byte 0x00000e30
	.4byte 0x00000e31
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	sub	sp, #8
	adds	r5, r0, #0
	bl 0x0200991c
	movs	r0, #30
	bl 0x02009914
	movs	r0, #148
	bl 0x020099c4
	movs	r0, #100
	bl 0x02009914
	movs	r1, #192
	lsls	r1, r1, #8
	movs	r2, #0
	movs	r0, #0
	bl 0x0200997c
	movs	r0, #40
	bl 0x02009914
	movs	r3, #8
	str	r3, [sp, #4]
	movs	r6, #3
	mov	r8, r3
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #82
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #3
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #85
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #88
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #91
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #94
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #97
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #100
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #79
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #82
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #85
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #88
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #91
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #94
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #97
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r2, #70
	movs	r3, #0
	movs	r1, #29
	movs	r0, #100
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #70
	bl 0x02009914
	movs	r0, #126
	bl 0x020099c4
	adds	r0, r5, #0
	movs	r1, #3
	bl 0x020099a4
	movs	r1, #0
	adds	r0, r5, #0
	bl 0x0200992c
	movs	r0, #20
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #97
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #94
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #91
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #88
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #85
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #29
	movs	r2, #70
	movs	r3, #0
	movs	r0, #82
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #100
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #97
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #94
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #91
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #88
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #85
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #82
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	mov	r3, r8
	str	r3, [sp, #4]
	movs	r1, #20
	movs	r2, #70
	movs	r3, #0
	movs	r0, #79
	str	r6, [sp, #0]
	bl 0x0200989c
	movs	r0, #154
	bl 0x020099c4
	movs	r0, #8
	bl 0x02009914
	bl 0x02009924
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.section .text.x02008ac8,"ax",%progbits
	.balign 4
	.global TorebiIzumi_RunSpringGame
	.thumb_func
TorebiIzumi_RunSpringGame:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #0
	mov	sl, r3
	sub	sp, #4
	bl 0x0200991c
	bl 0x020099ac
	ldr	r3, [pc, #612]
	mov	r9, r3
	ldr	r3, [pc, #612]
	mov	fp, r3
.L_02000aec:
	mov	r3, fp
	ldr	r3, [r3, #16]
	movs	r0, #229
	mov	r8, r3
	bl 0x02009904
	adds	r7, r0, #0
	mov	r0, r9
	bl 0x02009964
	movs	r0, #1
	movs	r1, #0
	negs	r0, r0
	bl 0x0200996c
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #4
	movs	r0, #0
	bl 0x020098a4
	ldr	r5, [pc, #568]
	adds	r6, r0, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x020098b4
	movs	r3, #0
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #6
	adds	r2, r6, #0
	movs	r3, #72
	bl 0x020098bc
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #8
	bl 0x020098b4
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r2, r6, #0
	movs	r3, #72
	movs	r1, #6
	adds	r0, r7, #0
	bl 0x020098bc
	mov	r0, sl
	bl 0x020098d4
	movs	r1, #2
	mov	sl, r0
	adds	r0, r6, #0
	bl 0x020098ac
	bl 0x020098cc
	movs	r3, #1
	negs	r3, r3
	cmp	sl, r3
	bne.n	.L_02000b74
	b.n	.L_02000d34
.L_02000b74:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_02000b86
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_02000bdc
	mov	r0, r9
	adds	r0, #1
	b.n	.L_02000b94
.L_02000b86:
	mov	r3, sl
	cmp	r3, #1
	bne.n	.L_02000bdc
	cmp	r7, #0
	bne.n	.L_02000bb4
	mov	r0, r9
	adds	r0, #2
.L_02000b94:
	bl 0x02009964
	movs	r0, #1
	negs	r0, r0
	movs	r1, #0
	bl 0x02009974
	movs	r0, #1
	bl 0x02009864
	b.n	.L_02000aec
.L_02000baa:
	movs	r0, #112
	bl 0x020099c4
	movs	r5, #0
	b.n	.L_02000c3c
.L_02000bb4:
	bl 0x0200990c
	cmp	r0, #0
	bne.n	.L_02000bdc
	mov	r0, r9
	adds	r0, #4
	bl 0x02009964
	movs	r0, #1
	movs	r1, #0
	negs	r0, r0
	bl 0x0200996c
	movs	r0, #0
	movs	r1, #0
	bl 0x02009934
	cmp	r0, #0
	beq.n	.L_02000bdc
	b.n	.L_02000d34
.L_02000bdc:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #15
	movs	r2, #9
	movs	r3, #4
	movs	r0, #20
	bl 0x020098a4
	ldr	r5, [pc, #360]
	adds	r6, r0, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl 0x020098b4
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #8
	bl 0x020098b4
	movs	r0, #5
	bl 0x02009864
	movs	r0, #116
	bl 0x020099c4
	ldr	r5, [pc, #324]
	movs	r7, #1
	b.n	.L_02000c20
.L_02000c1a:
	movs	r0, #1
	bl 0x02009864
.L_02000c20:
	ldr	r3, [r5, #0]
	ands	r3, r7
	cmp	r3, #0
	bne.n	.L_02000baa
	ldr	r3, [r5, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000c1a
	movs	r0, #113
	bl 0x020099c4
	movs	r5, #1
	negs	r5, r5
.L_02000c3c:
	adds	r0, r6, #0
	movs	r1, #2
	bl 0x020098ac
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	beq.n	.L_02000d34
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_02000c5c
	movs	r0, #1
	negs	r0, r0
	bl 0x020098f4
	b.n	.L_02000c68
.L_02000c5c:
	mov	r3, sl
	cmp	r3, #1
	bne.n	.L_02000c68
	movs	r0, #229
	bl 0x020098dc
.L_02000c68:
	mov	r0, sl
	bl 0x0200973c
	mov	r3, sl
	adds	r5, r0, #0
	cmp	r3, #0
	bne.n	.L_02000cb2
	cmp	r5, #4
	beq.n	.L_02000ca4
	ldr	r6, [pc, #228]
	lsls	r5, r5, #1
	ldrh	r0, [r6, r5]
	bl 0x020098f4
	movs	r0, #91
	bl 0x020099c4
	movs	r1, #5
	ldrh	r0, [r6, r5]
	bl 0x020098c4
	ldr	r0, [pc, #208]
	bl 0x02009964
	movs	r0, #1
	negs	r0, r0
	movs	r1, #0
	bl 0x02009974
	b.n	.L_02000d34
.L_02000ca4:
	movs	r0, #113
	bl 0x020099c4
	movs	r0, #10
	bl 0x02009914
	b.n	.L_02000d34
.L_02000cb2:
	lsls	r3, r5, #1
	adds	r3, r3, r5
	movs	r6, #0
	adds	r0, r3, #3
.L_02000cba:
	movs	r7, #0
	cmp	r6, r0
	bge.n	.L_02000cd6
	ldr	r2, [pc, #164]
	mov	ip, r0
	add	r2, fp
.L_02000cc6:
	ldrb	r3, [r2, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r6, #1
	adds	r2, #1
	adds	r7, r7, r3
	cmp	r6, ip
	blt.n	.L_02000cc6
.L_02000cd6:
	bl 0x02009874
	adds	r3, r7, #0
	muls	r3, r0
	mov	r1, fp
	lsrs	r2, r3, #16
	movs	r3, #142
	lsls	r3, r3, #1
	adds	r1, #1
	ldrsb	r3, [r1, r3]
	subs	r2, r2, r3
	movs	r6, #0
	cmp	r2, #0
	blt.n	.L_02000d08
	ldr	r1, [pc, #116]
	add	r1, fp
.L_02000cf6:
	adds	r6, #1
	cmp	r6, #14
	bgt.n	.L_02000d08
	adds	r1, #1
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_02000cf6
.L_02000d08:
	cmp	r6, #15
	bne.n	.L_02000d0e
	movs	r6, #14
.L_02000d0e:
	ldr	r2, [pc, #92]
	lsls	r3, r6, #2
	ldr	r0, [r2, r3]
	bl 0x020084bc
	movs	r3, #142
	lsls	r3, r3, #1
	mov	r0, fp
	adds	r1, r6, r3
	adds	r0, #1
	ldrb	r3, [r0, r1]
	lsls	r3, r3, #24
	asrs	r2, r3, #24
	cmp	r2, #1
	ble.n	.L_02000d34
	lsrs	r3, r3, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	strb	r3, [r0, r1]
.L_02000d34:
	bl 0x02009924
	movs	r0, #0
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000e43
	.4byte 0x02000240
	.4byte 0x00000e49
	.4byte 0x00000e4c
	.4byte 0x03001c94
	.4byte 0x0200a00c
	.4byte 0x00000e46
	.4byte 0x0000011d
	.2byte 0x9fd0
	.2byte 0x0200
	.section .text.x02008e5c,"ax",%progbits
	.p2align 2
	.global FieldScene_RunSecondaryScript
	.thumb_func
FieldScene_RunSecondaryScript:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r0, [pc, #524]
	mov	r9, r0
	movs	r1, #3
	mov	r2, r9
	sub	sp, #24
	mov	r8, r1
	adds	r2, #28
.L_02000e74:
	ldr	r3, [r2, #0]
	str	r3, [r2, #12]
	ldr	r3, [r2, #4]
	str	r3, [r2, #16]
	ldr	r3, [r2, #8]
	str	r3, [r2, #20]
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r4, r8
	subs	r2, #12
	cmp	r4, #0
	bne.n	.L_02000e74
	mov	r1, r9
	movs	r0, #2
	ldrsh	r3, [r1, r0]
	cmp	r3, #31
	bgt.n	.L_02000e9a
	b.n	.L_02001224
.L_02000e9a:
	ldr	r3, [r1, #4]
	ldr	r2, [r1, #64]
	adds	r3, r3, r2
	str	r3, [r1, #4]
	mov	r2, r9
	ldr	r1, [r1, #8]
	ldr	r0, [r2, #68]
	adds	r1, r1, r0
	ldr	r3, [r2, #12]
	str	r1, [r2, #8]
	ldr	r2, [r2, #72]
	mov	r4, r9
	adds	r3, r3, r2
	str	r3, [r4, #12]
	cmp	r1, #0
	ble.n	.L_02000ebc
	b.n	.L_0200121c
.L_02000ebc:
	mov	r1, r8
	str	r1, [r4, #8]
	cmp	r0, #0
	beq.n	.L_02000ee8
	str	r1, [r4, #68]
	ldr	r3, [pc, #432]
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_02000edc
	movs	r0, #17
	bl 0x0200993c
	movs	r1, #1
	bl 0x0200988c
	b.n	.L_02000ee8
.L_02000edc:
	movs	r0, #12
	bl 0x0200993c
	movs	r1, #1
	bl 0x0200988c
.L_02000ee8:
	mov	r2, r9
	ldr	r3, [r2, #76]
	cmp	r3, #0
	ble.n	.L_02000f72
	ldr	r3, [r2, #4]
	movs	r2, #240
	lsls	r2, r2, #15
	subs	r3, r2, r3
	mov	r4, r9
	asrs	r7, r3, #8
	movs	r6, #142
	ldr	r3, [r4, #12]
	lsls	r6, r6, #15
	subs	r6, r6, r3
	asrs	r6, r6, #8
	adds	r0, r7, #0
	muls	r0, r7
	adds	r3, r6, #0
	muls	r3, r6
	adds	r0, r0, r3
	ldr	r3, [pc, #360]
	bl 0x020099d8
	mov	sl, r0
	ldr	r0, [pc, #356]
	mov	r8, r0
	mov	r0, r8
	muls	r0, r7
	mov	r1, sl
	bl 0x02009854
	mov	r1, r9
	ldr	r3, [r1, #64]
	adds	r7, r3, r0
	str	r7, [r1, #64]
	mov	r0, r8
	muls	r0, r6
	mov	r1, sl
	bl 0x02009854
	mov	r2, r9
	ldr	r3, [r2, #72]
	adds	r2, r3, r0
	mov	r3, r9
	str	r2, [r3, #72]
	lsls	r3, r7, #6
	subs	r3, r3, r7
	lsls	r3, r3, #2
	adds	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_02000f50
	adds	r3, #255
.L_02000f50:
	asrs	r3, r3, #8
	mov	r4, r9
	str	r3, [r4, #64]
	lsls	r3, r2, #6
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_02000f64
	adds	r3, #255
.L_02000f64:
	mov	r0, r9
	asrs	r3, r3, #8
	str	r3, [r0, #72]
	ldr	r3, [r0, #76]
	subs	r3, #1
	str	r3, [r0, #76]
	b.n	.L_0200109a
.L_02000f72:
	mov	r1, r9
	ldr	r3, [r1, #64]
	movs	r1, #220
	muls	r3, r1
	cmp	r3, #0
	bge.n	.L_02000f80
	adds	r3, #255
.L_02000f80:
	asrs	r2, r3, #8
	mov	r3, r9
	str	r2, [r3, #64]
	ldr	r3, [r3, #72]
	muls	r3, r1
	cmp	r3, #0
	bge.n	.L_02000f90
	adds	r3, #255
.L_02000f90:
	ldr	r0, [pc, #240]
	asrs	r3, r3, #8
	mov	r4, r9
	str	r3, [r4, #72]
	adds	r3, r2, r0
	ldr	r2, [pc, #236]
	cmp	r3, r2
	bhi.n	.L_02000fa4
	movs	r3, #0
	str	r3, [r4, #64]
.L_02000fa4:
	mov	r1, r9
	ldr	r3, [r1, #72]
	ldr	r4, [pc, #216]
	adds	r3, r3, r4
	cmp	r3, r2
	bhi.n	.L_02000fb4
	movs	r3, #0
	str	r3, [r1, #72]
.L_02000fb4:
	mov	r0, r9
	ldr	r3, [r0, #64]
	cmp	r3, #0
	bne.n	.L_0200109a
	ldr	r3, [r0, #72]
	cmp	r3, #0
	bne.n	.L_0200109a
	ldr	r3, [pc, #180]
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_02000ff0
	movs	r0, #17
	bl 0x0200993c
	movs	r1, #2
	bl 0x0200988c
	movs	r0, #15
	movs	r1, #0
	bl 0x02008e44
	movs	r0, #14
	movs	r1, #0
	bl 0x02008e44
	movs	r0, #13
	movs	r1, #0
	bl 0x02008e44
	b.n	.L_02001014
.L_02000ff0:
	movs	r0, #12
	bl 0x0200993c
	movs	r1, #2
	bl 0x0200988c
	movs	r0, #10
	movs	r1, #0
	bl 0x02008e44
	movs	r0, #9
	movs	r1, #0
	bl 0x02008e44
	movs	r0, #8
	movs	r1, #0
	bl 0x02008e44
.L_02001014:
	mov	r1, r9
	ldr	r3, [r1, #4]
	movs	r2, #240
	lsls	r2, r2, #15
	ldr	r1, [r1, #12]
	subs	r2, r2, r3
	movs	r3, #142
	lsls	r3, r3, #15
	subs	r3, r3, r1
	asrs	r2, r2, #16
	asrs	r3, r3, #16
	adds	r4, r2, #0
	muls	r4, r2
	adds	r0, r3, #0
	muls	r0, r3
	adds	r2, r4, #0
	adds	r3, r0, #0
	adds	r2, r2, r3
	ldr	r3, [pc, #80]
	movs	r1, #1
	str	r1, [r3, #0]
	cmp	r2, #224
	bgt.n	.L_02001048
	ldr	r2, [pc, #76]
	movs	r3, #0
	b.n	.L_02001098
.L_02001048:
	movs	r3, #156
	lsls	r3, r3, #2
	cmp	r2, r3
	bgt.n	.L_02001056
	ldr	r3, [pc, #60]
	str	r1, [r3, #0]
	b.n	.L_0200109a
.L_02001056:
	movs	r4, #136
	lsls	r4, r4, #3
	cmp	r2, r4
	bgt.n	.L_02001064
	ldr	r2, [pc, #48]
	movs	r3, #2
	b.n	.L_02001098
.L_02001064:
	movs	r0, #210
	lsls	r0, r0, #3
	cmp	r2, r0
	bgt.n	.L_02001094
	ldr	r2, [pc, #32]
	movs	r3, #3
	b.n	.L_02001098
	.2byte 0x0000
	.4byte 0x0200a070
	.4byte 0x0200a0c0
	.4byte 0x030001d8
	.4byte 0x00001999
	.4byte 0x000003ff
	.4byte 0x000007fe
	.4byte 0x0200a134
	.2byte 0xa138
	.2byte 0x0200
.L_02001094:
	ldr	r2, [pc, #724]
	movs	r3, #4
.L_02001098:
	str	r3, [r2, #0]
.L_0200109a:
	movs	r2, #240
	lsls	r2, r2, #15
	mov	r3, r9
	movs	r1, #192
	mov	sl, r2
	movs	r0, #168
	ldr	r2, [r3, #12]
	movs	r7, #192
	lsls	r1, r1, #16
	movs	r4, #192
	lsls	r0, r0, #14
	lsls	r7, r7, #14
	mov	r8, r1
	lsls	r4, r4, #13
	adds	r5, r2, #0
	cmp	r2, r0
	bge.n	.L_020010f2
	movs	r3, #168
	lsls	r3, r3, #14
	subs	r3, r3, r2
	movs	r2, #42
	adds	r0, r3, #0
	muls	r0, r2
	movs	r1, #18
	str	r4, [sp, #4]
	bl 0x02009854
	movs	r1, #192
	lsls	r1, r1, #14
	movs	r3, #180
	adds	r7, r0, r1
	lsls	r3, r3, #15
	ldr	r4, [sp, #4]
	cmp	r7, r3
	ble.n	.L_020010e2
	adds	r7, r3, #0
.L_020010e2:
	mov	r2, r8
	subs	r2, r2, r0
	movs	r3, #150
	mov	r8, r2
	lsls	r3, r3, #16
	cmp	r8, r3
	bge.n	.L_020010f2
	mov	r8, r3
.L_020010f2:
	movs	r0, #204
	lsls	r0, r0, #15
	cmp	r5, r0
	ble.n	.L_02001130
	movs	r3, #42
	adds	r0, r5, #0
	muls	r0, r3
	ldr	r1, [pc, #620]
	adds	r0, r0, r1
	movs	r1, #18
	str	r4, [sp, #4]
	bl 0x02009854
	movs	r2, #192
	lsls	r2, r2, #14
	movs	r3, #180
	adds	r7, r0, r2
	lsls	r3, r3, #15
	ldr	r4, [sp, #4]
	cmp	r7, r3
	ble.n	.L_0200111e
	adds	r7, r3, #0
.L_0200111e:
	movs	r3, #192
	lsls	r3, r3, #16
	subs	r3, r3, r0
	mov	r8, r3
	movs	r3, #150
	lsls	r3, r3, #16
	cmp	r8, r3
	bge.n	.L_02001130
	mov	r8, r3
.L_02001130:
	mov	r0, r9
	ldr	r5, [r0, #4]
	movs	r1, #180
	lsls	r1, r1, #15
	adds	r6, r5, #0
	cmp	r5, r1
	bge.n	.L_02001172
	movs	r3, #180
	lsls	r3, r3, #15
	subs	r3, r3, r5
	lsls	r0, r3, #3
	adds	r0, r0, r3
	lsls	r0, r0, #1
	movs	r1, #42
	bl 0x02009854
	movs	r2, #192
	lsls	r2, r2, #13
	movs	r3, #168
	adds	r4, r0, r2
	lsls	r3, r3, #14
	cmp	r4, r3
	ble.n	.L_02001160
	adds	r4, r3, #0
.L_02001160:
	movs	r3, #240
	lsls	r3, r3, #15
	subs	r3, r3, r0
	mov	sl, r3
	movs	r3, #204
	lsls	r3, r3, #15
	cmp	sl, r3
	bge.n	.L_02001172
.L_02001170:
	mov	sl, r3
.L_02001172:
	movs	r0, #150
	lsls	r0, r0, #16
	cmp	r6, r0
	ble.n	.L_020011ac
	lsls	r0, r6, #3
	ldr	r1, [pc, #500]
	adds	r0, r0, r6
	lsls	r0, r0, #1
	adds	r0, r0, r1
	movs	r1, #42
	bl 0x02009854
	movs	r2, #192
	lsls	r2, r2, #13
	movs	r3, #168
	adds	r4, r0, r2
	lsls	r3, r3, #14
	cmp	r4, r3
	ble.n	.L_0200119a
	adds	r4, r3, #0
.L_0200119a:
	movs	r3, #240
	lsls	r3, r3, #15
	subs	r3, r3, r0
	mov	sl, r3
	movs	r3, #204
	lsls	r3, r3, #15
	cmp	sl, r3
	bge.n	.L_020011ac
	mov	sl, r3
.L_020011ac:
	cmp	r6, r7
	bge.n	.L_020011c6
	mov	r0, r9
	ldr	r3, [r0, #64]
	str	r7, [r0, #4]
	cmp	r3, #0
	bge.n	.L_020011c4
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r0, #64]
.L_020011c4:
	adds	r5, r7, #0
.L_020011c6:
	cmp	r5, r8
	ble.n	.L_020011e2
	mov	r2, r9
	ldr	r3, [r2, #64]
	mov	r1, r8
	str	r1, [r2, #4]
.L_020011d2:
	cmp	r3, #0
	ble.n	.L_020011e2
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	r0, r9
	str	r3, [r0, #64]
.L_020011e2:
	mov	r1, r9
	ldr	r2, [r1, #12]
	cmp	r2, r4
	bge.n	.L_020011fe
	ldr	r3, [r1, #72]
	str	r4, [r1, #12]
	cmp	r3, #0
	bge.n	.L_020011fc
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r1, #72]
.L_020011fc:
	adds	r2, r4, #0
.L_020011fe:
	cmp	r2, sl
	ble.n	.L_02001224
	mov	r3, r9
	mov	r2, sl
	str	r2, [r3, #12]
	ldr	r3, [r3, #72]
	cmp	r3, #0
	ble.n	.L_02001224
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	r4, r9
	str	r3, [r4, #72]
	b.n	.L_02001224
.L_0200121c:
	ldr	r1, [pc, #344]
	mov	r2, r9
	adds	r3, r0, r1
	str	r3, [r2, #68]
.L_02001224:
	movs	r3, #0
	mov	r8, r3
.L_02001228:
	mov	r4, r8
	lsls	r3, r4, #1
	ldr	r2, [pc, #332]
	add	r3, r8
	lsls	r3, r3, #3
	adds	r6, r3, r2
	movs	r0, #18
	ldrsh	r3, [r6, r0]
	ldrh	r2, [r6, #18]
	cmp	r3, #0
	ble.n	.L_02001242
	subs	r3, r2, #1
	strh	r3, [r6, #18]
.L_02001242:
	movs	r1, #20
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #20]
	cmp	r3, #0
	ble.n	.L_02001250
	subs	r3, r2, #1
	strh	r3, [r6, #20]
.L_02001250:
	mov	r2, r8
	cmp	r2, #1
	bgt.n	.L_020012f4
	movs	r4, #16
	ldrsh	r3, [r6, r4]
	movs	r5, #128
	lsls	r5, r5, #9
	cmp	r3, #1
	bne.n	.L_02001264
	lsls	r5, r5, #1
.L_02001264:
	cmp	r3, #2
	bne.n	.L_0200126c
	lsls	r3, r5, #1
	adds	r5, r3, r5
.L_0200126c:
	movs	r0, #18
	ldrsh	r3, [r6, r0]
	cmp	r3, #0
	ble.n	.L_02001282
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_0200127e
	movs	r0, #18
	b.n	.L_0200131a
.L_0200127e:
	movs	r0, #19
	b.n	.L_0200131a
.L_02001282:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_02001296
	movs	r0, #18
	bl 0x0200993c
	movs	r1, #1
	bl 0x0200988c
	b.n	.L_020012a2
.L_02001296:
	movs	r0, #19
	bl 0x0200993c
	movs	r1, #1
	bl 0x0200988c
.L_020012a2:
	movs	r4, #14
	ldrsh	r3, [r6, r4]
	ldrh	r2, [r6, #14]
	cmp	r3, #0
	bne.n	.L_020012f0
	movs	r0, #12
	ldrsh	r3, [r6, r0]
	cmp	r3, #0
	bne.n	.L_020012ba
	ldr	r3, [r6, #0]
	adds	r3, r3, r5
	b.n	.L_020012be
.L_020012ba:
	ldr	r3, [r6, #0]
	subs	r3, r3, r5
.L_020012be:
	str	r3, [r6, #0]
	movs	r1, #128
	ldr	r2, [r6, #0]
	lsls	r1, r1, #15
	cmp	r2, r1
	bgt.n	.L_020012d8
	movs	r3, #0
	strh	r3, [r6, #12]
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L_020012d8
	movs	r3, #30
	strh	r3, [r6, #14]
.L_020012d8:
	ldr	r4, [pc, #164]
	cmp	r2, r4
	bgt.n	.L_020012e0
	b.n	.L_02001410
.L_020012e0:
	movs	r3, #1
	mov	r0, r8
	strh	r3, [r6, #12]
	cmp	r0, #1
	beq.n	.L_020012ec
	b.n	.L_02001410
.L_020012ec:
	movs	r3, #30
	b.n	.L_0200140e
.L_020012f0:
	subs	r3, r2, #1
	b.n	.L_0200140e
.L_020012f4:
	mov	r1, r8
	cmp	r1, #2
	bne.n	.L_02001384
	movs	r2, #16
	ldrsh	r3, [r6, r2]
	movs	r5, #64
	negs	r5, r5
	cmp	r3, #1
	bne.n	.L_02001308
	lsls	r5, r5, #1
.L_02001308:
	cmp	r3, #2
	bne.n	.L_02001310
	lsls	r3, r5, #1
	adds	r5, r3, r5
.L_02001310:
	movs	r4, #18
	ldrsh	r3, [r6, r4]
	cmp	r3, #0
	ble.n	.L_02001326
	movs	r0, #20
.L_0200131a:
	bl 0x0200993c
	movs	r1, #3
	bl 0x0200988c
	b.n	.L_02001410
.L_02001326:
	movs	r0, #20
	bl 0x0200993c
	movs	r1, #2
	bl 0x0200988c
	movs	r1, #12
	ldrsh	r0, [r6, r1]
	bl 0x0200987c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	movs	r2, #224
	lsls	r2, r2, #15
	lsls	r3, r3, #4
	adds	r3, r3, r2
	str	r3, [r6, #0]
	movs	r3, #12
	ldrsh	r0, [r6, r3]
	bl 0x02009884
	lsls	r3, r0, #2
	adds	r3, r3, r0
	movs	r4, #144
	lsls	r3, r3, #3
	lsls	r4, r4, #15
	adds	r3, r3, r4
	str	r3, [r6, #8]
	ldrh	r3, [r6, #12]
	adds	r2, r3, r5
	ldrh	r3, [r6, #14]
	adds	r3, #1
	strh	r2, [r6, #12]
	b.n	.L_0200140e
	.2byte 0x0000
	.4byte 0x0200a138
	.4byte 0xef440000
	.4byte 0xf5740000
	.4byte 0xffffc000
	.4byte 0x0200a0d0
	.2byte 0xffff
	.2byte 0x00af
.L_02001384:
	ldr	r3, [pc, #24]
	ldrh	r2, [r6, #14]
	ands	r2, r3
	movs	r0, #16
	ldrsh	r3, [r6, r0]
	movs	r5, #64
	cmp	r3, #1
	bne.n	.L_02001396
	movs	r5, #128
.L_02001396:
	cmp	r3, #2
	bne.n	.L_020013a4
	lsls	r3, r5, #1
	adds	r5, r3, r5
	b.n	.L_020013a4
	.2byte 0x01ff
	.2byte 0x0000
.L_020013a4:
	movs	r1, #18
	ldrsh	r3, [r6, r1]
	cmp	r3, #0
	ble.n	.L_020013ba
	movs	r0, #21
	bl 0x0200993c
	movs	r1, #3
	bl 0x0200988c
	b.n	.L_0200140a
.L_020013ba:
	ldr	r3, [pc, #720]
	cmp	r2, r3
	bgt.n	.L_020013fe
	movs	r4, #12
	ldrsh	r0, [r6, r4]
	bl 0x0200987c
	movs	r3, #52
	muls	r3, r0
	movs	r0, #224
	lsls	r0, r0, #15
	adds	r3, r3, r0
	str	r3, [r6, #0]
	movs	r1, #12
	ldrsh	r0, [r6, r1]
	bl 0x02009884
	lsls	r3, r0, #1
	adds	r3, r3, r0
	movs	r2, #144
	lsls	r2, r2, #15
	lsls	r3, r3, #3
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldrh	r3, [r6, #12]
	adds	r3, r3, r5
	strh	r3, [r6, #12]
	movs	r0, #21
	bl 0x0200993c
	movs	r1, #2
	bl 0x0200988c
	b.n	.L_0200140a
.L_020013fe:
	movs	r0, #21
	bl 0x0200993c
	movs	r1, #3
	bl 0x0200988c
.L_0200140a:
	ldrh	r3, [r6, #14]
	adds	r3, #1
.L_0200140e:
	strh	r3, [r6, #14]
.L_02001410:
	movs	r4, #20
	ldrsh	r3, [r6, r4]
	cmp	r3, #0
	bne.n	.L_020014ce
	mov	r0, r9
	ldr	r3, [r0, #8]
	cmp	r3, #0
	bne.n	.L_020014ce
	ldr	r2, [r0, #4]
	ldr	r3, [r6, #0]
	subs	r3, r3, r2
	asrs	r7, r3, #16
	ldr	r2, [r0, #12]
	ldr	r3, [r6, #8]
	subs	r3, r3, r2
	asrs	r5, r3, #16
	adds	r2, r7, #0
	muls	r2, r7
	adds	r3, r5, #0
	muls	r3, r5
	adds	r0, r2, r3
	cmp	r0, #119
	bgt.n	.L_020014ce
	mov	r2, r9
	ldr	r1, [r2, #76]
	cmp	r1, #30
	ble.n	.L_020014ce
	movs	r4, #192
	mov	r3, r8
	lsls	r4, r4, #10
	cmp	r3, #1
	bgt.n	.L_0200147a
	movs	r0, #12
	ldrsh	r3, [r6, r0]
	cmp	r3, #0
	bne.n	.L_02001466
	ldr	r3, [r2, #64]
	cmp	r3, r4
	bge.n	.L_020014b2
	adds	r3, r1, #0
	subs	r3, #100
	str	r4, [r2, #64]
	b.n	.L_020014b0
.L_02001466:
	negs	r2, r4
	mov	r4, r9
	ldr	r3, [r4, #64]
	cmp	r3, r2
	ble.n	.L_020014b2
	adds	r3, r1, #0
	subs	r3, #100
	str	r2, [r4, #64]
	str	r3, [r4, #76]
	b.n	.L_020014b2
.L_0200147a:
	str	r4, [sp, #4]
	ldr	r3, [pc, #528]
	bl 0x020099d8
	ldr	r4, [sp, #4]
	adds	r2, r0, #0
	negs	r3, r7
	adds	r0, r3, #0
	muls	r0, r4
	adds	r1, r2, #0
	str	r2, [sp, #8]
	bl 0x02009854
	ldr	r2, [sp, #8]
	ldr	r4, [sp, #4]
	negs	r3, r5
	mov	r1, r9
	str	r0, [r1, #64]
	adds	r0, r3, #0
	muls	r0, r4
	adds	r1, r2, #0
	bl 0x02009854
	mov	r2, r9
	ldr	r3, [r2, #76]
	subs	r3, #100
	str	r0, [r2, #72]
.L_020014b0:
	str	r3, [r2, #76]
.L_020014b2:
	ldr	r0, [pc, #480]
	bl 0x020099c4
	movs	r3, #16
	ldrsh	r0, [r6, r3]
	movs	r1, #3
	adds	r0, #1
	bl 0x0200985c
	movs	r3, #36
	strh	r3, [r6, #18]
	movs	r3, #30
	strh	r0, [r6, #16]
	strh	r3, [r6, #20]
.L_020014ce:
	mov	r4, r8
	cmp	r4, #1
	beq.n	.L_020014fc
	cmp	r4, #1
	bgt.n	.L_020014de
	cmp	r4, #0
	beq.n	.L_020014ea
	b.n	.L_02001556
.L_020014de:
	mov	r0, r8
	cmp	r0, #2
	beq.n	.L_02001516
	cmp	r0, #3
	beq.n	.L_02001538
	b.n	.L_02001556
.L_020014ea:
	movs	r1, #16
	ldrsh	r2, [r6, r1]
	ldr	r3, [pc, #424]
	ldrb	r3, [r3, r2]
	lsls	r2, r2, #4
	adds	r2, #16
	str	r2, [sp, #0]
	movs	r0, #18
	b.n	.L_0200150c
.L_020014fc:
	movs	r4, #16
	ldrsh	r2, [r6, r4]
	ldr	r3, [pc, #404]
	ldrb	r3, [r3, r2]
	lsls	r2, r2, #4
	adds	r2, #16
	str	r2, [sp, #0]
	movs	r0, #19
.L_0200150c:
	adds	r1, r6, #0
	movs	r2, #0
	bl 0x02008dd0
	b.n	.L_02001556
.L_02001516:
	movs	r0, #12
	ldrsh	r3, [r6, r0]
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r4, #16
	ldrsh	r1, [r6, r4]
	subs	r2, r2, r3
	ldr	r3, [pc, #372]
	ldrb	r3, [r3, r1]
	lsls	r1, r1, #4
	adds	r1, #16
	str	r1, [sp, #0]
	movs	r0, #20
	adds	r1, r6, #0
	bl 0x02008dd0
	b.n	.L_02001556
.L_02001538:
	movs	r0, #12
	ldrsh	r3, [r6, r0]
	ldr	r2, [pc, #352]
	movs	r4, #16
	ldrsh	r1, [r6, r4]
	subs	r2, r2, r3
	ldr	r3, [pc, #340]
	ldrb	r3, [r3, r1]
	lsls	r1, r1, #4
	adds	r1, #16
	str	r1, [sp, #0]
	movs	r0, #21
	adds	r1, r6, #0
	bl 0x02008dd0
.L_02001556:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #4
	beq.n	.L_02001562
	b.n	.L_02001228
.L_02001562:
	mov	r2, r9
	ldr	r3, [r2, #4]
	str	r3, [r2, #52]
	movs	r3, #0
	str	r3, [r2, #56]
	ldr	r3, [r2, #12]
	str	r3, [r2, #60]
	ldr	r3, [pc, #304]
	ldr	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_020015f0
	mov	r1, r9
	movs	r6, #16
	adds	r1, #4
	movs	r0, #17
	movs	r2, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl 0x02008dd0
	mov	r1, r9
	adds	r1, #52
	movs	r0, #16
	movs	r2, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl 0x02008dd0
	mov	r1, r9
	adds	r1, #16
	movs	r0, #15
	movs	r2, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl 0x02008dd0
	mov	r1, r9
	adds	r1, #28
	movs	r0, #14
	movs	r2, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl 0x02008dd0
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	adds	r1, #40
	movs	r0, #13
	str	r6, [sp, #0]
	bl 0x02008dd0
	movs	r0, #15
	bl 0x0200993c
	movs	r1, #4
	bl 0x0200988c
	movs	r0, #14
	bl 0x0200993c
	movs	r1, #4
	bl 0x0200988c
	movs	r0, #13
	bl 0x0200993c
	movs	r1, #4
	bl 0x0200988c
	b.n	.L_02001666
.L_020015f0:
	mov	r1, r9
	movs	r6, #16
	adds	r1, #4
	movs	r0, #12
	movs	r2, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl 0x02008dd0
	mov	r1, r9
	adds	r1, #52
	movs	r0, #11
	movs	r2, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl 0x02008dd0
	mov	r1, r9
	adds	r1, #16
	movs	r0, #10
	movs	r2, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl 0x02008dd0
	mov	r1, r9
	adds	r1, #28
	movs	r0, #9
	movs	r2, #0
	movs	r3, #0
	str	r6, [sp, #0]
	bl 0x02008dd0
	mov	r1, r9
	movs	r2, #0
	movs	r3, #0
	adds	r1, #40
	movs	r0, #8
	str	r6, [sp, #0]
	bl 0x02008dd0
	movs	r0, #10
	bl 0x0200993c
	movs	r1, #4
	bl 0x0200988c
	movs	r0, #9
	bl 0x0200993c
	movs	r1, #4
	bl 0x0200988c
	movs	r0, #8
	bl 0x0200993c
	movs	r1, #4
	bl 0x0200988c
.L_02001666:
	mov	r3, r9
	ldrh	r2, [r3, #2]
	movs	r0, #1
	movs	r4, #2
	ldrsh	r3, [r3, r4]
	negs	r0, r0
	cmp	r3, r0
	beq.n	.L_0200167c
	adds	r3, r2, #1
	mov	r1, r9
	strh	r3, [r1, #2]
.L_0200167c:
	add	sp, #24
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000017f
	.4byte 0x030001d8
	.4byte 0x0000012d
	.4byte 0x0200a054
	.4byte 0x0200a057
	.4byte 0x0000ffff
	.2byte 0xa0c0
	.2byte 0x0200
	.section .rodata.part1,"a",%progbits
	.global TorebiIzumi_SceneTableA
TorebiIzumi_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000078
	.4byte 0xc00000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000080
	.4byte 0xc00000b0
	.4byte 0x00080000
	.4byte 0x00f80008
	.4byte 0x000000c8
	.4byte 0xffff000b
	.4byte 0x00000060
	.4byte 0xc00001b8
	.4byte 0x00080000
	.4byte 0x00f80120
	.4byte 0x000001c8
	.4byte 0xffff000c
	.4byte 0x00000090
	.4byte 0xc0000186
	.4byte 0x00080000
	.4byte 0x00f80120
	.4byte 0x000001c8
	.4byte 0xffff000d
	.4byte 0x00000078
	.4byte 0x80000098
	.4byte 0x00080000
	.4byte 0x00f80008
	.4byte 0x000000c8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global TorebiIzumi_SceneTableB
TorebiIzumi_SceneTableB:
	.4byte 0x000000bd
	.4byte 0x10114087
	.4byte 0xffffffff
	.4byte 0x00000089
	.4byte 0x1010d087
	.4byte 0xffffffff
	.4byte 0x10205087
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global gTorebiIzumiPlacements2
gTorebiIzumiPlacements2:
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff018d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte 0x00000007
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00300000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiIzumiPlacementsOther
gTorebiIzumiPlacementsOther:
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00015000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00520000
	.4byte 0x00015000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00520000
	.4byte 0x00015000
	.4byte 0xffff0065
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00015000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0001b000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00010000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00014000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00014000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00014000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00016000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiIzumiEventsOther
gTorebiIzumiEventsOther:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200819d
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00000e51
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020089f9
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00000e11
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x020080bd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00000e1c
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00000e1d
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00000e1e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00000e1f
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00000e20
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00000e21
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00000e22
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x02008179
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008155
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00000e15
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00000e16
	.4byte 0x00000000
	.4byte 0x09620011
	.4byte 0x00001f9c
	.4byte 0x00008d15
	.4byte 0x09620011
	.4byte 0x00001fb1
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000021e9
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000021fc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTorebiIzumiEvents2
gTorebiIzumiEvents2:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00000e37
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00000e36
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00000e38
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x02008075
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00000e3c
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00000e4e
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00000e4f
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00000e50
	.4byte 0x00000003
	.4byte 0xffff0050
	.4byte 0x02008105
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global TorebiIzumi_AlphaSteps
TorebiIzumi_AlphaSteps:
	.4byte 0x0a08090a
	.4byte 0x0c040b06
	.4byte 0x0e020d03
	.4byte 0x10000f01
	.4byte 0x00000017
	.4byte 0x0000007c
	.4byte 0x00000051
	.4byte 0x00000098
	.4byte 0x00000025
	.4byte 0x0000006c
	.4byte 0x000000ab
	.4byte 0x0000008e
	.4byte 0x00000030
	.4byte 0x000000a0
	.4byte 0x00000085
	.4byte 0x00000061
	.4byte 0x000000b7
	.4byte 0x000000ba
	.4byte 0x000000bd
	.4byte 0x000a0014
	.4byte 0x00010002
	.4byte 0x00000000
	.global TorebiIzumi_TopicIds
TorebiIzumi_TopicIds:
	.4byte 0x000000fa
	.4byte 0x000000fb
	.4byte 0x000000fc
	.4byte 0x00000100
	.4byte 0x00000101
	.4byte 0x00000102
	.4byte 0x00000106
	.4byte 0x00000107
	.4byte 0x00000108
	.4byte 0x000000b7
	.4byte 0x000000b6
	.4byte 0x000000b5
	.4byte 0x000000bd
	.4byte 0x000000ba
	.4byte 0x000000bc
	.4byte 0x00090100
	.2byte 0x0403
	.global TorebiIzumi_ActorTileX
TorebiIzumi_ActorTileX:
	.2byte 0xa050
	.2byte 0x4850
	.global TorebiIzumi_ActorTileZ
TorebiIzumi_ActorTileZ:
	.2byte 0x6820
	.2byte 0x4844
	.global TorebiIzumi_ActorHeadings
TorebiIzumi_ActorHeadings:
	.2byte 0x0000
	.4byte 0x00000001
	.2byte 0x8000
