# Copyright 2025 KU Leuven.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Fanchen Kong <fanchen.kong@kuleuven.be>

APP  = hemaia_d2d_transfer
SRCS = src/hemaia_d2d_transfer.c
BINGO_HOST ?= 0 # True to include the bingo runtime
INCL_DEVICE_BINARY ?= 0 # True to include the device binary

# PARTIAL_OUTPUTS += data/data.h
PARTIAL_OUTPUTS += data/data_intel_bert.h
PARTIAL_OUTPUTS += data/data_llama.h
PARTIAL_OUTPUTS += bert_sparsity_suite/dense.h
PARTIAL_OUTPUTS += bert_sparsity_suite/sparse70_unstructured.h
PARTIAL_OUTPUTS += bert_sparsity_suite/sparse80_1x4_block.h
PARTIAL_OUTPUTS += bert_sparsity_suite/sparse85_unstructured.h
PARTIAL_OUTPUTS += bert_sparsity_suite/sparse90_unstructured.h

# data/data.h: data/datagen.py data/params.hjson
# 	python3 $< -c data/params.hjson > $@
	
include ../common.mk