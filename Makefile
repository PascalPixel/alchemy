.DEFAULT_GOAL := help
TOOLS := tools
CARGO ?= cargo
export CARGO_TARGET_DIR := $(CURDIR)/tools/out/cargo-target
export PATH := $(CURDIR)/tools/out/binutils/bin:$(PATH)
CARGO_RUN := $(CARGO) run --offline --quiet --release --manifest-path
ALCHEMY ?= $(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml --
ALCHEMY_BIN := $(CARGO_TARGET_DIR)/release/alchemy
BUILD := $(ALCHEMY) build
CHECK := $(ALCHEMY) check
TARGET ?= tbs-ja
SHA1 := $(shell { command -v sha1sum || command -v shasum; } 2>/dev/null) -c

LIBGCC := tools/out/compiler-runtime/libgcc.a
MODERN_CC ?= /opt/homebrew/bin/arm-none-eabi-gcc

.PHONY: compiler-runtime worktree
compiler-runtime: toolchain-check compiler-source-check
	$(BUILD) runtime --output $(LIBGCC) \
	    _call_via_rX=gcc/config/arm/lib1funcs.asm \
	    _addsub_df=dp-bit.c \
	    _si_to_df=dp-bit.c \
	    _df_to_si=dp-bit.c \
	    _pack_df=dp-bit.c \
	    _unpack_df=dp-bit.c \
	    _thenan_df=dp-bit.c \
	    _lshrdi3=gcc/libgcc2.c

$(LIBGCC):
	$(MAKE) compiler-runtime

.PHONY: help bootstrap merge-drivers fetch-dependencies compilers compiler-sources compiler-source-check toolchain-check build-tools
.PHONY: compare compare-tla compare-all build-rom
.PHONY: precommit prepush verify land test test-integration lint-staged language-check
.PHONY: publication-check publication-staged-check hygiene-check coverage-report
.PHONY: progress figure deps similar worst clean modern

help:
	@printf '%s\n' \
	  'make bootstrap       fetch locked dependencies and build the approved toolchain' \
	  'make fetch-dependencies  download only the locked Rust dependencies' \
	  'make compare-all     compare linked source and both private ROM compositions' \
	  'make compare-editions  compare all twelve editions, with Japanese as the base' \
	  'make modern          build the six ☀️ editions with a modern GCC (MODERN_CC) into out/modern: no match, every warning in out/modern/warnings.txt' \
	  'make test            the Rust tests' \
	  'make verify          every check, the tests and all twelve editions compared' \
	  'make land            on staging, before committing a landing: make verify, then the measurement the push uploads' \
	  'make coverage-report measure both games for the push; CI draws the published figures' \
	  'make progress        report DONE code and data in all six editions from verified builds' \
	  'make figure          redraw PROGRESS.png and PROGRESS_CHART.png from the verified count' \
	  'make deps            map calls and blockers of not-yet-C code into out/reports/deps-*.tsv' \
	  'make similar         the nearest C twins of every not-yet-C function in either game, into out/reports/similar.tsv' \
	  'make worst           rank mechanical cleanup signals' \
	  'alchemy diff DRAFT.c compare a C draft with the listing it would replace (tools/out/cargo-target/release/alchemy)' \
	  'alchemy adopt ADDR LABEL [NAME]  take an adopted function out of the scaffolding of all six editions' \
	  'alchemy adopt-data OBJECT LABEL...  link a data object in all six editions and take its bytes out of their scaffolds' \
	  'alchemy queues --workers N --out-dir DIR  split not-yet-C work into queues by module run or overlay'

fetch-dependencies:
	$(CARGO) fetch --locked --manifest-path $(TOOLS)/Cargo.toml

bootstrap: fetch-dependencies
	$(ALCHEMY) bootstrap $(if $(BUNDLE),--from "$(BUNDLE)")
	$(MAKE) $(LIBGCC)
	$(MAKE) merge-drivers

merge-drivers:
	git config merge.alchemy-scaffold.name "edition scaffold by ROM bytes"
	git config merge.alchemy-scaffold.driver "tools/out/cargo-target/release/alchemy merge-driver scaffold %O %A %B"
	git config merge.alchemy-lines.name "linker script by lines"
	git config merge.alchemy-lines.driver "tools/out/cargo-target/release/alchemy merge-driver lines %O %A %B"

MAIN_CHECKOUT = $(shell git rev-parse --path-format=absolute --git-common-dir)/..
worktree:
	"$(MAIN_CHECKOUT)/tools/out/cargo-target/release/alchemy" worktree "$(MAIN_CHECKOUT)"

toolchain-check:
	$(ALCHEMY) bootstrap --check

compilers: compiler-sources

compiler-sources: compiler-source-check
	$(MAKE) -C agscc -j8

compiler-source-check:
	@set -e; repo=agscc; approved=a5b70c21a48df6b5e14514d756036073d5bf1725; \
	  test "$$(git rev-parse :$$repo)" = "$$approved" || { printf '%s gitlink is not approved\n' "$$repo"; exit 1; }; \
	  test "$$(env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE git -C "$$repo" rev-parse HEAD)" = "$$approved" || { printf '%s checkout is not approved\n' "$$repo"; exit 1; }; \
	  state=$$(env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE git -C "$$repo" status --porcelain --untracked-files=all -- . ':(exclude,glob)**/.DS_Store'); \
	  test -z "$$state" || { printf '%s compiler source is dirty\n' "$$repo"; exit 1; }
	@env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE $(MAKE) -C agscc source-check
	@printf 'compiler sources match the approved agscc toolchain\n'

build-tools:
	$(CARGO) build --offline --release --workspace --manifest-path $(TOOLS)/Cargo.toml

compare:
	$(BUILD) rom --target tbs-ja
	@grep -F ' out/tbs-ja/' rom.sha1 | $(SHA1) -

compare-tla:
	$(BUILD) rom --target tla-ja
	@grep -F ' out/tla-ja/' rom.sha1 | $(SHA1) -

compare-all:
	@$(MAKE) --no-print-directory -j2 compare compare-tla

EDITIONS := tbs-en tbs-de tbs-es tbs-fr tbs-it tla-en tla-de tla-es tla-fr tla-it
COMPARE_EDITIONS := $(addprefix compare-,$(EDITIONS))
CPUS := $(shell sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
.PHONY: compare-editions compare-other-editions compare-tbs-ja compare-tla-ja $(COMPARE_EDITIONS)
compare-editions:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(MAKE) --no-print-directory -j$(CPUS) ALCHEMY=$(ALCHEMY_BIN) compare compare-tla $(COMPARE_EDITIONS)
compare-other-editions:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(MAKE) --no-print-directory -j$(CPUS) ALCHEMY=$(ALCHEMY_BIN) $(COMPARE_EDITIONS)
compare-tbs-ja: compare
compare-tla-ja: compare-tla
$(COMPARE_EDITIONS): compare-%:
	$(BUILD) rom --target $*
	@grep -F ' out/$*/' rom.sha1 | $(SHA1) -

build-rom:
	$(BUILD) rom --target $(TARGET)

precommit:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(ALCHEMY_BIN) verify --pre-commit

prepush:
	$(ALCHEMY) verify --pre-push

verify:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(ALCHEMY_BIN) verify

land:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(ALCHEMY_BIN) verify --land

publication-check:
	$(CHECK) publication --tree
	$(CHECK) layout
	$(CHECK) dead
	$(CHECK) aliases
	$(CHECK) devices
	$(CHECK) hygiene

hygiene-check:
	$(CHECK) hygiene

publication-staged-check:
	$(CHECK) publication --staged
	$(CHECK) layout

lint-staged:
	$(CHECK) no-asm --target tbs-ja --target tla-ja --target tbs-en --target tla-en

language-check:
	@set -eu; scripts=$$(git ls-files --cached --others --exclude-standard | grep -E '\.(ts|tsx|js|mjs|cjs|py|sh)$$' || true); \
	test -z "$$scripts" || { printf 'Tooling is Rust (W1):\n%s\n' "$$scripts"; exit 1; }

test:
	$(CARGO) test --offline --quiet --release --workspace --manifest-path $(TOOLS)/Cargo.toml
	$(CARGO) clippy --offline --quiet --release --workspace --all-targets --manifest-path $(TOOLS)/Cargo.toml -- -D warnings

test-integration: toolchain-check
	$(CARGO) test --offline --quiet --release --workspace --manifest-path $(TOOLS)/Cargo.toml -- --ignored

coverage-report:
	$(CHECK) coverage --report

progress:
	$(CHECK) progress

figure:
	$(ALCHEMY_BIN) figure

deps:
	$(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- deps --build out/tbs-en --build out/tla-en \
	    --out-dir out/reports $(DEPS_FLAGS)

similar:
	$(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- similar --build out/tbs-en --build out/tla-en \
	    --out out/reports/similar.tsv $(SIMILAR_FLAGS)

worst:
	$(CARGO_RUN) $(TOOLS)/alchemy/Cargo.toml -- worst --out out/reports/worst.tsv

clean:
	@rm -rf out

# pret's `make modern`: the same source through a modern GCC, in out/modern
# only and never in rom.sha1 (alchemy build modern --help).
modern:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	$(ALCHEMY_BIN) build modern --cc "$(MODERN_CC)" $(addprefix --target ,$(MODERN_TARGETS))
