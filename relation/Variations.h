#pragma once

#include <vector>
#include <cstdint>

struct Variations {
	// A vector with all variations
	// zero based indexing
	// -1 indicates dropping the remaining columns
	std::vector<std::vector<int8_t>> raw;

	explicit Variations(uint8_t maxArity, std::initializer_list<std::initializer_list<int8_t>> variations = {}) {
		// Default variation
		auto variationIt = raw.insert(raw.end(), std::vector<int8_t>());
		for (int8_t i = 0; i < maxArity; i++) variationIt->push_back(i);

		for (auto variation : variations) {
			// If true, all subsequent values must be negative (and be ignored)
			auto mustBeNegative = false;

			variationIt = raw.insert(raw.end(), std::vector<int8_t>());
			for (auto v : variation) {
				if (v >= maxArity) throw -2;
				if (v < 0) {
					mustBeNegative = true;
					continue;
				}
				if (mustBeNegative) throw -3;
				else variationIt->push_back(v);
			}
		}
	}

	uint8_t count() const { return (uint8_t) raw.size(); }
};