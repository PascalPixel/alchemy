.syntax unified
	.thumb
	.global BattleObject_IsValidId
	.thumb_func
BattleObject_IsValidId:
	push {lr}
	cmp r0, #7
	bhi .L_0811c65a
	movs r0, #0
	b .L_0811c668
.L_0811c65a:
	adds r3, r0, #0
	subs r3, #128
	movs r0, #0
	cmp r3, #5
	bls .L_0811c668
	movs r0, #1
	negs r0, r0
.L_0811c668:
	pop {pc}
	.2byte 0x0000
