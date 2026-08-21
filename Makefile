# Alchemy has six executables: two builders and four contributor-facing hosts.
# Game-specific asset codecs are libraries behind `assets`; they are not public
# commands and are not copied when starting another decompilation project.

GCC296_CFLAGS := -O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi \
                 -fno-builtin -nostdinc -ffreestanding \
                 -fcall-used-r4 -Iinclude

TOOLS := tools
CARGO ?= cargo
CARGO_RUN := $(CARGO) run --offline --quiet --release --manifest-path

BUILD := $(CARGO_RUN) $(TOOLS)/build-stage/Cargo.toml --
ASSETS := $(CARGO_RUN) $(TOOLS)/build-assets/Cargo.toml --
CHECK := $(CARGO_RUN) $(TOOLS)/check/Cargo.toml --
COMPILER := $(CARGO_RUN) $(TOOLS)/compiler/Cargo.toml --

HOSTS := build-assets build-stage assets compiler overlay check
CORE_TESTS := candidate-compiler candidate-show permuter overlay-adopt \
              build-full extract-resource coverage-map check-publication
PORTABLE_TOOLS := alignment-tail asset-paths cache-entry canonical-json \
	generated-files no-asm-c build-stage build-claimed build-asm build-full \
	compiler compiler-core candidate-compiler candidate-show permuter \
	shape-sweep dashboard-server overlay overlay-disasm overlay-show \
	overlay-adopt overlay-call-targets check check-commit-progress \
	check-publication check-unmatchable core-retained-audit coverage-map \
	full-c-progress integrate-matches route-dump decomp-targets
TOOLING_LINE_LIMIT := 40000

.PHONY: help verify test lint build-tools tool-tests tooling-size \
	build-claimed build-asm build-assets build-full build-rom \
	standard-check pristine-options-check corpus-check core-retained-check \
	check-owners progress progress-check progress-subject \
	correspondence correspondence-check coverage coverage-check dashboard clean clean-preview

help:
	@printf '%s\n' \
		'make verify           authoritative byte-exact gate' \
		'make build-rom        rebuild the ROM' \
		'make build-full       rebuild and compare every owned byte' \
		'make build-assets     rebuild source assets' \
		'make test             focused Rust tests and policy checks' \
		'make tooling-size     enforce the portable-toolkit budget' \
		'make progress         print byte-exact progress' \
		'make progress-subject print the required commit prefix' \
		'make correspondence   match exact EN owners across GS1 editions' \
		'make coverage         refresh dashboard data and figures' \
		'make dashboard        serve the dashboard on localhost:4649'

build-claimed:
	$(BUILD) claimed

build-asm:
	$(BUILD) asm

build-assets:
	$(ASSETS)

build-full:
	$(BUILD) full

build-rom:
	$(BUILD) rom

dashboard:
	$(COMPILER) dashboard-server --bind 0.0.0.0:4649

progress:
	$(CHECK) progress

progress-check:
	$(CHECK) progress --check

progress-subject:
	$(CHECK) progress --subject

correspondence: build-claimed
	$(COMPILER) cross-edition --all --write recon/gs1/exact-correspondence.json
	$(COMPILER) cross-edition --all-overlays --write recon/gs1/exact-overlay-correspondence.json

correspondence-check: build-claimed
	$(COMPILER) cross-edition --all --write out/exact-correspondence.check.json
	$(COMPILER) cross-edition --all-overlays --write out/exact-overlay-correspondence.check.json
	cmp recon/gs1/exact-correspondence.json out/exact-correspondence.check.json
	cmp recon/gs1/exact-overlay-correspondence.json out/exact-overlay-correspondence.check.json

coverage: correspondence
	$(CHECK) coverage --write

coverage-check: correspondence-check
	$(CHECK) coverage --check

core-retained-check:
	$(CHECK) retained --check

check-owners:
	$(CHECK) owners

corpus-check:
	@test -f recon/gs1/project.json
	@if test -d draft; then \
		printf 'legacy draft/ directory found; use recon/gs1/<edition>/\n'; \
		exit 1; \
	fi
	@if find semantic -maxdepth 1 -name '*.c' -print | grep -q .; then \
		printf 'source hypotheses belong in recon/, not semantic/ metadata\n'; \
		exit 1; \
	fi
	@printf 'corpus ok: JA base, edition-qualified candidates, exact ownership separate\n'

