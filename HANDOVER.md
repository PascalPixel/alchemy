# Alchemy handover

Updated: 2026-07-29 (semantic-C speed lane)

This is the single authoritative session handover. Do not add dated handoff
files; update this file in place.

## Current objective

Eliminate ordinary assembly through a semantic-C speed lane, while preserving
the existing byte-exact build and its stricter metric.

Semantic-C sources live under `semantic/` and compile with
`bun run build:semantic`. They express reviewed behavior and target ABI but do
not claim original machine-code equality. Exact sources remain under
`assets/code/` and continue to require fully linked byte equality.

## Repository state

- Branch: `speed`
- The live Full-C metric is printed by `bun tools/full_c_progress.ts --subject`
  and recorded in each commit subject; regenerate the inventory/report before
  reading it.
- The GS1-English full build is byte-identical with zero ROM fallback.
- The source-only build owns all 8 MiB with zero unowned bytes.
- Semantic-C lane: **219,650 executable bytes across 483 compiling sources**:
  206,858 main bytes and 12,792 overlay bytes. Combined with exact C,
  **367,264 / 1,339,230 executable bytes** are now expressed as C, with
  971,966 remaining.
- The lane includes every still-live source from the curated near-match,
  hand-reviewed, prior, and manual candidate queues. Admission rejects
  exact duplicates, dead/nonordinary owners, overlaps, inline assembly,
  unresolved decompiler constructs, and target-compiler failures.
- Honest outer-owner additions include `resource_381:0054/1410`,
  `resource_394:03f0/0c2c/0e64..0fb4`, `resource_3bd:0474/0608/0ee0`, and
  `resource_3c8:1d48`. `resource_379:00dc` was rejected as a fake standalone
  owner because it is a shared entry using `resource_379:0074`'s
  saved-register frame; the exclusion is documented under `semantic/`.
- The 784-byte `resource_37a:2614` owner reconstructs the complete opening
  scene initializer: display and runtime setup, camera/effect tracks, object
  visibility, coordinate handoff, palette reset, and final shared-state
  installation. Its 69 calls across 61 resident service targets match the
  reference multiset exactly, including three mode-dependent services whose
  argument counts legitimately vary between calls.
- The 748-byte `0808e680` owner reconstructs packed action resolution:
  message/error paths, actor-resource costs, three resource selections,
  presentation setup, conditional effect sequences, and the special action
  handoff. All 49 ordinary call sites match the reference multiset, and its
  packed input, signed sentinel, and resource-record fields have explicit
  types.
- The 720-byte `080ab314` owner reconstructs the modal seven-page selection
  screen: window setup, wrapped navigation, confirmation/rejection, preview
  object replacement, both exit paths, and teardown. Its nested input/redraw
  loops, object-slot cleanup, and all 49 reference call sites are preserved
  without hidden-register or decompiler constructs.
- The 740-byte `080c2724` owner reconstructs battle-reward distribution:
  repeated multi-level experience gains, newly learned abilities, six stat
  gain reports, currency, and priority-ordered rare items with a full-inventory
  fallback. Its stack arrays and level-up result are typed explicitly, and all
  55 reference call sites remain distinct.
- The 732-byte `08096140` owner reconstructs a scripted object-effect sequence:
  target animation, two 24-slot effect passes, a radial spawned-object wave,
  and an eight-object finishing burst. Three calls whose arguments were lost
  by raw decompilation are repaired, byte-stride slot traversal is explicit,
  and all 52 reference call sites remain distinct.
- The 712-byte `080a4f08` owner reconstructs a bounded selection UI: two
  optional character totals, DMA-backed scratch rendering, wrapped navigation,
  acceptance/cancellation, and teardown. Its DMA writes are volatile, the raw
  stale fourth veneer argument is removed, and all 37 reference call sites
  remain distinct.
- The 708-byte `080ba2c0` owner reconstructs two-object battle-action setup:
  facing, battle-work generation, hardware windows, the scripted path, an
  optional interpolated effect, both action handlers, and cleanup. It reuses
  the adjacent exact source's work/slot layouts, repairs a missing secondary-ID
  argument, and preserves all 49 reference call sites.
- The 688-byte `080a35f8` owner reconstructs the interactive party-character
  selector: window/object setup, selected-character refresh, alternate-page
  mode, wrapped movement, validation, cancellation, and state writeback. Its
  actual one-argument ABI and missing validation argument are repaired, with
  all 30 reference call sites preserved.
- The 688-byte `080a60d4` owner reconstructs the character action selector:
  window/object setup, action refresh, wrapped navigation, ordinary and
  alternate validation, cancellation, and result writeback. Its input is
  correctly typed as a single halfword table, and all 29 reference call sites
  are preserved.
- The 664-byte `080b2b10` owner reconstructs the candidate purchase/application
  menu: initial filtering, wrapped navigation, validation, price and capacity
  failures, successful application, resource deduction, refresh, and cleanup.
  Its signed candidate table/count/category accesses are explicit, and all 40
  reference call sites are preserved.
- The 620-byte `080ba6ac` owner reconstructs battle-action execution followed
  by used-item reconciliation: work/child snapshots, the palette/action branch,
  actor cleanup, queued-slot removal and shifting, item breakage, and item
  transformation. It reuses the established battle-work layout and preserves
  all 26 reference call sites.
- The 612-byte `0808a8e4` owner reconstructs the persistent map-transition
  dispatcher: entry-mode initialization, flag and display teardown, special
  selection screens with palette preservation, map flag setup, and the next
  resource request. Its hardware accesses have explicit widths and all 34
  reference call sites are preserved.
- The 824-byte `080a38d0` owner reconstructs the interactive action-selection
  window: wrapped navigation, cursor and detail redraws, comparison and subject
  modes, confirmation rejection, cancellation, and final state writeback. Its
  action table and menu fields have explicit widths, its return preserves the
  reference signed-byte convention, and all 34 call sites are preserved.
- The 832-byte `08098cd8` owner reconstructs a scripted arrival sequence:
  twelve staged trail emitters, a growing central object, three linked child
  objects, group redirection, a bounded completion wait, optional retargeting
  of an existing object, event handoff, and cleanup. Its scene, object, vector,
  trail, and effect-node layouts are guarded, and all 43 call sites are
  preserved.
- The 870-byte `080a8114` owner reconstructs the two-pane character selector:
  availability rebuilding, wrapped navigation, content and alternate panes,
  cursor placement, confirmation and cancellation, shoulder-button character
  rotation, and teardown. Its character table, availability buffer, signed
  indices, and window handles are explicit, and all 46 call sites are
  preserved.
- The 832-byte `080a414c` owner reconstructs the six-panel action grid:
  availability discovery, saved-position restoration, wrapped two-dimensional
  navigation, mode-specific detail redraws, rejection of unavailable panels,
  selection-specific feedback, cancellation, and forced exit. Its sparse menu
  state and signed availability values are guarded, and all 47 call sites are
  preserved.
- The 980-byte `0801f200` owner reconstructs the variable roster/value display:
  feature and compact roster sources, display-mode normalization and restore,
  per-entry primary and comparison values, scaled indicators, and the optional
  four-digit footer. Its six-ID local bound and signed record fields replace
  the raw fictional stack object, and all 31 call sites are preserved.
