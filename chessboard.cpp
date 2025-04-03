#include "chessboard.h"

ChessBoard::ChessBoard() {

    //m_sound.setAudioOutput(&m_audioOutput);
    //m_audioOutput.setVolume(50);
    
}

ChessBoard::~ChessBoard() {
    
    delete m_lastChosenPiece;
    m_lastChosenPiece = nullptr;
    
    for(ChessPiece* elem : m_piecesArr)
        delete elem;
    
    m_piecesArr.clear();
    
}

void ChessBoard::setupBoard(){
    
    drawField();
    preparePieces();
    drawPieces();
    this->update();
    this->setBackgroundBrush(QBrush(QColor(38,37,34)));

    //m_sound.setSource(QUrl(":/sounds/notify.mp3"));
    //m_sound.play();
    
}

void ChessBoard::setPlayerSide(bool side) {
    m_playerSide = side;
}

bool ChessBoard::getPlayerSide() {
    return m_playerSide;
}

void ChessBoard::drawField() {

    int check = (m_playerSide ? 0 : 1);
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            
            QGraphicsRectItem* rectangle = new QGraphicsRectItem(i*m_cellSize, j*m_cellSize, m_cellSize, m_cellSize);

            if((i+j)%2 == check)
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

        elem->setMoveFlag((m_playerSide == elem->getPieceColor() ? true : false));
        elem->setScale(0.54); //80/180
        QGraphicsBlurEffect *blur = new QGraphicsBlurEffect();
        blur->setBlurRadius(1);
        elem->setGraphicsEffect(blur);
        
        connect(elem, &ChessPiece::newPosition, this, &ChessBoard::validateTurn);
        connect(elem, &ChessPiece::pieceIsChosen, this, &ChessBoard::catchChosenPiece);
        
        this->addItem(elem);
            
    }
    
}

