@ Far-call stubs: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall00D
Resource_FarCall00D:
	.global Runtime_BlankDisplayAndRunFar
	.thumb_func
Runtime_BlankDisplayAndRunFar:
	overlay_veneer Runtime_BlankDisplayAndRun
