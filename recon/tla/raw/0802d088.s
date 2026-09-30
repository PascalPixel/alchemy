.syntax unified
	.thumb
	.global Func_0802d088
	.thumb_func
Func_0802d088:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	adds r6, r1, #0
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #115
	adds r2, r5, r1
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, r0
	beq .L_0802d0de
	strb r0, [r2]
	cmp r0, #9
	bne .L_0802d0b4
	movs r3, #144
	lsls r3, r3, #4
	adds r3, #117
	adds r2, r5, r3
	movs r3, #12
	b .L_0802d0dc
.L_0802d0b4:
	cmp r0, #18
	bne .L_0802d0d2
	movs r0, #254
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0802d0d2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #117
	adds r2, r5, r0
	movs r3, #250
	b .L_0802d0dc
.L_0802d0d2:
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #117
	adds r2, r5, r1
	movs r3, #0
.L_0802d0dc:
	strb r3, [r2]
.L_0802d0de:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0802d0ec
	b .L_0802d22e
.L_0802d0ec:
	cmp r6, #0
	bne .L_0802d106
	movs r2, #144
	lsls r2, r2, #4
	adds r2, #117
	adds r3, r5, r2
	movs r0, #144
	ldrb r2, [r3]
	lsls r0, r0, #4
	adds r0, #116
	adds r3, r5, r0
	strb r2, [r3]
	b .L_0802d150
.L_0802d106:
	movs r1, #144
	lsls r1, r1, #4
	movs r0, #144
	adds r1, #117
	lsls r0, r0, #4
	adds r3, r5, r1
	adds r0, #116
	movs r2, #0
	ldrsb r2, [r3, r2]
	adds r3, r5, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	subs r0, r2, r3
	cmp r0, #0
	bne .L_0802d128
	b .L_0802d22e
.L_0802d128:
	ldr r3, .L_0802d230
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0802d136
	b .L_0802d22e
.L_0802d136:
	cmp r0, #0
	bge .L_0802d140
	movs r0, #1
	negs r0, r0
	b .L_0802d142
.L_0802d140:
	movs r0, #1
.L_0802d142:
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #116
	adds r2, r5, r1
	ldrb r3, [r2]
	adds r3, r3, r0
	strb r3, [r2]
.L_0802d150:
	movs r2, #144
	lsls r2, r2, #4
	adds r2, #116
	adds r6, r5, r2
	movs r3, #0
	ldrsb r3, [r6, r3]
	cmp r3, #0
	ble .L_0802d1b8
	ldr r0, .L_0802d234
	ldr r1, .L_0802d238
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_0802d194
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r0]
	movs r2, #252
	adds r3, r3, r0
	lsls r2, r2, #6
	adds r3, #4
	adds r2, #142
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0802d194:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r3, [r0]
	cmp r3, #31
	bgt .L_0802d22c
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r0]
	lsls r2, r2, #2
	movs r3, #0
	ldrsb r3, [r6, r3]
	adds r2, r2, r0
	lsls r3, r3, #16
	adds r2, #4
	b .L_0802d21a
.L_0802d1b8:
	ldr r0, .L_0802d234
	ldr r1, .L_0802d238
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_0802d1ec
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r0]
	movs r2, #252
	adds r3, r3, r0
	lsls r2, r2, #6
	adds r3, #4
	adds r2, #206
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0802d1ec:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r3, [r0]
	cmp r3, #31
	bgt .L_0802d22c
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r3, #1
	strh r3, [r0]
	adds r2, r2, r0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #116
	adds r3, r5, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r2, #4
	negs r3, r3
	lsls r3, r3, #16
.L_0802d21a:
	lsrs r3, r3, #16
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #84
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_0802d22c:
	strh r4, [r1]
.L_0802d22e:
	pop {r5, r6, pc}
.L_0802d230:
	.4byte Data_0300122c
.L_0802d234:
	.4byte Data_020038e0
.L_0802d238:
	.4byte 0x04000208
