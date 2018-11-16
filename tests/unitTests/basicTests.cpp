#include <gtest/gtest.h>
#include "../../index/ArrayIndex.h"
#include "../../relation/Column.h"

TEST(basic_check, test_array) {
	ArrayIndex<uint64_t> index;
	uint64_t values[] = {100, 42, 80, 4, 10};
	for (auto v : values) index.put(v);
	EXPECT_EQ(index.size(), 5);
	ASSERT_TRUE(index.contains(42));
	ASSERT_FALSE(index.contains(43));

	auto it = index.iterator();
	EXPECT_EQ(it->data(), 4);
	it->move();
	EXPECT_EQ(it->data(), 10);
	it->move();
	EXPECT_EQ(it->data(), 42);
	it->move();
	EXPECT_EQ(it->data(), 80);
	it->move();
	EXPECT_EQ(it->data(), 100);
	it->move();
	ASSERT_FALSE(it->hasData());

	index.put(42);
	index.put(42);
	EXPECT_EQ(index.size(), 5);
}

TEST(basic_check, test_array_custom_cap) {
	ArrayIndex<uint64_t> index1;
	for (uint16_t i = 0; i < 128; i++) index1.put(i);
	EXPECT_EQ(index1.size(), 128);

	EXPECT_ANY_THROW(index1.put(1000););

	ArrayIndex<uint64_t> index2(200);
	for (uint16_t i = 0; i < 128; i++) index2.put(i);
	EXPECT_EQ(index2.size(), 128);

	EXPECT_NO_THROW(index2.put(1000););
	EXPECT_EQ(index2.size(), 129);
}

TEST(basic_check, test_hash) {
	HashIndex<uint64_t> index;
	uint64_t values[] = {100, 42, 80, 4, 10};
	for (auto v : values) index.put(v);
	EXPECT_EQ(index.size(), 5);
	ASSERT_TRUE(index.contains(42));
	ASSERT_FALSE(index.contains(43));

	auto it = index.iterator();
	it->move();
	it->move();
	it->move();
	it->move();
	it->move();
	ASSERT_FALSE(it->hasData());

	index.put(42);
	index.put(42);
	EXPECT_EQ(index.size(), 5);
}

TEST(basic_check, test_column) {
	auto column = Column<uint64_t>::mkColumn(0); // value is not important
	uint64_t values[] = {100, 42, 80, 4, 10};
	for (auto v : values) column.put(v);
	EXPECT_EQ(column.values->size(), 5);
	EXPECT_NE(column.get(42), nullptr);
	EXPECT_EQ(column.get(43), nullptr);

	auto it = column.iterator();
	EXPECT_NE(column.get(4), nullptr);
	it->move();
	EXPECT_NE(column.get(10), nullptr);
	it->move();
	EXPECT_NE(column.get(42), nullptr);
	it->move();
	EXPECT_NE(column.get(80), nullptr);
	it->move();
	EXPECT_NE(column.get(100), nullptr);
	it->move();
	ASSERT_FALSE(it->hasData());
}