- The 954-byte `080b9ec0` owner reconstructs a battle-side transition:
  participant staging, opposing-side movement, fade-out, message or filtered
  action dispatch, battle-work child snapshots, restoration, and fade-in. Its
  action input, 84-byte work area, child matrix, and reused 14-entry actor list
  replace the raw overlapping stack scalars, and all 31 call sites are
  preserved.
- The 612-byte `080ba978` owner reconstructs camera orientation and a complete
  battle action: angle interpolation and flag overrides, transition selection,
  participant child snapshots, message or action dispatch, and actor cleanup.
  Its action input and 84-byte battle-work area replace the raw byte-sized
  stack fiction, and all 25 call sites are preserved.
- The 528-byte `080b1260` owner reconstructs the equipment comparison panel:
  bounded slot selection, temporary equip and restoration, before/after stat
  snapshots, three comparison rows, direction indicators, and the displaced
  item marker. Its two four-value stack arrays and actor inventory layout
  replace raw byte scalars, and all 15 call sites are preserved.
- The 572-byte `080965a8` owner reconstructs a scripted clone attack: eight
  linked afterimages, vertical actor removal, effect-node release, a 24-record
  trailing emission, delayed activation, and teardown. Its clone pointer array,
  actor/component layouts, vectors, and 72-byte effect records are explicit,
  and all 42 call sites are preserved.
- The 524-byte `080a8d34` owner reconstructs the five-entry inventory-row
  renderer: selected name and status, per-entry metadata, numbered glyphs,
  selected/unselected palettes, and tile-region highlights. Its true
  three-argument ABI and every six-argument drawing call are preserved, along
  with all 17 static call sites.
- The 512-byte `080a63e4` owner reconstructs the modal party-member selector:
  signed wrapped navigation, cursor/OAM updates, optional action and preview
  panes, independent direction inputs, confirmation, cancellation, and gated
  exit. Its menu fields and both full/truncated selected-ID writes are explicit,
  and all 19 static call sites are preserved.
- The 500-byte `080a93a4` owner reconstructs the five-entry equipment-row
  renderer and its four-row category marker. Its true three-argument ABI,
  selected-item metadata, and six-argument drawing calls are explicit, and all
  15 static call sites are preserved.
- The 488-byte `080b3444` owner reconstructs a cyclic value-selection screen:
  window and focus-object setup, wrapped navigation, validation, mapped-value
  selection with retry, cancellation, and teardown. Its two-output ABI and
  signed option table are explicit, and all 23 static call sites are preserved.
- The 484-byte `080b24e4` owner reconstructs a fixed-item recipient
  transaction: affordability gating, wrapped actor selection, rejection
  feedback, successful transfer and resource deduction, cancellation, and
  item-state restoration. Its signed actor and amount fields are explicit, and
  all 26 static call sites are preserved.
- The 428-byte `080b5534` owner reconstructs a non-returning controller/debug
  display: actor-name setup, panel selection, independent controller-driven
  value changes, bounded resource selection, and repeated redraw. Its decoded
  halfword buffer and volatile display/input state are explicit, and all 16
  static call sites are preserved.
- The 428-byte `080b7738` owner reconstructs battlefield effect
  synchronization and opposing-side display priorities. Its 14-ID buffer,
  actor/slot/effect/component layouts, signed orientation test, and four-entry
  component traversal are explicit, and all eight static call sites are
  preserved.
- The 464-byte `080b1614` owner reconstructs a modal bounded quantity picker:
  resource allocation, window/sprite setup, tile-buffer redraw, wrapped
  navigation, total-value display, confirmation, cancellation, and cleanup.
  Its DMA writes and sprite attribute are explicit, and all 23 static call
  sites are preserved.
- The 412-byte `080b8c1c` owner reconstructs one-member battle-work setup:
  transition orientation, side population, child-attribute snapshots, four
  preparation phases, execution, and participant release. Its 84-byte work
  layout and signed/unsigned side tests are explicit, and all 19 static call
  sites are preserved.
- The 412-byte `0801d9d4` owner reconstructs a variable-row localized window:
  event-dependent geometry and labels, row separators, font-tile upload,
  rendering-object setup, and signed table-driven row objects. Its window and
  runtime pointer fields are explicit, and all 15 static call sites are
  preserved.
- The 482-byte `080a7850` owner reconstructs a modal four-by-seven reference
  grid: unlocked-entry discovery, window geometry, localized labels and
  markers, completion/input waiting, and object/geometry restoration. Its
  four-row state arrays and volatile input flag are explicit, and all 16 static
  call sites are preserved.
- The 444-byte `080b1a14` owner reconstructs a modal actor-list dispatcher:
  three-window setup, wrapped actor navigation, validation, variant-specific
  dispatch, menu restoration, cancellation, and ordered teardown. Its signed
  actor/count/variant fields are explicit, and all 24 static call sites are
  preserved.
- The 440-byte `0801db70` owner reconstructs the interactive companion to the
  variable-row localized window: row highlighting, wrapped selection, cursor
  positioning, acceptance, cancellation, cleanup, and event-relative result
  mapping. Its signed result, row objects, and persistent selection are
  explicit, and all 19 static call sites are preserved.
- The 452-byte `08091a58` owner reconstructs item-selection and transfer
  bookkeeping: current-owner reporting, retry/cancel confirmation, selected
  actor/slot mutation, post-transfer reporting, and state restoration. Its
  output locals and item/state fields are explicit, and all 32 static call
  sites are preserved.
- The 412-byte `080b1f4c` owner reconstructs validated inventory-stack
  consumption and credit: special single-item mode, context-sensitive prompts,
  confirmation, repeated removal, total credit, menu refresh, and completion
  feedback. Its true ordinary three-argument ABI is proven by both callers,
  and all 18 static call sites are preserved.
- The 404-byte `0802977c` owner reconstructs a modal localization-glyph
  browser: two sentinel-terminated record tables, independent one/ten-step
  navigation, wrapped selection, glyph/detail redraw, and ordered window
  cleanup. Its signed halfword records and six-argument glyph helper are
  explicit, and all 11 static call sites are preserved.
- The 444-byte `0802106c` owner reconstructs a three-option modal menu:
  localized labels, tile and cursor setup, palette/DMA initialization,
  selection animation, wrapped navigation, confirmation, cancellation, and
  cleanup. Its window geometry and six-argument animation calls are explicit,
  and all 20 static call sites are preserved.
- The 476-byte `080a7d68` owner reconstructs a party-member preview selector:
  wrapped actor navigation, preview-variant cycling, marker geometry, inline
  preview-work rendering, confirmation/cancellation, and result writeback.
  Its signed selection/count and local-only preview variant are explicit, and
  all 20 static call sites are preserved.
- The 460-byte `080b0278` owner reconstructs a modal menu-state controller:
  selection normalization, actor-resource and display-object setup, feedback
  state dispatch, shared refresh behavior, fallback window handling, and
  teardown. Its true two-argument ABI and nested resource/state layouts are
  explicit, and all 26 static call sites are preserved.
- The 484-byte `080f2b70` owner reconstructs a timed display transition:
  palette/background and DMA setup, a 20-by-32 sequential tilemap, affine-state
  initialization, one- or three-stage timing, and input interruption. Its
  volatile MMIO widths and the palette initializer's true zero-argument ABI
  are explicit, and all 20 static call sites are preserved.
