.syntax unified
	.thumb
	.global Func_080af244
	.thumb_func
Func_080af244:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	bl Owner_GetState
	lsls r5, r5, #1
	adds r6, r5, #0
	adds r7, r0, #0
	adds r6, #216
	ldrh r3, [r7, r6]
	movs r5, #128
	lsls r5, r5, #1
	adds r5, #255
	ands r5, r3
	adds r0, r5, #0
	bl Item_GetDirect
	movs r1, #0
	cmp r5, #0
	bne .L_080af26e
	movs r0, #8
	b .L_080af294
.L_080af26e:
	ldrb r0, [r0, #3]
	movs r3, #8
	ands r3, r0
	cmp r3, #0
	beq .L_080af27a
	movs r1, #2
.L_080af27a:
	ldrh r2, [r7, r6]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_080af292
	movs r3, #2
	ands r3, r0
	cmp r3, #0
	beq .L_080af292
	movs r3, #1
	orrs r1, r3
.L_080af292:
	adds r0, r1, #0
.L_080af294:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
