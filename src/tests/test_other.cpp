#include "test.h"

TEST(UserInputTest, HandleDirectionChanges) {
  s21::Game& game = s21::Game::getGame();
  game.resetGame();

  userInput(Left, 1);
  EXPECT_EQ(game.getSnake().getDirection(), s21::Snake::Direction::Left);

  userInput(Right, 1);
  EXPECT_EQ(game.getSnake().getDirection(), s21::Snake::Direction::Right);

  userInput(Up, 1);
  EXPECT_EQ(game.getSnake().getDirection(), s21::Snake::Direction::Up);

  userInput(Down, 1);
  EXPECT_NE(game.getSnake().getDirection(), s21::Snake::Direction::Down);
}

TEST(UserInputTest, HandleBoost) {
  s21::Game& game = s21::Game::getGame();
  game.resetGame();

  userInput(Action, 1);
  EXPECT_TRUE(game.getBoost());
}

TEST(UserInputTest, HandlePause) {
  s21::Game& game = s21::Game::getGame();
  game.resetGame();

  userInput(Pause, 1);
  EXPECT_EQ(game.getGameInfo().pause, 1);

  userInput(Pause, 1);
  EXPECT_EQ(game.getGameInfo().pause, 0);
}

TEST(UserInputTest, HandleTerminate) {
  s21::Game& game = s21::Game::getGame();
  game.resetGame();

  userInput(Terminate, false);
  EXPECT_EQ(game.getPlaying(), s21::GAMEOVER);
  EXPECT_EQ(game.getGameInfo().pause, 0);
}

TEST(UpdateCurrentStateTest, UpdateGameState) {
  s21::Game& game = s21::Game::getGame();
  game.resetGame();

  game.getSnake().setDirection(s21::Snake::Direction::Right);
  game.updateSnake();

  updateCurrentState();

  std::pair<int, int> head = game.getSnake().getHead();
  EXPECT_EQ(head.first, START_X + 1);
  EXPECT_EQ(head.second, START_Y);

  EXPECT_EQ(game.getGameInfo().field[head.second][head.first], 2);
}

TEST(GetFrameDelayLeftTest, ReturnFrameDelayLeft) {
  long* frameDelayLeft = s21::getFrameDelayLeft();
  EXPECT_NE(frameDelayLeft, nullptr);
  EXPECT_EQ(*frameDelayLeft, FRAME_DELAY_NANO);
}