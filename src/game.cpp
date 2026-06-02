#include <cassert>
#include <iostream>

#include "constants.h"
#include "game.h"
#include "move-list.h"

Game::Game() {}

MoveList Game::debugPrint() {
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

  MoveList moveList = generatePseudoLegal();
  std::cout << "MOVE LIST SIZE: " << moveList.size() << '\n';

  return moveList;
}

void Game::printMovesOfSquare(MoveList& moveList, enum Square sq) {
  Piece& piece{m_board[constants::board64[sq]]};
  PieceList& pieceList{piece.color() == pieces::WHITE ? m_whiteList : m_blackList};
  uint8_t pieceListIndex{piece.pieceListIndex()};
  uint8_t moveListIndex{pieceList.moveIndex(pieceListIndex)};
  Move move{};

  piece.print();
  std::cout << " has the moves:\n";

  for (int i{0}; i < pieceList.moveCount(pieceListIndex); ++i) {
    move = moveList[moveListIndex + i];
    std::cout << move;
  }
}

void Game::makeMove(Move move) {
  std::byte color{m_whiteTurn ? pieces::WHITE : pieces::BLACK};
  Square kingside{m_whiteTurn ? H1 : H8};
  Square queenside{m_whiteTurn ? A1 : A8};
  PieceList& list = m_whiteTurn ? m_whiteList : m_blackList;
  PieceList& enemyList = m_whiteTurn ? m_blackList : m_whiteList;

  uint8_t from{move.from()};
  uint8_t to{move.to()};
  uint8_t flags{move.flags()};

  Piece& fromPiece{m_board[from]};
  Piece& toPiece{m_board[to]};

  m_whiteTurn = !m_whiteTurn;

  if (fromPiece.code() == pieces::PAWN) {
    m_halfMoveClock = 0;
  } else {
    ++m_halfMoveClock;
  }

  if (m_whiteTurn) { // Increment when it becomes whites turn
    ++m_fullMoveCount;
  }

  list.move(fromPiece.pieceListIndex(), to);
  if (toPiece.code() != pieces::EMPTY) {
    listRemoveHelper(enemyList, to);
  }
  m_board[to] = fromPiece;
  fromPiece.setCode(pieces::EMPTY);

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
      m_board[to + 1].setCode(pieces::ROOK);
      m_board[to + 1].setColor(color);
      m_board[constants::board64[queenside]].setCode(pieces::EMPTY);
      list.move(m_board[constants::board64[queenside]].pieceListIndex(), to + 1);
    } else { // Kingside
      m_board[to - 1].setCode(pieces::ROOK);
      m_board[to - 1].setColor(color);
      m_board[constants::board64[kingside]].setCode(pieces::EMPTY);
      list.move(m_board[constants::board64[kingside]].pieceListIndex(), to - 1);
    }
    return;
  }

  if (flags & (1 << 3)) { // pawn promotion shite
    m_board[to].setCode(flags & (0b0011));
    m_board[to].setColor(color); // redundant i think?
    return;
  }

  if (flags & (1 << 2)) { // Capture
    m_halfMoveClock = 0;
    if ((flags & 1)) { // En passant capture
      if (color == pieces::WHITE) {
        m_board[to - 10].setCode(pieces::EMPTY);
        listRemoveHelper(enemyList, to - 10);
      } else {
        m_board[to + 10].setCode(pieces::EMPTY);
        listRemoveHelper(enemyList, to + 10);
      }
    }
  }
}

