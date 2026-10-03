.syntax unified
	.thumb
	.global Resource_ClearSlotReferences
	.thumb_func
Resource_ClearSlotReferences:
	push {r5, lr}
	movs r4, #0
	cmp r0, #95
	bhi .L_08014266
	ldr r2, .L_08014270
	movs r1, #128
	movs r5, #255
	lsls r1, r1, #2
.L_08014250:
	ldrb r3, [r2]
	cmp r3, r0
	bne .L_0801425a
	strb r5, [r2]
	adds r4, #1
.L_0801425a:
	subs r1, #1
	adds r2, #1
	cmp r1, #0
	bne .L_08014250
	cmp r4, #0
	beq .L_0801426c
.L_08014266:
	movs r0, #1
	negs r0, r0
	b .L_0801426e
.L_0801426c:
	movs r0, #0
.L_0801426e:
	pop {r5, pc}
.L_08014270:
	.4byte ResourceBlockOwners
