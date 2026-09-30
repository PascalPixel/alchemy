.syntax unified
	.thumb
	.global Func_080c977c
	.thumb_func
Func_080c977c:
	push {r5, r6, lr}
	ldr r1, .L_080c991c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #86
	movs r4, #1
	adds r3, r1, r2
	negs r4, r4
	strh r0, [r3]
	cmp r0, r4
	beq .L_080c9794
	b .L_080c98a2
.L_080c9794:
	movs r4, #133
	lsls r4, r4, #2
	adds r3, r1, r4
	ldr r0, [r3]
	bl Owner_GetState
	adds r6, r0, #0
	movs r5, #56
	ldrsh r3, [r6, r5]
	cmp r3, #0
	bne .L_080c9810
	movs r5, #1
	strh r5, [r6, #56]
	lsls r5, r5, #14
	movs r0, #52
	ldrsh r1, [r6, r0]
	adds r0, r5, #0
	bl __divsi3
	movs r1, #128
	lsls r1, r1, #7
	cmp r0, r1
	bgt .L_080c97ca
	movs r5, #0
	cmp r0, #0
	blt .L_080c97ca
	adds r5, r0, #0
.L_080c97ca:
	lsls r3, r5, #16
	strh r5, [r6, #20]
	cmp r3, #0
	bne .L_080c97de
	movs r2, #56
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_080c97de
	movs r3, #1
	strh r3, [r6, #20]
.L_080c97de:
	movs r3, #58
	ldrsh r0, [r6, r3]
	movs r4, #54
	ldrsh r1, [r6, r4]
	lsls r0, r0, #14
	bl __divsi3
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_080c97fc
	movs r3, #0
	cmp r0, #0
	blt .L_080c97fc
	adds r3, r0, #0
.L_080c97fc:
	strh r3, [r6, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_080c9810
	movs r5, #58
	ldrsh r3, [r6, r5]
	cmp r3, #0
	beq .L_080c9810
	movs r3, #1
	strh r3, [r6, #22]
.L_080c9810:
	ldr r1, .L_080c991c
	movs r0, #249
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r5, #250
	lsls r5, r5, #1
	movs r4, #0
	ldrsh r2, [r3, r4]
	adds r3, r1, r5
	movs r5, #1
	negs r5, r5
	movs r4, #0
	ldrsh r0, [r3, r4]
	cmp r2, r5
	bne .L_080c983a
	cmp r0, r2
	beq .L_080c9868
	movs r5, #244
	lsls r5, r5, #1
	adds r3, r1, r5
	ldrh r2, [r3]
.L_080c983a:
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r1, r4
	strh r2, [r3]
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	beq .L_080c9856
	ldr r3, .L_080c991c
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	strh r0, [r3]
	b .L_080c9886
.L_080c9856:
	ldr r2, .L_080c991c
	movs r4, #245
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrh r3, [r3]
	movs r5, #241
	lsls r5, r5, #1
	adds r2, r2, r5
	b .L_080c9884
.L_080c9868:
	movs r0, #242
	lsls r0, r0, #1
	adds r3, r1, r0
	ldrh r2, [r3]
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r1, r4
	strh r2, [r3]
	movs r5, #243
	lsls r5, r5, #1
	adds r3, r1, r5
	ldrh r3, [r3]
	subs r0, #2
	adds r2, r1, r0
.L_080c9884:
	strh r3, [r2]
.L_080c9886:
	ldr r2, .L_080c991c
	movs r1, #240
	lsls r1, r1, #1
	movs r5, #241
	adds r3, r2, r1
	lsls r5, r5, #1
	movs r4, #0
	ldrsh r0, [r3, r4]
	adds r3, r2, r5
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_080ca3f4
	b .L_080c9918
.L_080c98a2:
	movs r5, #247
	lsls r5, r5, #1
	adds r3, r1, r5
	movs r0, #0
	ldrsh r2, [r3, r0]
	adds r5, #2
	adds r3, r1, r5
	movs r5, #0
	ldrsh r0, [r3, r5]
	cmp r2, r4
	bne .L_080c98c4
	cmp r0, r2
	beq .L_080c98f4
	movs r5, #244
	lsls r5, r5, #1
	adds r3, r1, r5
	ldrh r2, [r3]
.L_080c98c4:
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r1, r4
	strh r2, [r3]
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	beq .L_080c98e0
	ldr r3, .L_080c991c
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	strh r0, [r3]
	b .L_080c9918
.L_080c98e0:
	ldr r2, .L_080c991c
	movs r4, #245
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrh r3, [r3]
	movs r5, #241
	lsls r5, r5, #1
	adds r2, r2, r5
	strh r3, [r2]
	b .L_080c9918
.L_080c98f4:
	movs r0, #244
	lsls r0, r0, #1
	adds r3, r1, r0
	ldrh r2, [r3]
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r1, r4
	strh r2, [r3]
	movs r5, #245
	lsls r5, r5, #1
	adds r3, r1, r5
	ldrh r3, [r3]
	subs r0, #6
	adds r2, r1, r0
	strh r3, [r2]
	subs r0, #217
	bl GameFlag_SetBit
.L_080c9918:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080c991c:
	.4byte gPartyState
