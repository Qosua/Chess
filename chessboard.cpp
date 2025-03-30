#include "chessboard.h"

ChessBoard::ChessBoard() {
    
    
    
}
ChessBoard::~ChessBoard() {
    
    
    
}

void ChessBoard::setupBoard(){
    
    drawField();
    preparePieces();
    drawPieces();
    this->update();
    this->setBackgroundBrush(QBrush(QColor(38,37,34)));
    
}

void ChessBoard::drawField() {
    
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            
            QGraphicsRectItem* rectangle = new QGraphicsRectItem(i*m_cellSize, j*m_cellSize, m_cellSize, m_cellSize);
                
            if((i+j)%2 == 0)
                rectangle->setBrush(QBrush(QColor(235,236,208)));
            else
                rectangle->setBrush(QBrush(QColor(115,149,82)));
            
            rectangle->setPen(QPen(Qt::transparent, 0));
            
            this->addItem(rectangle);
            
        }
    }
    
}

void ChessBoard::drawPieces() {
    
    
    for(ChessPiece* elem : m_piecesArr) {
            
        elem->setScale(0.54);
        QGraphicsBlurEffect *blur = new QGraphicsBlurEffect();
        blur->setBlurRadius(1);
        elem->setGraphicsEffect(blur);
        
        connect(elem, &ChessPiece::newPosition, this, &ChessBoard::validateTurn);
        
        this->addItem(elem);
            
    }
    
}