void ChessBoard::preparePieces() {
    
    m_piecesArr.reserve(32);
    
    int inversCoords = m_playerSide ? 0 : 7*m_cellSize;
    
    for(int i = 0; i < 8; ++i){
        
        ChessPiece* piece1 = new ChessPiece(":/icons/wp.png", m_cellSize);
        piece1->setPos(i*m_cellSize,abs(inversCoords - 6*m_cellSize));
        piece1->setPieceColor(true);
        piece1->setType(PieceType::whitePawn);
        m_piecesArr.push_back(piece1);
        
        ChessPiece* piece2 = new ChessPiece(":/icons/bp.png", m_cellSize);
        piece2->setPos(i*m_cellSize,abs(inversCoords - 1*m_cellSize));
        piece2->setPieceColor(false);
        piece2->setType(PieceType::blackPawn);
        m_piecesArr.push_back(piece2);
        
    }
    
    //rook
    ChessPiece* piece1 = new ChessPiece(":/icons/wr.png", m_cellSize);
    piece1->setPos(0*m_cellSize,abs(inversCoords - 7*m_cellSize));
    piece1->setPieceColor(true);
    piece1->setType(PieceType::rook);
    m_piecesArr.push_back(piece1);
    
    ChessPiece* piece2 = new ChessPiece(":/icons/wr.png", m_cellSize);
    piece2->setPos(7*m_cellSize,abs(inversCoords - 7*m_cellSize));
    piece2->setPieceColor(true);
    piece2->setType(PieceType::rook);
    m_piecesArr.push_back(piece2);
    
    ChessPiece* piece3 = new ChessPiece(":/icons/br.png", m_cellSize);
    piece3->setPos(0*m_cellSize,abs(inversCoords - 0*m_cellSize));
    piece3->setPieceColor(false);
    piece3->setType(PieceType::rook);
    m_piecesArr.push_back(piece3);
    
    ChessPiece* piece4 = new ChessPiece(":/icons/br.png", m_cellSize);
    piece4->setPos(7*m_cellSize,abs(inversCoords - 0*m_cellSize));
    piece4->setPieceColor(false);
    piece4->setType(PieceType::rook);
    m_piecesArr.push_back(piece4);
    
    //knight
    ChessPiece* piece5 = new ChessPiece(":/icons/wn.png", m_cellSize);
    piece5->setPos(1*m_cellSize,abs(inversCoords - 7*m_cellSize));
    piece5->setPieceColor(true);
    piece5->setType(PieceType::knight);
    m_piecesArr.push_back(piece5);
    
    ChessPiece* piece6 = new ChessPiece(":/icons/wn.png", m_cellSize);
    piece6->setPos(6*m_cellSize,abs(inversCoords - 7*m_cellSize));
    piece6->setPieceColor(true);
    piece6->setType(PieceType::knight);
    m_piecesArr.push_back(piece6);
    
    ChessPiece* piece7 = new ChessPiece(":/icons/bn.png", m_cellSize);
    piece7->setPos(1*m_cellSize,abs(inversCoords - 0*m_cellSize));
    piece7->setPieceColor(false);
    piece7->setType(PieceType::knight);
    m_piecesArr.push_back(piece7);
    
    ChessPiece* piece8 = new ChessPiece(":/icons/bn.png", m_cellSize);
    piece8->setPos(6*m_cellSize,abs(inversCoords - 0*m_cellSize));
    piece8->setPieceColor(false);
    piece8->setType(PieceType::knight);
    m_piecesArr.push_back(piece8);
    
    //bishop
    ChessPiece* piece9 = new ChessPiece(":/icons/wb.png", m_cellSize);
    piece9->setPos(2*m_cellSize,abs(inversCoords - 7*m_cellSize));
    piece9->setPieceColor(true);
    piece9->setType(PieceType::bishop);
    m_piecesArr.push_back(piece9);
    
    ChessPiece* piece10 = new ChessPiece(":/icons/wb.png", m_cellSize);
    piece10->setPos(5*m_cellSize,abs(inversCoords - 7*m_cellSize));
    piece10->setPieceColor(true);
    piece10->setType(PieceType::bishop);
    m_piecesArr.push_back(piece10);
    
    ChessPiece* piece11 = new ChessPiece(":/icons/bb.png", m_cellSize);
    piece11->setPos(2*m_cellSize,abs(inversCoords - 0*m_cellSize));
    piece11->setPieceColor(false);
    piece11->setType(PieceType::bishop);
    m_piecesArr.push_back(piece11);
    
    ChessPiece* piece12 = new ChessPiece(":/icons/bb.png", m_cellSize);
    piece12->setPos(5*m_cellSize,abs(inversCoords - 0*m_cellSize));
    piece12->setPieceColor(false);
    piece12->setType(PieceType::bishop);
    m_piecesArr.push_back(piece12);
    
    //queen and king
    ChessPiece* piece13 = new ChessPiece(":/icons/wk.png", m_cellSize);
    if(inversCoords == 0)
        piece13->setPos(4*m_cellSize,7*m_cellSize);
    else
        piece13->setPos(4*m_cellSize,abs(inversCoords - 7*m_cellSize));
    piece13->setPieceColor(true);
    piece13->setType(PieceType::king);
    m_piecesArr.push_back(piece13);
    
    ChessPiece* piece14 = new ChessPiece(":/icons/wq.png", m_cellSize);
    if(inversCoords == 0)
        piece14->setPos(3*m_cellSize,7*m_cellSize);
    else
        piece14->setPos(3*m_cellSize,abs(inversCoords - 7*m_cellSize));
    piece14->setPieceColor(true);
    piece14->setType(PieceType::queen);
    m_piecesArr.push_back(piece14);
    
    ChessPiece* piece15 = new ChessPiece(":/icons/bk.png", m_cellSize);
    if(inversCoords == 0)
        piece15->setPos(4*m_cellSize,0*m_cellSize);
    else
        piece15->setPos(4*m_cellSize,abs(inversCoords - 0*m_cellSize));
    piece15->setPieceColor(false);
    piece15->setType(PieceType::king);
    m_piecesArr.push_back(piece15);
    
    ChessPiece* piece16 = new ChessPiece(":/icons/bq.png", m_cellSize);
    if(inversCoords == 0)
        piece16->setPos(3*m_cellSize,0*m_cellSize);
    else
        piece16->setPos(3*m_cellSize,abs(inversCoords - 0*m_cellSize));
    piece16->setPieceColor(false);
    piece16->setType(PieceType::queen);
    m_piecesArr.push_back(piece16);
    
}

