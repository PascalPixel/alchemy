.syntax unified
	.thumb
	.global Animation_ApplyChildArgument
	.thumb_func
Animation_ApplyChildArgument:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	movs r3, #127
	adds r6, r0, #0
	movs r4, #128
	ands r4, r7
	ands r7, r3
	ldrb r3, [r6, #24]
	sub sp, #8
	cmp r3, r7
	beq .L_08022b78
	movs r1, #0
	ldrb r2, [r6, #27]
	b .L_08022b72
.L_08022b20:
	lsls r3, r1, #2
	adds r3, #40
	ldr r5, [r6, r3]
	cmp r5, #0
	beq .L_08022b70
	ldr r3, [r5, #12]
	cmp r3, #0
	beq .L_08022b70
	movs r3, #0
	ldrsh r0, [r5, r3]
	str r1, [sp, #4]
	str r4, [sp, #0]
	bl Resource_GetMetadataRecordFar
	ldrb r3, [r0, #5]
	ldr r1, [sp, #4]
	ldr r4, [sp, #0]
	cmp r7, r3
	bge .L_08022b6e
	ldr r2, [r5, #12]
	lsls r3, r7, #2
	ldr r2, [r3, r2]
	ldrb r3, [r0, #4]
	str r2, [r5, #16]
	strb r3, [r5, #4]
	movs r3, #16
	strb r3, [r5, #21]
	cmp r4, #0
	bne .L_08022b5e
	strb r4, [r5, #20]
	strh r4, [r5, #2]
.L_08022b5e:
	cmp r1, #0
	bne .L_08022b6e
	ldrb r3, [r0, #7]
	ldrb r2, [r6, #27]
	strb r3, [r6, #23]
	ldrb r3, [r0, #6]
	strb r3, [r6, #22]
	b .L_08022b70
.L_08022b6e:
	ldrb r2, [r6, #27]
.L_08022b70:
	adds r1, #1
.L_08022b72:
	cmp r1, r2
	blt .L_08022b20
	strb r7, [r6, #24]
.L_08022b78:
	movs r0, #0
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
