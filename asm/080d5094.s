@ Reconstructed thumb disassembly of a call-graph-reached code region
@ (code interleaved with its data), verified byte-identical by build_asm.
.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_080b5088, 0x080b5088
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d4e4c, 0x080d4e4c
	.set sub_080d4ece, 0x080d4ece
	.set sub_080d4fe0, 0x080d4fe0
	.set sub_080d6888, 0x080d6888
	.set sub_080e155c, 0x080e155c
	.global Region_080d5094
Region_080d5094:
	bl sub_08004458
	movs r3, #7
	ands r0, r3
	adds r0, #24
	mov r5, r10
	str r0, [r7, #24]
	cmp r5, #4
	beq .L_080d50b4
	movs r0, #1
	movs r1, #128
	add r8, r0
	lsls r1, r1, #3
	adds r7, #28
	cmp r8, r1
	bne sub_080d4fe0
.L_080d50b4:
	ldr r2, [pc, #384]
	ldr r5, [sp, #32]
	ldr r7, [sp, #36]
	ldrb r3, [r2, r5]
	cmp r7, r3
	bne .L_080d5108
	ldr r2, [pc, #376]
	movs r3, #2
	add r2, r11
	str r3, [r2, #0]
	ldr r3, [pc, #372]
	mov r1, r11
	ldr r3, [r1, r3]
	ldr r3, [r3, #20]
	movs r0, #0
	mov r8, r0
	cmp r3, #0
	beq .L_080d5108
	ldr r5, [pc, #356]
	movs r6, #36
	add r5, r11
.L_080d50de:
	ldr r3, [r5, #0]
	ldrsh r0, [r3, r6]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #10
	mov r3, r8
	movs r2, #5
	bl sub_080d6888
	ldr r3, [r5, #0]
	movs r1, #1
	ldrsh r0, [r3, r6]
	bl sub_080b5088
	ldr r3, [r5, #0]
	movs r0, #1
	ldr r3, [r3, #20]
	add r8, r0
	adds r6, #2
	cmp r8, r3
	bne .L_080d50de
.L_080d5108:
	ldr r3, [sp, #32]
	ldr r1, [sp, #12]
	ldr r2, [sp, #8]
	adds r3, #1
	str r3, [sp, #32]
	adds r2, #28
	adds r1, #1
	ldr r3, [pc, #296]
	str r2, [sp, #8]
	str r1, [sp, #12]
	add r3, r11
	ldr r3, [r3, #0]
	ldr r2, [pc, #288]
	ldr r3, [r3, #24]
	ldr r5, [sp, #32]
	ldrb r3, [r2, r3]
	cmp r5, r3
	beq .L_080d512e
	b sub_080d4ece
.L_080d512e:
	movs r7, #0
	ldr r6, [pc, #276]
	mov r8, r7
.L_080d5134:
	ldr r5, [r6, #24]
	cmp r5, #0
	ble .L_080d51dc
	subs r3, r5, #1
	ldr r2, [r6, #8]
	str r3, [r6, #24]
	ldr r3, [r6, #0]
	ldr r1, [r6, #16]
	adds r4, r3, r2
	ldr r3, [r6, #4]
	adds r0, r3, r1
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	str r4, [r6, #0]
	str r0, [r6, #4]
	cmp r3, #0
	bge .L_080d515a
	adds r3, #63
.L_080d515a:
	asrs r3, r3, #6
	str r3, [r6, #8]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_080d516a
	adds r3, #63
.L_080d516a:
	asrs r3, r3, #6
	adds r2, r3, #0
	subs r2, #16
	str r2, [r6, #16]
	adds r3, r0, #0
	cmp r0, #0
	bge .L_080d517a
	adds r3, #255
.L_080d517a:
	asrs r3, r3, #8
	mov r12, r3
	cmp r3, #120
	ble .L_080d518e
	negs r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_080d51dc
.L_080d518e:
	cmp r4, #0
	blt .L_080d51dc
	asrs r7, r4, #8
	cmp r7, #126
	bgt .L_080d51dc
	cmp r0, #0
	blt .L_080d51dc
	adds r2, r5, #0
	subs r2, #17
	cmp r2, #0
	bge .L_080d51a6
	adds r2, #7
.L_080d51a6:
	asrs r5, r2, #3
	cmp r5, #0
	bgt .L_080d51ae
	movs r5, #1
.L_080d51ae:
	lsls r4, r5, #1
	ldr r2, [pc, #152]
	subs r3, r4, #2
	mov r1, r8
	movs r0, #1
	ands r0, r1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #28]
	adds r1, r2, r1
	lsrs r2, r5, #31
	adds r2, r5, r2
	asrs r2, r2, #1
	subs r2, r7, r2
	mov r7, r12
	str r5, [sp, #0]
	subs r3, r7, r5
	str r4, [sp, #4]
	ldr r5, [sp, #24]
	lsls r0, r0, #2
	ldr r4, [r0, r5]
	ldr r0, [sp, #40]
	bl sub_080072f4
.L_080d51dc:
	movs r7, #1
	movs r0, #128
	add r8, r7
	lsls r0, r0, #3
	adds r6, #28
	cmp r8, r0
	bne .L_080d5134
	movs r1, #16
	movs r0, #16
	bl sub_080e155c
	bl sub_080cd52c
	ldr r2, [pc, #88]
	movs r3, #1
	add r2, r11
	str r3, [r2, #0]
	movs r0, #1
	bl sub_080030f8
	ldr r1, [sp, #36]
	adds r1, #1
	str r1, [sp, #36]
	cmp r1, #96
	beq .L_080d5210
	b sub_080d4e4c
.L_080d5210:
	movs r0, #47
	bl sub_08002dd8
	movs r0, #46
	bl sub_08002dd8
	ldr r0, [pc, #52]
	bl sub_08004278
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
