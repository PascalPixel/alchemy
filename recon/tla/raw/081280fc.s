.syntax unified
	.thumb
	.global Func_081280fc
	.thumb_func
Func_081280fc:
	.global Summon_IsEntryFlagged
	.thumb_func
Summon_IsEntryFlagged:
	push {lr}
	movs r3, #193
	lsls r3, r3, #1
	cmp r0, r3
	bls .L_0812810a
	movs r0, #0
	b .L_0812811e
.L_0812810a:
	ldr r3, .L_08128120
	lsls r2, r0, #3
	adds r2, r2, r3
	ldrb r3, [r2, #2]
	movs r1, #0
	lsls r3, r3, #31
	cmp r3, #0
	beq .L_0812811c
	movs r1, #1
.L_0812811c:
	adds r0, r1, #0
.L_0812811e:
	pop {pc}
.L_08128120:
	.4byte Data_08130d0c
