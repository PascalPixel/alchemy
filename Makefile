# ---------------------------------------------------------------------------
# The compiler standard.
#
# This is the whole supported flag set. It mirrors the Makefile of
# Coaltergeist/camelot-gcc -- a separate project, not an upstream of this one --
# which reproduces the Golden Sun ROM byte-identically against this set and
# nothing else. Alchemy targets the
# same standard: Camelot shipped a makefile, not a per-file flag database.
#
# Anything a source needs beyond this is DEBT, not configuration. It records an
# admission that the reconstruction is wrong and a flag is standing in for the
# fix. `make routing-debt` reports how many sources still deviate;
# `make standard-check` asserts that these flags are the flags the build
# actually uses, so this block cannot drift away from the routing crate.
# ---------------------------------------------------------------------------
GCC296_CFLAGS := -O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi \
                 -fno-builtin -nostdinc -ffreestanding \
                 -fcall-used-r4 -Iinclude

# The only sanctioned deviations, matching upstream's three:
#   1. one overlay stem that never interworks, so needs no veneers
#   2. a small number of translation units the original built at -O1
#   3. the stock m4a ("Sappy") audio engine, which only matches under old_agbcc
NO_INTERWORK_CFLAGS := $(filter-out -mthumb-interwork,$(GCC296_CFLAGS))
O1_CFLAGS           := $(subst -O2,-O1,$(GCC296_CFLAGS))

TOOLS := tools
CARGO ?= cargo
CARGO_RUN := $(CARGO) run --offline --quiet --release --manifest-path
DISPATCH_MANIFEST := $(TOOLS)/dispatch/Cargo.toml
DISPATCH_RUN := $(CARGO_RUN) $(DISPATCH_MANIFEST) --
DISPATCH_GROUPS := assets check compiler decomp make metrics overlay search semantic

.PHONY: help verify test lint standard-check routing-debt crate-tests pristine-options-check \
	build-claimed build-asm build-assets build-semantic \
	build-full build-rom inventory semantic-check core-retained-check sanctum \
	progress progress-check progress-subject progress-history coverage coverage-check \
	showcase compiler-checks compiler-sweep compiler-cohort overlay-compiler-cohort \
	compiler-corpus compiler-batch overlay-candidate-check statement-order-check \
	compiler-lint compiler-self-test build-dispatch build-all clean clean-preview $(addprefix dispatch-,$(DISPATCH_GROUPS))

help:
	@printf '%s\n' \
		'make verify          authoritative byte-exact gate' \
		'make test            lint and every native self-test' \
		'make lint            repository architecture and policy gates' \
		'make build-asm       rebuild the assembled stage' \
		'make build-rom       rebuild the ROM' \
		'make build-assets    rebuild the asset tree' \
		'make progress        print byte-exact progress' \
		'make progress-subject print the Exact-C commit subject prefix' \
		'make progress-history refresh the progress history ledger' \
		'make coverage        refresh the dashboard maps' \
		'make showcase        run the overlay showcase' \
		'make compiler-checks run focused native compiler checks' \
		'make dispatch-GROUP ARGS=... run a registered native dispatch group'

# route-dump is a `check` subcommand now; invoke it through cargo like the rest.
ROUTE_DUMP := $(CARGO) run --offline --quiet --release --manifest-path $(TOOLS)/check/Cargo.toml -- route-dump