void ChessBoard::preparePieces() {
    
    m_piecesArr.reserve(32);
    
    for(int i = 0; i < 8; ++i){
        
        ChessPiece* piece1 = new ChessPiece(":/icons/wp.png", m_cellSize);
        piece1->setPos(i*m_cellSize, 6*m_cellSize);
        piece1->setPieceColor(true);
        piece1->setType(PieceType::whitePawn);
        m_piecesArr.push_back(piece1);
        
        ChessPiece* piece2 = new ChessPiece(":/icons/bp.png", m_cellSize);
        piece2->setPos(i*m_cellSize, 1*m_cellSize);
        piece2->setPieceColor(false);
        piece2->setType(PieceType::blackPawn);
        m_piecesArr.push_back(piece2);
        
    }
    
    //rook
    ChessPiece* piece1 = new ChessPiece(":/icons/wr.png", m_cellSize);
    piece1->setPos(0*m_cellSize, 7*m_cellSize);
    piece1->setPieceColor(true);
    piece1->setType(PieceType::rook);
    m_piecesArr.push_back(piece1);
    
    ChessPiece* piece2 = new ChessPiece(":/icons/wr.png", m_cellSize);
    piece2->setPos(7*m_cellSize, 7*m_cellSize);
    piece2->setPieceColor(true);
    piece2->setType(PieceType::rook);
    m_piecesArr.push_back(piece2);
    
    ChessPiece* piece3 = new ChessPiece(":/icons/br.png", m_cellSize);
    piece3->setPos(0*m_cellSize, 0*m_cellSize);
    piece3->setPieceColor(false);
    piece3->setType(PieceType::rook);
    m_piecesArr.push_back(piece3);
    
    ChessPiece* piece4 = new ChessPiece(":/icons/br.png", m_cellSize);
    piece4->setPos(7*m_cellSize, 0*m_cellSize);
    piece4->setPieceColor(false);
    piece4->setType(PieceType::rook);
    m_piecesArr.push_back(piece4);
    
    //knight
    ChessPiece* piece5 = new ChessPiece(":/icons/wn.png", m_cellSize);
    piece5->setPos(1*m_cellSize, 7*m_cellSize);
    piece5->setPieceColor(true);
    piece5->setType(PieceType::knight);
    m_piecesArr.push_back(piece5);
    
    ChessPiece* piece6 = new ChessPiece(":/icons/wn.png", m_cellSize);
    piece6->setPos(6*m_cellSize, 7*m_cellSize);
    piece6->setPieceColor(true);
    piece6->setType(PieceType::knight);
    m_piecesArr.push_back(piece6);
    
    ChessPiece* piece7 = new ChessPiece(":/icons/bn.png", m_cellSize);
    piece7->setPos(1*m_cellSize, 0*m_cellSize);
    piece7->setPieceColor(false);
    piece7->setType(PieceType::knight);
    m_piecesArr.push_back(piece7);
    
    ChessPiece* piece8 = new ChessPiece(":/icons/bn.png", m_cellSize);
    piece8->setPos(6*m_cellSize, 0*m_cellSize);
    piece8->setPieceColor(false);
    piece8->setType(PieceType::knight);
    m_piecesArr.push_back(piece8);
    
    //bishop
    ChessPiece* piece9 = new ChessPiece(":/icons/wb.png", m_cellSize);
    piece9->setPos(2*m_cellSize, 7*m_cellSize);
    piece9->setPieceColor(true);
    piece9->setType(PieceType::bishop);
    m_piecesArr.push_back(piece9);
    
    ChessPiece* piece10 = new ChessPiece(":/icons/wb.png", m_cellSize);
    piece10->setPos(5*m_cellSize, 7*m_cellSize);
    piece10->setPieceColor(true);
    piece10->setType(PieceType::bishop);
    m_piecesArr.push_back(piece10);
    
    ChessPiece* piece11 = new ChessPiece(":/icons/bb.png", m_cellSize);
    piece11->setPos(2*m_cellSize, 0*m_cellSize);
    piece11->setPieceColor(false);
    piece11->setType(PieceType::bishop);
    m_piecesArr.push_back(piece11);
    
    ChessPiece* piece12 = new ChessPiece(":/icons/bb.png", m_cellSize);
    piece12->setPos(5*m_cellSize, 0*m_cellSize);
    piece12->setPieceColor(false);
    piece12->setType(PieceType::bishop);
    m_piecesArr.push_back(piece12);
    
    //queen and king
    ChessPiece* piece13 = new ChessPiece(":/icons/wk.png", m_cellSize);
    piece13->setPos(3*m_cellSize, 7*m_cellSize);
    piece13->setPieceColor(true);
    piece13->setType(PieceType::king);
    m_piecesArr.push_back(piece13);
    
    ChessPiece* piece14 = new ChessPiece(":/icons/wq.png", m_cellSize);
    piece14->setPos(4*m_cellSize, 7*m_cellSize);
    piece14->setPieceColor(true);
    piece14->setType(PieceType::queen);
    m_piecesArr.push_back(piece14);
    
    ChessPiece* piece15 = new ChessPiece(":/icons/bk.png", m_cellSize);
    piece15->setPos(4*m_cellSize, 0*m_cellSize);
    piece15->setPieceColor(false);
    piece15->setType(PieceType::king);
    m_piecesArr.push_back(piece15);
    
    ChessPiece* piece16 = new ChessPiece(":/icons/bq.png", m_cellSize);
    piece16->setPos(3*m_cellSize, 0*m_cellSize);
    piece16->setPieceColor(false);
    piece16->setType(PieceType::queen);
    m_piecesArr.push_back(piece16);
    
}

void ChessBoard::validateTurn(QPointF newPos, QPointF oldPos) {
    
    ChessPiece* senderPiece = qobject_cast<ChessPiece*>(QObject::sender());
    
    ChessPiece* pieceOnPos = nullptr;
    
    
    for(auto* elem : m_piecesArr){
        
        if(elem == nullptr)
            continue;
        
        if(elem->pos() == newPos){
            pieceOnPos = elem;
            break;
        }
    }
    
    if(pieceOnPos == nullptr or pieceOnPos->getPieceColor() != senderPiece->getPieceColor()){
        
        senderPiece->setPos(newPos);
    }
    
    else{
        senderPiece->setPos(oldPos);
        
    }
    
}















