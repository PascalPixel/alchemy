.syntax unified
	.thumb
	.global Func_080dd754
	.thumb_func
Func_080dd754:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_080dd818
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl ObjectTable_Get
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #165
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	adds r6, r0, #0
	cmp r3, #0
	beq .L_080dd794
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #166
	adds r2, r5, r3
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldrh r1, [r2]
	cmp r3, #0
	beq .L_080dd794
	subs r3, r1, #1
	strh r3, [r2]
.L_080dd794:
	movs r1, #192
	lsls r1, r1, #4
	ldr r0, [r6, #8]
	adds r1, #168
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r0, #0
	bge .L_080dd7ae
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r0, r0, r1
.L_080dd7ae:
	movs r1, #209
	asrs r0, r0, #16
	lsls r1, r1, #8
	subs r0, r3, r0
	adds r1, #5
	ldr r3, .L_080dd81c
	mov lr, r3
	.2byte 0xf800
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #170
	adds r3, r5, r2
	adds r1, r0, #0
	ldr r2, [r6, #16]
	movs r0, #0
	ldrsh r4, [r3, r0]
	ldr r3, [r6, #12]
	subs r0, r2, r3
	cmp r0, #0
	bge .L_080dd7de
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r0, r0, r2
.L_080dd7de:
	asrs r3, r0, #16
	subs r3, r4, r3
	adds r0, r3, #0
	muls r0, r3
	adds r2, r1, #0
	muls r2, r1
	adds r3, r0, #0
	movs r1, #225
	adds r2, r2, r3
	lsls r1, r1, #4
	cmp r2, r1
	bge .L_080dd806
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #166
	adds r3, r5, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_080dd814
.L_080dd806:
	movs r1, #179
	movs r3, #128
	lsls r1, r1, #1
	lsls r3, r3, #6
	adds r2, r5, r1
	adds r3, #144
	strh r3, [r2]
.L_080dd814:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080dd818:
	.4byte gPartyState
.L_080dd81c:
	.4byte IwramMulQ16
