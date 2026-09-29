.syntax unified
	.thumb
	.section .text.x02008130,"ax",%progbits
	.balign 4
	.global Func_02000130
	.thumb_func
Func_02000130:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000130_0
	ldr r0, [pc, #16]
	b .L_02000130_1
.L_02000130_0:
	ldr r0, [pc, #16]
.L_02000130_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000033
	.4byte 0x0200a8a0
	.4byte 0x0200a798
	.section .text.x0200816c,"ax",%progbits
	.balign 4
	.global Func_0200016c
	.thumb_func
Func_0200016c:
	push {r5, lr}
	ldr r3, [pc, #88]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #80]
	cmp r2, r3
	bne .L_0200016c_0
	ldr r5, [pc, #76]
	adds r0, r5, #0
	bl 0x0200a240
	ldr r0, [pc, #72]
	bl 0x0200a210
	cmp r0, #0
	beq .L_0200016c_1
	movs r3, #131
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #0
	strb r3, [r2]
	movs r3, #182
	lsls r3, r3, #16
	str r3, [r5, #80]
	movs r3, #141
	lsls r3, r3, #18
	str r3, [r5, #88]
	movs r3, #2
	str r3, [r5, #76]
.L_0200016c_1:
	adds r0, r5, #0
	b .L_0200016c_2
.L_0200016c_0:
	ldr r0, [pc, #32]
	bl 0x0200a210
	cmp r0, #0
	beq .L_0200016c_3
	ldr r0, [pc, #28]
	b .L_0200016c_2
.L_0200016c_3:
	ldr r0, [pc, #28]
.L_0200016c_2:
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000033
	.4byte 0x0200aad0
	.4byte 0x00000881
	.4byte 0x0200aa58
	.4byte 0x0200a9e0
	.section .text.x020081ec,"ax",%progbits
	.balign 4
	.global Func_020001ec
	.thumb_func
Func_020001ec:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_020001ec_0
	ldr r0, [pc, #16]
	b .L_020001ec_1
.L_020001ec_0:
	ldr r0, [pc, #16]
.L_020001ec_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000033
	.4byte 0x0200adb8
	.4byte 0x0200ac80
	.section .text.x02008b70,"ax",%progbits
	.balign 4
	.global Func_02000b70
	.thumb_func
Func_02000b70:
	push {r5, r6, lr}
	ldr r6, [pc, #892]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #884]
	sub sp, #8
	cmp r2, r3
	bne .L_02000b70_0
	movs r0, #0
	bl 0x0200a248
	ldr r3, [pc, #872]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #192
	adds r5, r0, #0
	str r2, [r3]
	movs r0, #10
	movs r1, #9
	bl 0x0200a2b8
	ldr r0, [pc, #852]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_1
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a220
	ldr r0, [pc, #840]
	bl 0x0200a220
.L_02000b70_1:
	adds r3, r5, #0
	movs r2, #0
	adds r3, #100
	strh r2, [r3]
	movs r1, #200
	adds r3, #2
	strh r2, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #820]
	bl 0x0200a178
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #816]
	bl 0x0200a178
	movs r0, #11
	movs r1, #1
	bl 0x0200a328
	ldr r0, [pc, #804]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_2
	bl 0x02009960
.L_02000b70_2:
	ldr r0, [pc, #776]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_3
	b .L_02000b70_4
.L_02000b70_3:
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	beq .L_02000b70_5
	b .L_02000b70_4
.L_02000b70_5:
	bl 0x020099bc
	b .L_02000b70_4
.L_02000b70_0:
	ldr r3, [pc, #760]
	cmp r2, r3
	beq .L_02000b70_6
	b .L_02000b70_4
.L_02000b70_6:
	ldr r3, [pc, #728]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	ldr r2, [pc, #748]
	adds r3, r3, r1
	str r2, [r3]
	subs r2, #71
	adds r3, r6, r2
	movs r1, #0
	ldrsh r5, [r3, r1]
	cmp r5, #1
	beq .L_02000b70_7
	b .L_02000b70_8
.L_02000b70_7:
	movs r1, #15
	movs r0, #21
	bl 0x0200a2e8
	movs r0, #21
	bl 0x0200a248
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #21
	bl 0x0200a328
	ldr r0, [pc, #700]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_9
	movs r3, #10
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #7
	movs r2, #1
	movs r3, #1
	bl 0x0200a200
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #9
	movs r1, #125
	movs r3, #69
	movs r0, #3
	bl 0x0200a1e8
	bl 0x0200a1c8
	movs r0, #1
	bl 0x0200a170
	movs r0, #8
	movs r1, #2
	bl 0x0200a258
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200a2b0
	b .L_02000b70_4
.L_02000b70_9:
	ldr r0, [pc, #628]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_10
	ldr r0, [pc, #620]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_10
	movs r0, #10
	bl 0x0200a248
	movs r1, #0
	bl 0x0200a208
	movs r1, #174
	movs r2, #164
	lsls r2, r2, #16
	lsls r1, r1, #16
	movs r0, #9
	bl 0x0200a2b0
	movs r0, #9
	bl 0x0200a248
	movs r1, #0
	bl 0x0200a208
	movs r0, #9
	movs r1, #5
	bl 0x0200a2b8
	movs r1, #168
	movs r2, #152
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl 0x0200a2b0
	movs r0, #8
	bl 0x0200a248
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r0, #6]
	ldr r0, [pc, #544]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_11
	b .L_02000b70_4
.L_02000b70_11:
	bl 0x02008f90
	b .L_02000b70_4
.L_02000b70_10:
	movs r3, #10
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #10
	movs r1, #7
	movs r2, #1
	movs r3, #1
	bl 0x0200a200
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #125
	movs r2, #9
	movs r3, #69
	bl 0x0200a1e8
	bl 0x0200a1c8
	movs r0, #1
	bl 0x0200a170
	ldr r0, [pc, #468]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_12
	movs r1, #149
	movs r2, #232
	lsls r1, r1, #16
	lsls r2, r2, #15
	movs r0, #8
	bl 0x0200a2b0
	movs r0, #8
	bl 0x0200a248
	movs r5, #0
	strh r5, [r0, #6]
	movs r0, #9
	bl 0x0200a248
	adds r0, #102
	strh r5, [r0]
	ldr r1, [pc, #436]
	movs r0, #9
	bl 0x0200a258
	b .L_02000b70_4
.L_02000b70_12:
	movs r0, #8
	movs r1, #2
	bl 0x0200a258
	b .L_02000b70_4
.L_02000b70_8:
	cmp r5, #2
	bne .L_02000b70_13
	ldr r0, [pc, #396]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_14
	b .L_02000b70_4
.L_02000b70_14:
	movs r0, #11
	bl 0x0200a248
	movs r3, #1
	adds r0, #102
	strh r3, [r0]
	ldr r1, [pc, #388]
	movs r0, #11
	bl 0x0200a258
	b .L_02000b70_4
.L_02000b70_13:
	cmp r5, #4
	bne .L_02000b70_15
	ldr r0, [pc, #360]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_16
	movs r1, #182
	ldr r2, [pc, #368]
	movs r0, #12
	lsls r1, r1, #17
	bl 0x0200a2b0
	movs r1, #2
	movs r0, #12
	bl 0x0200a328
	movs r0, #12
	bl 0x0200a248
	adds r0, #89
	ldrb r3, [r0]
	movs r6, #4
	orrs r3, r6
	strb r3, [r0]
	movs r5, #3
	movs r3, #88
	movs r0, #6
	movs r1, #125
	movs r2, #22
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a1e8
	movs r1, #246
	ldr r2, [pc, #312]
	movs r0, #13
	lsls r1, r1, #17
	bl 0x0200a2b0
	movs r1, #2
	movs r0, #13
	bl 0x0200a328
	movs r0, #13
	bl 0x0200a248
	adds r0, #89
	ldrb r3, [r0]
	orrs r6, r3
	strb r6, [r0]
	movs r1, #125
	movs r0, #9
	movs r2, #28
	movs r3, #88
	b .L_02000b70_17
.L_02000b70_16:
	movs r0, #12
	bl 0x0200a248
	ldr r3, [pc, #268]
	str r3, [r0, #24]
	movs r0, #12
	bl 0x0200a248
	movs r1, #0
	bl 0x0200a208
	movs r1, #5
	movs r0, #12
	bl 0x0200a2b8
	movs r0, #13
	bl 0x0200a248
	movs r1, #0
	bl 0x0200a208
	movs r0, #13
	b .L_02000b70_18
.L_02000b70_15:
	cmp r5, #3
	bne .L_02000b70_19
	ldr r0, [pc, #200]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_20
	movs r1, #230
	movs r2, #129
	lsls r2, r2, #17
	movs r0, #15
	lsls r1, r1, #17
	bl 0x0200a2b0
	movs r1, #2
	movs r0, #15
	bl 0x0200a328
	movs r0, #15
	bl 0x0200a248
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	movs r1, #204
	movs r2, #132
	strb r3, [r0]
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #14
	bl 0x0200a2b0
	movs r0, #14
	bl 0x0200a248
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r0, #6]
	movs r1, #125
	movs r0, #12
	movs r2, #26
	movs r3, #70
.L_02000b70_17:
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a1e8
	b .L_02000b70_4
.L_02000b70_20:
	movs r1, #230
	movs r2, #129
	lsls r2, r2, #17
	movs r0, #14
	lsls r1, r1, #17
	bl 0x0200a2b0
	movs r1, #2
	movs r0, #14
	bl 0x0200a328
	movs r0, #14
	bl 0x0200a248
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl 0x0200a248
	ldr r3, [pc, #88]
	str r3, [r0, #24]
	movs r0, #15
	bl 0x0200a248
	movs r1, #0
	bl 0x0200a208
	movs r0, #15
.L_02000b70_18:
	movs r1, #5
	bl 0x0200a2b8
	b .L_02000b70_4
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000032
	.4byte 0x03001ebc
	.4byte 0x00000109
	.4byte 0x00000201
	.4byte 0x02009795
	.4byte 0x020098c5
	.4byte 0x00000203
	.4byte 0x00000033
	.4byte 0x00000209
	.4byte 0x00000881
	.4byte 0x0000082c
	.4byte 0x0000082a
	.4byte 0x0000082b
	.4byte 0x0200a4f4
	.4byte 0x02420000
	.4byte 0xffff0000
.L_02000b70_19:
	cmp r5, #7
	bne .L_02000b70_4
	ldr r0, [pc, #72]
	bl 0x0200a210
	cmp r0, #0
	beq .L_02000b70_4
	movs r0, #20
	bl 0x0200a248
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r0, #6]
	ldr r0, [pc, #56]
	bl 0x0200a210
	cmp r0, #0
	bne .L_02000b70_21
	movs r2, #161
	movs r0, #20
	ldr r1, [pc, #44]
	lsls r2, r2, #16
	bl 0x0200a2b0
	bl 0x020099e8
	b .L_02000b70_4
.L_02000b70_21:
	movs r1, #161
	movs r2, #166
	movs r0, #20
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200a2b0
.L_02000b70_4:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00000881
	.4byte 0x0000082e
	.4byte 0x028a0000
	.section .text.x020095b4,"ax",%progbits
	.align 2
	.global ImiruMura_SwayAndSpark
	.thumb_func
ImiruMura_SwayAndSpark:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	adds	r5, r6, #0
	adds	r5, #100
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	lsls	r0, r0, #10
	sub	sp, #12
	bl	0x0200a190
	adds	r1, r0, #0
	movs	r0, #192
	ldr	r3, [pc, #188]
	lsls	r0, r0, #11
	mov	ip, pc
	bx	r3
	ldr	r3, [pc, #184]
	ldr	r3, [r3, #0]
	adds	r3, r3, r0
	str	r3, [r6, #8]
	ldrh	r3, [r5, #0]
	adds	r3, #1
	strh	r3, [r5, #0]
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	adds	r2, r1, #0
	adds	r2, #64
	adds	r3, r2, #0
	cmp	r2, #0
	bge	.L_020015b4_40
	adds	r3, r1, #0
	adds	r3, #127
.L_020015b4_40:
	asrs	r3, r3, #6
	lsls	r3, r3, #6
	subs	r3, r2, r3
	strh	r3, [r5, #0]
	ldr	r3, [pc, #148]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl	0x0200a168
	cmp	r0, #0
	bne	.L_020015b4_f0
	ldr	r3, [r6, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r2, #128
	ldr	r3, [r6, #12]
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	0x0200a180
	adds	r6, r0, #0
	bl	0x0200a180
	adds	r1, r0, #0
	lsls	r0, r6, #1
	adds	r0, r0, r6
	adds	r2, r5, #0
	lsls	r0, r0, #1
	bl	0x0200a198
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	ldr	r0, [pc, #88]
	bl	0x0200a1b8
	adds	r5, r0, #0
	cmp	r5, #0
	beq	.L_020015b4_f0
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r1, #9]
	movs	r1, #0
	bl	0x0200a208
	adds	r0, r5, #0
	movs	r1, #1
	bl	0x0200a1a0
	ldr	r3, [pc, #56]
	adds	r2, r5, #0
	str	r3, [r5, #24]
	str	r3, [r5, #28]
	adds	r2, #35
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r3, [pc, #20]
	adds	r2, #50
	adds	r0, r5, #0
	movs	r1, #9
	strb	r3, [r2, #0]
	bl	0x0200a2f0
	ldr	r1, [pc, #32]
	adds	r0, r5, #0
	bl	0x0200a1b0
	b	.L_020015b4_f0
	.4byte 0x00000000
	.4byte 0x03000118
	.4byte 0x0200b1f0
	.4byte 0x03001e40
	.4byte 0x0000011d
	.4byte 0x00009999
	.4byte 0x0200a64c
.L_020015b4_f0:
	add	sp, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.section .rodata,"a",%progbits
	.global ImiruMura_KeyHeadings
ImiruMura_KeyHeadings:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global ImiruMura_SwayHeadings
ImiruMura_SwayHeadings:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
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
	.global ImiruMura_ActorScriptA
ImiruMura_ActorScriptA:
	.4byte 0x00000022
	.4byte 0x02008031
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global ImiruMura_ActorScriptB
ImiruMura_ActorScriptB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x00740000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global ImiruMura_ActorScriptC
ImiruMura_ActorScriptC:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x009b0000
	.4byte 0x00000000
	.4byte 0x00870000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000010
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffd000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000010
	.global ImiruMura_TurnScript
ImiruMura_TurnScript:
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffd000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000010
	.global ImiruMura_SparkScript
ImiruMura_SparkScript:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff999a
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000048
	.4byte 0x00000000
	.4byte 0x0000001b
	.global ImiruMura_MiaScriptA
ImiruMura_MiaScriptA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00a10000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00860000
	.4byte 0x00000000
	.4byte 0x008b0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00870000
	.4byte 0x00000000
	.4byte 0x00bd0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global ImiruMura_MiaScriptB
ImiruMura_MiaScriptB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a50000
	.4byte 0x00000000
	.4byte 0x00cf0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00930000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00960000
	.4byte 0x00000000
	.4byte 0x010c0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global ImiruMura_PrimaryScript
ImiruMura_PrimaryScript:
	.4byte 0x00000022
	.4byte 0x020080d9
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global ImiruMura_PrimaryScript2
ImiruMura_PrimaryScript2:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x0000017c
	.4byte 0x400001b1
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000120
	.4byte 0xc00001e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001e0
	.4byte 0x80000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000138
	.4byte 0x400001a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000001a8
	.4byte 0x40000188
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000001c8
	.4byte 0x400000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000178
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000078
	.4byte 0x400000d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000040
	.4byte 0x40000168
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000000e5
	.4byte 0x40000058
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
	.4byte 0x00000088
	.4byte 0xc00000f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000090
	.4byte 0xc00000f2
	.4byte 0x00200000
	.4byte 0x01000034
	.4byte 0x00000118
	.4byte 0xffff0002
	.4byte 0x0000008f
	.4byte 0xc0000254
	.4byte 0x002a0000
	.4byte 0x010001a8
	.4byte 0x00000277
	.4byte 0xffff0003
	.4byte 0x0000018f
	.4byte 0xc0000142
	.4byte 0x01400000
	.4byte 0x0218003a
	.4byte 0x00000168
	.4byte 0xffff0004
	.4byte 0x00000190
	.4byte 0xc0000293
	.4byte 0x01330000
	.4byte 0x0221017e
	.4byte 0x000002b9
	.4byte 0xffff0005
	.4byte 0x00000081
	.4byte 0xc0000393
	.4byte 0x00320000
	.4byte 0x015e02c2
	.4byte 0x000003b5
	.4byte 0xffff0006
	.4byte 0x000001be
	.4byte 0x40000339
	.4byte 0x01920000
	.4byte 0x029e02e4
	.4byte 0x00000399
	.4byte 0xffff0007
	.4byte 0x000002af
	.4byte 0xc0000108
	.4byte 0x02520000
	.4byte 0x03260031
	.4byte 0x00000127
	.4byte 0xffff0008
	.4byte 0x000000b6
	.4byte 0x40000304
	.4byte 0x00320000
	.4byte 0x015e02c2
	.4byte 0x000003b5
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global ImiruMura_SceneTable
ImiruMura_SceneTable:
	.4byte 0x00000032
	.4byte 0x00101033
	.4byte 0x00205033
	.4byte 0x00303033
	.4byte 0x00404033
	.4byte 0x00502033
	.4byte 0x00607033
	.4byte 0x00801034
	.4byte 0x00b09002
	.4byte 0x00c27002
	.4byte 0x00000033
	.4byte 0x00103032
	.4byte 0x00207032
	.4byte 0x00305032
	.4byte 0x00406032
	.4byte 0x00504032
	.4byte 0x00608033
	.4byte 0x00708032
	.4byte 0x00806033
	.4byte 0x000001ff
	.4byte 0xffff006c
	.4byte 0x0200a450
	.4byte 0x00630000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x00003000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00003000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00023000
	.4byte 0xffff00ea
	.4byte 0x00000001
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00630000
	.4byte 0x00000000
	.4byte 0x00ea0000
	.4byte 0x0000f000
	.4byte 0xffff006b
	.4byte 0x00000002
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00003000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00023000
	.4byte 0xffff00ea
	.4byte 0x00000001
	.4byte 0x01060000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x006f0000
	.4byte 0x00000000
	.4byte 0x00d70000
	.4byte 0x00004000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x00740000
	.4byte 0x00024000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00ae0000
	.4byte 0x00000000
	.4byte 0x00ad0000
	.4byte 0x00024000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x00760000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0000c000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00024000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x01de0000
	.4byte 0x00000000
	.4byte 0x01d40000
	.4byte 0x00024000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x01cc0000
	.4byte 0x00000000
	.4byte 0x010a0000
	.4byte 0x00005000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x01b40000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x033e0000
	.4byte 0x00014000
	.4byte 0x00000080
	.4byte 0x00000002
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00004000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00f90000
	.4byte 0x00000000
	.4byte 0x03400000
	.4byte 0x00010000
	.4byte 0x0000006b
	.4byte 0x00000001
	.4byte 0x02ae0000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00004000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x02cd0000
	.4byte 0x00000000
	.4byte 0x00e50000
	.4byte 0x0000b000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x000000df
	.4byte 0x00000007
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff003c
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff003d
	.4byte 0x0000000c
	.4byte 0x00008c15
	.4byte 0x0200000b
	.4byte 0x02009961
	.4byte 0x0000c602
	.4byte 0xffff0033
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0xffff0034
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0xffff0035
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0xffff0036
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0xffff0037
	.4byte 0x020086c1
	.4byte 0x0000c602
	.4byte 0xffff0038
	.4byte 0x020086c1
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte 0x02009991
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte 0x020081e1
	.4byte 0x00000000
	.4byte 0x08810008
	.4byte 0x0200821d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000163a
	.4byte 0x00000000
	.4byte 0x08810009
	.4byte 0x0200828d
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000163b
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x020082b9
	.4byte 0x00008d15
	.4byte 0x08810008
	.4byte 0x02008255
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001640
	.4byte 0x00008d15
	.4byte 0x08810009
	.4byte 0x00001530
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001641
	.4byte 0x00008d15
	.4byte 0x0881000a
	.4byte 0x00001531
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001642
	.4byte 0x00000023
	.4byte 0x0f5f0064
	.4byte 0x00200009
	.4byte 0x00000013
	.4byte 0x0f600065
	.4byte 0x001000b6
	.4byte 0x00000083
	.4byte 0x0f610066
	.4byte 0x001000e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x082c0008
	.4byte 0x00001532
	.4byte 0x00000000
	.4byte 0x082b0008
	.4byte 0x00001539
	.4byte 0x00000000
	.4byte 0x08810008
	.4byte 0x020085dd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001643
	.4byte 0x00000000
	.4byte 0x082c0009
	.4byte 0x02008385
	.4byte 0x00000000
	.4byte 0x082b0009
	.4byte 0x00001538
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008385
	.4byte 0x00000000
	.4byte 0x182b000a
	.4byte 0x0200865d
	.4byte 0x00000000
	.4byte 0x0881000b
	.4byte 0x0000153c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001649
	.4byte 0x00000000
	.4byte 0x0881000c
	.4byte 0x02008511
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x020087a5
	.4byte 0x00000000
	.4byte 0x0881000d
	.4byte 0x00001544
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0200888d
	.4byte 0x00000000
	.4byte 0x0881000f
	.4byte 0x00001548
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008975
	.4byte 0x00000000
	.4byte 0x0881000e
	.4byte 0x02008975
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001650
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008a3d
	.4byte 0x00000000
	.4byte 0x08810011
	.4byte 0x0000154c
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001656
	.4byte 0x00000000
	.4byte 0x08810012
	.4byte 0x02008531
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001657
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02008abd
	.4byte 0x00000000
	.4byte 0x08810014
	.4byte 0x02008551
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001672
	.4byte 0x00008d15
	.4byte 0x08810008
	.4byte 0x02008609
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001647
	.4byte 0x00008d15
	.4byte 0x082c0409
	.4byte 0x02008385
	.4byte 0x00008d15
	.4byte 0x082b0009
	.4byte 0x0000153a
	.4byte 0x00008d15
	.4byte 0x08810009
	.4byte 0x0000156e
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001648
	.4byte 0x00008d15
	.4byte 0x182b000a
	.4byte 0x02008669
	.4byte 0x00008d15
	.4byte 0x0881000b
	.4byte 0x0000153d
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000164a
	.4byte 0x00008d15
	.4byte 0x0881000c
	.4byte 0x00001542
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000164c
	.4byte 0x00008d15
	.4byte 0x0881000d
	.4byte 0x00001545
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000164e
	.4byte 0x00008d15
	.4byte 0x0881000f
	.4byte 0x0000154a
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001651
	.4byte 0x00008d15
	.4byte 0x0881000e
	.4byte 0x00001549
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001652
	.4byte 0x00008d15
	.4byte 0x08810010
	.4byte 0x00001550
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001658
	.4byte 0x00008d15
	.4byte 0x08810011
	.4byte 0x00001551
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001659
	.4byte 0x00008d15
	.4byte 0x08810012
	.4byte 0x00001552
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000165a
	.4byte 0x00008d15
	.4byte 0x08810013
	.4byte 0x02008689
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001673
	.4byte 0x00008d15
	.4byte 0x08810014
	.4byte 0x020085a5
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001674
	.4byte 0x0000c403
	.4byte 0x0881000a
	.4byte 0x020087a5
	.4byte 0x0000c403
	.4byte 0x0881000b
	.4byte 0x0200888d
	.4byte 0x00000013
	.4byte 0x0f260064
	.4byte 0x001000b9
	.4byte 0x00000033
	.4byte 0x0f620065
	.4byte 0x001000e3
	.4byte 0x00000173
	.4byte 0x0f630066
	.4byte 0x001000c4
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00402999
	.4byte 0x00000173
	.4byte 0xffff0066
	.4byte 0x0040299a
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
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00250028
	.4byte 0x00020004
	.4byte 0x00280005
	.4byte 0x00040027
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x00230021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00250023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020025
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x0002002b
	.4byte 0x00050002
	.4byte 0x002b0021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00270023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020027
	.4byte 0x00050002
	.4byte 0x002affff
	.4byte 0x00020030
	.4byte 0x00050002
	.4byte 0x00300028
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x002e002a
	.4byte 0x00020002
	.4byte 0x00280005
	.4byte 0x0002002e
	.4byte 0x00050002
	.4byte 0x0000ffff
	.global ImiruMura_ExitCellSteps
ImiruMura_ExitCellSteps:
	.4byte 0x0200b14c
	.4byte 0x0200b136
	.4byte 0x0200b162
	.4byte 0x0200b178
	.4byte 0x0200b120
	.4byte 0x0200b0f4
	.global ImiruMura_ExitCellPoints
ImiruMura_ExitCellPoints:
	.4byte 0x00180032
	.4byte 0x00160039
	.4byte 0x000d003b
	.4byte 0x000a0036
	.4byte 0x00080026
	.4byte 0x00130022
	.global ImiruMura_CellStepsA
ImiruMura_CellStepsA:
	.4byte 0x007d0001
	.4byte 0x00020001
	.4byte 0x0000000a
	.4byte 0x0001007d
	.4byte 0x000a0002
	.2byte 0xffff
	.global ImiruMura_CellStepsB
ImiruMura_CellStepsB:
	.2byte 0x0001
	.4byte 0x0001007d
	.4byte 0x000a0002
	.4byte 0x007d0002
	.4byte 0x00020001
	.4byte 0xffff000a
	.section .bss,"aw",%nobits
	.space 4
	.global ImiruMura_ArcOrigin
ImiruMura_ArcOrigin:
	.space 12
