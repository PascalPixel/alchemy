.syntax unified
	.thumb
	.global Func_08128124
	.thumb_func
Func_08128124:
	push {lr}
	movs r3, #193
	lsls r3, r3, #1
	cmp r0, r3
	bls .L_08128132
	movs r0, #1
	b .L_08128146
.L_08128132:
	ldr r3, .L_08128148
	lsls r2, r0, #3
	adds r2, r2, r3
	ldrb r3, [r2, #2]
	lsls r3, r3, #27
	lsrs r3, r3, #28
	adds r0, r3, #0
	cmp r3, #0
	bne .L_08128146
	movs r0, #1
.L_08128146:
	pop {pc}
.L_08128148:
	.4byte Summon_EntryTable
