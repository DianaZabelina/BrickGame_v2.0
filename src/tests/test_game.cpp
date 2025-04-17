#include "test.h"

TEST(GameTest, Constructor) {
  s21::Game& game = s21::Game::getGame();

  EXPECT_EQ(game.getField().getWidth(), FIELD_WIDTH);
  EXPECT_EQ(game.getField().getHeight(), FIELD_HEIGHT);

  EXPECT_GT(game.getSnake().getLength(), 0);

  std::pair<int, int> apple = game.getApple();
  EXPECT_GE(apple.first, 0);
  EXPECT_LT(apple.first, FIELD_WIDTH);
  EXPECT_GE(apple.second, 0);
  EXPECT_LT(apple.second, FIELD_HEIGHT);

  EXPECT_EQ(game.getAppleEaten(), 0);
  EXPECT_DOUBLE_EQ(game.getBoostFactor(), 1.5);

  EXPECT_EQ(game.getGameInfo().score, 0);
  EXPECT_EQ(game.getGameInfo().level, 1);
  EXPECT_EQ(game.getGameInfo().speed, 1);
  EXPECT_EQ(game.getGameInfo().pause, 0);
}

TEST(GameTest, SetLastActionTimeUpdatesTimeCorrectly) {
  s21::Game& game = s21::Game::getGame();
  game.resetGame();

  auto newTime = std::chrono::steady_clock::now();
  game.setLastActionTime(newTime);

  auto updatedTime = game.getLastActionTime();
  EXPECT_EQ(updatedTime, newTime);
}

TEST(GameTest, ResetSnake) {
  s21::Game& game = s21::Game::getGame();
  game.resetGame();

  std::deque<std::pair<int, int>>& body = game.getSnake().getBody();
  game.addSnake();

  for (size_t i = 0; i < body.size(); i++) {
    int x = body[i].first;
    int y = body[i].second;
    EXPECT_NE(game.getField().getBlock(x, y), 0);
  }

  game.resetSnake();

  for (size_t i = 0; i < body.size(); i++) {
    int x = body[i].first;
    int y = body[i].second;
    EXPECT_EQ(game.getField().getBlock(x, y), 0);
  }
}

TEST(GameTest, ResetGame) {
  s21::Game& game = s21::Game::getGame();

  game.getSnake().setDirection(s21::Snake::Direction::Right);
  game.updateSnake();
  game.getSnake().setDirection(s21::Snake::Direction::Down);
  game.updateSnake();
  game.getSnake().setDirection(s21::Snake::Direction::Left);
  game.updateSnake();
  game.getSnake().setDirection(s21::Snake::Direction::Up);
  game.updateSnake();
  game.setPlaying(s21::GAMEOVER);
  game.setBoost();
  game.setPause(1);

  game.resetGame();

  EXPECT_EQ(game.getSnake().getLength(), 4);
  EXPECT_EQ(game.getPlaying(), s21::PLAYING);
  EXPECT_EQ(game.getBoost(), 0);
  EXPECT_EQ(game.getGameInfo().score, 0);
  EXPECT_EQ(game.getGameInfo().level, 1);
  EXPECT_EQ(game.getInfoSpeed(), 1);
  EXPECT_EQ(game.getGameInfo().pause, 0);
}

TEST(GameTest, SnakeCollision) {
  s21::Game& game = s21::Game::getGame();

  game.getSnake().getBody().front() = {-1, 0};

  EXPECT_TRUE(game.snakeCollision());
}

TEST(GameTest, AppleCollision) {
  s21::Game& game = s21::Game::getGame();

  std::pair<int, int> apple = game.getApple();
  game.getSnake().getHead() = apple;

  EXPECT_FALSE(game.appleCollision());
}

TEST(GameTest, CalculateTurn) {
  s21::Game& game = s21::Game::getGame();

  std::pair<int, int> apple = game.getApple();
  game.getSnake().getBody().front() = apple;

  game.calculateTurn();

  EXPECT_GT(game.getGameInfo().score, 0);

  std::pair<int, int> newApple = game.getApple();
  EXPECT_NE(newApple, apple);
}