- The 356-byte `080f2d54` owner reconstructs a decompressed animated display:
  palette/tilemap/static-tile DMA, scroll-state initialization, fade setup,
  frame-counter-selected dynamic tiles, input polling, and timeout. Its
  resource offsets and volatile DMA widths are explicit, and all 11 static call
  sites are preserved.
- The 204-byte `080b1470` owner reconstructs a three-by-five inventory
  renderer: selected-item quantity/message handling, first-zero slot
  termination, bounded icon placement, and returned-object styling. Its
  15-slot actor inventory and full item-ID ABI are explicit, and all seven
  static call sites are preserved.
- The 276-byte `080f91e8` owner reconstructs two stepped audio/state ramps:
  countdown-triggered targeting, signed halfword advancement, original-delta
  overshoot clamping, and parameter updates. Its signed current/target values
  and unsigned step widths are explicit, and all four static call sites are
  preserved.
- Three larger candidates are deliberately parked rather than admitted with
  fake ordinary calls. `080d1714` is a 3,384-byte, 400-frame cinematic whose
  raw output splits an internal loop entry and hides ten runtime callback
  targets in `r4`/`r5`/`r7`; `0800aa0c` similarly has polymorphic callback
  thunks plus mangled stack sort arrays; `080ed104` needs renderer targets live
  in `ip` and `r4`; `080ce85c` alternates two renderer targets through `r4`;
  `080dc6bc` and `080d59b0` similarly select particle renderers through `r4`;
  and `080ceff8` combines three `r6` copy callbacks with two `r4` renderers.
  `080ccc38` and `080d9fc8` also require live `r4` renderer targets, while
  `080c1798` has two demonstrated live-`r2` residue dependencies across calls.
  `080d82b0` is behaviorally recovered but its particle blit is a genuine
  six-argument callback routed through live `r4`; it remains parked until that
  callback can be represented without pretending the call-via veneer is an
  ordinary function. `080109e8` likewise needs four calls routed through live
  `r2`/`r3`/`r4`, and `080e0564` needs two live-`r4` drawing callbacks plus a
  live-`r7` callback; both are behaviorally audited but remain outside the
  ordinary-ABI speed lane. `080ca1fc` and `080d85d0` each require a
  six-argument renderer target retained in live `r4`; `080ce4e8` has three
  polymorphic live-`r5` calls using two signatures. Their particle/cinematic
  behavior is audited, but their raw ordinary veneer calls are not admissible.
  `080d0ee0` is actually a 1,136-byte owner spanning its head, a mid-function
  literal pool, and the live `080d12a8` continuation, with an `r4` renderer.
  `080ddde0` similarly owns 1,304 bytes through `080de2f8`, including the live
  `080de0d4` continuation and four hidden renderer calls. Neither split head is
  independently admissible. `080d5c48` is well bounded but both of its
  six-argument drawing calls require the target in live `r7`, so its raw
  ordinary `08007300` calls remain inadmissible.
  Their boundaries, behavior, and callback debt have been audited for a later
  typed-function-pointer/backend pass.
- The latest main-ROM cohort adds 20,372 reviewed bytes. The largest repaired
  complete owners are `080bbb0c` (6,332 bytes), `080ea0d8` (5,756 bytes), and
  `080ab5e4` (4,888 bytes); ten smaller raw m2c owners passed the same admission
  gates without repair. Rejected mechanical copies were removed from
  `semantic/`, while their originals remain in `work/m2c-fresh-20260722/` as
  the next repair queue.
- The following speed cohort adds another 14,724 reviewed main-ROM bytes across
  16 owners. Each owner compiles and assembles independently, has no unresolved
  decompiler constructs or inline assembly, and preserves the reference's
  external-call multiset. The repaired sources correct raw m2c pointer scaling,
  access widths, stack aliases, and internal-entry control flow before
  admission.
- The next speed cohort adds 12,054 reviewed main-ROM bytes across 20 owners,
  led by the complete 4,224-byte `08027114` owner. It also replaces several
  decompiler artifacts with explicit local arrays or pointer parameters where
  the reference passes stack-backed data through ordinary ABI registers.
- The latest speed cohort adds 6,822 reviewed main-ROM bytes across 10 owners,
  led by the complete 3,804-byte `080f6440` owner and its 57-call audited
  control flow. The cohort also recovers save-slot replacement, M4A MEMACC,
  scene-transition, grid-input, tile-fade, and object-spawn behavior with
  explicit layouts and corrected hidden arguments.
- The following speed cohort adds 6,016 reviewed main-ROM bytes across nine
  owners, led by the complete 3,656-byte `080dea70` owner and its 101-call
  audited control flow. It restores internal frame-loop continuations, stacked
  call arguments, projection locals, record strides, and signed IDs, alongside
  serial-link buffering, UI borders, resource selection, and object searches.
- The latest speed cohort adds 6,682 reviewed main-ROM bytes across 13 owners,
  led by the complete 3,640-byte `080e7404` owner. It restores both of that
  owner's large frame-processing stages and overlapping stack workspace, plus
  battle-selection, portrait, scene-transition, tile-probe, palette-fade, and
  resource-allocation behavior. Every owner passes target compilation and the
  aggregate semantic admission gates; external-call multisets were audited
  against the reference owners.
- The next speed cohort adds 5,138 reviewed main-ROM bytes across ten owners.
  Its largest additions are the 1,168-byte effect-description renderer,
  864-byte two-choice UI, and 768-byte bordered-window compositor and combat
  status panel. The cohort also restores allocation-slot, active-entry,
  terrain-height, menu-grid, preset-value, and map-rule behavior. All ten
  owners compile and assemble independently and match their reference
  external-call multisets.
- The following 2,128-byte cohort adds four main-ROM owners: two related
  combat-effect selection menus, the text-layout and inter-word-spacing
  calculator, and the 32-cell resource debug viewer. Their shared menu
  navigation was expressed with explicit state layouts while preserving all
  reference call sites, including branch-specific sound and dialog paths.
- The latest 2,760-byte cohort adds the complete status-comparison overview
  and paginated effect list (`08022b44`) plus the interactive 15-entry
  selection editor (`08020bd8`). Both owners compile and assemble
  independently and exactly preserve their reference external-call
  multisets; the editor also restores its grid navigation, animated list
  cursor, add/delete/commit behavior, and palette DMA without decompiler
  constructs.
- The following 1,396-byte addition reconstructs the complete `080cfef4`
  scene loop: staged background blits, scanline-wave generation, palette
  transitions, object activation, and cleanup. Its eight branch-specific
  blit sites and full external-call multiset match the reference owner.
- The next 2,008-byte cohort adds `080bfba4`, which repairs party-entry
  grouping and emits four categories of stat/link results, and `0801d108`,
  the complete status-window and sprite constructor. Both retain every
  branch-specific message/UI call site and match their reference external-call
  multisets.
- The following 1,336-byte owner, `080a9f10`, reconstructs item/effect
  application for one combatant or the whole party: HP/PP restoration,
  revival, status clearing, permanent stat gains, result codes, and party
  recalculation. It corrects raw decompiler switch labels for effect IDs
  `0x104` through `0x109` and preserves all five mutation call sites.