# Assert that the flag set documented above is the flag set the build uses.
# Two places naming the standard is only safe if a gate compares them.
standard-check:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/check/Cargo.toml
	@printf '%s\n' $(GCC296_CFLAGS) | grep -v '^-I' | sort > /tmp/alchemy-standard-makefile.txt
	@$(ROUTE_DUMP) --standard | grep -v '^-I' | sort > /tmp/alchemy-standard-routing.txt
	@if diff -q /tmp/alchemy-standard-makefile.txt /tmp/alchemy-standard-routing.txt >/dev/null; then \
		printf 'standard ok: Makefile and routing agree on %s codegen flags\n' \
			"$$(wc -l < /tmp/alchemy-standard-makefile.txt | tr -d ' ')"; \
	else \
		printf 'standard MISMATCH between Makefile and routing:\n'; \
		diff /tmp/alchemy-standard-makefile.txt /tmp/alchemy-standard-routing.txt; \
		exit 1; \
	fi
	@# The include flag is compared separately: the routing crate absolutizes it
	@# so the build works from any working directory, while the Makefile and
	@# upstream both spell it relatively. Both must resolve to this repo's
	@# include/ directory, and neither affects code generation.
	@$(ROUTE_DUMP) --standard | grep '^-I' | sed 's/^-I//' | while read -r dir; do \
		if [ ! -d "$$dir" ]; then \
			printf 'standard MISMATCH: include path %s does not exist\n' "$$dir"; exit 1; \
		fi; \
		if [ "$$(cd "$$dir" && pwd -P)" != "$$(cd include && pwd -P)" ]; then \
			printf 'standard MISMATCH: include path %s is not this repo include/\n' "$$dir"; exit 1; \
		fi; \
	done
	@printf 'include ok: routed include path resolves to this repository include/\n'
	@# HARD RULE: no invented compiler options. Every routed flag must be a stock
	@# gcc 2.96 option or an already-grandfathered invention, and the grandfathered
	@# list may only shrink. See tools/route-dump/data/invented-flags.txt.
	@$(ROUTE_DUMP) --routed 2>/dev/null | cut -f3 | tr ' ' '\n' | grep '^-' | sort -u > /tmp/alchemy-routed-flags.txt
	@grep -v '^#' $(TOOLS)/route-dump/data/stock-flags.txt | grep . | sort -u > /tmp/alchemy-stock.txt
	@grep -v '^#' $(TOOLS)/route-dump/data/invented-flags.txt | grep . | sort -u > /tmp/alchemy-invented.txt
	@sort -u /tmp/alchemy-stock.txt /tmp/alchemy-invented.txt > /tmp/alchemy-allowed.txt
	@if comm -23 /tmp/alchemy-routed-flags.txt /tmp/alchemy-allowed.txt | grep -q .; then \
		printf 'INVENTED OPTION REJECTED: routed but neither stock nor grandfathered:\n'; \
		comm -23 /tmp/alchemy-routed-flags.txt /tmp/alchemy-allowed.txt | sed 's/^/  /'; \
		printf 'A byte match reached by inventing a compiler option is not a reconstruction.\n'; \
		exit 1; \
	fi
	@if comm -13 /tmp/alchemy-routed-flags.txt /tmp/alchemy-invented.txt | grep -q .; then \
		printf 'STALE: grandfathered inventions nothing routes any more; delete them from\n'; \
		printf 'tools/route-dump/data/invented-flags.txt so the list keeps shrinking:\n'; \
		comm -13 /tmp/alchemy-routed-flags.txt /tmp/alchemy-invented.txt | sed 's/^/  /'; \
		exit 1; \
	fi
	@printf 'options ok: %s stock, %s grandfathered inventions, 0 new\n' \
		"$$(wc -l < /tmp/alchemy-stock.txt | tr -d ' ')" \
		"$$(wc -l < /tmp/alchemy-invented.txt | tr -d ' ')"

# How far the tree still is from building on the standard alone.
routing-debt:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/check/Cargo.toml
	@$(ROUTE_DUMP) --debt

# ---------------------------------------------------------------------------
# Reclaim disk. This repository reached 64 GB decompiling a 32 MB game, because
# nothing ever pruned the intermediate trees.
#
# DELIBERATELY NOT `git clean -fdX`. roms/ is itself gitignored, so git clean
# removes it, and NO pathspec prevents that: an entirely-ignored directory is
# removed as a unit, so `:!roms`, `:(exclude)roms`, `:(exclude)roms/**` and
# `-e roms` were each verified to still list `Would remove roms/`. Naming the
# trees explicitly cannot delete the ROMs by construction.
# ---------------------------------------------------------------------------
CLEAN_TREES := out work build builds dist cmatch comparisons compiler-output \
               diffs disassembly dumps m2c objdump reports analysis \
               .cache target