void ChessBoard::validateTurn(QPointF newPos, QPointF oldPos) {
    
    ChessPiece* senderPiece = qobject_cast<ChessPiece*>(QObject::sender());
    
    if(!(newPos.x() >= 0 and newPos.x() <= m_cellSize*7 and newPos.y() >= 0 and newPos.y() <= m_cellSize*7)){
        senderPiece->setPos(oldPos);
        return;
    }

    //Dont eat your ally
    ChessPiece* pieceOnPos = nullptr;

    for(auto* elem : m_piecesArr) {
        
        if(elem == nullptr)
            continue;
        
        if(elem->pos() == newPos){
            pieceOnPos = elem;
            break;
        }
    }
        
    if(pieceOnPos == nullptr or pieceOnPos->getPieceColor() != senderPiece->getPieceColor()){

        //Check possibility of move by certain type of piece
        validateTurnWithType(newPos, oldPos, senderPiece);
    }
    else{

        senderPiece->setPos(oldPos);
        
    }

}

void ChessBoard::validateTurnWithType(QPointF newPos, QPointF oldPos, ChessPiece* senderPiece) {

    QPointF delta = newPos - oldPos;
    delta /= 80;
    delta.setY(delta.y()*-1);

    int reverseForPawn = m_playerSide ? 1 : -1;

    switch(senderPiece->getType()){

    case PieceType::knight:{

        if( (delta.x() == 1 and (delta.y() == 2 or delta.y() == -2)) or
            (delta.x() == 2 and (delta.y() == 1 or delta.y() == -1)) or
            (delta.x() == -1 and (delta.y() == 2 or delta.y() == -2)) or
            (delta.x() == -2 and (delta.y() == 1 or delta.y() == -1))){
            deletePieceAt(newPos);
            senderPiece->setPos(newPos);
            m_lastChosenPiece = nullptr;
            qDebug() << "Horse";
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    case PieceType::king:{

        if(std::abs(delta.x()) <= 1 and std::abs(delta.y()) <= 1){
            deletePieceAt(newPos);
            senderPiece->setPos(newPos);
            m_lastChosenPiece = nullptr;
            qDebug() << "King";
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    case PieceType::bishop:{
        
        if(std::abs(delta.x()/delta.x()) == 1 and std::abs(delta.y()/delta.x()) == 1 and !isPieceOnWay(oldPos, newPos)) {
            deletePieceAt(newPos);
            senderPiece->setPos(newPos);
            m_lastChosenPiece = nullptr;
            qDebug() << "bishop";
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    case PieceType::rook:{

        if((delta.x() == 0 or delta.y() == 0) and !isPieceOnWay(oldPos, newPos)){
            deletePieceAt(newPos);
            senderPiece->setPos(newPos);
            m_lastChosenPiece = nullptr;
            qDebug() << "rook";
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    case PieceType::queen:{

        if(!isPieceOnWay(oldPos, newPos) and ((std::abs(delta.x()/delta.x()) == 1 and std::abs(delta.y()/delta.x()) == 1) or
            (delta.x() == 0 or delta.y() == 0))){
            deletePieceAt(newPos);
            senderPiece->setPos(newPos);
            m_lastChosenPiece = nullptr;
            qDebug() << "queen";
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    case PieceType::whitePawn:{

        if(!isPieceOnWay(oldPos, newPos) and (delta.x() == 0 and
           (delta.y() == reverseForPawn*1 or (senderPiece->getTurnsCount() == 0 and delta.y() == reverseForPawn*2)))) {
            deletePieceAt(newPos);
            senderPiece->setPos(newPos);
            m_lastChosenPiece = nullptr;
            senderPiece->plusOneToTurn();
            qDebug() << "white pawn";
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    case PieceType::blackPawn:{

        if(!isPieceOnWay(oldPos, newPos) and (delta.x() == 0 and (delta.y() == -1*reverseForPawn or
          (senderPiece->getTurnsCount() == 0 and delta.y() == -2*reverseForPawn)))) {
            deletePieceAt(newPos);
            senderPiece->setPos(newPos);
            m_lastChosenPiece = nullptr;
            senderPiece->plusOneToTurn();
            qDebug() << "black pawn";
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    default:{
        senderPiece->setPos(oldPos);
    } break;

    }

}

void ChessBoard::changeCellColorAt(QPointF pos, bool isActivated) {
    
    QGraphicsRectItem* rect = qgraphicsitem_cast<QGraphicsRectItem*>(this->itemAt(pos, QTransform()));
    
    QPointF posCopy = pos;
    //QPointF pos = rect->scenePos(); //Why it doesnt work?
    posCopy.setX(int(posCopy.x()/80));
    posCopy.setY(int(posCopy.y()/80));

    int check = (m_playerSide ? 0 : 1);
    if(int(posCopy.x() + posCopy.y()) % 2 != check)
        if(isActivated)
            rect->setBrush(QBrush(QColor(185, 202, 67)));
        else
            rect->setBrush(QBrush(QColor(115,149,82)));
    
    else
        if(isActivated)
            rect->setBrush(QBrush(QColor(245, 246, 130)));
        else
            rect->setBrush(QBrush(QColor(235,236,208)));
}

int ChessBoard::abs(int num) {

    if(num < 0)
        return -num;
    return num;

}

void ChessBoard::highlightTips(ChessPiece *senderPiece) {

    switch(senderPiece->getType()) {
        
    case PieceType::blackPawn:{
        
        int reverse = m_playerSide ? 1 : -1;
        int moveCount = (senderPiece->getTurnsCount() == 0 ? 3 : 2);
        for(int i = 1; i < moveCount; ++i){

            QPointF pieceCoord =senderPiece->scenePos();
            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + reverse*i*80));

            if(piece == nullptr) {

                drawTipAt(pieceCoord.x(), pieceCoord.y() + reverse*i*80);

            }
            else {
                
                if(piece->getPieceColor() != senderPiece->getPieceColor())
                    drawTipAt(pieceCoord.x(), pieceCoord.y() + reverse*i*80);
                break;

            }

        }
    }break;

    case PieceType::whitePawn:{

        int reverse = m_playerSide ? 1 : -1;
        int moveCount = (senderPiece->getTurnsCount() == 0 ? 3 : 2);
        for(int i = 1; i < moveCount; ++i){

            QPointF pieceCoord =senderPiece->scenePos();
            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - reverse*i*80));

            if(piece == nullptr) {

                drawTipAt(pieceCoord.x(), pieceCoord.y() - reverse*i*80);

            }
            else {
                
                if(piece->getPieceColor() != senderPiece->getPieceColor())
                    drawTipAt(pieceCoord.x(), pieceCoord.y() - reverse*i*80);
                break;

            }

        }
    }break;
        
    case PieceType::knight:{
        
        QPointF pieceCoord = senderPiece->scenePos();
        
        qDebug() << pieceCoord;
        
        //1 2
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + 2 * m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() + m_cellSize, pieceCoord.y() + 2 * m_cellSize);
        
        //1 -2
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - 2 * m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() + m_cellSize, pieceCoord.y() - 2 * m_cellSize);
        
        //2 1
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() + m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() + m_cellSize);
        
        //2 -1
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() - m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() - m_cellSize);
        
        //-1 2
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + 2 * m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() - m_cellSize, pieceCoord.y() + 2 * m_cellSize);
        
        //-1 -2
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - 2 * m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() - m_cellSize, pieceCoord.y() - 2 * m_cellSize);
        
        //-2 1
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() + m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() + m_cellSize);
        
        //-2 -1
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() - m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() - m_cellSize);
        
    }break;
        
    case PieceType::king:{
        
        QPointF pieceCoord = senderPiece->scenePos();
        
        qDebug() << pieceCoord;
        
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize);
        
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y()));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() + m_cellSize, pieceCoord.y());
        
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize);
        
        piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x(), pieceCoord.y() - m_cellSize);
        
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize);
        
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y()));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() - m_cellSize, pieceCoord.y());
        
        piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize);
        
        piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + m_cellSize));
        if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x(), pieceCoord.y() + m_cellSize);
        
    }break;

    case PieceType::bishop:{

        QPointF pieceCoord = senderPiece->scenePos();

        int counter = std::min(pieceCoord.x()/m_cellSize, pieceCoord.y()/m_cellSize);
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() - i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() - i * m_cellSize, pieceCoord.y() - i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = std::min((m_cellSize*7 - pieceCoord.x())/m_cellSize, pieceCoord.y()/m_cellSize);
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y() - i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() + i * m_cellSize, pieceCoord.y() - i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = std::min((m_cellSize*7 - pieceCoord.x())/m_cellSize, (m_cellSize*7 - pieceCoord.y())/m_cellSize);
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y() + i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() + i * m_cellSize, pieceCoord.y() + i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = std::min(pieceCoord.x()/m_cellSize, (m_cellSize*7 - pieceCoord.y())/m_cellSize);
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() + i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() - i * m_cellSize, pieceCoord.y() + i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

    }break;

    case PieceType::rook:{

        QPointF pieceCoord = senderPiece->scenePos();

        int counter = pieceCoord.y()/m_cellSize;
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x(), pieceCoord.y() - i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = (m_cellSize*7 - pieceCoord.x())/m_cellSize;
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y()));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() + i * m_cellSize, pieceCoord.y());
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = (m_cellSize*7 - pieceCoord.y())/m_cellSize;
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x(), pieceCoord.y() + i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = pieceCoord.x()/m_cellSize;
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y()));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() - i * m_cellSize, pieceCoord.y());
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

    }break;

    case PieceType::queen:{

        QPointF pieceCoord = senderPiece->scenePos();

        int counter = std::min(pieceCoord.x()/m_cellSize, pieceCoord.y()/m_cellSize);
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() - i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() - i * m_cellSize, pieceCoord.y() - i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = std::min((m_cellSize*7 - pieceCoord.x())/m_cellSize, pieceCoord.y()/m_cellSize);
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y() - i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() + i * m_cellSize, pieceCoord.y() - i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = std::min((m_cellSize*7 - pieceCoord.x())/m_cellSize, (m_cellSize*7 - pieceCoord.y())/m_cellSize);
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y() + i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() + i * m_cellSize, pieceCoord.y() + i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = std::min(pieceCoord.x()/m_cellSize, (m_cellSize*7 - pieceCoord.y())/m_cellSize);
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() + i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() - i * m_cellSize, pieceCoord.y() + i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = pieceCoord.y()/m_cellSize;
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x(), pieceCoord.y() - i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = (m_cellSize*7 - pieceCoord.x())/m_cellSize;
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y()));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() + i * m_cellSize, pieceCoord.y());
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = (m_cellSize*7 - pieceCoord.y())/m_cellSize;
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + i *m_cellSize));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x(), pieceCoord.y() + i * m_cellSize);
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

        counter = pieceCoord.x()/m_cellSize;
        for(int i = 1; i < counter + 1; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y()));

            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                drawTipAt(pieceCoord.x() - i * m_cellSize, pieceCoord.y());
                if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                    i = counter + 1;
            }
            else
                i = counter + 1;
        }

    }break;

    }

    for(QGraphicsEllipseItem* elem : m_tipsArr) {

        elem->setScale(0.54); //80/180
        this->addItem(elem);

    }

}

