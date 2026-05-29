#include <cassert>
#include <iostream>

#include "constants.h"
#include "game.h"
#include "move-list.h"

Game::Game() {}

void Game::debugPrint() const {
  std::cout << "Game Debug:";

  m_board.debugPrint();

  std::cout << '\n' << (m_whiteTurn ? "WHITE" : "BLACK") << " TURN\n";
  std::cout << "CAN CASTLE KQkq: " << m_canCastleKQkq << '\n';
  std::cout << "EN PASSANT SQUARE INDEX: ";
  if (m_enPassant == -1)
    std::cout << "NONE";
  else
    std::cout << m_enPassant;
  std::cout << '\n';
  std::cout << "HALF MOVE CLOCK: " << m_halfMoveClock << '\n';
  std::cout << "FULL MOVE COUNT: " << m_fullMoveCount << '\n';

  MoveList list = generatePseudoLegal();
  std::cout << "MOVE LIST SIZE: " << list.size() << '\n';
}

void Game::appendMoveHelper(MoveList& moves, uint8_t initialSquare, int8_t offset, uint8_t flags) const {
  moves.appendMove(m_board.at(initialSquare), initialSquare, offset, flags);
}

void Game::appendCaptureHelper(MoveList& moves, uint8_t initialSquare, int8_t offset, uint8_t flags) const {
  moves.appendCapture(m_board.at(initialSquare), m_board.at(initialSquare + offset), initialSquare, offset, flags);
}

void Game::appendPawnPromotion(MoveList& moves, uint8_t from, int8_t offset) const {
  appendMoveHelper(moves, from, offset, 0b1000);
  appendMoveHelper(moves, from, offset, 0b1001);
  appendMoveHelper(moves, from, offset, 0b1010);
  appendMoveHelper(moves, from, offset, 0b1011);
}

void Game::appendPawnPromotionCapture(MoveList& moves, uint8_t from, int8_t offset) const {
  appendCaptureHelper(moves, from, offset, 0b1100);
  appendCaptureHelper(moves, from, offset, 0b1101);
  appendCaptureHelper(moves, from, offset, 0b1110);
  appendCaptureHelper(moves, from, offset, 0b1111);
}

void Game::generatePseudoLegalPawnMoves(MoveList& moves, std::byte color, std::byte enemy,
                                        uint8_t initialSquare) const {
  int frontOffset{10};
  int captureLeftOffset{11};
  int captureRightOffset{9};
  if (color != constants::WHITE) {
    frontOffset *= -1;
    captureLeftOffset *= -1;
    captureRightOffset *= -1;
  }
  if (m_board.isAtPawnHomeRankOfColor(enemy, initialSquare)) { // is promotable

    if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + frontOffset)) {
      appendPawnPromotion(moves, initialSquare, frontOffset);
    }

    if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureRightOffset)) {
      appendPawnPromotionCapture(moves, initialSquare, captureRightOffset);
    }

    if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureLeftOffset)) {
      appendPawnPromotionCapture(moves, initialSquare, captureLeftOffset);
    }

    return; // There should be no more legal moves for a pawn if its promotable
  }

  if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + frontOffset)) {

    moves.appendMove(constants::PAWN, initialSquare, frontOffset);

    if ((m_board.isAtPawnHomeRankOfColor(color, initialSquare)) &&
        (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + (frontOffset * 2)))) {
      moves.appendMove(constants::PAWN, initialSquare, frontOffset * 2, 0b0001);
    }
  }

  if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureRightOffset)) {
    appendCaptureHelper(moves, initialSquare, captureRightOffset);
  }

  if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureLeftOffset)) {
    appendCaptureHelper(moves, initialSquare, captureLeftOffset);
  }

  if (m_enPassant == -1) {
    return;
  }

  if (constants::board64[m_enPassant] == (initialSquare + captureRightOffset)) {
    moves.appendCapture(constants::PAWN, constants::PAWN, initialSquare, captureRightOffset, 0b0101);
    return;
  }

  if (constants::board64[m_enPassant] == (initialSquare + captureLeftOffset)) {
    moves.appendCapture(constants::PAWN, constants::PAWN, initialSquare, captureLeftOffset, 0b0101);
  }
}

MoveList Game::generatePseudoLegal() const {
  MoveList moves{};
  const PieceList& list{m_whiteTurn ? m_whiteList : m_blackList};
  std::byte color{m_whiteTurn ? constants::WHITE : constants::BLACK};
  std::byte enemy{color ^ std::byte{0x80}};

  for (int i{0}; i < list.size(); ++i) {
    uint8_t initialSquare{constants::board64[list[i]]};
    uint8_t pieceInt{static_cast<uint8_t>(m_board.at(initialSquare) & std::byte{0x07})};

    if (m_board.isPieceAtIndex(constants::PAWN, initialSquare)) {
      generatePseudoLegalPawnMoves(moves, color, enemy, initialSquare);
      continue;
    }

    for (int j{0}; j < constants::offset_count[pieceInt]; ++j) {
      for (int moveOffset{constants::offsets[pieceInt][j]}, move{initialSquare};;) {
        move += moveOffset;
        if (m_board.isPieceAtIndex(constants::SENTINAL, move)) {
          break;
        }
        if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, move)) {
          appendMoveHelper(moves, initialSquare, moveOffset);
        } else {
          if (m_board.isColorAtIndex(color, move)) {
            break;
          }
          appendCaptureHelper(moves, initialSquare, moveOffset);
          break;
        }
        if (!constants::slide[pieceInt])
          break;
      }
    }
  }
  return moves;
}

void Game::makeMove(Move move) {}