clean:
	@before=$$(du -sm . 2>/dev/null | cut -f1); \
	rm -rf $(CLEAN_TREES); \
	rm -rf $(TOOLS)/*/target; \
	rm -rf $(TOOLS)/scratch/*; \
	find . -depth -type d -empty -not -path './.git/*' -delete 2>/dev/null; \
	after=$$(du -sm . 2>/dev/null | cut -f1); \
	printf 'clean ok: %s MB -> %s MB (roms/ untouched: %s files)\n' \
	  "$$before" "$$after" "$$(ls roms/*.gba 2>/dev/null | wc -l | tr -d ' ')"

clean-preview:
	@printf 'would remove:\n'; \
	for t in $(CLEAN_TREES); do [ -e "$$t" ] && printf '  %-22s %s\n' "$$t" "$$(du -sh $$t 2>/dev/null | cut -f1)"; done; \
	printf '  %-22s %s\n' "tools/*/target" "$$(du -sch $(TOOLS)/*/target 2>/dev/null | tail -1 | cut -f1)"; \
	printf 'roms/ is NOT in that list and is never touched by this target.\n'

build-dispatch:
	$(CARGO) build --offline --quiet --release --manifest-path $(DISPATCH_MANIFEST)

$(addprefix dispatch-,$(DISPATCH_GROUPS)): dispatch-%:
	$(DISPATCH_RUN) $* $(ARGS)

build-claimed:
	$(CARGO_RUN) $(TOOLS)/build-stage/Cargo.toml -- claimed

build-asm:
	$(CARGO_RUN) $(TOOLS)/build-stage/Cargo.toml -- asm

build-assets:
	$(CARGO_RUN) $(TOOLS)/build-assets/Cargo.toml --

build-semantic:
	$(CARGO_RUN) $(TOOLS)/build-stage/Cargo.toml -- semantic

build-full:
	$(CARGO_RUN) $(TOOLS)/build-stage/Cargo.toml -- full

build-rom:
	$(CARGO_RUN) $(TOOLS)/build-stage/Cargo.toml -- rom

inventory:
	$(CARGO_RUN) $(TOOLS)/overlay/Cargo.toml -- inventory

semantic-check:
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- semantic-owner-scope --check

core-retained-check:
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- core-retained-audit --check

sanctum:
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- check-sanctum

progress:
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- progress

progress-check:
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- progress --check

progress-subject:
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- progress --subject

progress-history:
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- full-c-history --write

coverage:
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- coverage-map --write

coverage-check:
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- coverage-map --check

showcase:
	$(CARGO_RUN) $(TOOLS)/overlay/Cargo.toml -- showcase --all

compiler-checks: compiler-sweep compiler-cohort overlay-compiler-cohort compiler-corpus compiler-batch

compiler-sweep:
	$(CARGO_RUN) $(TOOLS)/compiler/Cargo.toml -- mode-sweep --self-test

compiler-cohort:
	$(CARGO_RUN) $(TOOLS)/compiler/Cargo.toml -- mode-cohort --self-test

overlay-compiler-cohort:
	$(CARGO_RUN) $(TOOLS)/overlay-mode-cohort/Cargo.toml -- --self-test

compiler-corpus:
	$(CARGO_RUN) $(TOOLS)/compiler/Cargo.toml -- corpus-regressionbin compiler-corpus-regression -- --self-test

compiler-batch:
	$(CARGO_RUN) $(TOOLS)/compiler/Cargo.toml -- search-modes --self-test

overlay-candidate-check:
	$(CARGO_RUN) $(TOOLS)/overlay/Cargo.toml -- candidate-rank --self-test

# Statement-order permutation now lives in shape-sweep's descent driver.
statement-order-check:
	$(CARGO_RUN) $(TOOLS)/compiler/Cargo.toml -- shape-sweep --self-test

compiler-lint:
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- alchemy-lints

compiler-self-test:
	$(CARGO_RUN) $(TOOLS)/alchemy-selftest/Cargo.toml --

# Tool-crate unit tests. Nothing ran these before, which is how 14 routing
# tests and 3 self-tests sat failing against flags deleted by the axe.
CRATE_TESTS := alchemy-routing alchemy-plan alchemy-bundle candidate-show \
               mode-sweep \
               shape-sweep dispatch
# QUARANTINED: alchemy-selftest does not compile. It imports four routing_data
# constants deleted as orphaned, and its remaining assertions require that
# -mgrouped-dma-store and -fthumb-group-control-last are routed -- INVENTED
# options the axe removed, which the no-invented-options rule forbids. It is
# named in KNOWN_BROKEN below so `build-all` reports it without hiding it.
# Fixing it means deleting those assertions; putting it back here is the last
# step of that work.

# A gate that cannot fail is worse than no gate. The former body piped cargo
# into `grep | head`, so the pipeline reported HEAD's status and a FAILED test
# line printed happily while make carried on. It shipped green over five real
# failures. Test cargo's own exit status, not the pipeline's.
crate-tests:
	@fail=0; \
	for c in $(CRATE_TESTS); do \
		printf "  %-28s " "$$c"; \
		if out=$$($(CARGO) test --offline --quiet --release --manifest-path $(TOOLS)/$$c/Cargo.toml 2>&1); then \
			printf '%s\n' "$$out" | grep -E '^test result' | head -1; \
		else \
			echo FAILED; \
			printf '%s\n' "$$out" | grep -E '^(error|thread|test result|assertion)' | head -6; \
			fail=1; \
		fi; \
	done; \
	test "$$fail" -eq 0

# Nothing compiled every crate, so a crate could stop building and both lint and
# crate-tests stayed green. Five crates were broken this way before anyone noticed.
#
# KNOWN_BROKEN is reported loudly every run and must only ever shrink. It exists
# so one known failure does not force the whole gate off, which is how the
# previous gate ended up unable to fail at all.
KNOWN_BROKEN := alchemy-selftest

build-all:
	@fail=0; \
	for d in $(TOOLS)/*/; do \
		[ -f "$$d/Cargo.toml" ] || continue; \
		name=$$(basename $$d); \
		if ! $(CARGO) build --offline --quiet --release --manifest-path "$$d/Cargo.toml" >/dev/null 2>&1; then \
			if echo " $(KNOWN_BROKEN) " | grep -q " $$name "; then \
				echo "  KNOWN BROKEN: $$name"; \
			else \
				echo "  FAILS TO BUILD: $$name"; fail=1; \
			fi; \
		fi; \
	done; \
	if [ "$$fail" -eq 0 ]; then echo "build-all ok: every crate compiles except the $(words $(KNOWN_BROKEN)) known-broken"; fi; \
	test "$$fail" -eq 0

lint: pristine-options-check standard-check crate-tests
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- architecture
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- source-citations
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- no-asm-c
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- decomp-targets --self-test
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- cache-key-lint
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- lang-ban
	$(MAKE) compiler-lint

test: lint compiler-self-test
	$(CARGO_RUN) $(TOOLS)/self-test/Cargo.toml --

verify:
	$(MAKE) inventory
	$(MAKE) test
	$(MAKE) build-full
	$(MAKE) build-semantic
	$(MAKE) semantic-check
	$(MAKE) core-retained-check
	$(CARGO_RUN) $(TOOLS)/check/Cargo.toml -- semantic-superseded --check
	$(MAKE) sanctum
	$(MAKE) progress-check

# The axe stays swung: the files that DEFINE compiler options must remain
# byte-identical to pristine gcc 2.96. An invented option cannot be routed if it
# cannot be declared, so this is the enforcement that outlives any routing table.
COMPILER_FORK_POINT := fdafcb49a2cf2a588da235cac24333a0c9f621d9
OPTION_FILES := gcc-2.96/gcc/toplev.c gcc-2.96/gcc/flags.h gcc-2.96/gcc/config/arm/arm.h

pristine-options-check:
	@# What must never grow is the set of OPTIONS the compiler accepts. Modelling
	@# a real Camelot behaviour unconditionally is allowed and is how a routed
	@# flag is retired; adding a switch to select it is not. So this compares the
	@# option DECLARATIONS against stock gcc 2.96 rather than whole files, which
	@# would also forbid the legitimate half.
	@cd alchemy-gcc && \
	for f in $(OPTION_FILES); do \
		git show $(COMPILER_FORK_POINT):$$f 2>/dev/null \
		  | grep -oE '"(f|m)[a-z0-9-]+"|ARM_FLAG_[A-Z0-9_]+|flag_[a-z0-9_]+' | sort -u > /tmp/opt-old.txt; \
		grep -oE '"(f|m)[a-z0-9-]+"|ARM_FLAG_[A-Z0-9_]+|flag_[a-z0-9_]+' $$f | sort -u > /tmp/opt-new.txt; \
		if comm -13 /tmp/opt-old.txt /tmp/opt-new.txt | grep -q .; then \
			printf 'INVENTED OPTION REJECTED in %s:\n' "$$f"; \
			comm -13 /tmp/opt-old.txt /tmp/opt-new.txt | sed 's/^/  /'; \
			printf 'A byte match reached by inventing an option is not a reconstruction.\n'; \
			printf 'Model the behaviour unconditionally instead, with no switch.\n'; \
			exit 1; \
		fi; \
	done
	@printf 'pristine ok: no compiler option beyond stock gcc 2.96 is declared\n'