- The next 1,920-byte owner, `080f7f78`, reconstructs the sliding-window
  compressor: modulo-1024 dictionary matching, lazy-match selection,
  short/long back-reference encoding, flag-byte packing, buffer flushes, and
  terminator emission. Four duplicated wraparound searches in the raw output
  are expressed by one reviewed match-search helper while preserving the
  reference's two refill and two flush call sites.
- The following 1,376-byte owner, `0801a98c`, reconstructs the linked-list
  entity and cursor renderer, including position easing, animation phases,
  visibility flags, priorities, selection tracking, and four render groups.
  The previously ambiguous `080b50b0` veneer was verified to take an entity ID
  and a two-word position output, removing the raw decompiler's missing ABI
  context before admission.
- The next 1,264-byte owner, `08099da4`, reconstructs a complete field
  interaction/cutscene: companion-object setup, vertical entrance, tile
  probing and collision retries, synchronized positioning, particle burst,
  camera handoff, timed animation, and cleanup. Its local particle pointer
  array replaces the raw stack alias while all 72 named external call sites
  remain intact.
- The following 1,208-byte owner, `080dd2c4`, reconstructs the timed
  emitter/particle scene, including 16 emitter tracks, a 1,024-record pool,
  character triggers, four blit paths, blend timing, frame presentation, and
  cleanup. The raw decompiler's scaled-pointer error was corrected to the
  reference's 28-byte particle stride before admission.
- The next 1,152-byte owner, `080f26ec`, reconstructs the complete title
  transition: staged tile and map DMA, display setup, skip handling, object
  creation, blend animation, and cleanup. Its shared frame-yield control flow
  preserves all 38 reference call sites, and its independently compiled target
  assembly matches the reference's named-call multiset.
- Remote exact-C integration adds 806 overlay bytes across seven owners:
  `resource_394:0e64`, `resource_3bc:13c0`, `resource_3bd:0eac/0f10/0f6c/0f94`,
  and `resource_3c8:1a50`. The former semantic `resource_394:0e64` copy was
  removed because the same owner is now byte-exact. The staged early-pool mode
  for `resource_394:0ee0` is path-scoped so it cannot perturb unrelated
  overlays that share the same load address.
- The next 1,152-byte main owner, `0801d4cc`, reconstructs the complete
  five-row configuration menu: preset cycling, value wrapping, three indicator
  groups, window redraw, cursor movement, accept/cancel handling, and persistent
  settings. Its independently compiled target assembly preserves every named
  reference call site, including all seven branch-specific sound calls.
- The following 1,132-byte owner, `080d3c80`, reconstructs a complete
  two-stage particle scene: direction-dependent emitter initialization, camera
  movement, delayed emitter activation, sixteen-particle bursts, animation
  selection, character triggers, frame presentation, and cleanup. Its target
  assembly preserves the reference's full external-call multiset, including
  seven distinct random-number sites and both rendering/update paths.
- The next 1,124-byte owner, `080db264`, reconstructs the complete polar-burst
  effect: 1,024-particle initialization, four rotating emission waves, three
  branch-specific sprite renderers, gravity and bounce updates, timed character
  triggers, and cleanup. Its target assembly preserves all 38 named reference
  call sites, including nine RNG sites and three sine/cosine pairs.
- The following 1,096-byte owner, `080d41a4`, reconstructs a staggered
  multi-character burst scene: camera motion, timed foreground/background
  strips, per-character particle emission, gravity and bounce animation,
  character triggers, presentation, and cleanup. Its target assembly preserves
  the complete reference call multiset, including both strip renderers and the
  independent particle renderer.
- The next 1,092-byte owner, `080db6e0`, reconstructs a dual-layer projected
  particle scene: optional grayscale palette setup, two initialized particle
  banks, camera synchronization, depth-scaled rendering, animated emitters,
  mode-specific character triggers, presentation, and cleanup. Its target
  assembly preserves the full reference call multiset, including both
  projection/draw/update paths and all six RNG sites.
- The following 1,068-byte owner, `080d3854`, reconstructs an 80-frame
  orbiting-strip scene: three layered strips, grouped four-spark animations,
  seven timed pulse events, character triggers, blend ramps, presentation, and
  cleanup. Its target assembly preserves every named reference call site,
  including four distinct renderers and all five RNG sites.
- The next 1,056-byte owner, `080ae2f4`, reconstructs a complete interactive
  selection UI: list reconciliation and count wrapping, two window renderers,
  animated indicators, directional navigation, confirmation, and cleanup. Its
  independently compiled target assembly preserves the reference's complete
  external-call multiset across every redraw and input branch.
- The following 2,316-byte owner, `0808f52c`, reconstructs the scanline-effect
  buffer generator for field transitions. Its state-selected gradient, iris,
  and perspective profiles each emit a complete 160-line horizontal-window
  buffer before toggling the active display buffer. Two apparent helper calls
  in the raw decompilation were repaired as internal early-return entries, and
  the resulting target assembly preserves all 16 named external call sites.
- The latest remote exact-C integration adds `resource_394:0f34/0f54/0fb4`
  and `resource_3bd:0ee0`, completing the former overlay's walk to its veneer
  bank. The three superseded semantic `resource_394` copies were removed.
  Both overlays' colliding `02000ee0` compiler routes remain independently
  path-scoped, and the combined build remains byte-identical.
- The next remote exact-C integration adds 44 functions and 3,104 exact bytes
  across `resource_373` and `resource_3c9`, including their larger nested-loop,
  switch, initializer, and warp-sequence owners. It also exposes 88 previously
  omitted executable alignment bytes in the denominator. The regenerated
  inventory is complete at 1,338,820 executable bytes, and both aggregate
  builds remain green.
- The next 1,620-byte owner, `08018038`, reconstructs the localized text and
  control-code parser. It expands names, numbers, punctuation, formatting,
  and conditional separators into the engine's wrapped 512-entry command
  buffer. The raw decompilation's missing three stack arguments to
  `08017e88` were restored from the reference ABI, and the target assembly
  preserves all 31 named external call sites.
- The following 1,592-byte owner, `080168f4`, reconstructs the queued field
  command interpreter for one moving map object. It updates delays, animation,
  direction, collision bounds, position, and sound state before advancing the
  command cursor. Both geometry paths restore the raw decompilation's missing
  seven-argument rectangle-helper ABI, and the target assembly preserves all
  18 named external call sites.
- The next 1,764-byte owner, `080d4604`, reconstructs a complete battle-effect
  scene: mode-specific origin and palette setup, staged particle groups,
  gravity updates, character effects, presentation, and resource cleanup.
  Its stack-backed coordinate output and all six-argument sprite transfers
  were restored explicitly, while the target assembly preserves all 41 named
  external call sites.
- The following 1,776-byte owner, `080dab74`, reconstructs a layered battle
  transition: two particle banks, vertical-range expansion, projected fragment
  bursts, character triggers, presentation, and cleanup. Three stack-backed
  vectors and the projection context are expressed explicitly, and the target
  assembly preserves all 42 named external call sites.
- The next 1,724-byte owner, `080e89ec`, reconstructs a staged battle
  transition with scanline darkening, two particle banks, a twenty-entry
  position trail, and a final projected burst. Its 160-byte trail history and
  projection workspaces are explicit C objects, and the target assembly
  preserves all 43 named external call sites.
