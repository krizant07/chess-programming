#include <cassert>
#include <iostream>

#include "constants.h"
#include "game.h"
#include "move-list.h"

Game::Game() {}

MoveList Game::debugPrint() const {
  std::byte color{m_whiteTurn ? pieces::WHITE : pieces::BLACK};
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

  for (int i{0}; i < list.size(); ++i) {
    std::cout << "Move #" << i << ":\n";
    std::cout << "From Piece: ";
    pieces::print(list[i].fromPiece() | color);
    std::cout << '\n';
    std::cout << "To Square:  " << int(list[i].to()) << '\n';
  }

  return list;
}

void Game::printList(std::byte color) const {
  std::cout << ((color == pieces::WHITE) ? "White" : "Black") << " List:\n";
  std::cout << "Black List:\n";
  const PieceList& list{color == pieces::WHITE ? m_whiteList : m_blackList};
  for (int i{0}; i < list.size(); ++i) {
    int8_t index{list[i]};
    if (index == -1)
      break;

    pieces::print(m_board.at(index) | pieces::BLACK);
    std::cout << "at " << int(index) << '\n';
  }
}

void Game::makeMove(Move move) {
  std::byte color{m_whiteTurn ? pieces::WHITE : pieces::BLACK};
  std::byte enemy{m_whiteTurn ? pieces::BLACK : pieces::WHITE};
  Square kingside{m_whiteTurn ? H1 : H8};
  Square queenside{m_whiteTurn ? A1 : A8};
  PieceList& list = m_whiteTurn ? m_whiteList : m_blackList;
  PieceList& enemyList = m_whiteTurn ? m_blackList : m_whiteList;

  uint8_t from{move.from()};
  uint8_t to{move.to()};
  uint8_t flags{move.flags()};
  std::byte fromPiece{move.fromPiece()};

  m_board[from] = constants::EMPTY_SQUARE;
  list.move(from, to);

  m_board[to] = fromPiece;
  enemyList.remove(to);

  m_whiteTurn = !m_whiteTurn;

  if (fromPiece == pieces::PAWN) {
    m_halfMoveClock = 0;
  } else {
    ++m_halfMoveClock;
  }

  if (m_whiteTurn) { // Increment when it becomes whites turn
    ++m_fullMoveCount;
  }

  if (flags == 1) { // double pawn push
    if (color == pieces::WHITE) {
      m_enPassant = from + 10;
    } else {
      m_enPassant = from - 10;
    }
    return;
  } else {
    m_enPassant = -1;
  }

  if (flags & (1 << 1)) { // Castling
    if (flags & 1) { // Queenside
      m_board[to + 1] = pieces::ROOK;
      m_board[constants::board64[queenside]] = constants::EMPTY_SQUARE;
      list.move(constants::board64[queenside], to + 1);
    } else { // Kingside
      m_board[to - 1] = pieces::ROOK;
      m_board[constants::board64[kingside]] = constants::EMPTY_SQUARE;
      list.move(constants::board64[kingside], to - 1);
    }
    return;
  }

  if (flags & (1 << 3)) { // pawn promotion shite
    m_board[to] = pieces::codeToPiece(flags & ((1 << 2) - 1));
    return;
  }

  if ((flags & (1 << 2)) && (flags & 1)) { // En passant capture
    if (color == pieces::WHITE) {
      m_board[to - 10] = constants::EMPTY_SQUARE;
      enemyList.remove(to - 10);
    } else {
      m_board[to + 10] = constants::EMPTY_SQUARE;
      enemyList.remove(to + 10);
    }
  }
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
  int frontOffset{color == pieces::WHITE ? 10 : -10};
  int captureLeftOffset{color == pieces::WHITE ? 11 : -11};
  int captureRightOffset{color == pieces::WHITE ? 9 : -9};
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

    moves.appendMove(pieces::PAWN, initialSquare, frontOffset);

    if ((m_board.isAtPawnHomeRankOfColor(color, initialSquare)) &&
        (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, initialSquare + (frontOffset * 2)))) {
      moves.appendMove(pieces::PAWN, initialSquare, frontOffset * 2, 0b0001);
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

  if (m_enPassant == (initialSquare + captureRightOffset)) {
    moves.appendCapture(pieces::PAWN, pieces::PAWN, initialSquare, captureRightOffset, 0b0101);
    return;
  }

  if (m_enPassant == (initialSquare + captureLeftOffset)) {
    moves.appendCapture(pieces::PAWN, pieces::PAWN, initialSquare, captureLeftOffset, 0b0101);
  }
}

void Game::handleCastleGeneration(MoveList& moves, uint8_t rookPosition, uint8_t kingPosition) const {
  if (kingPosition == E1) {
    if (rookPosition == H1 && m_canCastleKQkq[0]) {
      moves.appendMove(pieces::KING, 2, 0b0010);
    } else if (rookPosition == A1 && m_canCastleKQkq[1]) {
      moves.appendMove(pieces::KING, -2, 0b0011);
    }
  } else if (kingPosition == E8) {
    if (rookPosition == H8 && m_canCastleKQkq[2]) {
      moves.appendMove(pieces::KING, 2, 0b0010);
    } else if (rookPosition == A8 && m_canCastleKQkq[3]) {
      moves.appendMove(pieces::KING, -2, 0b0011);
    }
  }
}

MoveList Game::generatePseudoLegal() const {
  MoveList moves{};
  const PieceList& list{m_whiteTurn ? m_whiteList : m_blackList};
  std::byte color{m_whiteTurn ? pieces::WHITE : pieces::BLACK};
  std::byte enemy{color ^ std::byte{(1 << 7)}};

  for (int i{0}; i < list.size(); ++i) {
    if (list[i] == -1) {
      assert(false && "Yo this should never happen yo");
    }
    uint8_t initialSquare{static_cast<uint8_t>(list[i])};
    uint8_t pieceInt{static_cast<uint8_t>(m_board.at(initialSquare) & std::byte{0x07})};

    if (m_board.isPieceAtIndex(pieces::PAWN, initialSquare)) {
      generatePseudoLegalPawnMoves(moves, color, enemy, initialSquare);
      continue;
    }

    for (int j{0}; j < pieces::offset_count[pieceInt]; ++j) {
      for (int moveOffset{pieces::offsets[pieceInt][j]}, move{initialSquare};;) {
        move += moveOffset;
        if (m_board.isPieceAtIndex(constants::SENTINAL, move)) {
          break;
        }
        if (m_board.isPieceAtIndex(constants::EMPTY_SQUARE, move)) {
          appendMoveHelper(moves, initialSquare, moveOffset);
        } else {
          if (m_board.isColorAtIndex(color, move)) {
            if (m_board.isPieceAtIndex(pieces::ROOK, initialSquare) && m_board.isPieceAtIndex(pieces::KING, move)) {
              handleCastleGeneration(moves, initialSquare, move);
            }
            break;
          }
          appendCaptureHelper(moves, initialSquare, moveOffset);
          break;
        }
        if (!pieces::slide[pieceInt])
          break;
      }
    }
  }
  return moves;
}
