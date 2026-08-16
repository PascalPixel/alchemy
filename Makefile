# ---------------------------------------------------------------------------
# The compiler standard.
#
# This is the whole supported flag set. It mirrors the Makefile of the upstream
# compiler project, Coaltergeist/camelot-gcc, which reproduces the Golden Sun
# ROM byte-identically against this set and nothing else. Alchemy targets the
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

.PHONY: help verify test lint standard-check routing-debt pristine-options-check \
	build-claimed build-asm build-assets build-semantic \
	build-full build-rom inventory semantic-check core-retained-check sanctum \
	progress progress-check progress-subject progress-history coverage coverage-check \
	showcase compiler-checks compiler-sweep compiler-cohort overlay-compiler-cohort \
	compiler-corpus compiler-batch overlay-candidate-check statement-order-check \
	compiler-lint compiler-self-test build-dispatch $(addprefix dispatch-,$(DISPATCH_GROUPS))

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

ROUTE_DUMP := $(TOOLS)/route-dump/target/release/route-dump

# Assert that the flag set documented above is the flag set the build uses.
# Two places naming the standard is only safe if a gate compares them.
standard-check:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/route-dump/Cargo.toml
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
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/route-dump/Cargo.toml
	@$(ROUTE_DUMP) --debt

build-dispatch:
	$(CARGO) build --offline --quiet --release --manifest-path $(DISPATCH_MANIFEST)

$(addprefix dispatch-,$(DISPATCH_GROUPS)): dispatch-%:
	$(DISPATCH_RUN) $* $(ARGS)

build-claimed:
	$(CARGO_RUN) $(TOOLS)/build-claimed/Cargo.toml --

build-asm:
	$(CARGO_RUN) $(TOOLS)/build-asm/Cargo.toml --

build-assets:
	$(CARGO_RUN) $(TOOLS)/build-assets/Cargo.toml --

build-semantic:
	$(CARGO_RUN) $(TOOLS)/build-semantic/Cargo.toml --

build-full:
	$(CARGO_RUN) $(TOOLS)/build-full/Cargo.toml --

build-rom:
	$(CARGO_RUN) $(TOOLS)/build-rom/Cargo.toml --

inventory:
	$(CARGO_RUN) $(TOOLS)/overlay-inventory/Cargo.toml --

semantic-check:
	$(CARGO_RUN) $(TOOLS)/semantic-owner-scope/Cargo.toml -- --check

core-retained-check:
	$(CARGO_RUN) $(TOOLS)/core-retained-audit/Cargo.toml -- --check

sanctum:
	$(CARGO_RUN) $(TOOLS)/check-sanctum/Cargo.toml --

progress:
	$(CARGO_RUN) $(TOOLS)/full-c-progress/Cargo.toml --

progress-check:
	$(CARGO_RUN) $(TOOLS)/full-c-progress/Cargo.toml -- --check

progress-subject:
	$(CARGO_RUN) $(TOOLS)/full-c-progress/Cargo.toml -- --subject

progress-history:
	$(CARGO_RUN) $(TOOLS)/full-c-history/Cargo.toml -- --write

coverage:
	$(CARGO_RUN) $(TOOLS)/coverage-map/Cargo.toml -- --write

coverage-check:
	$(CARGO_RUN) $(TOOLS)/coverage-map/Cargo.toml -- --check

showcase:
	$(CARGO_RUN) $(TOOLS)/overlay-showcase/Cargo.toml -- --all

compiler-checks: compiler-sweep compiler-cohort overlay-compiler-cohort compiler-corpus compiler-batch

compiler-sweep:
	$(CARGO_RUN) $(TOOLS)/mode-sweep/Cargo.toml -- --self-test

compiler-cohort:
	$(CARGO_RUN) $(TOOLS)/mode-cohort/Cargo.toml -- --self-test

overlay-compiler-cohort:
	$(CARGO_RUN) $(TOOLS)/overlay-mode-cohort/Cargo.toml -- --self-test

compiler-corpus:
	$(CARGO_RUN) $(TOOLS)/compiler-corpus-regression/Cargo.toml --bin compiler-corpus-regression -- --self-test

compiler-batch:
	$(CARGO_RUN) $(TOOLS)/search-compiler-modes/Cargo.toml -- --self-test

overlay-candidate-check:
	$(CARGO_RUN) $(TOOLS)/overlay-candidate-rank/Cargo.toml -- --self-test

statement-order-check:
	$(CARGO_RUN) $(TOOLS)/statement-order-sweep-main/Cargo.toml -- --self-test

compiler-lint:
	$(CARGO_RUN) $(TOOLS)/alchemy-lints/Cargo.toml --

compiler-self-test:
	$(CARGO_RUN) $(TOOLS)/alchemy-selftest/Cargo.toml --

lint: pristine-options-check standard-check
	$(CARGO_RUN) $(TOOLS)/architecture/Cargo.toml --
	$(CARGO_RUN) $(TOOLS)/source-citations/Cargo.toml --
	$(CARGO_RUN) $(TOOLS)/no-asm-c/Cargo.toml --
	$(CARGO_RUN) $(TOOLS)/decomp-targets/Cargo.toml -- --self-test
	$(CARGO_RUN) $(TOOLS)/cache-key-lint/Cargo.toml --
	$(CARGO) run --offline --quiet --release --manifest-path $(TOOLS)/Cargo.toml --bin lang-ban --
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
	$(CARGO_RUN) $(TOOLS)/semantic-superseded/Cargo.toml -- --check
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