- The following 1,720-byte owner, `080e2974`, reconstructs a table-configured
  battle particle scene: palette and sprite selection, a large initialized
  particle pool, staged character hits, per-group bursts, rendering, and
  cleanup. Its coordinate and projected-vector workspaces and a missing
  `0x30` by `0x10` blit ABI are explicit, while the target assembly preserves
  all 48 named external call sites.
- The next 1,576-byte owner, `080d2d98`, reconstructs a long battle particle
  sequence around a moving scene anchor: a recycled 1,024-entry pool, layered
  strips, timed sounds and character hits, motion damping, and final object
  cleanup. Its projection workspace and character-object accesses are explicit,
  and the target assembly preserves all 50 named external call sites.
- The following 1,556-byte owner, `080cf8e0`, reconstructs a multi-character
  projectile and impact scene with per-character streams, collision bursts,
  variant-selected sprite families, and cleanup. The raw output's unresolved
  resource-setup mode is restored as `2`; all five branch-local setup calls and
  the complete 45-call external multiset remain distinct in target assembly.
- The next 1,660-byte owner, `080de2f8`, reconstructs a shared projectile-orbit
  effect: mode-selected palette and duration, target interpolation, forty
  orbiting particles, a secondary burst, output coordinates, and display-state
  restoration. Five stack-backed three-word vectors are explicit C objects,
  and target assembly preserves the complete named external-call multiset.
- The following 1,152-byte owner, `080191cc`, reconstructs the eight-list
  map-object sprite updater: per-mode positioning, animation curves, random
  offsets, linked-object traversal, and sprite submission. Its shared
  eight-byte transform workspace and three previously hidden helper arguments
  are explicit, while target assembly preserves all 17 named external calls.
- The next 1,642-byte cohort adds three main-ROM owners: formation collapse and
  display-record creation (`0801be80`), encoded-effect record queueing
  (`0801a088`), and shared-runtime arena initialization (`08077d38`). The
  repaired sources restore typed linked-list traversal, table lookups, DMA
  setup, and helper ABIs; each target assembly preserves its complete external
  call multiset.
- The following 1,424-byte cohort adds the paired affine battle-background
  setup owners `080cd594` and `080cd86c`. Their variant-specific display modes,
  DMA descriptor queues, affine matrices, tilemap initialization, and final
  scene-state writes are explicit C; target assembly preserves the complete
  11- and 12-call external multisets.
- The next 604-byte owner, `08018a50`, reconstructs encoded-text measurement:
  control-code scanning, per-row glyph counts and widths, widest-row selection,
  total-width adjustment, and optional distributed spacing. Four row slots and
  the sixteen accumulated widths replace raw stack aliases, and target
  assembly preserves its sole division-helper call.
- The following 836-byte owner, `08097c3c`, reconstructs an interactive
  destination-selection and movement scene: temporary object setup, cursor
  polling, collision and map-owner checks, movement animation, state transfer,
  and cleanup. Two stack-backed coordinate triplets and the object-return ABI
  are explicit, while target assembly preserves all 38 named external calls.
- The next 548-byte owner, `08097644`, reconstructs a double-buffered wave
  update and its periodic traveling map effect: sample generation, graphics
  buffer selection, endpoint lookup, object creation, angle calculation, and
  lifetime cycling. Two coordinate triplets and the true halfword sample
  stride are explicit, and target assembly preserves all 16 named calls.
- The accompanying 572-byte owner, `080aafb8`, reconstructs the categorized
  ability-grid renderer across all active entries. Its byte-count table,
  twenty-byte record stride, category tests, icon selection, and window
  placement correct raw pointer-scaling errors; target assembly preserves all
  15 named calls.
- The following 798-byte owner, `080a5cc0`, reconstructs the complete
  item/effect selection state machine: list setup, category-dependent prompts,
  target selection, confirmation, application, failure handling, and return
  values. Its state-field pointer ABI is explicit, and target assembly
  preserves all 33 named external calls.
- The next 808-byte owner, `080999f0`, reconstructs a staged arrival and
  departure presentation: object interpolation, scale animation, randomized
  particle bursts, alternate sprite emission, delays, and cleanup. Three
  stack-backed coordinate triplets and object-return ABIs are explicit, while
  target assembly preserves all 34 named external calls.
- The following 820-byte owner, `080a7a34`, reconstructs the cyclic
  entry-selection menu: initial palette and cursor setup, redraws, directional
  navigation, page cycling, confirmation/cancel handling, and committed state.
  Its halfword entry table and two-byte cursor strides correct raw
  pointer-scaling errors, and target assembly preserves all 29 named calls.
- The next 588-byte owner, `0809ae64`, reconstructs an approach, impact,
  displacement, and return presentation with object interpolation, scale
  animation, target movement, timed sounds, and cleanup. Its three
  stack-backed coordinate triplets and object-return ABI are explicit, while
  target assembly preserves all 29 named calls.
- The following 632-byte owner, `0808e23c`, reconstructs item-owner resolution
  and field-use dispatch: party inventory scanning, owner selection, dialog
  setup, direct and scripted item behavior, special-event handling, flags, and
  cleanup. Its item-record and helper-return pointers are explicit, and target
  assembly preserves all 38 named external calls.
- The next 716-byte owner, `0808d0c8`, reconstructs the interactive palette
  editor: palette bank, color-index, and RGB-component navigation; five-bit
  channel adjustment; grid refresh; flash preview; and exit cleanup. Its
  palette walk uses the reference's true halfword stride, and target assembly
  preserves all six named external calls.
- The following 240-byte owner, `080196c4`, reconstructs text-token decoding
  into a bounded halfword buffer, including the two extended control forms and
  their payload slots. Its twelve-byte parser state and temporary DMA-backed
  workspace are explicit, and target assembly preserves all seven named
  external calls.
- The next 260-byte owner, `0801868c`, reconstructs text-window measurement,
  pixel-to-tile conversion, narrow-display adjustment, and origin clamping
  within the 30-by-20 layout. Its seven-argument pointer ABI is explicit, and
  target assembly preserves both alternative measurement-helper calls.
- The following 336-byte owner, `080170f8`, reconstructs bordered-window
  composition in the 32-column screen tilemap: dimension validation, clearing,
  mode-specific corners, repeated edges, interior fill, and dirty-state
  publication. Correct halfword cursor advances replace the raw pointer
  scaling, and target assembly preserves all four named external calls.
- Remote exact-C round 14 adds eight overlay functions across `resource_3b1`,
  `resource_3bf`, and `resource_3c9`, increasing exact C by 528 bytes. Four
  unrelated `resource_3c9` functions share a verified
  `-fno-strict-aliasing` route, while the executable inventory correction adds
  72 bytes to the honest denominator.
- The next 260-byte semantic owner, `080108e4`, reconstructs parity-banked
  metatile loading: cache invalidation, asset decompression, sixteen DMA row
  transfers, and optional expansion of the 16-by-16 index grid through the
  tile-definition table into paired VRAM rows. Its byte and halfword strides
  are explicit, and target assembly preserves all three named external calls.
- The following 280-byte owner, `080162d4`, reconstructs allocation and
  initialization of the engine's eight 0x24-byte window-work records. It
  recovers free-slot selection, geometry, presentation flags, and immediate
  versus synchronous opening, while target assembly preserves all four named
  external calls.
- The next 324-byte owner, `080f731c`, reconstructs presentation-workspace
  initialization: particle-pool clearing, eight seeded records, a mirrored
  forty-sample curve, scene-state reset, blend reset, and creation of the two
  text resources. Correct record and halfword strides replace the raw
  pointer-scaling artifacts, and all five named external calls are preserved.
