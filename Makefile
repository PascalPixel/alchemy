# Maintained source is built through ordinary Make and GNU linker rules.
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
TARGET ?= tbs-en
TARGET_GAME := $(firstword $(subst -, ,$(TARGET)))
GCC296_CFLAGS := -O2 -mthumb -mthumb-interwork -mcpu=arm7tdmi -nostdinc -fcall-used-r4
SHA1 := $(shell { command -v sha1sum || command -v shasum; } 2>/dev/null) -c

ifeq ($(TARGET_GAME),tbs)
include games/THE\ BROKEN\ SEAL/BUILD.MK
else ifeq ($(TARGET_GAME),tla)
include games/THE\ LOST\ AGE/BUILD.MK
else
$(error unknown game in TARGET=$(TARGET))
endif

# Compiler library members come from the pinned toolchain's own sources/rules.
LIBGCC := tools/out/compiler-runtime/libgcc.a

.PHONY: compiler-runtime worktree
native: compiler-runtime
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

.PHONY: help native bootstrap compilers compiler-sources compiler-source-check toolchain-check build-tools
.PHONY: compare compare-tla compare-all build-full build-rom
.PHONY: precommit prepush verify land verify-clean test tool-tests test-integration lint lint-staged lint-production
.PHONY: standard-check rustfmt-check native-format-check language-check corpus-check index-sync-check
.PHONY: publication-tree-check publication-staged-check tooling-index-check coverage coverage-check
.PHONY: progress progress-subject progress-report progress-check prepare-inputs raw similar deps clean

help:
	@printf '%s\n' \
	  'make bootstrap       install the approved toolchain from pinned source' \
	  'make native          compile/link the maintained source list' \
	  'make compare-all     compare linked source and both private ROM compositions' \
	  'make compare-editions  compare all twelve editions, the other ten through their recon scaffold' \
	  'make test            Rust tests, formatting and source policy' \
	  'make verify          verify source, publication and both ROM compositions' \
	  'make coverage        update README and both published figures' \
	  'make progress        report DONE from the linker maps of verified builds' \
	  'make raw             generate private disassembly under out/' \
	  'make similar         rank not-yet-C functions against C into out/reports/similar.tsv' \
	  'make deps            map calls and blockers of not-yet-C code into out/reports/deps-*.tsv'

bootstrap:
	$(ALCHEMY) bootstrap $(if $(BUNDLE),--from "$(BUNDLE)")

# In a new worktree, before its first build: link the main checkout's ROMs
# and installed tools, and clone its tool and game builds copy-on-write so
# the first build is incremental.
MAIN_CHECKOUT = $(abspath $(shell git rev-parse --path-format=absolute --git-common-dir)/..)
worktree:
	ln -sfn "$(MAIN_CHECKOUT)/roms" roms
	mkdir -p tools/out && for d in binutils compiler-runtime compilers; do ln -sfn "$(MAIN_CHECKOUT)/tools/out/$$d" tools/out/$$d; done
	cp -c -R "$(MAIN_CHECKOUT)/tools/out/cargo-target" tools/out/ && cp -c -R "$(MAIN_CHECKOUT)/out" .

toolchain-check:
	$(ALCHEMY) bootstrap --check

compilers: compiler-sources

compiler-sources: compiler-source-check
	sh agscc/build.sh
	$(MAKE) -C agbcc/gcc old -j1
	cd agbcc/gcc_arm && ./configure --target=arm-elf --host=i386-linux-gnu && $(MAKE) cc1 && mv cc1 ../agbcc_arm && rm -f config.bak

compiler-source-check:
	@set -e; for repo in agbcc agscc; do \
	  case "$$repo" in \
	    agbcc) approved=da598c1d918402c42c0c0d7128ba14567f3175e9;; \
	    agscc) approved=002c4218261a33c270df0a2582bcadb5a82019f0;; \
	  esac; \
	  test "$$(git rev-parse :$$repo)" = "$$approved" || { printf '%s gitlink is not approved\n' "$$repo"; exit 1; }; \
	  test "$$(env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE git -C "$$repo" rev-parse HEAD)" = "$$approved" || { printf '%s checkout is not approved\n' "$$repo"; exit 1; }; \
	  state=$$(env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE git -C "$$repo" status --porcelain --untracked-files=all -- . ':(exclude,glob)**/.DS_Store'); \
	  test -z "$$state" || { printf '%s compiler source is dirty\n' "$$repo"; exit 1; }; \
	done
	@printf 'compiler sources match approved submodules\n'

build-tools:
	$(CARGO) build --offline --release --workspace --manifest-path $(TOOLS)/Cargo.toml

compare:
	$(BUILD) rom --target tbs-en
	@grep -F ' out/tbs-en/' rom.sha1 | $(SHA1) -

compare-tla:
	$(BUILD) rom --target tla-en
	@grep -F ' out/tla-en/' rom.sha1 | $(SHA1) -

compare-all: compare compare-tla

