// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2024 The Undal Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UNDAL_RANDOMENV_H
#define UNDAL_RANDOMENV_H

#include <crypto/sha512.h>

/** Gather non-cryptographic environment data that changes over time. */
void RandAddDynamicEnv(CSHA512& hasher);

/** Gather non-cryptographic environment data that does not change over time. */
void RandAddStaticEnv(CSHA512& hasher);

#endif // UNDAL_RANDOMENV_H