- The following 236-byte owner, `08011bf4`, reconstructs the cyclic animated
  transfer engine: active-record traversal, countdown handling, phase-based
  rotation into a sixteen-halfword buffer, DMA publication, and interval
  reload. Its 0x2c-byte record and stack buffer replace raw aliases, and both
  the reference and target assembly have an empty external-call multiset.
- The next 224-byte owner, `0800b6b8`, reconstructs indexed map-asset loading:
  layer-cache publication, source lookup, decompression, in-place relocation
  of the leading offset table, optional five-table character remapping, and
  dimension return. The remap bound is recovered from the decompressor's
  return register, and target assembly preserves all three external calls.
- The following 228-byte owner, `080faa58`, reconstructs music-player song
  replacement: magic-lock validation, idle/priority admission, player reset,
  active-track population, unused-track clearing, and song-flag application.
  Typed song and 0x50-byte track layouts replace raw offsets, and target
  assembly preserves both reset call sites plus the flag call.
- The next 240-byte owner, `080a7f44`, reconstructs one-position reordering of
  the active identifier list: boundary validation, stack-backed reorder,
  registry removal and reinsertion, compaction, and count publication. Target
  assembly preserves all three external calls. `0801a7f4` was rejected as a
  nonordinary hidden-context gap, and `08016018` was skipped because its three
  calls require an unexpressed `r9` input; neither was admitted.
- Remote exact-C round 15 adds eight overlay functions across `resource_3b1`
  and `resource_3bf`, increasing exact C by 1,748 bytes without changing the
  executable denominator. The source-only and full byte-identical builds both
  pass after integration.
- The next 240-byte semantic owner, `080ccaec`, reconstructs affine-background
  scene initialization: workspace allocation, display-register setup,
  mode-specific palette selection and transfer, counter initialization, and
  registration of both frame callbacks. Target assembly preserves all seven
  named external calls.
- The following 248-byte owner, `08097194`, reconstructs battle-presentation
  teardown: clearing 24 effect slots, bounded waiting for two transient object
  callbacks, callback removal, saved-position restoration, event publication,
  queued-sound playback, and workspace release. The shared effect-slot layout
  is reused, and target assembly preserves all six external calls.
- The next 272-byte owner, `0808ace0`, reconstructs rule-driven initialization
  of a seven-value sequence: area and wildcard-subarea matching, a fifteen-bit
  event condition, high-bit one-slot shift, sequential publication, timing
  reset, and consumer refresh. Its eight-byte rule layout is explicit, and
  target assembly preserves both external calls.
- The following 300-byte owner, `08028574`, reconstructs the cyclic
  single-value selector: label redraw, dual-axis navigation, wrapping,
  confirmation, cancellation, and their distinct sounds. Typed selection,
  count, label-base, and window fields replace raw accesses, and target
  assembly preserves all seven external calls, including four sound sites.
- The next 328-byte owner, `08017c8c`, reconstructs encoded-halfword text
  rendering: rolling-buffer termination, control-token payload consumption,
  row reset and column advance, glyph emission, two non-advancing glyphs, and
  dirty-state publication. Correct halfword strides replace raw pointer
  scaling, and target assembly preserves its sole external call.
- The following 268-byte owner, `08095938`, reconstructs the five-stage
  orbiting effect: angular buildup and decay, transformed reference-object
  targeting, one-eighth easing, stage transitions, teardown, and final vector
  projection. Teardown deliberately reloads the DMA-cleared effect fields
  before projection, and target assembly preserves all four external calls.
- Remote exact-C round 16 completes the `resource_3bf` linear walk through
  `0x1150`, adding eighteen overlay functions and 774 exact-C bytes. The
  corrected executable inventory exposes another 16 bytes in the denominator;
  both aggregate builds remain green after integration.
- The next 314-byte semantic owner, `080a1e38`, reconstructs category-driven
  ordering of a packed fifteen-identifier list. It builds the requested
  category sequence, repeatedly selects the greatest matching nine-bit value,
  honors the criterion's required-attribute bit, and publishes the reordered
  active prefix. Its explicit stack arrays replace raw frame aliases, and
  target assembly preserves both named external calls.
- The following 590-byte owner, `08018cac`, reconstructs display-record
  creation for encoded field-object characters. It handles alternate object
  rendering, glyph decoding into a 128-byte stack tile, VRAM upload, special
  existing-tile coordinates, record-mode selection, and final registration.
  Explicit record and DMA layouts replace raw frame aliases and offset macros
  at the descriptor boundary, while target assembly preserves all eleven named
  external call sites.
- The next 460-byte owner, `0801ba68`, reconstructs bidirectional insertion
  into an animated linked list. It creates the entering record, shifts every
  existing record by one sixteen-pixel slot, waits for motion completion,
  retires and releases the record leaving the opposite edge, and repairs both
  neighboring links. A typed 40-byte record replaces raw offset accesses, and
  target assembly preserves all eight branch-local external call sites.
- The following 408-byte owner, `0801b810`, reconstructs the selection
  page-back transition. It updates the cursor within short pages, inserts a
  leading record while moving through longer pages, or slides and repopulates
  the complete five-record window at the first position before redrawing the
  selection. The shared typed record layout replaces raw pointer arithmetic,
  and target assembly preserves all ten external call sites.
- The paired 428-byte owner, `0801b664`, reconstructs the forward selection
  transition. It wraps short pages, inserts newly exposed trailing records,
  or animates the long-page list home before repopulating and spacing all five
  records. Together with `0801b810`, both directions now share the recovered
  40-byte list-record layout; target assembly preserves all ten external call
  sites.
- The next 312-byte owner, `0801b010`, reconstructs the selection-description
  window manager. It allocates or resizes the window for the selected entry,
  handles the type-six placement state, presents the window, and chooses the
  entry-specific or type-specific message. The previously unresolved fifth
  allocation argument is recovered as the reference's type value `6`, and
  target assembly preserves all nine external call sites.
- The following 440-byte owner, `080901c0`, reconstructs the five-profile
  field-transition dispatcher. It separates the encoded profile and variant,
  initializes the shared transition state, chooses the appropriate frame
  callbacks, and records duration and blend parameters for the asynchronous
  profiles. Typed callback prototypes replace the raw unknown declarations,
  and target assembly preserves all seventeen external call sites.
- The next 392-byte owner, `0801ccc0`, reconstructs the configurable
  seven-color UI palette ramp. It derives three channels from the engine's
  curve table, applies and clamps the brightness adjustment, and feeds the
  resulting triplet through each of the seven fixed color transforms directly
  into palette RAM. An explicit three-halfword stack array replaces the raw
  frame alias, and target assembly preserves all eleven external call sites.
- The following 476-byte owner, `0809c138`, reconstructs a scene interlude
  with temporary object-priority forcing, palette and background DMA, callback
  setup and teardown, optional event triggering, and queued restoration of the
  blend register. Its sixteen-byte priority save and register-restore record
  replace raw stack aliases, and all touched scene, runtime, interrupt, and
  display state is restored; target assembly preserves all sixteen calls.
