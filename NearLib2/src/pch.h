#pragma once

// min/max いらない
#define NOMINMAX

// GDI いらない
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

#define WIN32_LEAN_AND_MEAN

#include <algorithm>
#include <chrono>
#include <map>
#include <unordered_map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include <DirectXMath.h>
#include <wrl/client.h>
