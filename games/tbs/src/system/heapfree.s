@ C2: Runtime_ReleaseHeapBlock branches on lsrs flags without cmp/cmn;
@ agscc's Thumb cbranchsi4 and negated_cbranchsi4 always emit a compare.
@ C2: Runtime_ReleaseHeapBlock and MemFree share the one trailing literal
@ pool; GCC's per-function arm_reorg emits and clears each function's pool.
@ Admitted under Pascal's delegation, 2026-10-09. The two-byte alignment
@ before MemFree remains linker fill, outside the credited code.
@ Thumb LDR cannot relocate across these sections in the stock assembler;
@ retain the native PC-relative operands for their one shared word.
.syntax unified
	.section .text.release, "ax", %progbits
	.balign 2
	.thumb
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
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
	.global MemFree
	.thumb_func
MemFree:
	ldr	r4, [pc, #8]
	movs	r1, #4
	lsrs	r2, r0, #22
	ands	r2, r1
	str	r0, [r2, r4]
	bx	lr
.L_heap_heads:
	.4byte	gWorkSlot
