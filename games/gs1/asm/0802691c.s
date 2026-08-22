@ 複数領域関数の分岐継続部。
@ 後続する定数兼リテラルプールを同一ファイルに保持する。
.syntax unified
	.thumb
	.set sub_08002322, 0x08002322
	.set sub_080162d4, 0x080162d4
	.set sub_0801965c, 0x0801965c
	.set sub_0801e71c, 0x0801e71c
	.set sub_0801e7c0, 0x0801e7c0
	.set sub_08026b44, 0x08026b44
	.set sub_08026b46, 0x08026b46
	.set sub_08026b4a, 0x08026b4a
	.set sub_08026b8c, 0x08026b8c
	.set sub_08077008, 0x08077008
	.set sub_080b50b8, 0x080b50b8
	.global Continuation_0802691c
Continuation_0802691c:
	ldr r7, [pc, #488]
	adds r3, r6, r7
	ldrb r3, [r3, #0]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r5, #0
	cmp r3, #0
	beq .L_0802692e
	movs r5, #1
.L_0802692e:
	movs r0, #156
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_0802693c
	adds r5, #1
.L_0802693c:
	ldr r1, [pc, #460]
	adds r3, r6, r1
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_08026948
	adds r5, #1
.L_08026948:
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_08026956
	adds r5, #1
.L_08026956:
	ldr r4, [pc, #440]
	adds r3, r6, r4
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_08026962
	adds r5, #1
.L_08026962:
	ldr r7, [pc, #432]
	adds r3, r6, r7
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_0802696e
	adds r5, #1
.L_0802696e:
	movs r0, #160
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_0802697c
	adds r5, #1
.L_0802697c:
	cmp r5, #0
	bne .L_08026982
	movs r5, #1
.L_08026982:
	movs r3, #9
	subs r1, r3, r5
	cmp r1, #3
	bgt .L_0802698c
	movs r1, #4
.L_0802698c:
	mov r2, r11
	ldr r3, [r2, #0]
	cmp r3, #0
	bge .L_08026996
	adds r3, #7
.L_08026996:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #9
	cmp r3, #29
	ble .L_080269a2
	movs r0, #14
.L_080269a2:
	movs r2, #6
	adds r3, r5, #2
	str r2, [sp, #0]
	movs r2, #16
	bl sub_080162d4
	ldr r4, [pc, #344]
	adds r3, r6, r4
	ldrb r3, [r3, #0]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r9, r0
	movs r5, #0
	cmp r3, #0
	beq .L_080269ce
	ldr r0, [pc, #340]
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl sub_0801e7c0
	movs r5, #1
.L_080269ce:
	movs r7, #156
	lsls r7, r7, #1
	adds r3, r6, r7
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_080269e8
	lsls r3, r5, #3
	ldr r0, [pc, #316]
	mov r1, r9
	movs r2, #0
	bl sub_0801e7c0
	adds r5, #1
.L_080269e8:
	ldr r0, [pc, #288]
	adds r3, r6, r0
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_08026a00
	lsls r3, r5, #3
	ldr r0, [pc, #296]
	mov r1, r9
	movs r2, #0
	bl sub_0801e7c0
	adds r5, #1
.L_08026a00:
	movs r1, #158
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_08026a1a
	lsls r3, r5, #3
	ldr r0, [pc, #276]
	mov r1, r9
	movs r2, #0
	bl sub_0801e7c0
	adds r5, #1
.L_08026a1a:
	ldr r2, [pc, #244]
	adds r3, r6, r2
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_08026a32
	lsls r3, r5, #3
	ldr r0, [pc, #256]
	mov r1, r9
	movs r2, #0
	bl sub_0801e7c0
	adds r5, #1
.L_08026a32:
	ldr r4, [pc, #224]
	adds r3, r6, r4
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_08026a4a
	lsls r3, r5, #3
	ldr r0, [pc, #236]
	mov r1, r9
	movs r2, #0
	bl sub_0801e7c0
	adds r5, #1
.L_08026a4a:
	movs r7, #160
	lsls r7, r7, #1
	adds r3, r6, r7
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_08026a64
	lsls r3, r5, #3
	ldr r0, [pc, #212]
	mov r1, r9
	movs r2, #0
	bl sub_0801e7c0
	adds r5, #1
.L_08026a64:
	cmp r5, #0
	beq .L_08026a6a
	b sub_08026b8c
.L_08026a6a:
	movs r0, #2
	bl sub_0801e71c
	ldr r0, [pc, #192]
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl sub_0801e7c0
	movs r0, #15
	bl sub_0801e71c
	b sub_08026b8c
	ldr r0, [sp, #84]
	cmp r0, #255
	bne .L_08026a8c
	b sub_08026b8c
.L_08026a8c:
	ldr r1, [sp, #28]
	ldr r2, [sp, #24]
	ldrh r0, [r1, r2]
	bl sub_08077008
	ldr r3, [sp, #28]
	ldr r4, [sp, #24]
	add r5, sp, #108
	mov r8, r0
	adds r1, r5, #0
	ldrh r0, [r3, r4]
	bl sub_080b50b8
	ldr r3, [pc, #144]
	ldr r0, [r3, #0]
	lsls r0, r0, #12
	bl sub_08002322
	cmp r0, #0
	bge .L_08026ab8
	ldr r6, [pc, #132]
	adds r0, r0, r6
.L_08026ab8:
	ldr r2, [r5, #4]
	asrs r3, r0, #15
	adds r2, r2, r3
	movs r3, #148
	str r2, [r5, #4]
	lsls r3, r3, #1
	add r3, r8
	ldrb r3, [r3, #0]
	cmp r3, #125
	beq .L_08026ad6
	cmp r3, #122
	beq .L_08026ad6
	movs r7, #0
	add r6, sp, #120
	b .L_08026af2
.L_08026ad6:
	movs r3, #148
	lsls r3, r3, #1
	add r3, r8
	ldrb r3, [r3, #0]
	ldr r0, [pc, #96]
	cmp r3, #125
	bne .L_08026ae6
	adds r0, #1
.L_08026ae6:
	add r6, sp, #120
	adds r1, r6, #0
	movs r2, #14
	bl sub_0801965c
	b sub_08026b4a
.L_08026af2:
	cmp r7, #13
	bgt sub_08026b44
	mov r0, r8
	ldrb r3, [r0, r7]
	lsls r2, r7, #1
	strh r3, [r6, r2]
	adds r7, #1
	cmp r3, #0
	bne .L_08026af2
	b sub_08026b46

	.balign 4, 0
	.global LiteralPool_08026b08
LiteralPool_08026b08:
	.4byte 0x00000131
	.4byte 0x0000013b
	.4byte 0x0000013d
	.4byte 0x00000141
	.4byte 0x000008a4
	.4byte 0x000008a5
	.4byte 0x000008a6
	.4byte 0x000008a7
	.4byte 0x000008a8
	.4byte 0x000008a9
	.4byte 0x000008aa
	.4byte 0x000008a3
	.4byte 0x03001e40
	.4byte 0x00007fff
	.4byte 0x0000080e