- The next 456-byte owner, `08095c08`, reconstructs the six-stage
  orbiting-object effect: randomized launch, forward travel, target capture,
  controller-derived return launch, backward travel, and teardown. Explicit
  three-word launch and return vectors replace raw stack aliases, while the
  object render bits, saved position, stage flags, timers, and sounds follow
  the reference's shared update paths; target assembly preserves all thirteen
  external call sites.
- The following 432-byte owner, `08098698`, reconstructs the 24-projectile
  scene emitter. It chooses a direction-dependent launch point, creates each
  projectile, copies the base sprite's render attributes, installs its update
  behavior, and assigns a randomized three-axis velocity before pacing the
  next emission. Explicit position and velocity vectors replace raw frame
  aliases, and target assembly preserves all fifteen external call sites.
- The next 412-byte owner, `0801f818`, reconstructs the compact game-state
  snapshot at `0x02000000`: current-character data, world state, party values,
  sentinel-terminated identifiers, settings, event counts, and a checksum over
  the associated 968-word payload. Correct byte and word strides replace the
  raw decompiler's scaled pointers, and target assembly preserves all ten
  external call sites.
- The following 376-byte owner, `080f0678`, reconstructs title-presentation
  initialization: VRAM clear and seed transfers, the complete 1,024-byte
  command table with 24 fixed records, 96 grouped tile records, and eight
  terminators, callback registration, and creation of 32 presentation
  objects. Typed eight-byte records replace the raw halfword-scaled walk, and
  target assembly preserves all four external call sites.
- The next 368-byte owner, `08028c04`, reconstructs both layouts of the
  multi-window selection dialog: resource loading, asynchronous callback
  setup, primary and secondary window allocation, all branch-specific content
  rows, selector execution, callback removal, and window teardown. Explicit
  window pointers replace the raw pointer-to-field alias, and target assembly
  preserves all twenty-seven external call sites.
- The following 344-byte owner, `0809641c`, reconstructs the rising
  projectile's launch, two travel passes, reversal-or-advance decision, and
  cleanup. It now forms a complete semantic pair with the already-exact
  `08096574` position-easing callback; an explicit three-word vector replaces
  the raw frame alias, and target assembly preserves all thirteen calls.
- The next 936-byte family, `08010424`, `080105d4`, and `08010788`,
  reconstructs three related tile-map rectangle transfers. Each copies or
  merges the source tile index into its destination, tests the tile against
  three fixed-point object rectangles, and publishes covered tile graphics on
  the object's corresponding display layer. Their shared six-argument ABI,
  three-anchor layout, 128-entry row stride, and zero-call control flow are
  explicit; all three compile and assemble independently.
- The following 368-byte owner, `080e6d3c`, reconstructs four predefined
  object formations around a caller-provided origin: a nine-object 3-by-3
  grid and three table-driven layouts of twelve or eight objects. It selects
  the correct projection pair, forms each four-word position request, and
  preserves the reference's four layout-specific render sites plus its two
  grid-coordinate helper calls.
- The next 444-byte owner, `080b2328`, reconstructs combat-effect purchase
  validation, error reporting, temporary entry removal, the full confirmation
  animation, cost deduction, menu refresh, and optional follow-up application.
  Its target assembly preserves all 44 branch-specific call sites. Two larger
  scene candidates screened immediately beforehand remain parked: `080e6638`
  loads a renderer callback into live `r4`, and `080dc454` loads its blitter
  context into live `r5`; neither hidden input is expressible by an ordinary
  C call, so no misleading semantic source was admitted.
- The following 460-byte owner, `08095dd0`, reconstructs a scripted
  slot-specific scene: three staged cues, normalized origin capture, creation
  of 24 randomized particles, two state transitions, and restoration of the
  scene objects. Its independently audited ABI restores the raw decompiler's
  missing slot arguments, particle mode, and three-word position workspace;
  target assembly preserves all 37 static call sites.
- The next 596-byte owner, `0808e9c0`, reconstructs the map spawn-record
  loader: sequence filtering, provider-backed record lookup, class-3 and
  class-19 object creation, fixed-point coordinate publication, and the
  ten-entry runtime output table. Its complete range includes a 32-byte
  literal pool that earlier screening omitted; the target source preserves
  all fourteen named call sites and uses only ordinary ABIs.
- The following 1,098-byte owner, `080fae58`, reconstructs the complete
  four-channel programmable-sound update: channel startup and shutdown,
  attack/decay/sustain/release and pseudo-echo state transitions, wave RAM
  uploads, pitch compensation, pan, volume, and hardware triggering. The
  implementation was independently checked against both the ROM owner and
  pret's public `CgbSound` reconstruction; its target assembly preserves the
  two volume-helper sites and the single oscillator-off site.
- The next 1,028-byte owner, `0801de5c`, reconstructs the encoded-glyph and
  tile uploader: all 27 control-token cases, transparent eight-row rendering,
  in-place 8bpp-to-4bpp packing, circular tile allocation, paired tilemap
  updates, and explicit VRAM row stores. Its first relocated transfer now
  names the scratch allocation that the reference retained incidentally in
  live `r2`; all six external call sites remain distinct in target assembly.
- The following 632-byte owner, `080b88d0`, reconstructs a party-side scene
  transition: object validation, group selection, paired fades, live-member
  filtering, sentinel termination, stack-backed request construction, and
  display restoration. The formerly implicit `080c9010` request argument is
  explicit, signed and unsigned ID domains remain distinct, and all 25
  reference call sites are preserved.
- The next 708-byte owner, `0808fefc`, reconstructs five field-transition
  command profiles and their display-register queueing. Four interrupt-guarded
  queue paths preserve the reference's volatile widths, insertion order, and
  unusual `0x0208` IME disable value; effect-state endpoints, odd Thumb
  callbacks, and all 19 external call sites remain explicit.
- The following 836-byte owner, `08092c40`, reconstructs object-anchored
  dialog and resource-window placement: fallback object resolution,
  five-output geometry calls, horizontal and vertical clipping, style
  selection, paired handle creation, and completion polling. It preserves
  signed fixed-point rounding, the resolved-value versus masked-ID domains,
  both geometry-adjustment call sites, and all 17 external calls. This owner
  takes remaining semantic debt below one million executable bytes.

## Toolchain on this host (linux-x64)

All compiler bundles were rebuilt from the pinned alchemy-gcc commit
`64d757f` at `/home/user/alchemy-gcc` and re-admitted: gcc296 (gs1),
old_agbcc, gs2, and both experimental comparison compilers
(pret-early-thumb, gcc2951 — previously macOS-only, now per-host digests).
Admission evidence: the composed source-only image reproduces gs1-en.gba
byte-identically (SHA-1 5c4695205413df7db52b9a184815a07783999971) and
`bun run verify` is green end to end. `roms/gs1-en.gba` can be recomposed
from the source-only build products when absent; binutils-arm-none-eabi,
gperf, and bun 1.3.14 are required host packages.

## Session results (this session)

resource_3bc linear walk: exact adoptions 058c (68), 05d0 (2), 05d4 (10),
05e0 (120), 0658 (76), 06a4 (110), 0714 (88), 0a20 (34), 0a44 (64),
0a84 (80), 0ad4 (92), 0b30 (112), 0ba0 (2), 0d70 (18), 0d84 (24),
0d9c (8), 137c (68), 2710 (72), 2758 (308), 2a50 (66). resource_3c8:
094c (30). Two work streams (both interrupted by a container restart;
their work below survived on disk and is committed) adopted the
small-queue members resource_371:0030, 372:0030, 383:0048, 394:0be4,
399:0030, 3a8:00ac, 3ac:004c, 3ce:007c, walked resource_383 through
02c0/02e8/0310/0354/0378/0400/0454/04bc/04f4, and walked resource_3c8
through 0b08/0b98/0c5c/0cc8/0e7c/0e80/0e88/0f1c.