build-tools:
	@set -e; for host in $(HOSTS); do \
		printf '  build %-20s' "$$host"; \
		$(CARGO) build --offline --quiet --release \
			--manifest-path $(TOOLS)/$$host/Cargo.toml; \
		printf ' ok\n'; \
	done

tool-tests:
	@set -e; for crate in $(HOSTS) $(CORE_TESTS); do \
		printf '  test  %-20s' "$$crate"; \
		$(CARGO) test --offline --quiet --release \
			--manifest-path $(TOOLS)/$$crate/Cargo.toml; \
		printf ' ok\n'; \
	done

tooling-size:
	@lines=$$(find $(addprefix $(TOOLS)/,$(PORTABLE_TOOLS)) -type f \
		\( -name '*.rs' -o -name '*.js' -o -name '*.css' \) \
		-not -path '*/target/*' -print0 | xargs -0 cat | wc -l | tr -d ' '); \
	if [ "$$lines" -gt $(TOOLING_LINE_LIMIT) ]; then \
		printf 'portable tooling is %s lines; limit is %s\n' "$$lines" '$(TOOLING_LINE_LIMIT)'; \
		exit 1; \
	fi; \
	printf 'portable tooling ok: %s / %s lines\n' "$$lines" '$(TOOLING_LINE_LIMIT)'

lint: standard-check pristine-options-check
	@rustfmt --edition 2021 --check $$(find $(TOOLS) -path '*/src/*.rs' -type f -not -path '*/target/*')
	$(CHECK) no-asm

test: lint tooling-size tool-tests
	$(CHECK) publication --self-test

verify: test build-full corpus-check core-retained-check check-owners progress-check coverage-check

standard-check:
	@printf '%s\n' $(GCC296_CFLAGS) | grep -v '^-I' | sort > /tmp/alchemy-standard-makefile.txt
	@$(CHECK) routes --standard | grep -v '^-I' | sort > /tmp/alchemy-standard-routing.txt
	@diff -u /tmp/alchemy-standard-makefile.txt /tmp/alchemy-standard-routing.txt
	@printf 'compiler standard ok\n'

COMPILER_UPSTREAM := 4087bd2bd7c064da935c2a1bf798b814a125eded
OPTION_FILES := gs1cc/gcc/toplev.c gs1cc/gcc/flags.h \
	gs1cc/gcc/config/arm/arm.h gs2cc/gcc/toplev.c gs2cc/gcc/flags.h \
	gs2cc/gcc/config/arm/arm.h agbcc/gcc_arm/toplev.c \
	agbcc/gcc_arm/flags.h agbcc/gcc_arm/config/arm/arm.h

pristine-options-check:
	@cd alchemy-gcc && set -e; for file in $(OPTION_FILES); do \
		test -f "$$file" || { printf 'missing compiler option source: %s\n' "$$file"; exit 1; }; \
		git show $(COMPILER_UPSTREAM):$$file > /tmp/compiler-options-upstream.c; \
		grep -oE '"(f|m)[a-z0-9-]+"|ARM_FLAG_[A-Z0-9_]+|flag_[a-z0-9_]+' \
		  /tmp/compiler-options-upstream.c | sort -u > /tmp/compiler-options-stock.txt; \
		grep -oE '"(f|m)[a-z0-9-]+"|ARM_FLAG_[A-Z0-9_]+|flag_[a-z0-9_]+' $$file \
		  | sort -u > /tmp/compiler-options-current.txt; \
		if comm -13 /tmp/compiler-options-stock.txt /tmp/compiler-options-current.txt | grep -q .; then \
			printf 'invented compiler option in %s\n' "$$file"; exit 1; \
		fi; \
	done
	@printf 'compiler options pristine\n'

CLEAN_TREES := out work build builds dist cmatch comparisons compiler-output \
	diffs disassembly dumps m2c objdump reports analysis .cache target

clean-preview:
	@printf 'would remove generated trees (never roms/):\n'; \
	for tree in $(CLEAN_TREES); do \
		if [ -e "$$tree" ]; then printf '  %-22s %s\n' "$$tree" "$$(du -sh "$$tree" | cut -f1)"; fi; \
	done

clean:
	@for tree in $(CLEAN_TREES); do \
		if [ -e "$$tree" ]; then rm -rf -- "$$tree"; fi; \
	done
	@find $(TOOLS) -type d -name target -prune -exec rm -rf -- {} +
	@printf 'generated trees removed; roms/ untouched\n'
