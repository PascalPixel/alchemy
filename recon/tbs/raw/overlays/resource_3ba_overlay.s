.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200c194
	.global Func_02000038
	.thumb_func
Func_02000038:
	movs r0, #0
	bx lr
	.global Func_0200003c
	.thumb_func
Func_0200003c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200c1dc
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200c1f4
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {r5, r6, r7, lr}
	ldr r7, [pc, #224]
	ldr r3, [r7]
	sub sp, #8
	cmp r3, #6
	beq .L_0200004c_0
	cmp r3, #6
	bhi .L_0200004c_1
	cmp r3, #0
	beq .L_0200004c_2
	b .L_0200004c_3
.L_0200004c_1:
	cmp r3, #60
	beq .L_0200004c_4
	cmp r3, #66
	bne .L_0200004c_3
	b .L_0200004c_0
.L_0200004c_4:
	movs r6, #50
	movs r5, #38
	movs r0, #92
	movs r1, #33
	movs r2, #2
	movs r3, #2
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bc00
	movs r3, #54
	str r3, [sp, #0]
	movs r0, #92
	movs r1, #33
	movs r2, #2
	movs r3, #2
	str r5, [sp, #4]
	bl 0x0200bc00
	movs r3, #12
	str r3, [sp, #4]
	movs r0, #50
	movs r1, #25
	movs r2, #6
	movs r3, #1
	str r6, [sp, #0]
	bl 0x0200bbf8
	movs r0, #16
	movs r1, #11
	bl 0x0200bd00
	b .L_0200004c_3
.L_0200004c_0:
	movs r3, #50
	str r3, [sp, #0]
	movs r5, #38
	movs r0, #92
	movs r1, #31
	movs r2, #2
	movs r3, #2
	str r5, [sp, #4]
	bl 0x0200bc00
	movs r3, #54
	str r3, [sp, #0]
	movs r0, #92
	movs r1, #31
	movs r2, #2
	movs r3, #2
	str r5, [sp, #4]
	bl 0x0200bc00
	movs r0, #16
	movs r1, #10
	bl 0x0200bd00
	b .L_0200004c_3
.L_0200004c_2:
	movs r6, #50
	movs r5, #38
	movs r0, #92
	movs r1, #29
	movs r2, #2
	movs r3, #2
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bc00
	movs r3, #54
	str r3, [sp, #0]
	movs r2, #2
	movs r3, #2
	movs r0, #92
	movs r1, #29
	str r5, [sp, #4]
	bl 0x0200bc00
	movs r0, #16
	movs r1, #12
	bl 0x0200bd00
	movs r3, #12
	str r3, [sp, #4]
	movs r0, #50
	movs r3, #1
	movs r1, #24
	movs r2, #6
	str r6, [sp, #0]
	bl 0x0200bbf8
	movs r3, #120
	str r3, [r7]
.L_0200004c_3:
	ldr r3, [r7]
	subs r3, #1
	str r3, [r7]
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200c41c
	.global Func_02000134
	.thumb_func
Func_02000134:
	push {r5, lr}
	ldr r3, [pc, #24]
	ldr r5, [pc, #24]
	movs r2, #0
	str r2, [r3]
	adds r0, r5, #0
	bl 0x0200bb18
	bl 0x0200be1c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200c41c
	.4byte 0x0200804d
	.global Func_02000158
	.thumb_func
Func_02000158:
	push {lr}
	ldr r3, [pc, #20]
	movs r2, #66
	movs r1, #200
	str r2, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #12]
	bl 0x0200bb10
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200c41c
	.4byte 0x0200804d
	.global Func_02000178
	.thumb_func
Func_02000178:
	push {r5, r6, lr}
	movs r0, #10
	bl 0x0200bb08
	ldr r2, [pc, #32]
	ldr r3, [r2]
	movs r5, #0
	cmp r3, #22
	beq .L_02000178_0
	adds r6, r2, #0
.L_02000178_1:
	movs r0, #1
	adds r5, #1
	bl 0x0200bb08
	cmp r5, #119
	bgt .L_02000178_0
	ldr r3, [r6]
	cmp r3, #22
	bne .L_02000178_1
.L_02000178_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0200c41c
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	push {r5, lr}
	sub sp, #8
	movs r3, #23
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r1, #13
	movs r2, #3
	movs r0, #27
	bl 0x0200bbf8
	movs r0, #9
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl 0x0200bbe8
	ldr r3, [r5, #12]
	cmp r3, #0
	bne .L_020001a8_0
	cmp r0, #0
	bne .L_020001a8_0
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	ldr r2, [r5, #8]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl 0x0200bbf8
.L_020001a8_0:
	movs r0, #10
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r1, [r5, #8]
	movs r0, #196
	asrs r1, r1, #20
	lsls r0, r0, #2
	bl 0x0200bc70
	ldr r2, [r5, #8]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl 0x0200bbf8
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000238
	.thumb_func
Func_02000238:
	push {lr}
	bl 0x0200b8f8
	bl 0x020081a8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000248
	.thumb_func
Func_02000248:
	push {lr}
	sub sp, #8
	movs r3, #47
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #24
	movs r2, #1
	movs r3, #1
	movs r0, #47
	bl 0x0200bbf8
	ldr r0, [pc, #8]
	bl 0x0200bc60
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000303
	.global Func_02000270
	.thumb_func
Func_02000270:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r0, [pc, #180]
	sub sp, #8
	bl 0x0200bc60
	movs r0, #13
	bl 0x0200bcb8
	adds r5, r0, #0
	bl 0x0200bca0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200bd70
	movs r0, #150
	movs r1, #1
	movs r2, #200
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	bl 0x0200bd78
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200bba8
	bl 0x0200bd80
	movs r2, #0
	mov r10, r2
	adds r3, r5, #0
	mov r2, r10
	adds r3, #85
	strb r2, [r3]
	ldr r6, [pc, #112]
	ldr r3, [pc, #112]
	movs r2, #128
	ldr r1, [r5, #8]
	mov r8, r3
	str r3, [r5, #52]
	str r6, [r5, #48]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	lsls r2, r2, #12
	bl 0x0200bbd8
	movs r0, #14
	bl 0x0200bcb8
	adds r5, r0, #0
	adds r3, r5, #0
	mov r2, r10
	adds r3, #85
	strb r2, [r3]
	mov r3, r8
	movs r2, #128
	ldr r1, [r5, #8]
	lsls r2, r2, #14
	str r3, [r5, #52]
	str r6, [r5, #48]
	ldr r3, [r5, #16]
	bl 0x0200bbd8
	adds r0, r5, #0
	bl 0x0200bbe0
	movs r0, #45
	bl 0x0200bc98
	movs r3, #41
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #43
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl 0x0200bbf8
	bl 0x0200bca8
	sub sp, #-8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000301
	.4byte 0x0000cccc
	.4byte 0x00006666
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r2, [sp, #0]
	ldr r3, [pc, #212]
	movs r2, #250
	str r1, [sp, #4]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl 0x0200bcb8
	adds r6, r0, #0
	adds r0, r5, #0
	bl 0x0200bcb8
	adds r7, r0, #0
	bl 0x0200bca0
	ldr r3, [sp, #4]
	lsls r3, r3, #16
	mov r11, r3
	ldr r3, [r6, #8]
	ldr r2, [pc, #176]
	add r3, r11
	movs r5, #128
	lsls r5, r5, #12
	ands r3, r2
	adds r1, r3, r5
	ldr r3, [sp, #0]
	lsls r3, r3, #16
	mov r9, r3
	ldr r3, [r6, #16]
	add r3, r9
	mov r10, r2
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r2, [r6, #48]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r5
	mov r8, r2
	str r2, [r6, #52]
	adds r0, r6, #0
	ldr r2, [r6, #12]
	bl 0x0200bbd8
	adds r0, r6, #0
	movs r1, #27
	bl 0x0200bba8
	ldr r3, [r7, #8]
	mov r2, r10
	add r3, r11
	ands r3, r2
	adds r1, r3, r5
	ldr r3, [r7, #16]
	add r3, r9
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r2, [r7, #48]
	mov r2, r8
	adds r3, r3, r5
	str r2, [r7, #52]
	adds r0, r7, #0
	ldr r2, [r7, #12]
	bl 0x0200bbd8
	ldr r3, [sp, #4]
	cmp r3, #0
	blt .L_0200033c_0
	ldr r2, [sp, #0]
	cmp r2, #0
	bge 0x020083ea
.L_0200033c_0:
	adds r0, r7, #0
	movs r1, #4
	bl 0x0200bba8
.L_020003e8:
	b .L_020003e8_0
	.2byte 0x1c38
	.2byte 0x2103
	.2byte 0xf003
	.2byte 0xfbdb
.L_020003e8_0:
	movs r0, #226
	bl 0x0200bdf8
	adds r0, r6, #0
	bl 0x0200bbe0
	movs r1, #2
	adds r0, r7, #0
	bl 0x0200bba8
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200bdf8
	bl 0x0200bca8
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0xfff0
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	push {r5, r6, lr}
	ldr r3, [pc, #92]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl 0x0200bcb8
	ldr r3, [r0, #16]
	movs r6, #48
	asrs r0, r3, #20
	negs r6, r6
	cmp r0, #8
	bgt .L_0200042c_0
	movs r6, #48
.L_0200042c_0:
	str r0, [sp, #4]
	movs r3, #1
	movs r5, #64
	movs r0, #67
	movs r1, #8
	movs r2, #3
	str r5, [sp, #0]
	bl 0x0200bbf8
	adds r2, r6, #0
	movs r1, #0
	movs r0, #17
	bl 0x0200833c
	movs r0, #17
	bl 0x0200bcb8
	ldr r3, [r0, #16]
	asrs r0, r3, #20
	str r0, [sp, #4]
	movs r1, #24
	movs r0, #64
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200bbf8
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.global Func_02000490
	.thumb_func
Func_02000490:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #164]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl 0x0200bcb8
	ldr r1, [pc, #152]
	ldr r3, [r0, #8]
	asrs r7, r3, #20
	ldr r3, [r1]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_02000490_0
	movs r5, #1
	negs r5, r5
.L_02000490_0:
	ldr r3, [r1]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_02000490_1
	movs r5, #1
.L_02000490_1:
	movs r0, #17
	bl 0x0200bcb8
	ldr r3, [r0, #16]
	asrs r6, r3, #20
	cmp r7, #63
	bne .L_02000490_2
	cmp r6, #11
	beq .L_02000490_3
	movs r6, #160
	b .L_02000490_4
.L_02000490_2:
	cmp r7, #67
	bne .L_02000490_5
	cmp r6, #11
	bne .L_02000490_6
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	beq .L_02000490_3
.L_02000490_6:
	movs r6, #96
	b .L_02000490_4
.L_02000490_5:
	cmp r6, #11
	bne .L_02000490_7
	movs r6, #96
	b .L_02000490_8
.L_02000490_7:
	movs r6, #160
.L_02000490_8:
	negs r6, r6
.L_02000490_4:
	movs r3, #3
	movs r5, #9
	movs r0, #72
	movs r1, #9
	movs r2, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bbf8
	adds r1, r6, #0
	movs r2, #0
	movs r0, #18
	bl 0x0200833c
	movs r0, #18
	bl 0x0200bcb8
	ldr r3, [r0, #8]
	movs r1, #25
	asrs r7, r3, #20
	movs r0, #63
	movs r2, #1
	movs r3, #3
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bbf8
.L_02000490_3:
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x03001ae8
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #700]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [r3, #0]
	adds	r0, r5, #0
	sub	sp, #12
	bl 0x0200bcb8
	str	r0, [sp, #8]
	movs	r0, #12
	bl 0x0200bcb8
	adds	r7, r0, #0
	ldr	r0, [pc, #676]
	bl 0x0200bc60
	bl 0x0200bca0
	movs	r1, #8
	adds	r0, r5, #0
	bl 0x0200bd00
	movs	r0, #6
	bl 0x0200bc98
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r7, #48]
	ldr	r3, [pc, #648]
	movs	r0, #239
	str	r3, [r7, #52]
	mov	r8, r3
	bl 0x0200bdf8
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x0200bba8
	ldr	r1, [r7, #8]
	ldr	r2, [pc, #632]
	ldr	r3, [r7, #16]
	adds	r1, r1, r2
	adds	r0, r7, #0
	movs	r2, #0
	bl 0x0200bbd8
	movs	r0, #6
	bl 0x0200bc98
	adds	r0, r5, #0
	movs	r1, #2
	bl 0x0200bd00
	ldr	r1, [pc, #608]
	movs	r0, #27
	bl 0x0200bb40
	movs	r3, #240
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r0, [r0, #0]
	adds	r1, r7, #0
	bl 0x0200bbc0
	adds	r0, r5, #0
	ldr	r1, [pc, #588]
	mov	r2, r8
	bl 0x0200bcc8
	ldr	r2, [sp, #8]
	ldr	r3, [pc, #580]
	ldr	r1, [r2, #8]
	adds	r0, r2, #0
	adds	r1, r1, r3
	ldr	r3, [r2, #16]
	movs	r2, #0
	bl 0x0200bbd8
	adds	r0, r5, #0
	bl 0x0200bcf0
	adds	r0, r5, #0
	movs	r1, #1
	bl 0x0200bd00
	adds	r0, r7, #0
	bl 0x0200bbe0
	movs	r1, #1
	adds	r0, r7, #0
	bl 0x0200bba8
	movs	r0, #144
	lsls	r0, r0, #1
	bl 0x0200bdf8
	movs	r0, #213
	bl 0x0200bdf8
	movs	r0, #15
	bl 0x0200bc98
	bl 0x0200bca8
	movs	r5, #7
	movs	r0, #37
	movs	r1, #7
	movs	r2, #1
	movs	r3, #4
	movs	r6, #34
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf8
	movs	r3, #37
	str	r3, [sp, #0]
	movs	r0, #36
	movs	r1, #7
	movs	r2, #1
	movs	r3, #4
	str	r5, [sp, #4]
	bl 0x0200bbf8
	ldr	r0, [pc, #480]
	bl 0x0200bc58
	mov	sl, r0
	cmp	r0, #0
	beq.n	.L_020006ea
	bl 0x0200bca0
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200bd70
	movs	r0, #138
	movs	r1, #1
	movs	r2, #200
	lsls	r0, r0, #18
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x0200bd78
	bl 0x0200bd80
	movs	r5, #38
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #96
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #97
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #98
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #99
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r0, #100
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #15
	bl 0x0200bc98
	bl 0x0200bca8
	b.n	.L_020007f8
.L_020006ea:
	ldr	r0, [pc, #312]
	bl 0x0200bc60
	bl 0x0200bca0
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200bd70
	movs	r0, #150
	movs	r1, #1
	movs	r2, #200
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	lsls	r0, r0, #18
	bl 0x0200bd78
	bl 0x0200bd80
	movs	r0, #13
	bl 0x0200bcb8
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	ldr	r2, [pc, #256]
	ldr	r3, [pc, #256]
	str	r2, [r7, #48]
	mov	r9, r2
	movs	r2, #128
	str	r3, [r7, #52]
	ldr	r1, [r7, #8]
	lsls	r2, r2, #12
	mov	r8, r3
	ldr	r3, [r7, #16]
	bl 0x0200bbd8
	adds	r0, r7, #0
	movs	r1, #3
	bl 0x0200bba8
	movs	r5, #38
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #96
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #97
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #98
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
	movs	r2, #1
	movs	r3, #3
	movs	r0, #99
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #3
	bl 0x0200bc98
	movs	r1, #29
.L_020007a2:
	movs	r2, #1
	movs	r3, #3
	movs	r0, #100
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl 0x0200bbf0
	movs	r0, #14
	bl 0x0200bcb8
	adds	r7, r0, #0
	adds	r3, r7, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	mov	r2, r9
	mov	r3, r8
	str	r2, [r7, #48]
	movs	r2, #128
	ldr	r1, [r7, #8]
	lsls	r2, r2, #14
	str	r3, [r7, #52]
	ldr	r3, [r7, #16]
	bl 0x0200bbd8
	adds	r0, r7, #0
	bl 0x0200bbe0
	movs	r0, #15
	bl 0x0200bc98
	bl 0x0200bca8
	movs	r3, #41
	movs	r2, #12
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #43
	movs	r1, #12
	movs	r2, #1
	movs	r3, #1
	bl 0x0200bbf8
.L_020007f8:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x00000302
	.4byte 0x00003333
	.4byte 0xffd00000
	.4byte 0x00000ccc
	.4byte 0x00004ccc
	.4byte 0xffe80000
	.4byte 0x00000301
	.4byte 0x0000cccc
	.2byte 0x6666
	.2byte 0x0000
	.global Func_02000830
	.thumb_func
Func_02000830:
	push {lr}
	movs r0, #192
	lsls r0, r0, #2
	bl 0x0200bc60
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000840
	.thumb_func
Func_02000840:
	push {r5, r6, lr}
	bl 0x0200af90
	bl 0x0200bca0
	movs r1, #127
	movs r0, #120
	bl 0x0200b0ac
	adds r6, r0, #0
	bl 0x0200afa0
	movs r5, #9
.L_02000840_0:
	movs r0, #8
	subs r5, #1
	bl 0x0200bcd0
	cmp r5, #0
	bge .L_02000840_0
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #165
	movs r0, #8
	lsls r1, r1, #3
	movs r2, #192
	bl 0x0200bce0
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #161
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #3
	bl 0x0200bce8
	movs r0, #8
	movs r1, #1
	bl 0x0200bd00
	movs r2, #0
	movs r1, #8
	movs r0, #0
	bl 0x0200bd28
	movs r0, #10
	bl 0x0200bc98
	movs r0, #8
	movs r1, #3
	bl 0x0200bd00
	movs r1, #3
	movs r0, #0
	bl 0x0200bd08
	movs r0, #20
	bl 0x0200bc98
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200bcc8
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200bcc8
	movs r1, #162
	movs r0, #0
	lsls r1, r1, #3
	movs r2, #192
	bl 0x0200bce0
	movs r1, #164
	movs r2, #192
	movs r0, #8
	lsls r1, r1, #3
	bl 0x0200bce8
	movs r0, #0
	movs r1, #16
	bl 0x0200bd00
	movs r1, #9
	movs r0, #8
	bl 0x0200bd00
	movs r0, #10
	bl 0x0200bc98
	movs r1, #0
	subs r1, r1, r6
	adds r1, #1
	movs r0, #72
	bl 0x0200bd90
	ldr r3, [pc, #40]
	ldr r2, [pc, #40]
	adds r3, r3, r2
	movs r2, #3
	strb r2, [r3]
	ldr r5, [pc, #36]
	movs r1, #4
	adds r0, r5, #0
	bl 0x0200bd98
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200bda0
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200bc60
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0000022b
	.4byte 0x0000008f
	.global Func_02000954
	.thumb_func
Func_02000954:
	push {lr}
	bl 0x0200ba60
	cmp r0, #0
	bne .L_02000954_0
	bl 0x0200bda8
	b .L_02000954_1
.L_02000954_0:
	bl 0x02008238
.L_02000954_1:
	pop {r0}
	bx r0
	.global Func_0200096c
	.thumb_func
Func_0200096c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200c420
	.global Func_02000974
	.thumb_func
Func_02000974:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #24
	bl 0x0200bcc0
	movs r0, #25
	bl 0x0200bcc0
	movs r0, #1
	bl 0x0200bc90
	bl 0x0200bca0
	movs r1, #165
	movs r2, #192
	movs r0, #8
	lsls r1, r1, #19
	lsls r2, r2, #16
	bl 0x0200bcf8
	movs r1, #161
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #19
	lsls r2, r2, #16
	bl 0x0200bcf8
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bd20
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bd20
	cmp r5, #0
	bge .L_02000974_0
	movs r0, #8
	movs r1, #10
	bl 0x0200bd00
	movs r0, #0
	movs r1, #35
	bl 0x0200bd00
	b .L_02000974_1
.L_02000974_0:
	movs r0, #8
	movs r1, #8
	bl 0x0200bd00
	movs r0, #0
	movs r1, #28
	bl 0x0200bd00
.L_02000974_1:
	movs r0, #1
	bl 0x0200bb08
	movs r0, #163
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #16
	movs r3, #0
	lsls r0, r0, #19
	bl 0x0200bd78
	adds r0, r5, #0
	bl 0x0200a844
	bl 0x0200bca8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000a10
	.thumb_func
Func_02000a10:
	push {lr}
	bl 0x0200bcb8
	cmp r0, #0
	beq .L_02000a10_0
	ldr r3, [r0, #8]
	ldr r2, [r0, #16]
	asrs r3, r3, #19
	subs r3, #94
	asrs r2, r2, #19
	cmp r3, #1
	bhi .L_02000a10_0
	cmp r2, #23
	ble .L_02000a10_0
	cmp r2, #26
	bgt .L_02000a10_0
	adds r2, r0, #0
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
.L_02000a10_0:
	pop {r0}
	bx r0
	.global Func_02000a3c
	.thumb_func
Func_02000a3c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #832]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	movs r0, #162
	str r2, [r3]
	lsls r0, r0, #1
	sub sp, #12
	bl 0x0200bc60
	movs r0, #9
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl 0x0200bbe8
	ldr r3, [r5, #12]
	cmp r3, #0
	bne .L_02000a3c_0
	cmp r0, #0
	bne .L_02000a3c_0
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	ldr r2, [r5, #8]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl 0x0200bbf8
.L_02000a3c_0:
	movs r0, #196
	lsls r0, r0, #2
	bl 0x0200bc68
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02000a3c_1
	movs r6, #25
.L_02000a3c_1:
	movs r0, #10
	bl 0x0200bcb8
	movs r2, #128
	lsls r2, r2, #12
	mov r9, r2
	lsls r3, r6, #20
	adds r5, r0, #0
	add r3, r9
	movs r1, #0
	str r3, [r5, #8]
	mov r8, r1
	adds r3, r5, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r7, #2
	subs r3, #50
	strb r7, [r3]
	movs r3, #12
	str r3, [sp, #4]
	movs r2, #1
	movs r0, #14
	movs r1, #13
	mov r10, r3
	movs r3, #1
	str r6, [sp, #0]
	bl 0x0200bbf8
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #668]
	bl 0x0200bb10
	movs r0, #15
	bl 0x0200bcb8
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	ldr r0, [pc, #648]
	bl 0x0200bc58
	cmp r0, #0
	beq .L_02000a3c_2
	adds r0, r5, #0
	movs r1, #4
	bl 0x0200bba8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bc10
	adds r3, r5, #0
	adds r3, #89
	mov r1, r8
	strb r1, [r3]
	movs r2, #3
	subs r3, #54
	strb r2, [r3]
	movs r3, #47
	mov r2, r10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #24
	movs r2, #1
	movs r3, #1
	bl 0x0200bbf8
.L_02000a3c_2:
	movs r0, #17
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r3, [r5, #16]
	asrs r2, r3, #20
	adds r3, r5, #0
	adds r3, #85
	mov r1, r8
	strb r1, [r3]
	subs r3, #50
	strb r7, [r3]
	movs r3, #64
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #24
	movs r2, #3
	movs r3, #1
	movs r0, #64
	bl 0x0200bbf8
	movs r0, #18
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r2, r3, #20
	adds r3, r5, #0
	adds r3, #85
	mov r1, r8
	strb r1, [r3]
	subs r3, #50
	strb r7, [r3]
	movs r3, #9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #63
	movs r1, #25
	movs r2, #1
	movs r3, #3
	bl 0x0200bbf8
	ldr r0, [pc, #508]
	bl 0x0200bc58
	cmp r0, #0
	beq .L_02000a3c_3
	movs r6, #34
	movs r5, #7
	movs r0, #37
	movs r1, #7
	movs r2, #1
	movs r3, #4
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200bbf8
	movs r3, #37
	str r3, [sp, #0]
	movs r0, #36
	movs r1, #7
	movs r2, #1
	movs r3, #4
	str r5, [sp, #4]
	bl 0x0200bbf8
	movs r3, #38
	str r3, [sp, #4]
	movs r0, #100
	movs r1, #29
	movs r2, #1
	movs r3, #3
	str r6, [sp, #0]
	bl 0x0200bbf0
.L_02000a3c_3:
	movs r0, #13
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r0, [pc, #440]
	bl 0x0200bc58
	cmp r0, #0
	beq .L_02000a3c_4
	movs r3, #41
	mov r2, r10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #43
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl 0x0200bbf8
	adds r3, r5, #0
	adds r3, #85
	mov r1, r8
	strb r1, [r3]
	ldr r3, [pc, #404]
	str r3, [r5, #52]
	ldr r3, [pc, #404]
	mov r2, r9
	str r3, [r5, #48]
	str r2, [r5, #12]
	adds r0, r5, #0
	movs r1, #3
	bl 0x0200bba8
	b .L_02000a3c_5
.L_02000a3c_4:
	adds r0, r5, #0
	movs r1, #2
	bl 0x0200bba8
.L_02000a3c_5:
	movs r0, #14
	bl 0x0200bcb8
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	movs r1, #120
	movs r0, #24
	bl 0x0200aea0
	movs r1, #127
	movs r0, #25
	bl 0x0200aea0
	ldr r3, [pc, #356]
	movs r1, #225
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #1
	cmp r3, #4
	bls .L_02000a3c_6
	b .L_02000a3c_7
.L_02000a3c_6:
	ldr r2, [pc, #340]
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	movs r0, r0
	ldrh r0, [r6, #34]
	lsls r0, r0, #8
	ldrh r0, [r2, #40]
	lsls r0, r0, #8
	ldrh r2, [r0, #42]
	lsls r0, r0, #8
	ldrh r0, [r3, #42]
	lsls r0, r0, #8
	ldrh r6, [r4, #42]
	lsls r0, r0, #8
	movs r2, #192
	lsls r2, r2, #16
	str r2, [sp, #0]
	movs r2, #24
	str r2, [sp, #4]
	movs r3, #163
	movs r2, #25
	str r2, [sp, #8]
	lsls r3, r3, #19
	movs r1, #8
	movs r2, #4
	movs r0, #0
	bl 0x0200b764
	movs r3, #19
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #2
	movs r0, #127
	bl 0x0200bc00
	movs r0, #19
	bl 0x0200bcc0
	movs r0, #20
	bl 0x0200bcc0
	movs r0, #21
	bl 0x0200bcc0
	movs r0, #22
	bl 0x0200bcc0
	movs r0, #23
	bl 0x0200bcc0
	ldr r0, [pc, #236]
	bl 0x0200bc58
	cmp r0, #0
	bne .L_02000a3c_8
	movs r0, #17
	bl 0x0200bdf8
	movs r0, #0
	bl 0x02009910
	bl 0x02009898
	movs r0, #1
	bl 0x02008a10
	movs r0, #2
	bl 0x02008a10
	movs r0, #3
	bl 0x02008a10
	movs r0, #1
	bl 0x0200a738
.L_02000a3c_8:
	movs r0, #1
	movs r1, #0
	bl 0x0200bde8
	movs r0, #2
	movs r1, #0
	bl 0x0200bde8
	movs r0, #3
	movs r1, #0
	bl 0x0200bde8
	ldr r0, [pc, #164]
	bl 0x0200b84c
	b .L_02000a3c_7
	.2byte 0x21c8
	.2byte 0x0109
	.2byte 0x4827
	.2byte 0xf002
	.2byte 0xfefb
	.2byte 0x2018
	.2byte 0xf002
	.2byte 0xffd0
	.2byte 0x2019
	.2byte 0xf002
	.2byte 0xffcd
	.2byte 0x4821
	.2byte 0xf002
	.2byte 0xff96
	.2byte 0x2800
	.2byte 0xd121
	.2byte 0xf000
	.2byte 0xfdb2
	.2byte 0x2001
	.2byte 0xf000
	.2byte 0xfdeb
	.2byte 0x2000
	.2byte 0xf001
	.2byte 0xfcfc
	.2byte 0xe018
	.2byte 0x481a
	.2byte 0xf002
	.2byte 0xff88
	.2byte 0x2800
	.2byte 0xd113
	.2byte 0x2013
	.2byte 0xf000
	.2byte 0xf833
	.2byte 0xf000
	.2byte 0xffe5
	.2byte 0xe00d
	.2byte 0x2001
	.2byte 0xf7ff
	.2byte 0xfe0b
	.2byte 0x2004
	.2byte 0xf003
	.2byte 0xf812
	.2byte 0xe006
	.2byte 0x2001
	.2byte 0x4240
	.2byte 0xf7ff
	.2byte 0xfe03
	.2byte 0x2005
	.2byte 0xf003
	.2byte 0xf80a
.L_02000a3c_7:
	movs r0, #0
	sub sp, #-12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x0200804d
	.4byte 0x00000303
	.4byte 0x00000302
	.4byte 0x00000301
	.4byte 0x00006666
	.4byte 0x0000cccc
	.4byte 0x02000240
	.4byte 0x02008c5c
	.4byte 0x00000109
	.4byte 0x000000e4
	.2byte 0x99e1
	.2byte 0x0200
	.global Func_02000db8
	.thumb_func
Func_02000db8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	bl 0x0200bcb8
	movs r3, #10
	ldrsh r2, [r0, r3]
	mov r9, r2
	movs r3, #18
	ldrsh r2, [r0, r3]
	mov r10, r2
	bl 0x0200bca0
	movs r1, #128
	movs r2, #128
	adds r0, r7, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #128
	movs r2, #128
	movs r0, #1
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #128
	movs r2, #128
	movs r0, #2
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #128
	movs r2, #128
	movs r0, #3
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	mov r3, r10
	lsls r5, r3, #16
	mov r2, r9
	ldr r3, [pc, #964]
	lsls r6, r2, #16
	movs r0, #0
	adds r2, r5, r3
	adds r1, r6, #0
	bl 0x0200bcf8
	ldr r3, [pc, #956]
	ldr r2, [pc, #956]
	adds r3, r3, r5
	mov r8, r3
	adds r1, r6, r2
	movs r0, #1
	mov r2, r8
	bl 0x0200bcf8
	movs r2, #128
	lsls r2, r2, #13
	adds r1, r6, r2
	movs r0, #2
	mov r2, r8
	bl 0x0200bcf8
	ldr r3, [pc, #932]
	movs r0, #3
	adds r2, r5, r3
	adds r1, r6, #0
	bl 0x0200bcf8
	ldr r2, [pc, #924]
	adds r5, r5, r2
	adds r2, r5, #0
	adds r1, r6, #0
	adds r0, r7, #0
	bl 0x0200bcf8
	movs r0, #0
	bl 0x0200bcb8
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r1, #0
	movs r0, #0
	bl 0x0200bd68
	bl 0x0200bdb0
	bl 0x0200bdc0
	ldr r0, [pc, #884]
	bl 0x0200bd30
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	movs r2, #60
	movs r0, #3
	ldr r1, [pc, #872]
	bl 0x0200bd60
	movs r0, #3
	movs r1, #0
	bl 0x0200bd40
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200bd10
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	movs r2, #60
	movs r0, #2
	ldr r1, [pc, #836]
	bl 0x0200bd60
	movs r0, #2
	movs r1, #0
	bl 0x0200bd40
	movs r2, #0
	movs r1, #2
	adds r0, r7, #0
	bl 0x0200bd20
	movs r0, #20
	bl 0x0200bc98
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200bd08
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	movs r2, #60
	movs r0, #1
	ldr r1, [pc, #788]
	bl 0x0200bd60
	movs r0, #1
	movs r1, #0
	bl 0x0200bd40
	movs r2, #60
	movs r0, #3
	ldr r1, [pc, #768]
	bl 0x0200bd60
	movs r0, #3
	movs r1, #0
	bl 0x0200bd40
	movs r1, #129
	adds r0, r7, #0
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bd60
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd48
	cmp r0, #0
	beq .L_02000db8_0
	b .L_02000db8_1
.L_02000db8_0:
	ldr r0, [pc, #732]
	bl 0x0200bd30
	movs r1, #3
	movs r0, #2
	bl 0x0200bd00
	movs r0, #2
	bl 0x0200bc98
	movs r1, #3
	movs r0, #1
	bl 0x0200bd00
	movs r0, #2
	bl 0x0200bc98
	movs r1, #3
	movs r0, #3
	bl 0x0200bd00
	movs r0, #1
	bl 0x0200bc98
	movs r0, #0
	movs r1, #3
	bl 0x0200bd08
	adds r0, r7, #0
	movs r1, #3
	bl 0x0200bd08
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	adds r0, r7, #0
	bl 0x0200bd50
	movs r0, #20
	bl 0x0200bc98
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200bd70
	movs r0, #156
	movs r1, #1
	movs r2, #208
	lsls r2, r2, #15
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200bd78
	bl 0x0200bd80
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x0200bd70
	movs r0, #194
	movs r1, #1
	movs r2, #208
	lsls r2, r2, #15
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	bl 0x0200bd78
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	bl 0x0200bd80
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200bd70
	movs r0, #155
	movs r1, #1
	movs r2, #168
	movs r3, #1
	lsls r0, r0, #19
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200bd78
	bl 0x0200bd80
	movs r1, #192
	movs r2, #0
	adds r0, r7, #0
	lsls r1, r1, #7
	bl 0x0200bd20
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	movs r0, #163
	movs r1, #1
	movs r2, #168
	movs r3, #1
	lsls r0, r0, #19
	negs r1, r1
	lsls r2, r2, #16
	bl 0x0200bd78
	bl 0x0200bd80
	movs r2, #0
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd20
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	movs r0, #0
	movs r1, #0
	bl 0x0200bd68
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200bd18
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd48
	cmp r0, #0
	beq .L_02000db8_2
	b .L_02000db8_0
.L_02000db8_2:
	movs r1, #2
	adds r0, r7, #0
	bl 0x0200bd18
	ldr r0, [pc, #400]
	bl 0x0200bd30
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
.L_02000db8_1:
	ldr r0, [pc, #388]
	bl 0x0200bd30
	adds r0, r7, #0
	movs r1, #2
	bl 0x0200bd18
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bd50
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bd50
	movs r1, #128
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl 0x0200bd50
	movs r0, #1
	movs r1, #2
	bl 0x0200bd18
	movs r0, #1
	movs r1, #0
	bl 0x0200bd40
	movs r0, #2
	movs r1, #2
	bl 0x0200bd18
	movs r0, #2
	movs r1, #0
	bl 0x0200bd40
	movs r0, #3
	movs r1, #3
	bl 0x0200bd08
	movs r0, #3
	movs r1, #0
	bl 0x0200bd40
	movs r1, #3
	movs r0, #3
	bl 0x0200bd00
	movs r0, #1
	bl 0x0200bc98
	movs r1, #3
	movs r0, #1
	bl 0x0200bd00
	movs r0, #2
	bl 0x0200bc98
	movs r1, #3
	movs r0, #2
	bl 0x0200bd00
	movs r0, #1
	bl 0x0200bc98
	movs r1, #3
	movs r0, #0
	bl 0x0200bd08
	movs r0, #6
	bl 0x0200bc98
	movs r0, #1
	movs r1, #2
	bl 0x0200bd00
	movs r0, #0
	bl 0x0200bcb8
	cmp r0, #0
	beq .L_02000db8_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x0200bcd8
.L_02000db8_3:
	movs r0, #2
	movs r1, #2
	bl 0x0200bd00
	movs r0, #0
	bl 0x0200bcb8
	cmp r0, #0
	beq .L_02000db8_4
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x0200bcd8
.L_02000db8_4:
	movs r0, #3
	movs r1, #2
	bl 0x0200bd00
	movs r0, #0
	bl 0x0200bcb8
	cmp r0, #0
	beq .L_02000db8_5
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x0200bcd8
.L_02000db8_5:
	mov r5, r9
	subs r5, #16
	mov r2, r10
	adds r0, r7, #0
	adds r1, r5, #0
	subs r2, #64
	bl 0x0200bce8
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x0200bcf8
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x0200bcf8
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x0200bcf8
	mov r2, r10
	adds r0, r7, #0
	adds r1, r5, #0
	subs r2, #16
	bl 0x0200bce8
	adds r0, r7, #0
	mov r1, r9
	mov r2, r10
	bl 0x0200bce8
	movs r1, #192
	adds r0, r7, #0
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bd50
	bl 0x0200bca8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0xffd00000
	.4byte 0xffd80000
	.4byte 0xfff00000
	.4byte 0xffe00000
	.4byte 0xffb00000
	.4byte 0x000020cb
	.4byte 0x00000101
	.4byte 0x000020d5
	.4byte 0x000020d4
	.4byte 0x000020e1
	.global Func_02001214
	.thumb_func
Func_02001214:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #448]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r6, r0, #0
	cmp r3, #2
	bne .L_02001214_0
	bl 0x02009b5c
	b .L_02001214_1
.L_02001214_0:
	bl 0x0200bca0
	adds r0, r6, #0
	movs r1, #1
	bl 0x02009d64
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02001214_2
	b .L_02001214_3
.L_02001214_2:
	ldr r0, [pc, #408]
	bl 0x0200bd30
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200bd70
	movs r0, #164
	movs r1, #1
	movs r2, #168
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl 0x0200bd78
	bl 0x0200bd80
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200bd40
	movs r1, #140
	lsls r1, r1, #1
	movs r2, #200
	movs r0, #0
	bl 0x0200a910
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #180
	lsls r1, r1, #1
	movs r2, #200
	movs r0, #0
	bl 0x0200bce8
	movs r0, #30
	bl 0x0200bc98
	movs r1, #129
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200bd60
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200bd40
	movs r1, #156
	lsls r1, r1, #1
	movs r2, #200
	movs r0, #0
	bl 0x0200bce8
	movs r0, #30
	bl 0x0200bc98
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bd50
	movs r1, #131
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bd60
	movs r5, #148
	movs r1, #192
	movs r2, #192
	lsls r5, r5, #1
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	adds r1, r5, #0
	movs r2, #184
	movs r0, #0
	bl 0x0200ae50
	adds r1, r5, #0
	movs r2, #152
	movs r0, #0
	bl 0x0200ae50
	movs r1, #156
	lsls r1, r1, #1
	movs r2, #152
	movs r0, #0
	bl 0x0200ae50
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #15
	movs r0, #0
	bl 0x0200bd50
	bl 0x0200b8f8
	movs r0, #0
	bl 0x0200abac
	bl 0x0200b8f8
	movs r0, #0
	bl 0x0200abac
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #152
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #184
	bl 0x0200bce8
	movs r0, #0
	adds r1, r5, #0
	movs r2, #192
	bl 0x0200bce8
	movs r0, #0
	adds r1, r5, #0
	movs r2, #200
	bl 0x0200bce8
	movs r2, #15
	movs r1, #0
	movs r0, #0
	bl 0x0200bd50
	bl 0x0200b8f8
	movs r0, #0
	bl 0x0200abac
	bl 0x0200b8f8
	movs r0, #0
	bl 0x0200abac
	movs r0, #0
	movs r1, #1
	bl 0x0200bd00
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200bd40
	movs r0, #0
	bl 0x0200aaec
	movs r0, #0
	movs r1, #0
	bl 0x0200bd68
	movs r1, #156
	movs r2, #168
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x0200bcf8
	adds r0, r6, #0
	movs r1, #1
	bl 0x02009e20
	b .L_02001214_4
.L_02001214_3:
	cmp r7, #1
	bne .L_02001214_4
	ldr r0, [pc, #40]
	bl 0x0200bd30
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200bd40
.L_02001214_4:
	adds r1, r6, #0
	movs r2, #1
	adds r0, r7, #0
	bl 0x02009e7c
	bl 0x0200bca8
.L_02001214_1:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000208c
	.4byte 0x0000208b
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #476]
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	adds	r7, r0, #0
	cmp	r3, #2
	bne.n	.L_02001404
	bl 0x02009b5c
	b.n	.L_020015be
.L_02001404:
	bl 0x0200bca0
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x02009d64
	mov	sl, r0
	cmp	r0, #0
	beq.n	.L_02001418
	b.n	.L_0200159c
.L_02001418:
	ldr	r0, [pc, #436]
	bl 0x0200bd30
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200bd70
	movs	r0, #148
	movs	r1, #1
	movs	r2, #240
	lsls	r2, r2, #15
	movs	r3, #1
	negs	r1, r1
	lsls	r0, r0, #18
	bl 0x0200bd78
	bl 0x0200bd80
	movs	r0, #60
	bl 0x0200bc98
	movs	r0, #192
	movs	r1, #192
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200bd70
	movs	r0, #152
	movs	r1, #1
	movs	r2, #216
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r0, r0, #18
	negs	r1, r1
	bl 0x0200bd78
	bl 0x0200bd80
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bd40
	movs	r1, #64
	movs	r2, #0
	movs	r0, #56
	bl 0x0200ad28
	movs	r0, #60
	bl 0x0200bc98
	movs	r2, #10
	movs	r1, #96
	movs	r0, #160
	bl 0x0200ad8c
	movs	r0, #70
	bl 0x0200bc98
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200bd40
	bl 0x0200ade8
	movs	r0, #2
	bl 0x0200bb08
	movs	r0, #13
	bl 0x0200bcb8
	adds	r3, r0, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	ldr	r6, [pc, #288]
	ldr	r3, [pc, #292]
	movs	r2, #128
	mov	r8, r3
	ldr	r1, [r0, #8]
	str	r3, [r0, #52]
	lsls	r2, r2, #12
	ldr	r3, [r0, #16]
	str	r6, [r0, #48]
	bl 0x0200bbd8
	movs	r0, #14
	bl 0x0200bcb8
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	mov	r3, r8
	movs	r2, #128
	ldr	r1, [r5, #8]
	lsls	r2, r2, #14
	str	r3, [r5, #52]
	str	r6, [r5, #48]
	ldr	r3, [r5, #16]
	bl 0x0200bbd8
	adds	r0, r5, #0
	bl 0x0200bbe0
	movs	r0, #45
	bl 0x0200bc98
	movs	r0, #13
	bl 0x0200bcb8
	adds	r3, r0, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	mov	r3, r8
	movs	r2, #192
	ldr	r1, [r0, #8]
	str	r3, [r0, #52]
	lsls	r2, r2, #13
	ldr	r3, [r0, #16]
	str	r6, [r0, #48]
	bl 0x0200bbd8
	movs	r0, #14
	bl 0x0200bcb8
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #85
	mov	r2, sl
	strb	r2, [r3, #0]
	mov	r3, r8
	movs	r2, #0
	ldr	r1, [r5, #8]
	str	r3, [r5, #52]
	str	r6, [r5, #48]
	ldr	r3, [r5, #16]
	bl 0x0200bbd8
	adds	r0, r5, #0
	bl 0x0200bbe0
	movs	r0, #15
	bl 0x0200bc98
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bd40
	movs	r1, #64
	movs	r2, #0
	movs	r0, #56
	bl 0x0200ad28
	movs	r0, #30
	bl 0x0200bc98
	movs	r1, #96
	movs	r2, #10
	movs	r0, #160
	bl 0x0200ad8c
	movs	r0, #40
	bl 0x0200bc98
	movs	r2, #10
	movs	r1, #64
	movs	r0, #56
	bl 0x0200ad8c
	movs	r0, #70
	bl 0x0200bc98
	movs	r1, #0
	adds	r0, r7, #0
	bl 0x0200bd40
	bl 0x0200ade8
	movs	r0, #2
	bl 0x0200bb08
	movs	r0, #0
	movs	r1, #0
	bl 0x0200bd68
	adds	r0, r7, #0
	movs	r1, #2
	bl 0x02009e20
	b.n	.L_020015b0
.L_0200159c:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_020015b0
	ldr	r0, [pc, #56]
	bl 0x0200bd30
	adds	r0, r7, #0
	movs	r1, #0
	bl 0x0200bd40
.L_020015b0:
	adds	r1, r7, #0
	movs	r2, #2
	mov	r0, sl
	bl 0x02009e7c
	bl 0x0200bca8
.L_020015be:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00002090
	.4byte 0x0000cccc
	.4byte 0x00006666
	.2byte 0x208f
	.2byte 0x0000
	.global Func_020015e0
	.thumb_func
Func_020015e0:
	push {r5, r6, lr}
	ldr r3, [pc, #248]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r5, r0, #0
	cmp r3, #2
	bne .L_020015e0_0
	bl 0x02009b5c
	b .L_020015e0_1
.L_020015e0_0:
	bl 0x0200bca0
	adds r0, r5, #0
	movs r1, #3
	bl 0x02009d64
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020015e0_2
	ldr r0, [pc, #208]
	bl 0x0200bd30
	bl 0x02008134
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200bd70
	movs r0, #210
	movs r1, #1
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #18
	negs r1, r1
	bl 0x0200bd78
	bl 0x0200bd80
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200bd40
	bl 0x02008158
	movs r0, #60
	bl 0x0200bc98
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bd40
	movs r1, #184
	lsls r1, r1, #2
	movs r2, #200
	movs r0, #0
	bl 0x0200a910
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl 0x0200bd50
	bl 0x02008178
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #204
	lsls r1, r1, #2
	movs r2, #200
	movs r0, #0
	bl 0x0200bce8
	movs r0, #30
	bl 0x0200bc98
	movs r2, #60
	movs r0, #0
	ldr r1, [pc, #80]
	bl 0x0200bd60
	movs r1, #0
	adds r0, r5, #0
	bl 0x0200bd40
	movs r0, #0
	bl 0x0200aaec
	movs r0, #0
	movs r1, #0
	bl 0x0200bd68
	adds r0, r5, #0
	movs r1, #3
	bl 0x02009e20
	b .L_020015e0_3
.L_020015e0_2:
	cmp r6, #1
	bne .L_020015e0_3
	ldr r0, [pc, #44]
	bl 0x0200bd30
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bd40
.L_020015e0_3:
	adds r1, r5, #0
	movs r2, #3
	adds r0, r6, #0
	bl 0x02009e7c
	bl 0x0200bca8
.L_020015e0_1:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x00002095
	.4byte 0x00000105
	.4byte 0x00002094
	.global Func_020016ec
	.thumb_func
Func_020016ec:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #412]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r6, r0, #0
	cmp r3, #2
	bne .L_020016ec_0
	bl 0x02009b5c
	b 0x02009884
.L_020016ec_0:
	bl 0x0200bca0
	adds r0, r6, #0
	movs r1, #4
	bl 0x02009d64
	adds r7, r0, #0
	cmp r7, #0
	beq .L_020016ec_1
	b 0x02009864
.L_020016ec_1:
	ldr r0, [pc, #372]
	bl 0x0200bd30
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200bd70
	movs r0, #136
	movs r1, #1
	movs r2, #168
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #19
	negs r1, r1
	bl 0x0200bd78
	bl 0x0200bd80
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200bd40
	movs r2, #0
	movs r1, #72
	movs r0, #120
	bl 0x0200ad28
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200bd40
	bl 0x0200ade8
	movs r0, #15
	bl 0x0200bc98
	movs r1, #246
	lsls r1, r1, #2
	movs r2, #200
	movs r0, #0
	bl 0x0200a910
	movs r2, #10
	movs r0, #0
	movs r1, #0
	bl 0x0200bd50
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200bd40
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200bd50
	movs r1, #131
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bd60
	movs r5, #250
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	lsls r5, r5, #2
	bl 0x0200bcc8
	adds r1, r5, #0
	movs r2, #192
	movs r0, #0
	bl 0x0200ae50
	adds r1, r5, #0
	movs r2, #176
	movs r0, #0
	bl 0x0200ae50
	movs r1, #254
	lsls r1, r1, #2
	movs r2, #168
	movs r0, #0
	bl 0x0200ae50
	movs r0, #15
	bl 0x0200bc98
	movs r1, #160
	movs r2, #0
	movs r0, #18
	bl 0x0200833c
	movs r0, #136
	movs r1, #1
.L_020017e2:
	movs r2, #168
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #19
	negs r1, r1
	bl 0x0200bd78
	movs r1, #1
	movs r0, #0
	bl 0x0200bd00
	movs r0, #10
	bl 0x0200bc98
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #149
	lsls r1, r1, #3
	movs r2, #168
	movs r0, #0
	bl 0x0200bce8
	movs r0, #10
	bl 0x0200bc98
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #30
	bl 0x0200bd50
	movs r1, #129
	movs r2, #60
	movs r0, #0
	lsls r1, r1, #1
	bl 0x0200bd60
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200bd40
	movs r0, #0
	bl 0x0200aaec
	movs r0, #0
	movs r1, #0
	bl 0x0200bd68
	movs r1, #254
	movs r2, #168
	movs r0, #18
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl 0x0200bcf8
	adds r0, r6, #0
	movs r1, #4
	bl 0x02009e20
	b .L_020017e2_0
	.2byte 0x2f01
	.2byte 0xd106
	.2byte 0x480a
	.2byte 0xf002
	.2byte 0xfa61
	.2byte 0x1c30
	.2byte 0x2100
	.2byte 0xf002
	.2byte 0xfa65
.L_020017e2_0:
	adds r1, r6, #0
	movs r2, #4
	adds r0, r7, #0
	bl 0x02009e7c
	bl 0x0200bca8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x2099
	.2byte 0x0000
	.2byte 0x2098
	.2byte 0x0000
	.global Func_02001898
	.thumb_func
Func_02001898:
	push {r5, r6, lr}
	movs r0, #224
	lsls r0, r0, #2
	bl 0x0200bc68
	adds r5, r0, #0
	movs r0, #226
	lsls r0, r0, #2
	bl 0x0200bc68
	movs r6, #128
	lsls r6, r6, #12
	adds r2, r0, #0
	lsls r5, r5, #20
	adds r5, r5, r6
	lsls r2, r2, #20
	adds r2, r2, r6
	adds r1, r5, #0
	movs r0, #1
	bl 0x0200bcf8
	movs r0, #228
	lsls r0, r0, #2
	bl 0x0200bc68
	adds r5, r0, #0
	movs r0, #230
	lsls r0, r0, #2
	bl 0x0200bc68
	lsls r5, r5, #20
	adds r2, r0, #0
	adds r5, r5, r6
	lsls r2, r2, #20
	adds r2, r2, r6
	adds r1, r5, #0
	movs r0, #2
	bl 0x0200bcf8
	movs r0, #232
	lsls r0, r0, #2
	bl 0x0200bc68
	adds r5, r0, #0
	movs r0, #234
	lsls r0, r0, #2
	bl 0x0200bc68
	lsls r5, r5, #20
	adds r2, r0, #0
	adds r5, r5, r6
	lsls r2, r2, #20
	adds r2, r2, r6
	movs r0, #3
	adds r1, r5, #0
	bl 0x0200bcf8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_02001910
	.thumb_func
Func_02001910:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200bc88
	movs r0, #1
	bl 0x0200bc88
	movs r0, #2
	bl 0x0200bc88
	movs r0, #3
	bl 0x0200bc88
	movs r0, #5
	bl 0x0200bc88
	adds r0, r5, #0
	bl 0x0200bc80
	ldr r3, [pc, #76]
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r3, r1
	str r5, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bd68
	adds r0, r5, #0
	bl 0x0200bc40
	adds r5, r0, #0
	ldrh r3, [r5, #52]
	ldr r1, [pc, #52]
	strh r3, [r5, #56]
	ldrh r3, [r5, #54]
	ldr r2, [pc, #40]
	strh r3, [r5, #58]
	adds r3, r5, r1
	strb r2, [r3]
	movs r2, #56
	ldrsh r0, [r5, r2]
	movs r3, #52
	ldrsh r1, [r5, r3]
	lsls r0, r0, #14
	bl 0x0200bb00
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_02001910_0
	movs r3, #0
	cmp r0, #0
	blt .L_02001910_0
	adds r3, r0, #0
	b .L_02001910_0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x00000131
.L_02001910_0:
	strh r3, [r5, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02001910_1
	movs r1, #56
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_02001910_1
	movs r3, #1
	strh r3, [r5, #20]
.L_02001910_1:
	movs r2, #58
	ldrsh r0, [r5, r2]
	movs r3, #54
	ldrsh r1, [r5, r3]
	lsls r0, r0, #14
	bl 0x0200bb00
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_02001910_2
	movs r3, #0
	cmp r0, #0
	blt .L_02001910_2
	adds r3, r0, #0
.L_02001910_2:
	strh r3, [r5, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02001910_3
	movs r1, #58
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_02001910_3
	movs r3, #1
	strh r3, [r5, #22]
.L_02001910_3:
	bl 0x0200bde0
	pop {r5}
	pop {r0}
	bx r0
	.global Func_020019e0
	.thumb_func
Func_020019e0:
	push {r5, lr}
	ldr r3, [pc, #56]
	movs r1, #250
	ldr r5, [r3]
	ldr r3, [pc, #52]
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r2, [r3]
	cmp r2, #0
	beq .L_020019e0_0
	subs r1, #118
	adds r3, r5, r1
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r3, r3, #26
	cmp r3, r2
	bne .L_020019e0_0
	ldr r0, [pc, #32]
	bl 0x0200bc58
	cmp r0, #0
	beq .L_020019e0_0
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #99
	strh r3, [r2]
.L_020019e0_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000141
	.global Func_02001a28
	.thumb_func
Func_02001a28:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #280]
	ldr r3, [r3]
	mov r9, r3
	movs r3, #128
	movs r2, #8
	lsls r3, r3, #13
	mov r10, r2
	mov r8, r3
	movs r2, #250
	ldr r3, [pc, #264]
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r7, [r3]
	adds r0, r7, #0
	bl 0x0200bcb8
	adds r6, r0, #0
	bl 0x0200bca0
	movs r5, #8
.L_02001a28_5:
	adds r0, r5, #0
	bl 0x0200bcb8
	cmp r0, #0
	beq .L_02001a28_0
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_02001a28_0
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #165
	bne .L_02001a28_0
	ldr r2, [r6, #8]
	ldr r3, [r0, #8]
	subs r2, r2, r3
	cmp r2, #0
	bge .L_02001a28_1
	ldr r3, [pc, #204]
	adds r2, r2, r3
.L_02001a28_1:
	asrs r1, r2, #16
	ldr r3, [r0, #16]
	ldr r2, [r6, #16]
	subs r0, r2, r3
	cmp r0, #0
	bge .L_02001a28_2
	ldr r2, [pc, #188]
	adds r0, r0, r2
.L_02001a28_2:
	asrs r0, r0, #16
	cmp r0, #0
	bgt .L_02001a28_0
	adds r2, r1, #0
	cmp r2, #0
	bge .L_02001a28_3
	negs r2, r2
.L_02001a28_3:
	cmp r0, #0
	bge .L_02001a28_4
	negs r0, r0
.L_02001a28_4:
	adds r0, r2, r0
	cmp r0, r8
	bge .L_02001a28_0
	mov r10, r5
	mov r8, r0
.L_02001a28_0:
	adds r5, #1
	cmp r5, #66
	ble .L_02001a28_5
	ldr r0, [pc, #152]
	bl 0x0200bd30
	movs r1, #0
	mov r0, r10
	bl 0x0200bd40
	movs r3, #224
	lsls r3, r3, #1
	add r3, r9
	mov r8, r3
	movs r3, #128
	lsls r3, r3, #2
	mov r2, r8
	str r3, [r2]
	movs r2, #228
	lsls r2, r2, #1
	add r2, r9
	movs r3, #15
	str r3, [r2]
	movs r0, #20
	bl 0x0200bc98
	bl 0x0200bdb8
	bl 0x0200bdc0
	ldr r1, [r6, #8]
	movs r3, #220
	lsls r5, r7, #4
	lsls r3, r3, #2
	adds r0, r5, r3
	asrs r1, r1, #20
	bl 0x0200bc70
	ldr r1, [r6, #16]
	movs r2, #222
	lsls r2, r2, #2
	asrs r1, r1, #20
	adds r0, r5, r2
	adds r7, #1
	bl 0x0200bc70
	cmp r7, #3
	ble 0x02009b26
	movs r0, #10
.L_02001b18:
	bl 0x0200bd88
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200bc60
	b .L_02001b18_0
	.2byte 0x1c38
	.2byte 0xf7ff
	.2byte 0xfef2
	.2byte 0xf002
	.2byte 0xf940
	.2byte 0xf002
	.2byte 0xf946
	.2byte 0x2300
	.2byte 0x4642
	.2byte 0x6013
.L_02001b18_0:
	bl 0x0200bca8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x1ebc
	.2byte 0x0300
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0xffff
	.2byte 0x0000
	.2byte 0x2085
	.2byte 0x0000
	.global Func_02001b5c
	.thumb_func
Func_02001b5c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, [pc, #248]
	adds r7, r0, #0
	ldr r0, [r5]
	mov r9, r0
	adds r0, r7, #0
	bl 0x0200bcb8
	adds r0, r7, #0
	bl 0x0200bcb8
	ldr r3, [pc, #232]
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r6, [r3]
	adds r0, r6, #0
	bl 0x0200bcb8
	mov r11, r0
	bl 0x0200bca0
	ldr r3, [pc, #212]
	mov r8, r3
	mov r0, r8
	bl 0x0200bd30
	movs r1, #0
	adds r0, r7, #0
	bl 0x0200bd38
	ldr r2, [r5]
	ldr r0, [pc, #196]
	ldr r1, [pc, #200]
	adds r3, r2, r0
	strh r1, [r3]
	ldr r3, [pc, #196]
	adds r2, r2, r3
	movs r3, #4
	strh r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200bcb0
	mov r10, r0
	cmp r0, #0
	bne .L_02001b5c_0
	mov r0, r8
	adds r0, #1
	bl 0x0200bd30
	adds r0, r7, #0
	movs r1, #0
	movs r7, #224
	bl 0x0200bd40
	lsls r7, r7, #1
	movs r3, #128
	movs r2, #228
	lsls r3, r3, #2
	add r7, r9
	lsls r2, r2, #1
	add r2, r9
	str r3, [r7]
	movs r3, #15
	str r3, [r2]
	bl 0x0200bdb8
	bl 0x0200bdc0
	mov r0, r11
	ldr r1, [r0, #8]
	movs r2, #220
	lsls r5, r6, #4
	lsls r2, r2, #2
	adds r0, r5, r2
	asrs r1, r1, #20
	bl 0x0200bc70
	mov r3, r11
	ldr r1, [r3, #16]
	movs r2, #222
	lsls r2, r2, #2
	asrs r1, r1, #20
	adds r0, r5, r2
	adds r6, #1
	bl 0x0200bc70
	cmp r6, #3
	ble .L_02001b5c_1
	movs r0, #10
	bl 0x0200bd88
	movs r0, #141
	lsls r0, r0, #1
	bl 0x0200bc60
	b .L_02001b5c_2
.L_02001b5c_1:
	adds r0, r6, #0
	bl 0x02009910
	bl 0x0200bdb0
	bl 0x0200bdc0
	mov r3, r10
	str r3, [r7]
	b .L_02001b5c_2
.L_02001b5c_0:
	mov r0, r8
	adds r0, #2
	bl 0x0200bd30
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bd40
.L_02001b5c_2:
	bl 0x0200bca8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00002086
	.4byte 0x00000cc2
	.4byte 0x00002089
	.4byte 0x00000cc4
	.global Func_02001c7c
	.thumb_func
Func_02001c7c:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	bl 0x0200bcb8
	movs r3, #10
	ldrsh r7, [r0, r3]
	movs r3, #18
	ldrsh r6, [r0, r3]
	bl 0x0200bca0
	bl 0x0200bc78
	cmp r0, #1
	bgt .L_02001c7c_0
	ldr r0, [pc, #124]
	bl 0x0200bd30
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bd48
	cmp r0, #0
	bne .L_02001c7c_1
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #128
	movs r2, #128
	adds r0, r5, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	adds r2, r6, #0
	adds r2, #64
	adds r1, r7, #0
	adds r0, r5, #0
	bl 0x0200bce0
	movs r0, #15
	bl 0x0200bc98
	movs r0, #0
	adds r1, r7, #0
	adds r2, r6, #0
	bl 0x0200bce8
	adds r2, r6, #0
	movs r0, #0
	adds r2, #32
	adds r1, r7, #0
	bl 0x0200bce8
	bl 0x0200bdb8
	bl 0x0200bdc0
	movs r0, #11
	bl 0x0200bd88
	b .L_02001c7c_1
.L_02001c7c_0:
	ldr r0, [pc, #28]
	bl 0x0200bd30
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bd40
.L_02001c7c_1:
	bl 0x0200bca8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000020e5
	.4byte 0x000020e8
	.global Func_02001d20
	.thumb_func
Func_02001d20:
	push {lr}
	movs r0, #224
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200bc70
	movs r0, #226
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200bc70
	movs r0, #228
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200bc70
	movs r0, #230
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200bc70
	movs r0, #232
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200bc70
	movs r0, #234
	lsls r0, r0, #2
	movs r1, #0
	bl 0x0200bc70
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001d64
	.thumb_func
Func_02001d64:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r6, r0, #0
	bl 0x0200bdd0
	movs r1, #5
	adds r0, r5, #0
	bl 0x0200bc20
	ldr r3, [pc, #140]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #132]
	cmp r2, r3
	bne .L_02001d64_0
	ldr r0, [pc, #128]
	b .L_02001d64_1
.L_02001d64_0:
	ldr r3, [pc, #128]
	cmp r2, r3
	bne .L_02001d64_2
	ldr r0, [pc, #128]
	b .L_02001d64_1
.L_02001d64_2:
	ldr r0, [pc, #128]
.L_02001d64_1:
	bl 0x0200bd30
	adds r0, r6, #0
	movs r1, #0
	bl 0x0200bd40
	movs r2, #128
	lsls r2, r2, #2
	adds r0, r5, r2
	bl 0x0200bc58
	cmp r0, #0
	bne .L_02001d64_3
	movs r3, #130
	lsls r3, r3, #2
	adds r5, r5, r3
	adds r0, r5, #0
	bl 0x0200bc58
	cmp r0, #0
	beq .L_02001d64_4
	movs r0, #0
	bl 0x0200bc30
	cmp r0, #1
	bne .L_02001d64_5
.L_02001d64_3:
	movs r0, #2
	b .L_02001d64_6
.L_02001d64_5:
	cmp r0, #2
	beq .L_02001d64_7
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_02001d64_6
.L_02001d64_7:
	movs r0, #3
	b .L_02001d64_6
.L_02001d64_4:
	adds r0, r5, #0
	bl 0x0200bc60
	ldr r0, [pc, #52]
	bl 0x0200bd30
	movs r1, #0
	adds r0, r6, #0
	bl 0x0200bd38
	movs r0, #0
	movs r1, #0
	bl 0x0200bcb0
.L_02001d64_6:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008f
	.4byte 0x00002076
	.4byte 0x00000090
	.4byte 0x00002078
	.4byte 0x0000207a
	.4byte 0x0000207c
	.global Func_02001e20
	.thumb_func
Func_02001e20:
	push {r5, lr}
	adds r5, r0, #0
	adds r0, r1, #0
	movs r1, #5
	bl 0x0200bc20
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02001e20_0
	ldr r0, [pc, #44]
	b .L_02001e20_1
.L_02001e20_0:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_02001e20_2
	ldr r0, [pc, #40]
	b .L_02001e20_1
.L_02001e20_2:
	ldr r0, [pc, #40]
.L_02001e20_1:
	adds r0, #1
	bl 0x0200bd30
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bd40
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000008f
	.4byte 0x00002076
	.4byte 0x00000090
	.4byte 0x00002078
	.4byte 0x0000207a
	.global Func_02001e7c
	.thumb_func
Func_02001e7c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r1
	sub sp, #12
	adds r6, r0, #0
	mov r0, r10
	str r2, [sp, #0]
	bl 0x0200bcb8
	adds r5, r0, #0
	movs r2, #10
	ldrsh r0, [r5, r2]
	mov r9, r0
	movs r0, #18
	ldrsh r3, [r5, r0]
	mov r11, r3
	cmp r6, #3
	beq .L_02001e7c_0
	bl 0x0200bc78
	adds r7, r0, #0
	cmp r7, #0
	ble .L_02001e7c_1
	ldr r3, [pc, #536]
	movs r0, #252
	lsls r0, r0, #1
	add r2, sp, #4
	adds r1, r3, r0
	adds r5, r7, #0
.L_02001e7c_2:
	ldrb r3, [r1]
	subs r5, #1
	strb r3, [r2]
	adds r1, #1
	adds r2, #1
	cmp r5, #0
	bne .L_02001e7c_2
.L_02001e7c_1:
	cmp r7, #1
	bgt .L_02001e7c_3
	ldr r0, [pc, #512]
	b .L_02001e7c_4
.L_02001e7c_3:
	ldr r2, [sp, #0]
	movs r3, #128
	lsls r3, r3, #2
	adds r0, r2, r3
	bl 0x0200bc58
	cmp r0, #0
	beq .L_02001e7c_5
	ldr r0, [pc, #496]
	b .L_02001e7c_4
.L_02001e7c_5:
	cmp r6, #2
	bne .L_02001e7c_6
	movs r0, #6
	movs r5, #0
	bl 0x0200bb08
	b .L_02001e7c_7
.L_02001e7c_6:
	ldr r0, [pc, #480]
	bl 0x0200bd30
	movs r1, #0
	mov r0, r10
	bl 0x0200bd38
	movs r0, #0
	movs r1, #0
	bl 0x0200bcb0
	adds r5, r0, #0
.L_02001e7c_7:
	cmp r5, #0
	bne .L_02001e7c_0
	cmp r5, r7
	bge .L_02001e7c_8
	add r6, sp, #4
	adds r5, r7, #0
.L_02001e7c_9:
	ldrb r0, [r6]
	lsls r0, r0, #24
	asrs r0, r0, #24
	subs r5, #1
	adds r6, #1
	bl 0x0200bc88
	cmp r5, #0
	bne .L_02001e7c_9
.L_02001e7c_8:
	cmp r7, #0
	ble .L_02001e7c_10
	add r6, sp, #4
	adds r5, r7, #0
.L_02001e7c_12:
	ldrb r3, [r6]
	lsls r3, r3, #24
	asrs r0, r3, #24
	adds r6, #1
	cmp r0, #0
	beq .L_02001e7c_11
	bl 0x0200bc80
.L_02001e7c_11:
	subs r5, #1
	cmp r5, #0
	bne .L_02001e7c_12
.L_02001e7c_10:
	bl 0x0200bdf0
	mov r8, r0
	cmp r7, #0
	ble .L_02001e7c_13
	add r6, sp, #4
	adds r5, r7, #0
.L_02001e7c_14:
	ldrb r0, [r6]
	lsls r0, r0, #24
	asrs r0, r0, #24
	subs r5, #1
	adds r6, #1
	bl 0x0200bc88
	cmp r5, #0
	bne .L_02001e7c_14
.L_02001e7c_13:
	cmp r7, #0
	ble .L_02001e7c_15
	add r6, sp, #4
	adds r5, r7, #0
.L_02001e7c_16:
	ldrb r0, [r6]
	lsls r0, r0, #24
	asrs r0, r0, #24
	subs r5, #1
	adds r6, #1
	bl 0x0200bc80
	cmp r5, #0
	bne .L_02001e7c_16
.L_02001e7c_15:
	movs r0, #1
	negs r0, r0
	cmp r8, r0
	bne .L_02001e7c_17
.L_02001e7c_0:
	ldr r0, [pc, #336]
.L_02001e7c_4:
	bl 0x0200bd30
	mov r0, r10
	movs r1, #0
	bl 0x0200bd40
	b .L_02001e7c_18
.L_02001e7c_17:
	movs r1, #1
	mov r0, r8
	bl 0x0200bc20
	ldr r0, [pc, #316]
	bl 0x0200bd30
	mov r0, r10
	movs r1, #0
	bl 0x0200bd40
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #128
	movs r2, #128
	mov r0, r8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r1, #128
	movs r2, #128
	mov r0, r10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bcc8
	movs r0, #0
	bl 0x0200bcb8
	cmp r0, #0
	beq .L_02001e7c_19
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	mov r0, r8
	bl 0x0200bcf8
.L_02001e7c_19:
	mov r5, r11
	adds r5, #16
	mov r6, r9
	adds r2, r5, #0
	adds r6, #16
	mov r0, r8
	mov r1, r9
	bl 0x0200bce8
	adds r1, r6, #0
	adds r2, r5, #0
	movs r0, #0
	bl 0x0200bce8
	movs r2, #30
	mov r0, r8
	movs r1, #0
	bl 0x0200bd28
	mov r0, r8
	movs r1, #3
	bl 0x0200bd00
	subs r5, #32
	movs r0, #0
	movs r1, #3
	bl 0x0200bd08
	adds r2, r5, #0
	mov r0, r10
	mov r1, r9
	bl 0x0200bce8
	adds r2, r5, #0
	adds r1, r6, #0
	mov r0, r10
	bl 0x0200bce0
	movs r0, #0
	mov r1, r8
	bl 0x0200bde8
	adds r2, r5, #0
	mov r0, r8
	mov r1, r9
	bl 0x0200bce8
	mov r0, r10
	movs r1, #1
	bl 0x0200bd00
	movs r1, #128
	mov r0, r10
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bd50
	mov r2, r11
	subs r2, #48
	mov r0, r8
	mov r1, r9
	bl 0x0200bce8
	adds r2, r5, #0
	mov r0, r10
	mov r1, r9
	bl 0x0200bce8
	mov r1, r9
	mov r2, r11
	mov r0, r10
	bl 0x0200bce8
	mov r0, r8
	bl 0x0200bc88
	movs r3, #128
	ldr r2, [sp, #0]
	lsls r3, r3, #2
	adds r0, r2, r3
	bl 0x0200bc60
	mov r0, r8
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r1, [r5, #8]
	mov r0, r8
	movs r2, #220
	lsls r6, r0, #4
	lsls r2, r2, #2
	asrs r1, r1, #20
	adds r0, r6, r2
	bl 0x0200bc70
	ldr r1, [r5, #16]
	movs r3, #222
	lsls r3, r3, #2
	asrs r1, r1, #20
	adds r0, r6, r3
	bl 0x0200bc70
.L_02001e7c_18:
	sub sp, #-12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x00002083
	.4byte 0x00002084
	.4byte 0x0000207d
	.4byte 0x0000207e
	.4byte 0x0000207f
	.global Func_020020e8
	.thumb_func
Func_020020e8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	mov r8, r1
	bl 0x0200bc40
	mov r1, r8
	adds r5, r0, #0
	adds r0, r7, #0
	bl 0x0200bc48
	movs r6, #0
	adds r5, #216
.L_020020e8_1:
	ldrh r3, [r5]
	adds r5, #2
	cmp r3, r8
	bne .L_020020e8_0
	adds r0, r7, #0
	adds r1, r6, #0
	bl 0x0200bc50
.L_020020e8_0:
	adds r6, #1
	cmp r6, #14
	ble .L_020020e8_1
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02002124
	.thumb_func
Func_02002124:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #229
	lsls r0, r0, #5
	bl 0x0200bb50
	ldr r7, [pc, #104]
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #1
	negs r2, r2
	adds r6, r0, #0
	cmp r3, r2
	bne .L_02002124_0
	bl 0x0200bb80
	strh r0, [r7]
.L_02002124_0:
	ldr r3, [pc, #88]
	ldrb r3, [r3, r5]
	mov r8, r3
	cmp r5, #8
	bne .L_02002124_1
	movs r5, #4
.L_02002124_1:
	ldr r0, [pc, #80]
	bl 0x0200bb98
	adds r1, r6, #0
	bl 0x0200bb60
	mov r2, r8
	adds r0, r6, r2
	ldr r3, [pc, #68]
	ldr r1, [pc, #68]
	ldr r2, [pc, #72]
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	lsls r2, r5, #10
	adds r2, r2, r6
	movs r1, #128
	adds r2, #160
	lsls r1, r1, #3
	movs r3, #0
	ldrsh r0, [r7, r3]
	bl 0x0200bb78
	movs r2, #128
	ldr r1, [pc, #36]
	lsls r2, r2, #24
.L_02002124_2:
	ldr r3, [r1, #8]
	ands r3, r2
	cmp r3, #0
	bne .L_02002124_2
	adds r0, r6, #0
	bl 0x0200bb58
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200c57c
	.4byte 0x0200be44
	.4byte 0x000000e7
	.4byte 0x040000d4
	.4byte 0x050003e0
	.4byte 0x84000008
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r0, [pc, #340]
	sub	sp, #20
	str	r0, [sp, #8]
	ldr	r3, [pc, #336]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #336]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	mov	sl, r0
	mov	r8, r3
.L_020021e0:
	ldr	r1, [pc, #324]
	movs	r2, #0
	ldrsh	r4, [r1, r2]
	ldrh	r3, [r1, #0]
	cmp	r4, #0
	bne.n	.L_020022b6
	ldr	r5, [pc, #316]
	ldr	r0, [r5, #0]
	ldrh	r3, [r0, #0]
	movs	r2, #128
	lsls	r3, r3, #16
	adds	r0, #2
	asrs	r3, r3, #16
	lsls	r2, r2, #6
	str	r0, [r5, #0]
	cmp	r3, r2
	beq.n	.L_0200227c
	cmp	r3, r2
	bgt.n	.L_02002218
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_020022a4
	movs	r2, #128
	lsls	r2, r2, #5
	cmp	r3, r2
	beq.n	.L_02002264
	b.n	.L_020021e0
.L_02002218:
	movs	r2, #128
	lsls	r2, r2, #7
	cmp	r3, r2
	beq.n	.L_02002236
	cmp	r3, r2
	bgt.n	.L_0200222e
	movs	r1, #192
	lsls	r1, r1, #6
	cmp	r3, r1
	beq.n	.L_0200224c
	b.n	.L_020021e0
.L_0200222e:
	ldr	r2, [pc, #256]
	cmp	r3, r2
	beq.n	.L_0200229a
	b.n	.L_020021e0
.L_02002236:
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	ldr	r2, [pc, #248]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #240]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #240]
	b.n	.L_02002292
.L_0200224c:
	ldr	r2, [pc, #232]
	ldr	r1, [pc, #240]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #220]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #224]
	b.n	.L_02002292
.L_02002264:
	ldr	r2, [pc, #224]
	ldr	r1, [pc, #228]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #216]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #216]
	b.n	.L_02002292
.L_0200227c:
	ldr	r2, [pc, #216]
	ldr	r1, [pc, #220]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	strh	r3, [r2, #0]
	adds	r2, r0, #2
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #208]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #208]
.L_02002292:
	adds	r2, #2
	str	r2, [r5, #0]
	strh	r4, [r3, #0]
	b.n	.L_020021e0
.L_0200229a:
	ldrh	r3, [r0, #0]
	strh	r3, [r1, #0]
	adds	r3, r0, #2
	str	r3, [r5, #0]
	b.n	.L_020021e0
.L_020022a4:
	ldr	r0, [pc, #192]
	bl 0x0200bb18
	ldr	r3, [pc, #116]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200bb68
	b.n	.L_0200266c
.L_020022b6:
	subs	r3, #1
	strh	r3, [r1, #0]
	ldr	r3, [pc, #148]
	movs	r5, #0
	ldrsh	r7, [r3, r5]
	mov	r9, r3
	cmp	r7, #0
	bne.n	.L_020022d0
	ldr	r3, [pc, #128]
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	mov	fp, r0
	b.n	.L_02002302
.L_020022d0:
	ldr	r3, [pc, #120]
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	ldr	r2, [pc, #124]
	ldr	r3, [pc, #108]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	ldrh	r5, [r2, #0]
	adds	r5, #1
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	subs	r3, r3, r6
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200bb00
	adds	r6, r6, r0
	mov	fp, r6
	cmp	r5, r7
	blt.n	.L_02002302
	ldr	r3, [pc, #24]
	mov	r0, r9
	strh	r3, [r0, #0]
.L_02002302:
	ldr	r1, [pc, #92]
	movs	r2, #0
	ldrsh	r7, [r1, r2]
	mov	r9, r1
	cmp	r7, #0
	bne.n	.L_0200236c
	ldr	r3, [pc, #72]
	movs	r0, #0
	ldrsh	r5, [r3, r0]
	str	r5, [sp, #4]
	b.n	.L_0200239e
	.4byte 0x00000000
	.4byte 0x0200c7c0
	.4byte 0x0200c57c
	.4byte 0x03001b10
	.4byte 0x0200c79c
	.4byte 0x0200c7a0
	.4byte 0x00007fff
	.4byte 0x0200c770
	.4byte 0x0200c7f8
	.4byte 0x0200c76c
	.4byte 0x0200c7f0
	.4byte 0x0200c77c
	.4byte 0x0200c778
	.4byte 0x0200c768
	.4byte 0x0200c754
	.4byte 0x0200c7fc
	.4byte 0x0200c794
	.4byte 0x0200c798
	.4byte 0x0200c7a8
	.4byte 0x0200c784
	.2byte 0xa1b9
	.2byte 0x0200
.L_0200236c:
	ldr	r3, [pc, #72]
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	ldr	r3, [pc, #72]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	ldr	r2, [pc, #68]
	ldrh	r5, [r2, #0]
	adds	r5, #1
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	subs	r3, r3, r6
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200bb00
	adds	r6, r6, r0
	str	r6, [sp, #4]
	cmp	r5, r7
	blt.n	.L_0200239e
	ldr	r3, [pc, #24]
	mov	r5, r9
	strh	r3, [r5, #0]
.L_0200239e:
	ldr	r0, [pc, #36]
	movs	r1, #0
	ldrsh	r7, [r0, r1]
	mov	r9, r0
	cmp	r7, #0
	bne.n	.L_020023cc
	ldr	r3, [pc, #28]
	movs	r2, #0
	ldrsh	r6, [r3, r2]
	b.n	.L_020023fc
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200c798
	.4byte 0x0200c794
	.4byte 0x0200c784
	.4byte 0x0200c76c
	.2byte 0xc7f8
	.2byte 0x0200
.L_020023cc:
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #104]
	movs	r5, #0
	ldrsh	r6, [r3, r5]
	ldrh	r5, [r2, #0]
	ldr	r3, [pc, #100]
	adds	r5, #1
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	strh	r5, [r2, #0]
	lsls	r5, r5, #16
	subs	r3, r3, r6
	asrs	r5, r5, #16
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200bb00
	adds	r6, r6, r0
	cmp	r5, r7
	blt.n	.L_020023fc
	ldr	r3, [pc, #56]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_020023fc:
	add	r0, sp, #12
	ldr	r3, [r0, #4]
	ldr	r2, [pc, #60]
	ands	r3, r2
	str	r3, [r0, #4]
	mov	r3, fp
	lsls	r1, r3, #16
	ldr	r3, [sp, #12]
	lsrs	r1, r1, #16
	ands	r3, r2
	ldr	r2, [pc, #48]
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #12]
	bl 0x0200bb88
	ldr	r2, [pc, #36]
	ldr	r3, [r2, #0]
	lsls	r0, r0, #16
	adds	r3, r3, r6
	asrs	r0, r0, #16
	str	r3, [r2, #0]
	b.n	.L_0200244c
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200c77c
	.4byte 0x0200c7f0
	.4byte 0x0200c7f8
	.4byte 0xffff0000
	.4byte 0x0000ffff
	.2byte 0xc770
	.2byte 0x0200
.L_0200244c:
	cmp	r3, #0
	bge.n	.L_02002452
	adds	r3, #255
.L_02002452:
	asrs	r6, r3, #8
	ldr	r3, [pc, #552]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	cmp	r3, #2
	bne.n	.L_02002460
	b.n	.L_020025b8
.L_02002460:
	cmp	r3, #2
	bgt.n	.L_0200246a
	cmp	r3, #1
	beq.n	.L_02002474
	b.n	.L_0200260a
.L_0200246a:
	cmp	r3, #3
	beq.n	.L_020024ea
	cmp	r3, #4
	beq.n	.L_02002566
	b.n	.L_0200260a
.L_02002474:
	lsls	r0, r0, #25
	ldr	r4, [pc, #524]
	movs	r5, #0
	movs	r7, #56
	mov	r9, r0
.L_0200247e:
	lsls	r3, r5, #5
	subs	r3, #48
	mov	r0, fp
	muls	r0, r3
	adds	r3, r0, #0
	cmp	r3, #0
	bge.n	.L_0200248e
	adds	r3, #255
.L_0200248e:
	asrs	r3, r3, #8
	adds	r3, r6, r3
	ldr	r1, [pc, #500]
	adds	r2, r3, #0
	adds	r3, #152
	adds	r2, #88
	cmp	r3, r1
	bhi.n	.L_020024de
	ldr	r3, [pc, #492]
	ldr	r1, [sp, #8]
	ands	r2, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	orrs	r3, r4
	mov	r2, r9
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r0, r1, #0
	orrs	r3, r2
	str	r0, [sp, #8]
	stmia	r1!, {r3}
	movs	r3, #244
	adds	r0, r1, #0
	lsls	r3, r3, #8
	mov	r1, r8
	orrs	r3, r1
	stmia	r0!, {r3}
	movs	r1, #12
	adds	r2, r0, #0
	mov	r0, sl
	add	sl, r1
	movs	r1, #236
	str	r4, [sp, #0]
	str	r2, [sp, #8]
	bl 0x0200bb90
	ldr	r4, [sp, #0]
.L_020024de:
	movs	r2, #8
	adds	r5, #1
	add	r8, r2
	cmp	r5, #3
	ble.n	.L_0200247e
	b.n	.L_0200260a
.L_020024ea:
	lsls	r0, r0, #25
	ldr	r4, [pc, #404]
	movs	r5, #0
	movs	r7, #48
	mov	r9, r0
.L_020024f4:
	lsls	r3, r5, #5
	subs	r3, #16
	mov	r0, fp
	muls	r0, r3
	adds	r3, r0, #0
	cmp	r3, #0
	bge.n	.L_02002504
	adds	r3, #255
.L_02002504:
	asrs	r3, r3, #8
	adds	r3, r6, r3
	ldr	r1, [pc, #380]
	adds	r2, r3, #0
	adds	r3, #152
	adds	r2, #88
	cmp	r3, r1
	bhi.n	.L_0200255a
	ldr	r3, [pc, #372]
	ldr	r1, [sp, #8]
	ands	r2, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	orrs	r3, r4
	mov	r2, r9
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r0, r1, #0
	orrs	r3, r2
	str	r0, [sp, #8]
	stmia	r1!, {r3}
	ldr	r3, [pc, #344]
	adds	r0, r1, #0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #244
	lsls	r2, r2, #8
	add	r3, r8
	orrs	r3, r2
	stmia	r0!, {r3}
	movs	r1, #12
	adds	r2, r0, #0
	mov	r0, sl
	add	sl, r1
	movs	r1, #236
	str	r4, [sp, #0]
	str	r2, [sp, #8]
	bl 0x0200bb90
	ldr	r4, [sp, #0]
.L_0200255a:
	movs	r2, #8
	adds	r5, #1
	add	r8, r2
	cmp	r5, #1
	ble.n	.L_020024f4
	b.n	.L_0200260a
.L_02002566:
	adds	r3, r6, #0
	movs	r5, #152
	adds	r2, r6, #0
	adds	r3, #120
	lsls	r5, r5, #1
	movs	r7, #48
	ldr	r4, [pc, #288]
	adds	r2, #56
	cmp	r3, r5
	bcs.n	.L_0200260a
	ldr	r3, [pc, #272]
	mov	r1, sl
	ands	r2, r3
	movs	r3, #0
	stmia	r1!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	lsls	r2, r0, #25
	orrs	r3, r4
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r5, r1, #0
	orrs	r3, r2
	str	r5, [sp, #8]
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #8]
	ldr	r3, [pc, #240]
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	movs	r2, #244
	add	r3, r8
	lsls	r2, r2, #8
	orrs	r3, r2
	str	r3, [r1, #0]
	mov	r0, sl
	movs	r1, #236
	bl 0x0200bb90
	b.n	.L_0200260a
.L_020025b8:
	adds	r3, r6, #0
	movs	r1, #152
	movs	r4, #128
	adds	r2, r6, #0
	adds	r3, #152
	lsls	r1, r1, #1
	movs	r7, #48
	lsls	r4, r4, #24
	adds	r2, #88
	cmp	r3, r1
	bcs.n	.L_0200260a
	ldr	r3, [pc, #188]
	mov	r5, sl
	ands	r2, r3
	movs	r3, #0
	stmia	r5!, {r3}
	lsls	r3, r2, #16
	orrs	r3, r7
	lsls	r2, r0, #25
	orrs	r3, r4
	orrs	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r5, #0
	orrs	r3, r2
	str	r1, [sp, #8]
	stmia	r5!, {r3}
	adds	r0, r5, #0
	str	r0, [sp, #8]
	ldr	r3, [pc, #156]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r2, #244
	add	r3, r8
	lsls	r2, r2, #8
	orrs	r3, r2
	str	r3, [r5, #0]
	mov	r0, sl
	movs	r1, #236
	bl 0x0200bb90
.L_0200260a:
	ldr	r0, [pc, #140]
	ldr	r1, [pc, #140]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02002638
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	strh	r2, [r0, #0]
	movs	r2, #252
	adds	r3, #4
	lsls	r2, r2, #6
	stmia	r3!, {r2}
	ldr	r2, [pc, #112]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02002638:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r3, [r0, #0]
	cmp	r3, #31
	bgt.n	.L_0200266a
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r0, #0]
	ldr	r5, [sp, #4]
	movs	r3, #16
	lsls	r2, r2, #2
	subs	r3, r3, r5
	adds	r2, r2, r0
	lsls	r3, r3, #8
	adds	r2, #4
	orrs	r3, r5
	stmia	r2!, {r3}
	ldr	r3, [pc, #64]
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_0200266a:
	strh	r4, [r1, #0]
.L_0200266c:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0200c790
	.4byte 0x80004000
	.4byte 0x0000012f
	.4byte 0x000001ff
	.4byte 0x0200c764
	.4byte 0xc0004000
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x04000050
	.2byte 0x0052
	.2byte 0x0400
	.global Func_020026a8
	.thumb_func
Func_020026a8:
	push {r5, r6, lr}
	ldr r3, [pc, #48]
	adds r5, r0, #0
	adds r6, r1, #0
	ldr r2, [pc, #44]
	strh r5, [r3]
	movs r1, #200
	lsls r3, r6, #4
	lsls r1, r1, #4
	strh r3, [r2]
	ldr r0, [pc, #36]
	bl 0x0200bb10
	ldr r1, [pc, #36]
	cmp r5, #2
	bne .L_020026a8_0
	ldr r1, [pc, #32]
.L_020026a8_0:
	cmp r5, #4
	bne .L_020026a8_1
	ldr r1, [pc, #32]
.L_020026a8_1:
	cmp r5, #3
	bne .L_020026a8_2
	cmp r6, #0
	beq .L_020026a8_3
	ldr r1, [pc, #24]
	b .L_020026a8_2
	.4byte 0x0200c790
	.4byte 0x0200c764
	.4byte 0x0200a1b9
	.4byte 0x0200c57e
	.4byte 0x0200be4e
	.4byte 0x0200c5aa
	.4byte 0x0200be76
.L_020026a8_3:
	ldr r1, [pc, #28]
.L_020026a8_2:
	ldr r2, [pc, #24]
	ldr r3, [pc, #28]
	strh r2, [r3]
	ldr r3, [pc, #28]
	str r1, [r3]
	ldr r3, [pc, #28]
	strh r2, [r3]
	ldr r3, [pc, #28]
	strh r2, [r3]
	ldr r2, [pc, #28]
	movs r3, #0
	str r3, [r2]
	b .L_020026a8_4
	.4byte 0x00000000
	.4byte 0x0200c628
	.4byte 0x0200c79c
	.4byte 0x0200c7a0
	.4byte 0x0200c7f8
	.4byte 0x0200c76c
	.4byte 0x0200c770
.L_020026a8_4:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002738
	.thumb_func
Func_02002738:
	push {r5, lr}
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02002738_0
	bl 0x0200bca0
	bl 0x0200bdb0
	bl 0x0200bdc0
	movs r0, #30
	bl 0x0200bc98
	movs r0, #89
	bl 0x0200bdf8
	movs r0, #0
	bl 0x0200a124
	movs r0, #1
	movs r1, #0
	bl 0x0200a6a8
	movs r0, #120
	bl 0x0200bc98
	bl 0x0200bca8
	b .L_02002738_1
.L_02002738_0:
	movs r0, #247
	bl 0x0200bdf8
	bl 0x0200bca0
	bl 0x0200bdb0
	bl 0x0200bdc0
	lsls r2, r5, #4
	ldr r3, [pc, #176]
	subs r2, r2, r5
	lsls r2, r2, #2
	strh r2, [r3, #30]
	movs r0, #30
	bl 0x0200bc98
	adds r0, r5, #0
	adds r0, #90
	bl 0x0200bdf8
	adds r0, r5, #0
	bl 0x0200a124
	movs r0, #1
	movs r1, #0
	bl 0x0200a6a8
	movs r0, #120
	bl 0x0200bc98
	b .L_02002738_2
.L_02002738_3:
	movs r0, #1
	bl 0x0200bb08
.L_02002738_2:
	bl 0x0200be00
	cmp r0, #0
	bne .L_02002738_3
	ldr r0, [pc, #120]
	bl 0x0200bdf8
	movs r0, #5
	bl 0x0200a124
	movs r1, #0
	movs r0, #2
	bl 0x0200a6a8
	movs r0, #236
	bl 0x0200bdf8
	movs r0, #60
	bl 0x0200bc98
	movs r1, #1
	movs r0, #2
	bl 0x0200a6a8
	movs r0, #236
	bl 0x0200bdf8
	movs r0, #60
	bl 0x0200bc98
	movs r0, #6
	bl 0x0200a124
	movs r1, #0
	movs r0, #2
	bl 0x0200a6a8
	movs r0, #236
	bl 0x0200bdf8
	movs r0, #60
	bl 0x0200bc98
	movs r0, #7
	bl 0x0200a124
	movs r1, #0
	movs r0, #4
	bl 0x0200a6a8
	movs r0, #237
	bl 0x0200bdf8
	bl 0x0200bdd8
	bl 0x0200bca8
	ldr r0, [pc, #20]
	bl 0x0200bc60
.L_02002738_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200c57e
	.4byte 0x00000121
	.4byte 0x00000123
	.global Func_02002844
	.thumb_func
Func_02002844:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r0, #247
	bl 0x0200bdf8
	bl 0x0200bdb0
	bl 0x0200bdc0
	lsls r3, r5, #4
	ldr r2, [pc, #164]
	subs r3, r3, r5
	lsls r6, r3, #2
	strh r6, [r2, #26]
	ldr r1, [pc, #160]
	adds r2, r5, #0
	cmp r5, #0
	bge .L_02002844_0
	negs r2, r5
.L_02002844_0:
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	strh r3, [r1, #26]
	cmp r5, #0
	bge .L_02002844_1
	movs r0, #30
	bl 0x0200bc98
	movs r0, #86
	bl 0x0200bdf8
	movs r0, #8
	bl 0x0200a124
	movs r1, #1
	movs r0, #3
	bl 0x0200a6a8
	lsls r0, r5, #4
	subs r0, r5, r0
	lsls r0, r0, #2
	adds r0, #60
	bl 0x0200bc98
	movs r0, #0
	b .L_02002844_2
.L_02002844_1:
	movs r0, #30
	bl 0x0200bc98
	adds r0, r5, #0
	adds r0, #90
	bl 0x0200bdf8
	movs r0, #4
	bl 0x0200a124
	movs r1, #0
	movs r0, #3
	bl 0x0200a6a8
	adds r0, r6, #0
	adds r0, #60
	bl 0x0200bc98
	movs r0, #8
.L_02002844_2:
	ldr r1, [pc, #64]
	movs r2, #0
	bl 0x0200bd60
	b .L_02002844_3
.L_02002844_4:
	movs r0, #1
	bl 0x0200bb08
.L_02002844_3:
	bl 0x0200be00
	cmp r0, #0
	bne .L_02002844_4
	movs r0, #19
	bl 0x0200bdf8
	movs r0, #30
	bl 0x0200bc98
	ldr r0, [pc, #32]
	bl 0x0200bdf8
	bl 0x0200bdb8
	bl 0x0200bdc0
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0200c628
	.4byte 0x0200be76
	.4byte 0x00000105
	.4byte 0x00000121
	.global Func_02002910
	.thumb_func
Func_02002910:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #428]
	sub sp, #4
	ldr r6, [r3]
	mov r11, r0
	mov r8, r1
	mov r10, r2
	bl 0x0200bcb8
	movs r3, #1
	strb r3, [r6, #6]
	movs r3, #4
	adds r7, r0, #0
	strb r3, [r6, #7]
	ldr r3, [r7, #8]
	ldr r2, [pc, #404]
	str r3, [r2]
	ldr r3, [r7, #16]
	ldr r2, [pc, #400]
	str r3, [r2]
	ldr r0, [r7, #80]
	ldrh r3, [r7, #6]
	ldr r2, [pc, #396]
	mov r9, r0
	str r3, [r2]
	mov r0, r11
	movs r1, #2
	bl 0x0200bd58
	adds r2, r7, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r5, #1
	orrs r5, r3
	strb r5, [r2]
	movs r5, #128
	lsls r5, r5, #7
	adds r0, r7, #0
	strh r5, [r7, #6]
	movs r1, #3
	bl 0x0200bc10
	adds r0, r7, #0
	movs r1, #0
	bl 0x0200bba8
	adds r0, r7, #0
	movs r1, #1
	bl 0x0200bba8
	mov r3, r10
	lsls r3, r3, #16
	mov r10, r3
	mov r1, r8
	lsls r1, r1, #16
	mov r0, r11
	mov r2, r10
	mov r8, r1
	bl 0x0200bcf8
	movs r0, #0
	adds r1, r5, #0
	movs r2, #0
	bl 0x0200bd20
	ldr r4, [pc, #316]
	ldr r6, [pc, #316]
	ldrh r3, [r6]
	adds r1, r3, #0
	strh r6, [r6]
	ldrh r2, [r4]
	cmp r2, #31
	bgt .L_02002910_0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r4
	strh r2, [r4]
	movs r2, #240
	adds r3, #4
	lsls r2, r2, #4
	stmia r3!, {r2}
	ldr r2, [pc, #288]
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02002910_0:
	strh r1, [r6]
	mov r0, r9
	movs r2, #13
	ldrb r1, [r0, #5]
	negs r2, r2
	adds r3, r2, #0
	ands r3, r1
	movs r1, #4
	orrs r3, r1
	strb r3, [r0, #5]
	ldrb r3, [r0, #17]
	ands r2, r3
	orrs r2, r1
	strb r2, [r0, #17]
	movs r0, #252
	str r4, [sp, #0]
	bl 0x0200bdf8
	ldr r4, [sp, #0]
	movs r5, #0
.L_02002910_2:
	movs r1, #128
	lsls r2, r5, #12
	lsls r1, r1, #5
	adds r3, r2, r1
	str r3, [r7, #24]
	movs r3, #248
	lsls r3, r3, #9
	subs r3, r3, r2
	str r3, [r7, #28]
	ldrh r3, [r6]
	adds r0, r3, #0
	strh r6, [r6]
	ldrh r3, [r4]
	cmp r3, #31
	bgt .L_02002910_1
	lsls r1, r3, #1
	adds r1, r1, r3
	adds r3, #1
	strh r3, [r4]
	movs r3, #15
	lsls r1, r1, #2
	subs r3, r3, r5
	adds r1, r4, r1
	lsls r3, r3, #8
	adds r2, r5, #1
	adds r1, #4
	orrs r3, r2
	stmia r1!, {r3}
	ldr r3, [pc, #184]
	stmia r1!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r1]
.L_02002910_1:
	strh r0, [r6]
	movs r0, #1
	str r4, [sp, #0]
	bl 0x0200bb08
	adds r5, #2
	ldr r4, [sp, #0]
	cmp r5, #15
	ble .L_02002910_2
	ldr r1, [pc, #144]
	ldr r0, [pc, #148]
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02002910_3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	movs r2, #16
	stmia r3!, {r2}
	ldr r2, [pc, #124]
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02002910_3:
	strh r4, [r0]
	movs r3, #136
	lsls r3, r3, #9
	str r3, [r7, #24]
	movs r3, #240
	lsls r3, r3, #8
	str r3, [r7, #28]
	movs r0, #1
	bl 0x0200bc98
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r0, #13
	bl 0x0200bc98
	mov r3, r9
	movs r2, #13
	ldrb r1, [r3, #5]
	negs r2, r2
	adds r3, r2, #0
	mov r0, r9
	ands r3, r1
	strb r3, [r0, #5]
	ldrb r3, [r0, #17]
	ands r2, r3
	strb r2, [r0, #17]
	movs r1, #3
	mov r0, r11
	bl 0x0200bd08
	movs r0, #20
	bl 0x0200bc98
	sub sp, #-4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x03001e68
	.4byte 0x0200c804
	.4byte 0x0200c75c
	.4byte 0x0200c788
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x04000050
	.4byte 0x04000052
	.global Func_02002aec
	.thumb_func
Func_02002aec:
	push {r5, r6, r7, lr}
	ldr r3, [pc, #40]
	adds r6, r0, #0
	ldr r7, [r3]
	bl 0x0200bcb8
	ldr r3, [pc, #32]
	movs r1, #249
	lsls r1, r1, #1
	adds r2, r3, r1
	ldrb r3, [r2]
	adds r5, r0, #0
	cmp r3, #1
	bne .L_02002aec_0
	movs r3, #0
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200bd00
	b .L_02002aec_1
	.2byte 0x0000
	.4byte 0x03001e68
	.4byte 0x02000240
.L_02002aec_0:
	movs r1, #128
	adds r0, r6, #0
	lsls r1, r1, #7
	movs r2, #30
	bl 0x0200bd50
	adds r0, r6, #0
	movs r1, #3
	bl 0x0200bd00
	movs r0, #30
	bl 0x0200bc98
.L_02002aec_1:
	movs r2, #0
	movs r3, #15
	strb r2, [r7, #7]
	strb r3, [r7, #6]
	ldr r3, [pc, #84]
	ldr r3, [r3]
	str r3, [r5, #8]
	ldr r3, [pc, #80]
	ldr r3, [r3]
	str r3, [r5, #16]
	ldr r3, [pc, #80]
	ldr r3, [r3]
	strh r3, [r5, #6]
	movs r3, #128
	lsls r3, r3, #24
	adds r0, r5, #0
	str r3, [r5, #56]
	str r3, [r5, #64]
	adds r0, #85
	movs r3, #3
	str r2, [r5, #36]
	str r2, [r5, #44]
	ldr r1, [pc, #44]
	strb r3, [r0]
	adds r3, r5, #0
	adds r3, #34
	strb r1, [r3]
	adds r0, r5, #0
	str r2, [r5, #12]
	str r2, [r5, #20]
	movs r1, #1
	bl 0x0200bc10
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bba8
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200bba8
	movs r0, #1
	bl 0x0200bb08
	b .L_02002aec_2
	.4byte 0x00000000
	.4byte 0x0200c804
	.4byte 0x0200c75c
	.4byte 0x0200c788
.L_02002aec_2:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002bac
	.thumb_func
Func_02002bac:
	push {r5, lr}
	bl 0x0200bcb8
	adds r5, r0, #0
	bl 0x0200bbc8
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02002bcc
	.thumb_func
Func_02002bcc:
	push {r5, lr}
	ldr r5, [pc, #24]
	movs r2, #0
	ldrsh r3, [r5, r2]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_02002bcc_0
	bl 0x0200bc38
	strh r0, [r5]
.L_02002bcc_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200c6a6
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #180]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #176]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	ldr	r2, [pc, #172]
	lsrs	r3, r3, #5
	ldr	r0, [pc, #172]
	mov	sl, r3
	movs	r3, #0
	ldrsh	r7, [r2, r3]
	mov	fp, r0
	mov	r9, r2
	cmp	r7, #0
	beq.n	.L_02002c80
	ldr	r3, [pc, #160]
	ldrh	r5, [r3, #0]
	adds	r5, #1
	strh	r5, [r3, #0]
	ldr	r0, [pc, #156]
	ldr	r1, [pc, #160]
	ldr	r3, [pc, #160]
	mov	r8, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	lsls	r5, r5, #16
	subs	r2, r2, r3
	asrs	r5, r5, #16
	ldrh	r6, [r1, #0]
	adds	r0, r5, #0
	muls	r0, r2
	adds	r1, r7, #0
	bl 0x0200bb00
	ldr	r2, [pc, #136]
	adds	r6, r6, r0
	mov	r1, r8
	strh	r6, [r1, #0]
	mov	r8, r2
	ldr	r3, [pc, #128]
	ldr	r2, [pc, #132]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	ldrh	r6, [r2, #0]
	movs	r1, #0
	ldrsh	r2, [r2, r1]
	subs	r3, r3, r2
	adds	r0, r5, #0
	muls	r0, r3
	adds	r1, r7, #0
	bl 0x0200bb00
	mov	r2, r8
	adds	r6, r6, r0
	strh	r6, [r2, #0]
	cmp	r5, r7
	blt.n	.L_02002c7a
	ldr	r3, [pc, #52]
	mov	r0, r9
	strh	r3, [r0, #0]
.L_02002c7a:
	ldr	r2, [pc, #96]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #0]
.L_02002c80:
	ldr	r2, [pc, #88]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #13
	bgt.n	.L_02002d0c
	ldr	r3, [pc, #48]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r3, [pc, #56]
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	mov	r0, fp
	movs	r3, #0
	stmia	r0!, {r3}
	subs	r1, #8
	ldr	r3, [pc, #56]
	lsls	r1, r1, #16
	b.n	.L_02002ce4
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200c6a6
	.4byte 0x03001b10
	.4byte 0x0200c78c
	.4byte 0x0200c7b0
	.4byte 0x0200c750
	.4byte 0x0200c7f4
	.4byte 0x0200c7a4
	.4byte 0x0200c760
	.4byte 0x0200c780
	.4byte 0x0200c800
	.4byte 0x0200c7bc
	.4byte 0x0200c774
	.2byte 0xc758
	.2byte 0x0200
.L_02002ce4:
	subs	r2, #8
	orrs	r2, r1
	movs	r4, #128
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	lsls	r4, r4, #23
	lsls	r3, r3, #28
	orrs	r2, r4
	orrs	r2, r3
	movs	r3, #128
	stmia	r0!, {r2}
	lsls	r3, r3, #3
	mov	r2, sl
	orrs	r2, r3
	str	r2, [r0, #0]
	movs	r1, #255
	mov	r0, fp
	bl 0x0200bb90
	b.n	.L_02002d14
.L_02002d0c:
	cmp	r3, #19
	ble.n	.L_02002d14
	ldr	r3, [pc, #16]
	strh	r3, [r2, #0]
.L_02002d14:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.2byte 0x0000
	.global Func_02002d28
	.thumb_func
Func_02002d28:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r0, #0
	mov r8, r1
	adds r5, r2, #0
	bl 0x0200abcc
	ldr r3, [pc, #44]
	strh r6, [r3]
	ldr r3, [pc, #44]
	mov r2, r8
	strh r2, [r3]
	ldr r3, [pc, #28]
	ldr r2, [pc, #40]
	ands r5, r3
	strh r5, [r2]
	ldr r3, [pc, #40]
	ldr r2, [pc, #20]
	strh r2, [r3]
	ldr r3, [pc, #36]
	movs r1, #200
	strh r2, [r3]
	lsls r1, r1, #4
	ldr r0, [pc, #32]
	bl 0x0200bb10
	b .L_02002d28_0
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0200c7f4
	.4byte 0x0200c780
	.4byte 0x0200c758
	.4byte 0x0200c774
	.4byte 0x0200c78c
	.4byte 0x0200abed
.L_02002d28_0:
	pop {r3}
	mov r8, r3
	pop {r5, r6}
.L_02002d86:
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02002d8c
	.thumb_func
Func_02002d8c:
	push {lr}
	ldr r3, [pc, #52]
	strh r0, [r3]
	ldr r3, [pc, #52]
	strh r1, [r3]
	ldr r3, [pc, #52]
	ldr r1, [pc, #52]
	ldrh r3, [r3]
	strh r3, [r1]
	ldr r3, [pc, #52]
	ldr r1, [pc, #52]
	ldrh r3, [r3]
	strh r3, [r1]
	ldr r3, [pc, #52]
	strh r2, [r3]
	ldr r2, [pc, #52]
	ldr r3, [pc, #16]
	movs r1, #200
	strh r3, [r2]
	lsls r1, r1, #4
	ldr r0, [pc, #44]
	bl 0x0200bb10
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000000
	.4byte 0x0200c760
	.4byte 0x0200c800
	.4byte 0x0200c7f4
	.4byte 0x0200c7a4
	.4byte 0x0200c780
	.4byte 0x0200c7bc
	.4byte 0x0200c78c
	.4byte 0x0200c750
	.4byte 0x0200abed
	.global Func_02002de8
	.thumb_func
Func_02002de8:
	push {r5, lr}
	ldr r0, [pc, #28]
	bl 0x0200bb18
	ldr r5, [pc, #24]
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl 0x0200bb68
	ldr r3, [pc, #8]
	strh r3, [r5]
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffffffff
	.4byte 0x0200abed
	.4byte 0x0200c6a6
	.global Func_02002e10
	.thumb_func
Func_02002e10:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r2, #0
	bl 0x0200bdc8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02002e10_0
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	asrs r3, r3, #1
	str r3, [r5, #52]
	adds r3, r5, #0
	movs r2, #0
	adds r3, #91
	strb r2, [r3]
	bl 0x0200bbc8
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200bba8
	lsls r1, r6, #16
	ldr r2, [r5, #12]
	lsls r3, r7, #16
	adds r0, r5, #0
	bl 0x0200bbd8
.L_02002e10_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_02002e50
	.thumb_func
Func_02002e50:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r2, #0
	bl 0x0200bdc8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02002e50_0
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #48]
	asrs r3, r3, #1
	str r3, [r5, #52]
	adds r3, r5, #0
	movs r2, #0
	adds r3, #91
	strb r2, [r3]
	bl 0x0200bbc8
	adds r0, r5, #0
	movs r1, #5
	bl 0x0200bba8
	lsls r1, r6, #16
	ldr r2, [r5, #12]
	lsls r3, r7, #16
	adds r0, r5, #0
	bl 0x0200bbd8
	adds r0, r5, #0
	bl 0x0200bbe0
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200bba8
.L_02002e50_0:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .text.x0200af64,"ax",%progbits
	.p2align 2
	.global Func_02002f64
	.thumb_func
Func_02002f64:
	push {r5, lr}
	adds r0, #8
	movs r3, #0
	ldr r5, [pc, #32]
	strb r3, [r0]
	movs r2, #7
	subs r0, #1
	movs r4, #15
.L_02002f64_0:
	adds r3, r1, #0
	ands r3, r4
	ldrb r3, [r5, r3]
	subs r2, #1
	strb r3, [r0]
	lsrs r1, r1, #4
	subs r0, #1
	cmp r2, #0
	bge .L_02002f64_0
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0200bfd0
	.global Func_02002f90
	.thumb_func
Func_02002f90:
	bx lr
	.2byte 0x0000
	.global Func_02002f94
	.thumb_func
Func_02002f94:
	ldr r2, [pc, #4]
	movs r3, #9
	strh r3, [r2]
	bx lr
	.4byte 0x02001000
	.global Func_02002fa0
	.thumb_func
Func_02002fa0:
	push {r5, lr}
	ldr r5, [pc, #28]
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #9
	beq .L_02002fa0_0
.L_02002fa0_1:
	movs r0, #1
	bl 0x0200bb08
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #9
	bne .L_02002fa0_1
.L_02002fa0_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x02001000
	.global Func_02002fc4
	.thumb_func
Func_02002fc4:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldr r3, [r5, #40]
	movs r2, #255
	adds r3, #255
	lsls r2, r2, #1
	sub sp, #12
	cmp r3, r2
	bhi .L_02002fc4_0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02002fc4_0:
	bl 0x0200bb20
	movs r3, #100
	muls r3, r0
	lsrs r3, r3, #16
	cmp r3, #9
	bhi .L_02002fc4_1
	ldr r3, [r5, #8]
	mov r6, sp
	str r3, [r6]
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl 0x0200bb20
	adds r5, r0, #0
	bl 0x0200bb20
	lsls r5, r5, #4
	adds r1, r0, #0
	adds r2, r6, #0
	adds r0, r5, #0
	bl 0x0200bb30
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	ldr r0, [pc, #56]
	bl 0x0200bbb8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02002fc4_1
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r1, #0
	bl 0x0200bc10
	ldr r1, [pc, #32]
	adds r0, r5, #0
	bl 0x0200bbb0
	adds r0, r5, #0
	movs r1, #1
	bl 0x0200bba8
	adds r0, r5, #0
	movs r1, #0
	bl 0x0200bba8
.L_02002fc4_1:
	sub sp, #-12
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x0000011d
	.4byte 0x0200bfe4
	.global Func_02003058
	.thumb_func
Func_02003058:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	movs r2, #100
	adds r2, r2, r5
	movs r3, #0
	ldrsh r0, [r2, r3]
	mov r8, r2
	bl 0x0200bcb8
	ldr r2, [r5, #12]
	movs r3, #144
	lsls r3, r3, #14
	adds r2, r2, r3
	adds r6, r0, #0
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	bl 0x0200bbd8
	adds r3, r6, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	ldr r1, [pc, #28]
	adds r0, r6, #0
	bl 0x0200bbb0
	movs r0, #83
	bl 0x0200bdf8
	mov r2, r8
	strh r5, [r2]
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0200c008
	.global Func_020030ac
	.thumb_func
Func_020030ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, [pc, #236]
	mov r10, r0
	ldr r0, [pc, #236]
	mov r8, r1
	ldr r5, [r3]
	bl 0x0200bc58
	ldr r3, [pc, #232]
	adds r7, r0, #0
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	bl 0x0200bcb8
	adds r3, r5, #0
	adds r6, r0, #0
	adds r3, #232
	ldr r2, [r3]
	movs r0, #192
	ldr r3, [r6, #8]
	lsls r0, r0, #12
	adds r1, r2, r0
	cmp r2, r3
	blt .L_020030ac_0
	ldr r3, [pc, #200]
	adds r1, r2, r3
.L_020030ac_0:
	cmp r7, #0
	beq .L_020030ac_1
	adds r3, r5, #0
	adds r3, #236
	ldr r3, [r3]
	movs r0, #128
	lsls r0, r0, #13
	adds r4, r3, r0
	adds r3, r5, #0
	adds r3, #228
	b .L_020030ac_2
.L_020030ac_1:
	adds r3, r5, #0
	adds r3, #236
	ldr r3, [r3]
	ldr r2, [pc, #172]
	adds r4, r3, r2
	adds r3, r5, #0
	adds r3, #226
.L_020030ac_2:
	ldrh r3, [r3]
	adds r5, r6, #0
	adds r5, #100
	strh r3, [r5]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #9
	movs r2, #0
	str r3, [r6, #48]
	adds r0, r6, #0
	adds r3, r4, #0
	bl 0x0200bbd8
	ldr r0, [pc, #120]
	bl 0x0200bc60
	adds r0, r6, #0
	ldr r1, [pc, #128]
	bl 0x0200bbb0
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #0
	beq .L_020030ac_3
.L_020030ac_4:
	movs r0, #1
	bl 0x0200bb08
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_020030ac_4
.L_020030ac_3:
	cmp r7, #0
.L_02003152:
	bne .L_02003152_0
	mov r1, r10
	movs r0, #0
	bl 0x0200a0e8
	mov r0, r10
	movs r1, #2
	bl 0x0200bc20
	b 0x0200b176
.L_02003152_0:
	mov r1, r8
	movs r0, #0
	bl 0x0200a0e8
	mov r0, r8
	movs r1, #2
.L_02003172:
	bl 0x0200bc20
	ldr r3, [pc, #52]
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #1
	bl 0x0200bc20
	movs r1, #3
	ldr r0, [pc, #48]
	bl 0x0200bc18
	adds r0, r6, #0
	bl 0x0200bbd0
	adds r0, r7, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x1f3c
	.2byte 0x0300
	.2byte 0x0211
	.2byte 0x0000
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff4
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xc6fc
	.2byte 0x0200
	.4byte 0x0000096a
	.section .text.x0200b3a0,"ax",%progbits
	.p2align 2
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #916]
	ldr	r3, [r3, #0]
	adds	r1, r3, #0
	sub	sp, #20
	mov	r8, r1
.L_020033b8:
	str	r3, [sp, #12]
	str	r3, [sp, #16]
	mov	r7, r8
	adds	r7, #216
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	ldr	r2, [pc, #896]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	lsrs	r3, r3, #5
	str	r3, [sp, #8]
	mov	r3, r8
	adds	r3, #230
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	subs	r3, #10
	mov	fp, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_020033ee
	mov	r6, r8
	adds	r6, #218
	movs	r3, #2
	strh	r3, [r6, #0]
	b.n	.L_0200345c
.L_020033ee:
	movs	r0, #131
	lsls	r0, r0, #1
	bl 0x0200bc58
	cmp	r0, #0
	beq.n	.L_0200340e
	mov	r6, r8
	adds	r6, #218
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #0]
	cmp	r3, #0
	ble.n	.L_0200345c
	subs	r3, r2, #1
	strh	r3, [r6, #0]
	b.n	.L_0200345c
.L_0200340e:
	mov	r6, r8
	adds	r6, #218
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	ldrh	r2, [r6, #0]
	cmp	r3, #1
	bgt.n	.L_0200345c
	adds	r3, r2, #1
	movs	r2, #128
	strh	r3, [r6, #0]
	lsls	r2, r2, #9
	lsls	r3, r3, #16
	cmp	r3, r2
	bne.n	.L_0200345c
	ldr	r3, [pc, #800]
	ldr	r0, [pc, #800]
	ldr	r1, [pc, #804]
	ldr	r2, [pc, #804]
	stmia	r3!, {r0, r1, r2}
.L_02003434:
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #2
.L_0200343a:
	bl 0x0200bb50
	adds	r5, r0, #0
	adds	r1, r5, #0
	ldr	r0, [pc, #792]
	bl 0x0200bb60
	movs	r1, #128
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	lsls	r1, r1, #2
	adds	r2, r5, #0
	bl 0x0200bb78
	adds	r0, r5, #0
	bl 0x0200bb58
.L_0200345c:
	movs	r1, #0
	ldrsh	r2, [r6, r1]
	cmp	r2, #0
	bne.n	.L_02003472
	ldr	r3, [sp, #12]
	adds	r3, #216
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl 0x0200bb70
	b.n	.L_02003732
.L_02003472:
	lsls	r3, r2, #1
.L_02003474:
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r7, r3, #0
	subs	r7, #8
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	lsls	r3, r3, #4
	str	r3, [sp, #4]
	movs	r2, #128
	ldr	r1, [sp, #4]
	lsls	r2, r2, #8
	movs	r3, #104
	mov	r9, r2
	ldr	r2, [sp, #12]
	subs	r4, r3, r1
	movs	r3, #0
	stmia	r2!, {r3}
	lsls	r3, r4, #16
	adds	r1, r2, #0
	str	r1, [sp, #12]
	orrs	r3, r7
	mov	r1, r9
	orrs	r3, r1
	stmia	r2!, {r3}
	ldr	r3, [sp, #8]
	movs	r5, #228
	lsls	r5, r5, #8
	adds	r1, r2, #0
	orrs	r3, r5
	str	r1, [sp, #12]
	stmia	r2!, {r3}
	adds	r1, r2, #0
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	movs	r6, #0
	add	r8, r2
	bl 0x0200bb90
	cmp	r6, fp
	bcs.n	.L_0200350c
	ldr	r3, [sp, #8]
	movs	r1, #128
	adds	r3, #2
	orrs	r3, r5
.L_020034d2:
	lsls	r1, r1, #23
	ldr	r5, [sp, #12]
	mov	r9, r1
	mov	sl, r3
.L_020034da:
	lsls	r2, r6, #4
	movs	r3, #96
	subs	r4, r3, r2
	movs	r3, #0
	str	r3, [r5, #0]
	lsls	r3, r4, #16
	mov	r2, r9
	orrs	r3, r7
	orrs	r3, r2
	str	r3, [r5, #4]
	mov	r3, sl
	str	r3, [r5, #8]
	ldr	r1, [sp, #12]
	adds	r1, #12
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	adds	r6, #1
	add	r8, r2
	adds	r5, #12
	bl 0x0200bb90
	cmp	r6, fp
	bcc.n	.L_020034da
.L_0200350c:
	ldr	r2, [sp, #12]
	movs	r6, #0
	movs	r3, #128
	stmia	r2!, {r6}
	lsls	r3, r3, #8
	mov	r9, r3
	movs	r3, #224
	adds	r1, r2, #0
	lsls	r3, r3, #15
	str	r1, [sp, #12]
	orrs	r3, r7
	mov	r1, r9
	orrs	r3, r1
	stmia	r2!, {r3}
	ldr	r5, [sp, #8]
	adds	r1, r2, #0
	movs	r2, #228
	lsls	r2, r2, #8
	adds	r5, #6
	orrs	r5, r2
	stmia	r1!, {r5}
	mov	r0, r8
	adds	r3, r1, #0
	mov	sl, r2
	movs	r1, #255
	movs	r2, #12
.L_02003540:
	add	r8, r2
	str	r3, [sp, #12]
	bl 0x0200bb90
	ldr	r1, [sp, #12]
	stmia	r1!, {r6}
.L_0200354c:
	adds	r3, r1, #0
	str	r3, [sp, #12]
	movs	r3, #240
	lsls	r3, r3, #15
	mov	r2, r9
	orrs	r3, r7
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #21
	orrs	r3, r2
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #12]
	stmia	r1!, {r5}
	adds	r3, r1, #0
	movs	r1, #12
	mov	r0, r8
	add	r8, r1
	movs	r1, #255
	str	r3, [sp, #12]
	bl 0x0200bb90
	cmp	r6, fp
	bcs.n	.L_020035ce
	ldr	r4, [sp, #8]
	movs	r2, #128
	movs	r1, #128
	mov	r3, sl
	adds	r4, #2
	lsls	r2, r2, #23
	lsls	r1, r1, #16
	ldr	r5, [sp, #12]
	mov	r9, r2
	orrs	r4, r3
	mov	sl, r1
.L_02003592:
	movs	r3, #0
	str	r3, [r5, #0]
	mov	r2, sl
	adds	r3, r7, #0
	orrs	r3, r2
	mov	r1, r9
	movs	r2, #128
	orrs	r3, r1
	lsls	r2, r2, #21
	orrs	r3, r2
	str	r3, [r5, #4]
	str	r4, [r5, #8]
	ldr	r2, [sp, #12]
	mov	r0, r8
	adds	r2, #12
	movs	r3, #12
	movs	r1, #255
	str	r4, [sp, #0]
	str	r2, [sp, #12]
	add	r8, r3
	bl 0x0200bb90
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r6, #1
	adds	r5, #12
	add	sl, r1
	ldr	r4, [sp, #0]
	cmp	r6, fp
	bcc.n	.L_02003592
.L_020035ce:
	movs	r2, #128
	ldr	r4, [sp, #4]
	lsls	r2, r2, #8
	mov	r9, r2
	ldr	r2, [sp, #12]
	movs	r3, #0
	adds	r4, #128
	stmia	r2!, {r3}
	mov	fp, r3
	lsls	r3, r4, #16
	orrs	r7, r3
	mov	r3, r9
	orrs	r7, r3
	movs	r3, #128
	lsls	r3, r3, #21
	adds	r1, r2, #0
	orrs	r7, r3
	str	r1, [sp, #12]
	stmia	r2!, {r7}
	ldr	r3, [sp, #8]
	adds	r1, r2, #0
	movs	r2, #228
	lsls	r2, r2, #8
	orrs	r3, r2
	mov	sl, r2
	adds	r2, r1, #0
	stmia	r2!, {r3}
	adds	r1, r2, #0
	movs	r3, #12
	str	r1, [sp, #12]
	mov	r0, r8
	movs	r1, #255
	add	r8, r3
	bl 0x0200bb90
	ldr	r3, [pc, #328]
	ldr	r3, [r3, #0]
	movs	r2, #15
	ands	r3, r2
	cmp	r3, #4
	bhi.n	.L_02003622
	b.n	.L_02003732
.L_02003622:
	ldr	r3, [sp, #16]
	movs	r1, #128
	adds	r3, #224
	lsls	r1, r1, #23
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	mov	r9, r1
	bl 0x0200bdc8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_020036b0
	ldr	r3, [sp, #16]
	adds	r3, #232
	ldr	r3, [r3, #0]
	ldr	r0, [r6, #8]
	movs	r5, #224
	lsls	r5, r5, #12
	subs	r0, r0, r3
	adds	r1, r5, #0
	bl 0x0200bb00
	ldr	r3, [sp, #16]
	adds	r3, #236
	ldr	r3, [r3, #0]
	adds	r4, r0, #0
	ldr	r0, [r6, #16]
	adds	r4, #112
	adds	r1, r5, #0
	subs	r0, r0, r3
	str	r4, [sp, #0]
	bl 0x0200bb00
	ldr	r3, [sp, #16]
	adds	r3, #218
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, r0, r3
	ldr	r1, [sp, #12]
	subs	r7, r0, #4
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	stmia	r1!, {r3}
.L_02003680:
	ldr	r4, [sp, #0]
	adds	r2, r1, #0
	lsls	r3, r4, #16
	str	r2, [sp, #12]
	orrs	r7, r3
	mov	r2, r9
	orrs	r7, r2
	stmia	r1!, {r7}
	adds	r3, r1, #0
	str	r3, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r1, sl
	adds	r3, #12
	orrs	r3, r1
	ldr	r1, [sp, #12]
	stmia	r1!, {r3}
	adds	r2, r1, #0
	str	r2, [sp, #12]
	mov	r0, r8
	movs	r2, #12
	movs	r1, #255
	add	r8, r2
	bl 0x0200bb90
.L_020036b0:
	ldr	r3, [sp, #16]
	adds	r3, #222
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl 0x0200bdc8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_02003732
	ldr	r3, [sp, #16]
	adds	r3, #232
	ldr	r3, [r3, #0]
	ldr	r0, [r6, #8]
	movs	r5, #224
	lsls	r5, r5, #12
	subs	r0, r0, r3
	adds	r1, r5, #0
	bl 0x0200bb00
	ldr	r3, [sp, #16]
	adds	r3, #236
	ldr	r3, [r3, #0]
	adds	r4, r0, #0
	ldr	r0, [r6, #16]
	adds	r4, #112
	adds	r1, r5, #0
	subs	r0, r0, r3
	str	r4, [sp, #0]
	bl 0x0200bb00
	ldr	r3, [sp, #16]
	adds	r3, #218
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
.L_020036fa:
	adds	r0, r0, r3
	ldr	r1, [sp, #12]
	subs	r7, r0, #4
	movs	r3, #255
	ands	r7, r3
	mov	r3, fp
	stmia	r1!, {r3}
	ldr	r4, [sp, #0]
	adds	r2, r1, #0
	lsls	r3, r4, #16
	str	r2, [sp, #12]
	orrs	r7, r3
	mov	r2, r9
	orrs	r7, r2
	stmia	r1!, {r7}
	adds	r3, r1, #0
	str	r3, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r1, sl
	adds	r3, #8
	ldr	r2, [sp, #12]
	orrs	r3, r1
	str	r3, [r2, #0]
	mov	r3, r8
	adds	r0, r3, #0
	movs	r1, #255
	bl 0x0200bb90
.L_02003732:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f3c
	.4byte 0x03001b10
	.4byte 0x040000d4
	.4byte 0x0200bef4
	.4byte 0x050003c0
	.4byte 0x80000010
	.4byte 0x0200bf14
	.2byte 0x1e40
	.2byte 0x0300
	.global Func_02003764
	.thumb_func
Func_02003764:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	mov r8, r0
	mov r10, r1
	movs r0, #59
	ldr r1, [pc, #172]
	ldr r5, [sp, #40]
	ldr r6, [sp, #44]
	str r3, [sp, #0]
	mov r9, r2
	bl 0x0200bb40
	adds r7, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bb50
	adds r3, r7, #0
	adds r3, #222
	mov r2, r8
	strh r2, [r3]
	adds r3, #2
	mov r2, r10
	strh r2, [r3]
	adds r3, #2
	strh r5, [r3]
	adds r3, #2
	strh r6, [r3]
	mov r2, r9
	adds r3, #2
	strh r2, [r3]
	ldr r2, [sp, #0]
	adds r3, #2
	str r2, [r3]
	ldr r2, [sp, #36]
	adds r3, #4
	str r2, [r3]
	mov r11, r0
	mov r0, r8
	bl 0x0200bcb8
	adds r6, r0, #0
	mov r0, r10
	bl 0x0200bcb8
	adds r5, r0, #0
	ldr r0, [pc, #92]
	bl 0x0200bc58
	cmp r0, #0
.L_020037d4:
	bne .L_020037d4_0
	ldr r2, [sp, #0]
	lsls r3, r2, #1
	ldr r2, [r6, #8]
	subs r3, r3, r2
	str r3, [r5, #8]
	ldr r3, [r6, #16]
	str r3, [r5, #16]
.L_020037d4_0:
	ldr r2, [pc, #60]
	adds r3, r7, #0
	adds r3, #218
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	mov r1, r11
	ldr r0, [pc, #60]
	bl 0x0200bb60
	bl 0x0200bb80
	adds r3, r7, #0
	adds r3, #216
	strh r0, [r3]
	movs r1, #128
	lsls r0, r0, #16
	mov r2, r11
	lsls r1, r1, #2
	asrs r0, r0, #16
	bl 0x0200bb78
	ldr r1, [pc, #32]
	ldr r0, [pc, #36]
	bl 0x0200bb10
	mov r0, r11
	bl 0x0200bb58
	sub sp, #-4
	b .L_020037d4_1
	.2byte 0x0000
	.4byte 0x00000000
	.2byte 0x7170
	.2byte 0x0000
	.2byte 0x0109
	.2byte 0x0000
	.4byte 0x0200bf14
	.4byte 0x00000c76
	.4byte 0x0200b3a1
.L_020037d4_1:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.global Func_0200384c
	.thumb_func
Func_0200384c:
	push {r5, r6, lr}
	ldr r3, [pc, #60]
	ldr r6, [r3]
	ldr r5, [pc, #60]
	bl 0x0200bb98
	adds r1, r6, #0
	adds r1, #240
	bl 0x0200bb60
	ldr r0, [pc, #48]
	bl 0x0200bc58
	cmp r0, #0
	bne .L_0200384c_0
	movs r3, #1
	strh r3, [r5]
	strh r3, [r5, #2]
	adds r3, r6, #0
	adds r3, #224
	ldrh r3, [r3]
	strh r0, [r5, #8]
	strh r3, [r5, #4]
	strh r0, [r5, #6]
.L_0200384c_0:
	ldr r1, [pc, #24]
	ldr r0, [pc, #28]
	bl 0x0200bb10
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001f3c
	.4byte 0x02001000
	.4byte 0x00000109
	.4byte 0x00000c85
	.4byte 0x0200b1c1
	.global Func_020038a0
	.thumb_func
Func_020038a0:
	ldr r3, [pc, #8]
	ldr r3, [r3]
	adds r3, #220
	strh r0, [r3]
	bx lr
	.2byte 0x0000
	.4byte 0x03001f3c
	.global Func_020038b0
	.thumb_func
Func_020038b0:
	push {r5, r6, lr}
	ldr r3, [pc, #64]
	adds r4, r0, #0
	ldr r2, [r3]
	ldr r3, [r4]
	adds r1, r2, #0
	movs r5, #8
	asrs r6, r3, #20
	adds r1, #52
.L_020038b0_2:
	ldmia r1!, {r0}
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r6, r3
	bne .L_020038b0_0
	ldr r2, [r4, #4]
	ldr r3, [r0, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020038b0_0
	ldr r2, [r4, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_020038b0_1
.L_020038b0_0:
	adds r5, #1
	cmp r5, #65
	bls .L_020038b0_2
	movs r0, #0
.L_020038b0_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.global Func_020038f8
	.thumb_func
Func_020038f8:
	push {r5, r6, r7, lr}
	mov r7, r11
.L_020038fc:
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #328]
	movs r0, #250
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #12
	bl 0x0200bcb8
	ldrh r3, [r0, #6]
	ldr r2, [pc, #312]
	lsrs r3, r3, #12
	lsls r5, r3, #2
	ldr r1, [r2, r5]
	ldr r3, [pc, #308]
	mov r9, r2
	adds r2, r1, #0
	ands r2, r3
	mov r10, r3
	ldr r3, [r0, #8]
	mov r7, sp
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r0, #12]
	str r3, [r7, #4]
	ldr r3, [r0, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	mov r8, r0
	str r3, [r7, #8]
	adds r0, r7, #0
	mov r1, r8
	bl 0x0200b8b0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020038fc_0
	mov r0, r9
	ldr r1, [r0, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r0, r7, #0
	adds r1, r6, #0
	bl 0x0200b8b0
	cmp r0, #0
	beq .L_020038fc_1
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_020038fc_0
.L_020038fc_1:
	ldr r3, [r6, #8]
	str r3, [r7]
	movs r0, #128
	ldr r3, [r6, #12]
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r0, r7, #0
	str r3, [r7, #8]
	adds r1, r6, #0
	bl 0x0200b8b0
	cmp r0, #0
	beq .L_020038fc_2
	adds r3, r0, #0
	adds r3, #89
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_020038fc_0
.L_020038fc_2:
	movs r3, #0
	adds r2, r6, #0
	adds r2, #34
	mov r11, r3
	movs r3, #2
	strb r3, [r2]
	mov r0, r9
	ldr r1, [r0, r5]
	mov r3, r10
	adds r2, r1, #0
	ands r2, r3
	ldr r3, [r6, #8]
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
	bl 0x0200bc08
	cmp r0, #0
	bgt .L_020038fc_0
	ldr r5, [pc, #116]
	movs r1, #8
	mov r0, r8
	bl 0x0200bba8
	movs r0, #15
	bl 0x0200bb08
	str r5, [r6, #48]
	str r5, [r6, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r6, #0
	bl 0x0200bbd8
	mov r0, r8
	str r5, [r0, #48]
	str r5, [r0, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl 0x0200bbd8
	movs r0, #238
	bl 0x0200bdf8
	adds r0, r6, #0
	bl 0x0200bbe0
	movs r0, #144
	lsls r0, r0, #1
	bl 0x0200bdf8
	ldr r3, [r7]
	str r3, [r6, #8]
	ldr r3, [r7, #8]
	mov r2, r11
	str r3, [r6, #16]
	str r2, [r6, #36]
	str r2, [r6, #44]
	mov r0, r8
	movs r1, #1
	bl 0x0200bba8
.L_020038fc_0:
	sub sp, #-12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x02000240
	.4byte 0x0200c154
	.4byte 0xffff0000
	.4byte 0x00003333
	.global Func_02003a60
	.thumb_func
Func_02003a60:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #144]
	movs r1, #250
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #12
	bl 0x0200bcb8
	adds r6, r0, #0
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r7, r3, r2
	movs r3, #192
	lsls r3, r3, #8
	ldr r1, [pc, #116]
	ands r7, r3
	ldr r3, [r6, #8]
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	mov r5, sp
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	movs r0, #128
	ands r3, r1
	adds r3, r3, r2
	lsls r0, r0, #13
	mov r8, r1
	adds r2, r5, #0
	adds r1, r7, #0
	str r3, [r5, #8]
	bl 0x0200bb30
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200b8b0
	cmp r0, #0
	bne 0x0200baea
	ldr r3, [r6, #8]
	mov r2, r8
	movs r1, #128
	lsls r1, r1, #12
	ands r3, r2
	adds r3, r3, r1
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	movs r0, #128
	ands r3, r2
.L_02003ad4:
	adds r3, r3, r1
	lsls r0, r0, #14
	adds r1, r7, #0
	adds r2, r5, #0
.L_02003adc:
	str r3, [r5, #8]
	bl 0x0200bb30
	adds r0, r5, #0
	adds r1, r6, #0
	bl 0x0200b8b0
	sub sp, #-12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0000
	.2byte 0xfff0
	.include "games/THE BROKEN SEAL/SRC/FIELD/KOROSSEO_KAWA/IMPORT.INC"
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.4byte 0x20202000
	.4byte 0x40404060
	.4byte 0x10000080
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x2000001e
	.4byte 0x001e0000
	.4byte 0x001e7fff
	.4byte 0x1000ffff
	.4byte 0x00010080
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x1000003c
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x00f01000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060170
	.4byte 0x00067fff
	.4byte 0x00e01000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01601000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600d0
	.4byte 0x00067fff
	.4byte 0x01501000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000600c0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.4byte 0x377f10a0
	.4byte 0x121722bb
	.4byte 0x0ccc1172
	.4byte 0x7df07ef7
	.4byte 0x610b7ce7
	.4byte 0x50007c00
	.4byte 0x00147fff
	.4byte 0x2f1f0000
	.4byte 0x82fc0100
	.4byte 0x70462310
	.4byte 0x201abddc
	.4byte 0x648ad0cc
	.4byte 0xa8a89c76
	.4byte 0x81984754
	.4byte 0x6138edda
	.4byte 0x5f28474a
	.4byte 0xa01027d3
	.4byte 0xa0abb823
	.4byte 0xf0214faa
	.4byte 0x5557fc29
	.4byte 0x1c29f456
	.4byte 0xc0a8882e
	.4byte 0xf029560c
	.4byte 0x39f9b811
	.4byte 0xa3e78fa8
	.4byte 0xd88e8df8
	.4byte 0x60101ddd
	.4byte 0x2101d56c
	.4byte 0xa68033e0
	.4byte 0x1ca188ea
	.4byte 0xb037924e
	.4byte 0x8e40d46f
	.4byte 0x56a22701
	.4byte 0x9957005d
	.4byte 0x6e51113b
	.4byte 0x9f8022e5
	.4byte 0x3a3ae095
	.4byte 0x39edc9ab
	.4byte 0x9ddeeaec
	.4byte 0x378733aa
	.4byte 0xbb23a0e8
	.4byte 0x7b4bb0f7
	.4byte 0x0596cc80
	.4byte 0xc075eccf
	.4byte 0x3fd9b0ef
	.4byte 0xcc032ec8
	.4byte 0x8d5aec81
	.4byte 0xee419f03
	.4byte 0x7c20f81f
	.4byte 0x38be6e09
	.4byte 0xbe2a7c41
	.4byte 0x07316774
	.4byte 0xf7104b82
	.4byte 0xf17c90f8
	.4byte 0x00000001
	.4byte 0x33323130
	.4byte 0x37363534
	.4byte 0x42413938
	.4byte 0x46454443
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000001b
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x0200afc5
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000000a
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
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000010
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
	.4byte 0xffff0000
	.4byte 0x00000398
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000138
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000008f
	.4byte 0x00a0108f
	.4byte 0x00b0c08c
	.4byte 0x0041508c
	.4byte 0x0056208a
	.4byte 0x000001ff
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
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
	.4byte 0x0000c000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000007
	.4byte 0x02590000
	.4byte 0x00000000
	.4byte 0x00920000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00c20000
	.4byte 0x00024000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x03b00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00028000
	.4byte 0xffff00ec
	.4byte 0x00000007
	.4byte 0x04180000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00ed
	.4byte 0x00000007
	.4byte 0x03f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x04380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x05180000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x05180000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000202
	.4byte 0xffff005a
	.4byte 0x02008955
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02008239
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x02008239
	.4byte 0x00008602
	.4byte 0x0302000c
	.4byte 0x02008541
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte 0x0200842d
	.4byte 0x00004602
	.4byte 0xffff0014
	.4byte 0x0200842d
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte 0x02008491
	.4byte 0x00000602
	.4byte 0xffff0015
	.4byte 0x02008491
	.4byte 0x00000002
	.4byte 0x02100032
	.4byte 0x02008841
	.4byte 0x00000013
	.4byte 0x03080064
	.4byte 0x001000b5
	.4byte 0x00000013
	.4byte 0x03090065
	.4byte 0x001000ee
	.4byte 0x00008413
	.4byte 0x030a0067
	.4byte 0x001000b5
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x020081a9
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x020081a9
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x020081a9
	.4byte 0x00000c15
	.4byte 0x0303000f
	.4byte 0x02008249
	.4byte 0x00002115
	.4byte 0x0301000d
	.4byte 0x02008271
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x02009c7d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x02009215
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x020093e5
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020095e1
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020096ed
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000213c
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000213d
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x0000213e
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x0000213f
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002140
	.4byte 0x00000006
	.4byte 0xffff0063
	.4byte 0x02009a29
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x4000ffff
	.4byte 0x0800ff44
	.4byte 0x01801000
	.4byte 0x20000001
	.4byte 0x00010010
	.4byte 0x000e7fff
	.4byte 0x00003000
	.4byte 0x7fff0014
	.4byte 0x3000003c
	.4byte 0x00140800
	.4byte 0x003c7fff
	.4byte 0x1000ffff
	.4byte 0x00010200
	.4byte 0x00002000
	.4byte 0x10000001
	.4byte 0x00060100
	.4byte 0x00102000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060180
	.4byte 0x00067fff
	.4byte 0x01001000
	.4byte 0x7fff0006
	.4byte 0x20000006
	.4byte 0x001e0000
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060100
	.4byte 0x00067fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0xffff0006
	.4byte 0x00801000
	.4byte 0x20000001
	.4byte 0x00010000
	.4byte 0x01001000
	.4byte 0x20000006
	.4byte 0x00060010
	.4byte 0x003c7fff
	.4byte 0x01801000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060110
	.4byte 0x00067fff
	.4byte 0x01901000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x00060120
	.4byte 0x00067fff
	.4byte 0x00002000
	.4byte 0x1000001e
	.4byte 0x000601a0
	.4byte 0x00067fff
	.4byte 0x01301000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601b0
	.4byte 0x00067fff
	.4byte 0x01401000
	.4byte 0x7fff0006
	.4byte 0x10000006
	.4byte 0x000601c0
	.4byte 0x00067fff
	.4byte 0xffffffff
	.global Korosseo_RivalFinishScript
Korosseo_RivalFinishScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte 0x0200b059
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x0000001c
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x00000022
	.4byte 0x0200b059
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x0000001c
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0x00000096
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