Reusable shapes proved this session (all backed by exact installs):
- paired stack-argument locals assigned in consecutive statements, second
  pair x-first (resource_3bc_c_020005e0.c);
- named base-pointer copy to keep Data_02000240 base + runtime `250<<1`
  offset (`s16 *table = Data_02000240; *(s32 *)&table[250]`);
- `while (*p != a && *p != b)` guards duplicate their exit test only when
  the guard reads a direct constant dereference and the loop body reads a
  pointer local assigned inside the guard (resource_3bc_c_02000a84.c);
- masked coordinate as `z = obj->z & 0xFFF00000;` then `z + K` in the call
  (resource_3bc_c_02000ad4.c, 02000b30.c);
- signed `/ 65536` for the `(neg ? +0xffff : ) >> 16` shape, plus a copy
  variable for one abs and in-place negation for the other
  (resource_3bc_c_02002758.c);
- HImode constant stores pool as `ldrh .L` unless the value goes through an
  int-typed local; a pointer-local assigned before the value forces the
  reference's address-then-value order (resource_3bc_c_02002710.c);
- one C variable reused for two sequential object fetches produces the
  entry `mov r5, r0` copy (work/claude notes for 13c0).

## Later rounds (same session)

Three further rounds adopted ~80 more functions: resource_3ac and
resource_3ce walked to their veneer banks, resource_383 covered
0x052c-0x1e80, resource_399 through 0x0abc, resource_37a through 0x1510,
resource_3bd 0xa54-0xde8, resource_3c8 through 0x1f5e, plus the routed
adoptions below. New overlay flag routes (each entry has an exact-byte
proof recorded in its work/claude/notes file):
NO_SCHED_DEPEND_COUNT_OVERLAY_SOURCES (399 family + 3ce:0244),
THUMB_IMMEDIATE_LATENCY_OVERLAY_SOURCES (399:05dc/0a3c/0abc, 37a:1380),
NO_RERUN_CSE_AFTER_LOOP_OVERLAY_SOURCES (37a:0054/0108/0150/01ec,
399:0abc), NO_CSE_FOLLOW_SKIP_OVERLAY_SOURCES (383:082c).

Key new lever: hoisting a single int-typed named local into the entry
block (`g = 0x986;` before the first branch) makes gcse rematerialize
the pool constant at each later-block use, defeating the cross-call
CSE-share blocker EXCEPT when a use sits in the entry block itself or
the constant is spelled as a &Value_ symbol. After round 5: resource_383
is COMPLETE to its veneer bank (0x4c6c); 37a's 0x1510-0x296c stretch is
fully classified (adoptable members taken, rest parked); 3bd's 0x0c4
gap is hand-written assembly (not C) and 0x2a8-0x474 is covered.
After round 7, resource_399 and resource_3ca are COMPLETE to their
veneer banks (0x2168 and 0x1314); resource_371/372 are covered through
0x590/0x400, resource_3b0 through 0x4b0. New GROUPED_DMA_STORE_OVERLAY_
SOURCES route (3ca:10d4). Cross-overlay scheduler fingerprint worth a
compiler-lane look: a store-before-independent-load pair order that
would unlock seven parked functions at <=3 halfwords each (3b0:0030,
3b0:0468, 371:02f0, 372:0278/0398/03cc/0400 — see their notes).
After round 6, resource_381 is also COMPLETE to its veneer bank
(0x33d4): every prologue adopted, parked with a best draft, or parked
with a category note (the 3.5 KB root initializer at 0054, the 5.1 KB
1410 sheet, 301c, 2eb0, and the ip-call functions are the category
parks). Remaining frontiers: 399 0x18c4+ (u16-local HImode pool shapes),
3ca 0xda4+, 394 0xc2c (hi-reg giant), 3bd 0x474/0x608/0x8c0,
3c8 0x1d48/0x1f60 giants, 3b0 0x180+, 371/372 after 0x0030, and the
giant single functions (379:00dc, 3bc:0da4).

Round-5 escalations for the compiler lane: a register-allocation
parity disease (identical instruction streams, scratch/callee register
identities swapped) accounts for most near-misses (383:4754/47bc/47fc,
399:1704/174c and earlier 3bd:034c); and the inline
`ldr r3,=0x03000118; mov ip,pc; bx r3` IWRAM call idiom has no C
spelling (blocks 399:15b4, 3ca:0194). A mid-function pool-dump gap
blocks 37a:1510 (640 B, instruction-exact otherwise) — needs an
early-literal-pool variant that re-creates duplicate pool entries.

## Blockers discovered (documented under work/claude/notes/)

1. **resource_3bc runtime base is 0x02008000.** Jump-table words in the
   0da4 switch (0x0200_9234...) and callback constants (0x02008659 =
   offset 0x658|1) prove it. compileOverlayC links at OVERLAY_BASE
   0x02000000, so any function embedding compiler-generated absolute
   label addresses (switch tables) cannot verify until per-overlay link
   bases exist. Blocks 0da4 (~1.5 KB) and likely other giants.
2. **resource_3bc tail vintage question.** Five functions (0ba4, 0c5c,
   288c, 29ac, 2a94/2b50) show reference codegen our gs1 cc1 cannot emit
   from any measured source shape: cse never folds const+1/+2 into fresh
   pool words there, register-argument split constants rematerialize per
   call, and the three-way selection keeps cmp/bne+in-arm loads where our
   jump optimization threads them 4 bytes shorter. The gs2 (gcc 3.0)
   bundle derives `add #1` for the same spelling but is worse elsewhere.
   Everything before ~0x2758 in the same overlay matches gs1 exactly.
   Compiler-lane question; do not respin source variants (measured lists
   in the notes).
3. **Allocation-priority rotations** (076c at 164 hw with exact tail,
   13c0 at 36 hw): the short-lived shared constant outranks steps/state
   in our global allocator; reference ranks it below both and pays a
   caller-save pair. Same QTY_CMP_PRI family as LAWS.md.

## Best restart points

- resource_3c8: continue the walker's linear scan (check git log for its
  last adoption).
- resource_370:0054 is a 256-byte multi-descriptor grouped-DMA function
  (three stmia descriptor groups, mid-function pool) — compiler-lane.
- resource_379:00dc is one 2,524-byte function; large but single.
- resource_381 holds ~18 KB of discovered unconverted functions plus the
  3,548-byte semantic draft from the previous session (work/ was not
  preserved in this container; treat as fresh).
- The twelve-member 60-byte family (resource_373:0030 etc.) remains
  parked; no exact result across 1,633 configurations previously.

## Required checks

Before every exact-C commit:

```sh
bun tools/full_c_progress.ts --write-inventory
bun tools/full_c_progress.ts --write-report
bun run verify
```

Commit subjects must end in the live suffix printed by:

```sh
bun tools/full_c_progress.ts --subject
```

Clean-room rules remain authoritative in `PROVENANCE.md`.
