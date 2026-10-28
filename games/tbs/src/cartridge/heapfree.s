@ Why assembly: HeapSlot_Release branches on lsrs flags without cmp/cmn;
@ agscc's Thumb cbranchsi4 and negated_cbranchsi4 always emit a compare.
@ Why assembly: HeapSlot_Release and Heap_Release share the one trailing literal
@ pool; GCC's per-function arm_reorg emits and clears each function's pool.
@ Heap_Release's own six instructions (ldr, movs, lsrs, ands, str, bx lr) are
@ ordinary Thumb with no tell of their own. Its proof is that pool alone: the
@ single word .L_heap_heads after Heap_Release's bx lr is loaded by Heap_Release's
@ ldr r4, [pc, #8] and also by HeapSlot_Release's ldr r4, [pc, #32],
@ which reaches forward over its own bx lr and the whole of Heap_Release; and
@ HeapSlot_Release is the routine with the lsrs r3, r1, #22 / beq
@ above.
@ The two-byte alignment before Heap_Release is linker fill, outside the credited
@ code.
@ Thumb LDR cannot relocate across these sections in the stock assembler;
@ retain the native PC-relative operands for their one shared word.
.syntax unified
	.section .text.release, "ax", %progbits
	.balign 2
	.thumb
	.global HeapSlot_Release
	.thumb_func
HeapSlot_Release:
	ldr	r4, [pc, #32]
	lsls	r0, r0, #2
	ldr	r1, [r0, r4]
	lsrs	r3, r1, #22
	beq.n	.L_release_done
	movs	r2, #0
	str	r2, [r0, r4]
	movs	r0, #4
	ands	r3, r0
	str	r1, [r3, r4]
.L_release_done:
	bx	lr
	.section .text.free, "ax", %progbits
	.balign 4
	.global Heap_Release
	.thumb_func
Heap_Release:
	ldr	r4, [pc, #8]
	movs	r1, #4
	lsrs	r2, r0, #22
	ands	r2, r1
	str	r0, [r2, r4]
	bx	lr
.L_heap_heads:
	.4byte	gHeapSlots

@ Why assembly: Irq_StopOnGamePakRemoved has a reachable exit with no return or
@ interworking branch: when (IE & IF & 0x2000) is zero, its final bne falls
@ straight into Thumb System_Initialize while the CPU is still in ARM mode.
@ Neither a procedure return nor a tail call can have this linkage.
@ Both DE and ES contain exactly these eight ARM instructions, no pool.
@ agbcc's gcc_arm/config/arm/arm.c output_func_epilogue emits bx lr for
@ this frameless interworking exit; output_return_instruction does likewise
@ for an early return. The proof is the missing exit, not ands/bne (which
@ the ARM backend can emit). Preserve the fragment's fallthrough as shipped.
	.ifdef TBS_EDITION_DE
	.set GAMEPAK_STOP, 1
	.endif
	.ifdef TBS_EDITION_ES
	.set GAMEPAK_STOP, 1
	.endif
	.ifdef GAMEPAK_STOP
@ On a cartridge interrupt, stop the sound and halt.
	.section .text.gamepak, "ax", %progbits
	.balign 4
	.arm
	.global Irq_StopOnGamePakRemoved
Irq_StopOnGamePakRemoved:
	mov	ip, #0x04000000
	add	r3, ip, #0x200
	ldr	r2, [r3]
	and	r1, r2, r2, lsr #16
	mov	r2, #0
	ands	r0, r1, #0x2000
	strbne	r0, [r3, #-0x17c]
.L_gamepak_stop:
	bne	.L_gamepak_stop
	.endif
