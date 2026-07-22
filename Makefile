# AI-generated build script for this LeetCode workspace.
#
# Usage:
#   make 0001        # builds C/0001_*.c to bin/<problem_slug>
#   make 0001 debug  # builds C/0001_*.c to bin/debug with debug flags
#
# This is intentionally structured around problem IDs first, not C-specific
# target names, so other languages can be added later without changing the
# command shape.

SHELL := /bin/sh

CC ?= cc
CSTD ?= -std=c2x
CFLAGS ?= $(CSTD) -Wall -Wextra
LDFLAGS ?=
C_RELEASE_FLAGS ?= -O2
C_DEBUG_FLAGS ?= -g -O0 -DDEBUG

BIN_DIR := bin
C_DIR := C

DEBUG ?= $(if $(filter debug,$(MAKECMDGOALS)),1,)

ifneq ($(DEBUG),)
CFLAGS += $(C_DEBUG_FLAGS)
BUILD_MODE := debug
else
CFLAGS += $(C_RELEASE_FLAGS)
BUILD_MODE := release
endif

.PHONY: help debug clean list all build

help:
	@printf '%s\n' 'Usage:'
	@printf '%s\n' '  make <problem-id>        Build a problem by ID, e.g. make 0001'
	@printf '%s\n' '  make <problem-id> debug  Build with debug flags to bin/debug, e.g. make 0001 debug'
	@printf '%s\n' '  make all                 Build every known problem'
	@printf '%s\n' '  make list                List known C problem IDs'
	@printf '%s\n' '  make clean               Remove built binaries'

# Flag target: `make 0001 debug` uses this goal only to toggle flags above.
debug:
	@:

clean:
	@rm -rf '$(BIN_DIR)'

list:
	@set -eu; \
	if [ -d '$(C_DIR)' ]; then \
		find '$(C_DIR)' -maxdepth 1 -type f -name '[0-9][0-9][0-9][0-9]_*.c' | sort | sed 's#^$(C_DIR)/##; s#_.*##'; \
	fi

all:
	@set -eu; \
	ids=$$(find '$(C_DIR)' -maxdepth 1 -type f -name '[0-9][0-9][0-9][0-9]_*.c' | sort | sed 's#^$(C_DIR)/##; s#_.*##'); \
	if [ -z "$$ids" ]; then \
		printf '%s\n' 'No problems found.'; \
		exit 0; \
	fi; \
	for id in $$ids; do \
		$(MAKE) --no-print-directory build PROBLEM_ID="$$id" DEBUG='$(DEBUG)'; \
	done

build:
	@if [ -z '$(PROBLEM_ID)' ]; then \
		printf '%s\n' 'error: missing problem ID. Try: make 0001' >&2; \
		exit 2; \
	fi
	@set -eu; \
	mkdir -p '$(BIN_DIR)'; \
	sources=$$(find '$(C_DIR)' -maxdepth 1 -type f -name '$(PROBLEM_ID)_*.c' | sort); \
	count=$$(printf '%s\n' "$$sources" | sed '/^$$/d' | wc -l); \
	if [ "$$count" -eq 0 ]; then \
		printf 'error: no C source found for problem ID %s\n' '$(PROBLEM_ID)' >&2; \
		exit 2; \
	fi; \
	if [ "$$count" -gt 1 ]; then \
		printf 'error: multiple C sources found for problem ID %s:\n' '$(PROBLEM_ID)' >&2; \
		printf '%s\n' "$$sources" >&2; \
		exit 2; \
	fi; \
	src=$$(printf '%s\n' "$$sources"); \
	base=$$(basename "$$src" .c); \
	name=$${base#$(PROBLEM_ID)_}; \
	if [ -n '$(DEBUG)' ]; then \
		out='$(BIN_DIR)'/debug; \
	else \
		out='$(BIN_DIR)'/$$name; \
	fi; \
	printf '[%s] %s -> %s\n' '$(BUILD_MODE)' "$$src" "$$out"; \
	$(CC) $(CFLAGS) "$$src" -o "$$out" $(LDFLAGS)

# Problem IDs are invoked directly, e.g. `make 0001`.
%:
	@$(MAKE) --no-print-directory build PROBLEM_ID='$@' DEBUG='$(DEBUG)'
