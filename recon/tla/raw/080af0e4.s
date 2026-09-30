.syntax unified
	.thumb
	.global Func_080af0e4
	.thumb_func
Func_080af0e4:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r7, r0, #0
	bl Owner_GetState
	ldr r3, .L_080af114
	adds r6, r0, #0
	lsls r5, r5, #1
	adds r5, #216
	adds r6, #216
	strh r3, [r0, r5]
	adds r4, r6, #0
	movs r5, #0
	adds r1, r6, #0
	movs r0, #14
.L_080af102:
	ldrh r2, [r4]
	adds r4, #2
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_080af118
	strh r2, [r1]
	adds r5, #1
	adds r1, #2
	b .L_080af118
.L_080af114:
	.4byte 0x00000000
.L_080af118:
	subs r0, #1
	cmp r0, #0
	bge .L_080af102
	cmp r5, #14
	bgt .L_080af13c
	lsls r3, r5, #1
	ldr r2, .L_080af138
	adds r0, r3, r6
	movs r3, #15
	subs r5, r3, r5
.L_080af12c:
	subs r5, #1
	strh r2, [r0]
	adds r0, #2
	cmp r5, #0
	bne .L_080af12c
	b .L_080af13c
.L_080af138:
	.4byte 0x00000000
.L_080af13c:
	adds r0, r7, #0
	bl Owner_RecalculateStats
	movs r0, #2
	pop {r5, r6, r7, pc}
	.2byte 0x0000