void ChessBoard::deleteTips() {
    
    for(QGraphicsEllipseItem* elem : m_tipsArr) {
        
        delete elem;
        
    }
    
    m_tipsArr.clear();
    
}

ChessPiece* ChessBoard::findPeiceOnCoords(QPointF pos) {

    ChessPiece* ans = nullptr;

    for(int i = 0; i < m_piecesArr.size(); ++i){

        if(pos == m_piecesArr[i]->scenePos()){

            ans = m_piecesArr[i];
            break;

        }

    }

    return ans;
}

void ChessBoard::drawTipAt(qreal x, qreal y) {
    
    QGraphicsEllipseItem* tip = new QGraphicsEllipseItem;
    tip->setPos(x + m_cellSize/3 + 3, y + m_cellSize/3 + 3);
    tip->setRect(0,0,m_cellSize/1.5 - 10,m_cellSize/1.5 - 10);
    tip->setBrush(QBrush(QColor(40,40,40,70)));
    tip->setPen(QColor(40,40,40,70));
    m_tipsArr.push_back(tip);
    
}

bool ChessBoard::isPieceOnWay(QPointF oldPos, QPointF newPos) {
    
    bool ansFlag = false;
    qreal dx = newPos.x() - oldPos.x();
    qreal dy = newPos.y() - oldPos.y();
    int stepCount = std::max(abs(dx/m_cellSize), abs(dy/m_cellSize));
    
    QPointF deltaStep;
    deltaStep.setX(dx/stepCount);
    deltaStep.setY(dy/stepCount);
    
    for(int i = 1; i < stepCount; ++i){
        
        if(findPeiceOnCoords(oldPos + i * deltaStep) != nullptr)
            ansFlag = true;
        
    }
    
    qDebug() << ansFlag << "-is piece on way";
    return ansFlag;
    
}