# The other ten editions link through their scaffold under recon/<game>/<lang>,
# as pret's early builds linked a version through baserom.gba.
EDITIONS := tbs-ja tbs-de tbs-es tbs-fr tbs-it tla-ja tla-de tla-es tla-fr tla-it
COMPARE_EDITIONS := $(addprefix compare-,$(EDITIONS))
.PHONY: compare-editions compare-other-editions $(COMPARE_EDITIONS)
compare-editions: compare-all compare-other-editions
compare-other-editions: $(COMPARE_EDITIONS)
$(COMPARE_EDITIONS): compare-%:
	$(BUILD) rom --target $*
	@grep -F ' out/$*/' rom.sha1 | $(SHA1) -

build-full build-rom:
	$(BUILD) rom --target $(TARGET)

# Reading local references does not restore source catalogs or award credit.
prepare-inputs:
	@test -f roms/tbs-en.gba && test -f roms/tla-en.gba

raw:
	$(ALCHEMY) raw rebuild --target $(TARGET)

precommit:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(ALCHEMY_BIN) verify --pre-commit

prepush:
	$(ALCHEMY) verify --pre-push

verify:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(ALCHEMY_BIN) verify

# On main, before committing a landing: every gate, the tests and the
# publication (README and both figures), staged for the commit.
land:
	@$(CARGO) build --offline --quiet --release --manifest-path $(TOOLS)/alchemy/Cargo.toml
	@$(ALCHEMY_BIN) verify --land

verify-clean: toolchain-check
	$(MAKE) clean
	$(MAKE) verify

publication-tree-check:
	$(CHECK) publication --tree
	$(CHECK) layout

publication-staged-check:
	$(CHECK) publication --staged
	$(CHECK) layout

tooling-index-check:
	$(CHECK) publication --documents
	@test -f tools/alchemy/Cargo.toml && test -f tools/psynergy/Cargo.toml

standard-check:
	@set -e; actual=$$($(CHECK) routes --standard | grep -v '^-I' | sort); \
	expected=$$(printf '%s\n' $(GCC296_CFLAGS) | sort); \
	test "$$actual" = "$$expected" || { printf 'compiler flags differ\n'; exit 1; }
	@printf 'compiler standard ok\n'

rustfmt-check:
	@git ls-files -z --cached --others --exclude-standard '*.rs' | xargs -0 rustfmt --edition 2021 --check

native-format-check:
	$(ALCHEMY) format --check

lint lint-production: standard-check compiler-source-check rustfmt-check
	$(CHECK) no-asm --target tbs-en
	$(CHECK) no-asm --target tla-en

lint-staged: standard-check compiler-source-check
	@set -e; git diff --cached --name-only --diff-filter=d -- '*.rs' | while IFS= read -r source; do rustfmt --edition 2021 --check "$$source"; done
	$(CHECK) no-asm --target tbs-en
	$(CHECK) no-asm --target tla-en

language-check:
	@set -eu; scripts=$$(git ls-files --cached --others --exclude-standard | grep -E '\.(js|mjs|cjs|py|sh)$$' || true); \
	test -z "$$scripts" || { printf 'Use Rust or Bun TypeScript for tooling:\n%s\n' "$$scripts"; exit 1; }

corpus-check:
	@test ! -d draft
	@roots=$$(git ls-files -- games | cut -d/ -f2 | grep -vx COMMON | LC_ALL=C sort -u | tr '\n' '|'); \
	test "$$roots" = 'THE BROKEN SEAL|THE LOST AGE|'
	@printf 'corpus roots ok\n'

index-sync-check:
	@git diff --quiet --ignore-submodules -- || { printf 'stage tracked changes before make verify\n'; exit 1; }
	@set -e; untracked=$$(git ls-files --others --exclude-standard); test -z "$$untracked" || { printf 'untracked files:\n%s\n' "$$untracked"; exit 1; }

tool-tests:
	$(CARGO) test --offline --quiet --release --workspace --manifest-path $(TOOLS)/Cargo.toml

test-integration: toolchain-check
	$(CARGO) test --offline --quiet --release --workspace --manifest-path $(TOOLS)/Cargo.toml -- --ignored

test:
	@$(MAKE) --no-print-directory -j4 rustfmt-check native-format-check publication-tree-check tool-tests
	$(CHECK) no-asm --self-test

coverage:
	$(CHECK) coverage --write --publication

coverage-check:
	$(CHECK) coverage --check

progress:
	$(CHECK) progress

progress-subject:
	$(CHECK) progress --subject

progress-report:
	$(CHECK) progress --write-report

# A report for people only: the build and the count never read it.
similar:
	$(CARGO_RUN) $(TOOLS)/psynergy/Cargo.toml -- similar --build out/tbs-en --build out/tla-en \
	    --out out/reports/similar.tsv $(SIMILAR_FLAGS)

# A report for people only: the build and the count never read it.
deps:
	$(CARGO_RUN) $(TOOLS)/psynergy/Cargo.toml -- deps --build out/tbs-en --build out/tla-en \
	    --out-dir out/reports $(DEPS_FLAGS)

progress-check:
	$(CHECK) progress --check

clean:
	@rm -rf out
