#pragma once

#include <cassert>
#include <cstdint>

#define ARK_ASSERT assert
#define ARK_WTF(exp) ARK_ASSERT(false && (exp))

#define ARK_ARRAY_COUNT(a) (sizeof(a) / sizeof(a[0]))