@ Thumb interworking thunks matching the compiler runtime's _call_via_rX
@ helpers: gcc296 calls indirect targets with bl _call_via_rN, and each thunk
@ is a register branch padded to word alignment with a mov r8, r8 nop.
	.thumb

	.macro call_via register
	bx \register
	mov r8, r8
	.endm

	call_via r0
	call_via r1
	call_via r2
	call_via r3
	call_via r4
	call_via r5
	call_via r6
	call_via r7
	call_via r8
	call_via r9
	call_via sl
	call_via fp
	call_via ip
	call_via sp
