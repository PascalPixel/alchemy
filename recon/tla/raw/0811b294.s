.syntax unified
	.thumb
	.global Summon_FindSlot
	.thumb_func
Summon_FindSlot:
	push {r5, r6, lr}
	movs r5, #0
	b .L_0811b29c
.L_0811b29a:
	adds r5, #1
.L_0811b29c:
	cmp r5, #5
	bgt .L_0811b2b6
	adds r6, r5, #0
	adds r6, #128
	adds r0, r6, #0
	bl Owner_GetState
	movs r2, #149
	lsls r2, r2, #1
	adds r3, r0, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0811b29a
.L_0811b2b6:
	cmp r5, #6
	bne .L_0811b2c0
	movs r0, #1
	negs r0, r0
	b .L_0811b2c2
.L_0811b2c0:
	adds r0, r6, #0
.L_0811b2c2:
	pop {r5, r6, pc}
