.syntax unified
	.thumb
	.global Func_080b11f8
	.thumb_func
Func_080b11f8:
	push {r5, r6, lr}
	ldr r5, .L_080b1274
	movs r0, #206
	ldrh r3, [r5]
	lsls r0, r0, #7
	mov r12, r3
	adds r0, #116
	adds r5, #2
	cmp r12, r0
	bne .L_080b1268
	movs r6, #4
.L_080b120e:
	adds r0, r6, #0
	bl Owner_GetState
	movs r2, #14
	adds r0, #216
.L_080b1218:
	ldrh r3, [r5]
	subs r2, #1
	strh r3, [r0]
	adds r5, #2
	adds r0, #2
	cmp r2, #0
	bge .L_080b1218
	adds r0, r6, #0
	bl Owner_RefreshDerivedData
	adds r0, r6, #0
	adds r6, #1
	bl Owner_RecalculateStats
	cmp r6, #7
	ble .L_080b120e
	ldr r1, .L_080b1278
	ldrh r2, [r5]
	movs r4, #144
	lsls r4, r4, #2
	adds r3, r1, r4
	strh r2, [r3]
	adds r5, #2
	ldrh r2, [r5]
	adds r4, #2
	adds r3, r1, r4
	strh r2, [r3]
	movs r3, #134
	adds r5, #2
	lsls r3, r3, #2
	adds r2, r1, r3
	ldrh r3, [r5]
	subs r4, #40
	strh r3, [r2]
	movs r0, #0
	ldrh r3, [r5, #2]
	ldr r5, .L_080b1274
	adds r2, r1, r4
	strh r3, [r2]
	strh r0, [r5]
.L_080b1268:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_ClearBit
	pop {r5, r6, pc}
.L_080b1274:
	.4byte Data_020023c4
.L_080b1278:
	.4byte gPartyState