void ChessBoard::deletePieceAt(QPointF pos) {

    ChessPiece* piece = findPeiceOnCoords(pos);

    if(piece == nullptr)
        return;

    m_piecesArr.erase(std::find(m_piecesArr.begin(), m_piecesArr.end(), piece));
    delete piece;

}

void ChessBoard::catchChosenPiece(QPointF oldPos) {
    
    changeCellColorAt(m_lastChosenPos, false);

    m_lastChosenPiece = qobject_cast<ChessPiece*>(QObject::sender());
    m_lastChosenPos = oldPos;
    
    changeCellColorAt(m_lastChosenPos, true);
    
    deleteTips();
    highlightTips(m_lastChosenPiece);
    
}

void ChessBoard::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    
    if(event->button() != Qt::LeftButton){
        QGraphicsScene::mouseReleaseEvent(event);
        return;
    }
        
    qDebug() << "Mouse on scene released";
    if(m_lastChosenPiece != nullptr) {
        
        qDebug() << "m_lastChosenPiece not nullptr";

        QPointF newPos = event->scenePos();
        newPos.setX(int(newPos.x()/80)*80);
        newPos.setY(int(newPos.y()/80)*80);
        
        if(newPos != m_lastChosenPos){
        
            emit m_lastChosenPiece->newPosition(newPos, m_lastChosenPos);
            m_lastChosenPiece = nullptr;
            
            changeCellColorAt(m_lastChosenPos, false);
            
            deleteTips();

            //Here We lose piece focus
        
        }
        else{
            
            m_lastChosenPiece->setPos(m_lastChosenPos);
            
        }

    }

    QGraphicsScene::mouseReleaseEvent(event);

}















