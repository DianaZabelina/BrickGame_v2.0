#include "test.h"

TEST(FieldTest, Constructor) {
  s21::Field field;

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      EXPECT_EQ(field.getBlock(j, i), 0);
    }
  }
}

TEST(FieldTest, IsInside) {
  s21::Field field;

  EXPECT_TRUE(field.isInside(0, 0));
  EXPECT_TRUE(field.isInside(FIELD_WIDTH - 1, FIELD_HEIGHT - 1));

  EXPECT_FALSE(field.isInside(-1, 0));
  EXPECT_FALSE(field.isInside(0, -1));
  EXPECT_FALSE(field.isInside(FIELD_WIDTH, 0));
  EXPECT_FALSE(field.isInside(0, FIELD_HEIGHT));
}

TEST(FieldTest, GetBlock) {
  s21::Field field;

  EXPECT_EQ(field.getBlock(0, 0), 0);
  EXPECT_EQ(field.getBlock(FIELD_WIDTH - 1, FIELD_HEIGHT - 1), 0);
}

TEST(FieldTest, SetBlock) {
  s21::Field field;

  field.setBlock(0, 0, 1);
  EXPECT_EQ(field.getBlock(0, 0), 1);

  field.setBlock(FIELD_WIDTH - 1, FIELD_HEIGHT - 1, 1);
  EXPECT_EQ(field.getBlock(FIELD_WIDTH - 1, FIELD_HEIGHT - 1), 1);
}

TEST(FieldTest, ResetField) {
  s21::Field field;

  field.setBlock(0, 0, 1);
  field.setBlock(1, 1, 2);

  field.resetField();

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      EXPECT_EQ(field.getBlock(j, i), 0);
    }
  }
}