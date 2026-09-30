.syntax unified
	.thumb
	.global Func_0810aea4
	.thumb_func
Func_0810aea4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r5, r1, #0
	bl Owner_GetState
	adds r2, r0, #0
	cmp r5, #0
	bne .L_0810aec4
	ldrh r3, [r2, #52]
	adds r0, r7, #0
	strh r3, [r2, #56]
	bl Owner_RecalculateRatiosFar
	b .L_0810af1e
.L_0810aec4:
	cmp r5, #1
	bne .L_0810aece
	movs r3, #50
	adds r3, #255
	b .L_0810aed6
.L_0810aece:
	cmp r5, #2
	bne .L_0810aede
	movs r3, #160
	lsls r3, r3, #1
.L_0810aed6:
	adds r2, r2, r3
	movs r3, #0
	strb r3, [r2]
	b .L_0810af1e
.L_0810aede:
	cmp r5, #3
	bne .L_0810af1e
	movs r3, #128
	lsls r3, r3, #2
	adds r5, r2, #0
	mov r8, r3
	movs r6, #14
	adds r5, #216
.L_0810aeee:
	ldrh r2, [r5]
	mov r3, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0810af16
	ldrh r0, [r5]
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0810af16
	ldrh r2, [r5]
	mov r3, r8
	eors r3, r2
	strh r3, [r5]
	adds r0, r7, #0
	bl BattleUnit_Recalculate
.L_0810af16:
	subs r6, #1
	adds r5, #2
	cmp r6, #0
	bge .L_0810aeee
.L_0810af1e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
