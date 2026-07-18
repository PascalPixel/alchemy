@ 複数領域関数の終端継続部。
@ 後続するリテラルプールを同一ファイルに保持する。
.syntax unified
	.thumb
	.set sub_08002304, 0x08002304
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080ddeb8, 0x080ddeb8
	.set sub_080ddf1c, 0x080ddf1c
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080ed408, 0x080ed408
	.global Continuation_080de0d4
Continuation_080de0d4:
	movs r3, #2
	add r3, r8
	mov r9, r3
	ldr r4, [sp, #40]
	cmp r4, r9
	blt .L_080de1ca
	mov r3, r8
	adds r3, #24
	cmp r4, r3
	bge .L_080de1ca
	ldr r1, [sp, #20]
	ldr r3, [r1, #0]
	ldr r3, [r3, #24]
	ldr r2, [pc, #468]
	lsls r3, r3, #2
	adds r3, #1
	ldrb r3, [r2, r3]
	movs r0, #0
	mov r10, r0
	cmp r3, #0
	beq .L_080de1ca
	ldr r7, [pc, #456]
	movs r3, #3
	mov r9, r3
	add r7, r11
.L_080de106:
	mov r4, r10
	mov r0, r9
	ands r4, r0
	str r4, [sp, #8]
	bl sub_08004458
	ldr r3, [r7, #0]
	ldr r3, [r3, #24]
	ldr r1, [pc, #428]
	lsls r3, r3, #2
	adds r3, #2
	ldrb r5, [r1, r3]
	adds r1, r5, #0
	bl sub_08002304
	ldr r2, [sp, #24]
	ldr r2, [r2, #4]
	mov r8, r2
	mov r3, r8
	ldr r4, [sp, #8]
	subs r3, r3, r0
	subs r5, r5, r0
	ldr r0, [pc, #408]
	mov r8, r3
	ldrb r3, [r0, r4]
	mov r1, r8
	lsrs r3, r3, #1
	subs r1, r1, r3
	movs r2, #8
	mov r8, r1
	add r8, r2
	bl sub_08004458
	adds r5, #1
	ldr r3, [sp, #24]
	adds r1, r5, #0
	ldr r6, [r3, #0]
	bl sub_08002304
	ldr r4, [sp, #8]
	adds r6, r6, r0
	ldr r0, [pc, #372]
	lsrs r3, r5, #31
	adds r5, r5, r3
	ldrb r3, [r0, r4]
	asrs r5, r5, #1
	lsrs r3, r3, #1
	subs r6, r6, r5
	subs r6, r6, r3
	bl sub_08004458
	ldr r3, [pc, #356]
	mov r1, r9
	ands r0, r1
	ldrb r2, [r3, r0]
	mov r3, r9
	orrs r3, r2
	ldr r2, [r7, #0]
	ldr r1, [pc, #348]
	ldr r2, [r2, #24]
	ldrb r2, [r1, r2]
	movs r0, #47
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	bl sub_080ed408
	ldr r4, [sp, #8]
	ldr r2, [pc, #332]
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r2, [pc, #312]
	ldrb r3, [r2, r4]
	ldr r0, [pc, #304]
	str r3, [sp, #0]
	ldrb r3, [r0, r4]
	ldr r2, [pc, #320]
	str r3, [sp, #4]
	add r1, r11
	ldr r4, [r2, #0]
	mov r3, r8
	ldr r0, [sp, #48]
	adds r2, r6, #0
	bl sub_080072f4
	movs r0, #47
	bl sub_08002dd8
	movs r3, #1
	add r10, r3
	ldr r3, [r7, #0]
	ldr r3, [r3, #24]
	ldr r4, [pc, #260]
	lsls r3, r3, #2
	adds r3, #1
	ldrb r3, [r4, r3]
	cmp r10, r3
	bne .L_080de106
.L_080de1ca:
	ldr r0, [sp, #16]
	ldr r1, [sp, #12]
	ldr r3, [sp, #44]
	movs r2, #224
	lsls r2, r2, #4
	adds r0, #2
	adds r1, r1, r2
	adds r3, #1
	str r0, [sp, #16]
	str r3, [sp, #44]
	str r1, [sp, #12]
	ldr r2, [pc, #228]
	mov r4, r11
	ldr r3, [r4, r2]
	ldr r0, [sp, #44]
	ldr r3, [r3, #20]
	cmp r0, r3
	beq .L_080de1f0
	b sub_080ddf1c
.L_080de1f0:
	movs r1, #0
	ldr r6, [pc, #240]
	mov r10, r1
.L_080de1f6:
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_080de262
	subs r3, #1
	movs r2, #128
	str r3, [r6, #24]
	lsls r2, r2, #5
	adds r0, r6, #0
	movs r1, #60
	bl sub_080e3908
	movs r2, #208
	ldr r3, [r6, #4]
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_080de224
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_080de262
.L_080de224:
	ldr r2, [r6, #0]
	ldr r4, [pc, #192]
	cmp r2, r4
	bhi .L_080de262
	cmp r3, #0
	blt .L_080de262
	ldr r4, [r6, #24]
	cmp r4, #0
	bge .L_080de238
	adds r4, #15
.L_080de238:
	asrs r4, r4, #4
	adds r4, #1
	lsls r5, r4, #1
	ldr r0, [pc, #172]
	subs r1, r5, #2
	ldrh r1, [r0, r1]
	ldr r0, [sp, #28]
	adds r1, r0, r1
	lsrs r0, r4, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	asrs r2, r2, #16
	asrs r3, r3, #16
	subs r2, r2, r0
	subs r3, r3, r4
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #32]
	bl sub_080072f4
.L_080de262:
	movs r0, #1
	movs r1, #128
	add r10, r0
	lsls r1, r1, #3
	adds r6, #28
	cmp r10, r1
	bne .L_080de1f6
	movs r0, #2
	movs r1, #8
	bl sub_080e155c
	bl sub_080cd52c
	ldr r2, [pc, #112]
	movs r3, #1
	add r2, r11
	str r3, [r2, #0]
	movs r0, #1
	bl sub_080030f8
	ldr r2, [sp, #40]
	adds r2, #1
	str r2, [sp, #40]
	ldr r4, [sp, #20]
	ldr r3, [r4, #0]
	ldr r3, [r3, #20]
	lsls r3, r3, #3
	adds r3, #40
	cmp r2, r3
	beq .L_080de2a0
	b sub_080ddeb8
.L_080de2a0:
	ldr r0, [pc, #80]
	bl sub_08004278
	movs r0, #46
	bl sub_08002dd8
	bl sub_080cdbc0
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0

	.balign 4, 0
	.global LiteralPool_080de2c4
LiteralPool_080de2c4:
	.4byte 0x080eebd6
	.4byte 0x00007828
	.4byte 0x080eded0
	.4byte 0x080edeca
	.4byte 0x080eebe2
	.4byte 0x080eebe6
	.4byte 0x080edebe
	.4byte 0x03001f0c
	.4byte 0x02010000
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x080cd261
