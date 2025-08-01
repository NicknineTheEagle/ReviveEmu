#ifndef MURMURHASH_H
#define MURMURHASH_H

#pragma once

#include <cstdint>

uint32_t murmur3_32(const uint8_t* key, size_t len, uint32_t seed);

#endif
