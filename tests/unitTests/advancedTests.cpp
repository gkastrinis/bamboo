#include <gtest/gtest.h>
#include "../../index/ArrayIndex.h"
#include "../../relation/Column.h"
#include "../../relation/Relation.h"

TEST(advanced_check, relation_variations) {
	int8_t var[] = {
			0, 1, 2,
			2, 1, -1,
			2, 0, 1,
			0, -1, -1};
	Relation<int64_t> r(3, 4, var);
	int64_t values[] = {5, 10, 100};
	r.put(values);
	values[0] = 5;
	values[1] = 20;
	values[2] = 200;
	r.put(values);

	EXPECT_EQ(r.size(0), 2);
	EXPECT_EQ(r.size(1), 2);
	EXPECT_EQ(r.size(2), 2);
	EXPECT_EQ(r.size(3), 1);
}