void Game::listRemoveHelper(PieceList& list, uint8_t boardIndex) {
  const Piece& piece{m_board[boardIndex]};
  m_board[list.remove(piece.pieceListIndex())].setPieceListIndex(piece.pieceListIndex());
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

uint8_t Game::generatePseudoLegalPawnMoves(MoveList& moves, std::byte color, std::byte enemy,
                                           uint8_t initialSquare) const {
  uint8_t count{};
  int frontOffset{color == pieces::WHITE ? 10 : -10};
  int captureLeftOffset{color == pieces::WHITE ? 11 : -11};
  int captureRightOffset{color == pieces::WHITE ? 9 : -9};
  if (m_board.isAtPawnHomeRankOfColor(enemy, initialSquare)) { // is promotable

    if (m_board.isPieceAtIndex(pieces::EMPTY, initialSquare + frontOffset)) {
      appendPawnPromotion(moves, initialSquare, frontOffset);
      count += 4;
    }

    if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureRightOffset)) {
      appendPawnPromotionCapture(moves, initialSquare, captureRightOffset);
      count += 4;
    }

    if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureLeftOffset)) {
      appendPawnPromotionCapture(moves, initialSquare, captureLeftOffset);
      count += 4;
    }

    return count; // There should be no more legal moves for a pawn if its promotable
  }

  if (m_board.isPieceAtIndex(pieces::EMPTY, initialSquare + frontOffset)) {

    moves.appendMove(pieces::PAWN, initialSquare, frontOffset);
    ++count;

    if ((m_board.isAtPawnHomeRankOfColor(color, initialSquare)) &&
        (m_board.isPieceAtIndex(pieces::EMPTY, initialSquare + (frontOffset * 2)))) {
      moves.appendMove(pieces::PAWN, initialSquare, frontOffset * 2, 0b0001);
      ++count;
    }
  }

  if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureRightOffset)) {
    appendCaptureHelper(moves, initialSquare, captureRightOffset);
    ++count;
  }

  if (m_board.isPieceOfColorAtIndex(enemy, initialSquare + captureLeftOffset)) {
    appendCaptureHelper(moves, initialSquare, captureLeftOffset);
    ++count;
  }

  if (m_enPassant == -1) {
    return count;
  }

  if (m_enPassant == (initialSquare + captureRightOffset)) {
    moves.appendCapture(pieces::PAWN, pieces::PAWN, initialSquare, captureRightOffset, 0b0101);
    ++count;
    return count;
  }

  if (m_enPassant == (initialSquare + captureLeftOffset)) {
    moves.appendCapture(pieces::PAWN, pieces::PAWN, initialSquare, captureLeftOffset, 0b0101);
    ++count;
  }

  return count;
}

uint8_t Game::handleCastleGeneration(MoveList& moves, uint8_t rookPosition, uint8_t kingPosition) const {
  if (kingPosition == constants::board64[E1]) {
    if (rookPosition == constants::board64[H1] && m_canCastleKQkq[0]) {
      moves.appendMove(pieces::KING, constants::board64[kingPosition], 2, 0b0010);
      return 1;
    } else if (rookPosition == constants::board64[A1] && m_canCastleKQkq[1]) {
      moves.appendMove(pieces::KING, constants::board64[kingPosition], -2, 0b0011);
      return 1;
    }
  } else if (kingPosition == constants::board64[E8]) {
    if (rookPosition == constants::board64[H8] && m_canCastleKQkq[2]) {
      moves.appendMove(pieces::KING, constants::board64[kingPosition], 2, 0b0010);
      return 1;
    } else if (rookPosition == constants::board64[A8] && m_canCastleKQkq[3]) {
      moves.appendMove(pieces::KING, constants::board64[kingPosition], -2, 0b0011);
      return 1;
    }
  }
  return 0;
}

MoveList Game::generatePseudoLegal() {
  MoveList moves{};
  PieceList& list{m_whiteTurn ? m_whiteList : m_blackList};
  std::byte color{m_whiteTurn ? pieces::WHITE : pieces::BLACK};
  std::byte enemy{m_whiteTurn ? pieces::BLACK : pieces::WHITE};
  uint8_t count{};
  uint8_t moveListIndex{};

  for (int i{0}; i < list.size(); ++i) {
    count = 0;
    if (list[i] == -1) {
      assert(false && "Yo this should never happen yo");
    }
    uint8_t initialSquare{static_cast<uint8_t>(list[i])};
    uint8_t pieceInt{static_cast<uint8_t>(m_board.at(initialSquare) & std::byte{0x07})};

    if (m_board.isPieceAtIndex(pieces::PAWN, initialSquare)) {
      count = generatePseudoLegalPawnMoves(moves, color, enemy, initialSquare);
      list.setMoveCount(i, count);
      list.setMoveIndex(i, moveListIndex);
      moveListIndex += count;
      continue;
    }

    for (int j{0}; j < pieces::offset_count[pieceInt]; ++j) {
      for (int moveOffset{pieces::offsets[pieceInt][j]}, move{initialSquare};;) {
        move += moveOffset;
        if (m_board.isPieceAtIndex(pieces::SENTINAL, move)) {
          break;
        }
        if (m_board.isPieceAtIndex(pieces::EMPTY, move)) {
          appendMoveHelper(moves, initialSquare, moveOffset);
          ++count;
        } else {
          if (m_board.isColorAtIndex(color, move)) {
            if (m_board.isPieceAtIndex(pieces::ROOK, initialSquare) && m_board.isPieceAtIndex(pieces::KING, move)) {
              count += handleCastleGeneration(moves, initialSquare, move);
            }
            break;
          }
          appendCaptureHelper(moves, initialSquare, moveOffset);
          ++count;
          break;
        }
        if (!pieces::slide[pieceInt])
          break;
      }
    }
    list.setMoveCount(i, count);
    list.setMoveIndex(i, moveListIndex);

    moveListIndex += count;
  }
  return moves;
}
