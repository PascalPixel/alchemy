.syntax unified
	.thumb
	.global Func_080ca4a8
	.thumb_func
Func_080ca4a8:
	push {r5, r6, r7, lr}
	ldr r7, .L_080ca504
	movs r2, #240
	ldr r0, .L_080ca508
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldmia r0!, {r2}
	cmp r2, #0
	beq .L_080ca4e2
	cmp r2, r1
	beq .L_080ca4e2
	movs r4, #255
	movs r6, #128
	lsls r4, r4, #8
	lsls r6, r6, #24
	adds r4, #255
.L_080ca4cc:
	adds r3, r2, #0
	ands r3, r6
	cmp r3, #0
	beq .L_080ca4d8
	adds r5, r4, #0
	ands r5, r2
.L_080ca4d8:
	ldmia r0!, {r2}
	cmp r2, #0
	beq .L_080ca4e2
	cmp r2, r1
	bne .L_080ca4cc
.L_080ca4e2:
	ldr r3, .L_080ca50c
	cmp r5, r3
	bne .L_080ca4f8
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ca4f6
	ldr r5, .L_080ca510
.L_080ca4f6:
	ldr r7, .L_080ca504
.L_080ca4f8:
	movs r2, #251
	lsls r2, r2, #1
	adds r3, r7, r2
	strh r5, [r3]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ca504:
	.4byte gPartyState
.L_080ca508:
	.4byte Data_080ef984
.L_080ca50c:
	.4byte 0x0000006a
.L_080ca510:
	.4byte 0x00000069
