.syntax unified
	.thumb
	.global Func_080ae118
	.thumb_func
Func_080ae118:
	push {r5, r6, lr}
	movs r6, #10
.L_080ae11c:
	adds r0, r6, #0
	subs r5, r6, #1
	adds r0, #67
	bl GameFlag_SetBit
	movs r1, #1
	adds r2, r5, #0
	movs r0, #7
	bl Djinn_AddToOwner
	adds r6, #1
	movs r0, #7
	movs r1, #1
	adds r2, r5, #0
	bl Djinn_Activate
	cmp r6, #11
	ble .L_080ae11c
	movs r0, #0
	bl Func_080ae358
	bl Party_CountActiveOwners
	cmp r0, #0
	ble .L_080ae166
	ldr r3, .L_080ae168
	movs r2, #134
	lsls r2, r2, #2
	adds r5, r3, r2
	adds r6, r0, #0
.L_080ae158:
	ldrb r0, [r5]
	subs r6, #1
	adds r5, #1
	bl Owner_RecalculateStats
	cmp r6, #0
	bne .L_080ae158
.L_080ae166:
	pop {r5, r6, pc}
.L_080ae168:
	.4byte gPartyState
