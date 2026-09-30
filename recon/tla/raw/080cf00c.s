.syntax unified
	.thumb
	.global Func_080cf00c
	.thumb_func
Func_080cf00c:
	push {r5, r6, r7, lr}
	adds r6, r1, #0
	adds r7, r2, #0
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r5, #0
	beq .L_080cf04e
	movs r2, #1
	negs r2, r2
	cmp r6, r2
	bne .L_080cf02e
	adds r3, r5, #0
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r6, r3, #16
.L_080cf02e:
	cmp r7, r2
	bne .L_080cf03c
	adds r3, r5, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r7, r3, #16
.L_080cf03c:
	str r6, [r5, #8]
	str r7, [r5, #16]
	movs r0, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Map_GetTerrainHeightFar
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_080cf04e:
	pop {r5, r6, r7, pc}
