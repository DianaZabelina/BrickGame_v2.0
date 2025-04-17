#include "test.h"

TEST(SnakeTest, Constructor) {
  s21::Snake snake;

  std::deque<std::pair<int, int>> expectedBody = {{START_X, START_Y},
                                                  {START_X, START_Y + 1},
                                                  {START_X, START_Y + 2},
                                                  {START_X, START_Y + 3}};
  EXPECT_EQ(snake.getBody(), expectedBody);

  EXPECT_EQ(snake.getDirection(), s21::Snake::Direction::Up);

  EXPECT_EQ(snake.getLastDirection(), s21::Snake::Direction::Up);
}

TEST(SnakeTest, IsOpposite) {
  s21::Snake snake;

  EXPECT_FALSE(snake.isOpposite(s21::Snake::Direction::Left));
  EXPECT_FALSE(snake.isOpposite(s21::Snake::Direction::Right));
  EXPECT_FALSE(snake.isOpposite(s21::Snake::Direction::Up));

  EXPECT_TRUE(snake.isOpposite(s21::Snake::Direction::Down));

  snake.setLastDirection(s21::Snake::Direction::Down);
  EXPECT_EQ(snake.getLastDirection(), s21::Snake::Direction::Down);
  EXPECT_FALSE(snake.isOpposite(s21::Snake::Direction::Down));
}

TEST(SnakeTest, SetDirection) {
  s21::Snake snake;

  snake.setDirection(s21::Snake::Direction::Left);
  EXPECT_EQ(snake.getDirection(), s21::Snake::Direction::Left);

  snake.setDirection(s21::Snake::Direction::Right);
  EXPECT_EQ(snake.getDirection(), s21::Snake::Direction::Right);

  snake.setDirection(s21::Snake::Direction::Up);
  EXPECT_EQ(snake.getDirection(), s21::Snake::Direction::Up);

  snake.setDirection(s21::Snake::Direction::Down);
  EXPECT_NE(snake.getDirection(), s21::Snake::Direction::Down);
}

TEST(SnakeTest, setLastDirection) {
  s21::Snake snake;

  snake.setLastDirection(s21::Snake::Direction::Left);
  EXPECT_EQ(snake.getLastDirection(), s21::Snake::Direction::Left);

  snake.setLastDirection(s21::Snake::Direction::Right);
  EXPECT_EQ(snake.getLastDirection(), s21::Snake::Direction::Right);

  snake.setLastDirection(s21::Snake::Direction::Up);
  EXPECT_EQ(snake.getLastDirection(), s21::Snake::Direction::Up);
}