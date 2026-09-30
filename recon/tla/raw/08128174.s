.syntax unified
	.thumb
	.global Func_08128174
	.thumb_func
Func_08128174:
	push {lr}
	movs r3, #193
	lsls r3, r3, #1
	cmp r0, r3
	bls .L_08128182
	movs r0, #0
	b .L_0812818e
.L_08128182:
	ldr r3, .L_08128190
	lsls r2, r0, #3
	adds r2, r2, r3
	ldrb r0, [r2, #3]
	lsls r0, r0, #31
	lsrs r0, r0, #31
.L_0812818e:
	pop {pc}
.L_08128190:
	.4byte Summon_EntryTable
