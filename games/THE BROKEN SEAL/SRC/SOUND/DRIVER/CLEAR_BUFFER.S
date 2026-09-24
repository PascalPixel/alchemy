@ Clears 64 bytes at r0 with four stmia of r1-r4 set to zero, keeping the
@ caller's r4 in ip instead of on the stack. The game route treats r4 as
@ scratch and old_agbcc would push {r4, lr}; neither parks a register in ip.
@ It is entry 35 of the sound jump table at 0x080fb7a0. Every TBS and TLA
@ edition has these bytes.
@ The zero halfword of alignment at 0x080f9a2e never runs and carries no
@ credit.
@ credit: library — TBS/TLA common sound-driver mixer and clear-buffer module
.syntax unified
	.thumb
	.global Func_080f9a18
	.thumb_func
Func_080f9a18:
	mov	ip, r4
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	movs	r4, #0
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	stmia	r0!, {r1, r2, r3, r4}
	mov	r4, ip
	bx	lr
AlchemyUncredited_080f9a2e:
	.2byte	0x0000
AlchemyUncreditedEnd_080f9a2e:
