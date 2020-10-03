#pragma once

#include <cstring>

struct StrIntern {
	const char *str;
	uint16_t strSize;
	uint64_t intern;

	explicit StrIntern(const char *source) {
		strSize = std::strlen(source);
		str = new char[strSize + 1];
		std::memcpy(str, source, strSize + 1);
	}

	// https://stackoverflow.com/questions/7666509/hash-function-for-string
	uint64_t hash() {
		uint64_t hash = 5381;
		char c;
		while (c = *str++)
			hash = ((hash << 5) + hash) + c; // hash * 33 + c
		return hash;
	}
};
