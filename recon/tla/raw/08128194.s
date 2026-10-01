@ Reads byte4 of one8-byte summon entry, returning0 outside indices0..386.
@ Its complete28-byte extent, table pointer included, matches all six editions.
.syntax unified
	.thumb
	.global Summon_GetEntryByte4
	.thumb_func
Summon_GetEntryByte4:
	push {lr}
	movs r3, #193
	lsls r3, r3, #1
	cmp r0, r3
	bls .L_081281a2
	movs r0, #0
	b .L_081281aa
.L_081281a2:
	ldr r3, .L_081281ac
	lsls r2, r0, #3
	adds r2, #4
	ldrb r0, [r3, r2]
.L_081281aa:
	pop {pc}
.L_081281ac:
	.4byte Summon_EntryTable
