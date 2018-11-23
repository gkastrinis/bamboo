#include <gtest/gtest.h>
#include "../../relation/Relation.h"

TEST(advanced_check, relation) {
	Relation<int64_t> r(3);
	int64_t values[3];
	values[0] = 5;
	values[1] = 10;
	values[2] = 100;
	r.put(values);
	values[0] = 5;
	values[1] = 20;
	values[2] = 200;
	r.put(values);

	EXPECT_EQ(r.size(), 2);
}

TEST(advanced_check, relation_variations) {
	Variations v(3, {{2, 1},
	                 {2, 0, 1},
	                 {0}});
	Relation<int64_t> r(3, v);
	int64_t values[3];
	values[0] = 5;
	values[1] = 10;
	values[2] = 100;
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