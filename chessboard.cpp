#include "chessboard.h"

ChessBoard::ChessBoard() {

    m_soundPlayer.setAudioOutput(&m_audioOutput);
    m_audioOutput.setVolume(50);
    
}

ChessBoard::~ChessBoard() {
    
    delete m_lastChosenPiece;
    m_lastChosenPiece = nullptr;
    
    for(ChessPiece* elem : m_piecesArr)
        delete elem;
    
    for(QGraphicsEllipseItem* elem : m_tipsArr)
        delete elem;
    
    m_piecesArr.clear();
    m_tipsArr.clear();
    
}

void ChessBoard::setupBoard(){
    
    drawField();
    preparePieces();
    drawPieces();
    this->update();
    this->setBackgroundBrush(QBrush(QColor(38,37,34)));

    m_soundPlayer.setSource(QUrl::fromLocalFile(notifySoundPath));
    m_soundPlayer.play();
    
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
    
    if(m_playerSide) {
        m_playerKing = piece13;
        m_enemyKing = piece15;
        
    }
    else {
        m_playerKing = piece15;
        m_enemyKing = piece13;
        
        
    }
    
    
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
        
        if(m_playerSide == senderPiece->getPieceColor()){

            if( (delta.x() == 1 and (delta.y() == 2 or delta.y() == -2)) or
                (delta.x() == 2 and (delta.y() == 1 or delta.y() == -1)) or
                (delta.x() == -1 and (delta.y() == 2 or delta.y() == -2)) or
                (delta.x() == -2 and (delta.y() == 1 or delta.y() == -1))){
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_playerKing != nullptr and isPieceChecked(m_playerKing)) {
                    
                    senderPiece->setPos(oldPos);
                    
                }
                else {
                    
                    bool captureFlag = deletePiece(pieceToDel);
                    if(isPieceChecked(m_enemyKing)){
                        m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                        m_soundPlayer.play();
                        
                    }
                    else{
                        if(!captureFlag) {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                            m_soundPlayer.play();
                        }
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                qDebug() << "Horse";
            }
            else
                senderPiece->setPos(oldPos);
        }
        else{
            
            if( (delta.x() == 1 and (delta.y() == 2 or delta.y() == -2)) or
                (delta.x() == 2 and (delta.y() == 1 or delta.y() == -1)) or
                (delta.x() == -1 and (delta.y() == 2 or delta.y() == -2)) or
                (delta.x() == -2 and (delta.y() == 1 or delta.y() == -1))){
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_enemyKing != nullptr and isPieceChecked(m_enemyKing)) {
                    
                    senderPiece->setPos(oldPos);
                    
                }
                else {
                    
                    bool captureFlag = deletePiece(pieceToDel);
                    if(isPieceChecked(m_playerKing)){
                        m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                        m_soundPlayer.play();
                        
                    }
                    else{
                        if(!captureFlag) {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                            m_soundPlayer.play();
                        }
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                qDebug() << "Horse";
            }
            else
                senderPiece->setPos(oldPos);
            
        }


    } break;

    case PieceType::king:{
        
        if(m_playerSide == senderPiece->getPieceColor()){
            
            if(std::abs(delta.x()) <= 1 and std::abs(delta.y()) <= 1){
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_playerKing != nullptr and isPieceChecked(m_playerKing)) {
                    
                    senderPiece->setPos(oldPos);
                    
                }
                else {
                    
                    bool captureFlag = deletePiece(pieceToDel);
                    if(isPieceChecked(m_enemyKing)){
                        m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                        m_soundPlayer.play();
                        
                    }
                    else{
                        if(!captureFlag) {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                            m_soundPlayer.play();
                        }
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                qDebug() << "King";
            }
            else
                senderPiece->setPos(oldPos);
        
        }
        else{
            
            if(std::abs(delta.x()) <= 1 and std::abs(delta.y()) <= 1){
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_enemyKing != nullptr and isPieceChecked(m_enemyKing)) {
                    
                    senderPiece->setPos(oldPos);
                    
                }
                else {
                    
                    bool captureFlag = deletePiece(pieceToDel);
                    if(isPieceChecked(m_playerKing)){
                        m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                        m_soundPlayer.play();
                        
                    }
                    else{
                        if(!captureFlag) {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                            m_soundPlayer.play();
                        }
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                qDebug() << "King";
            }
            else
                senderPiece->setPos(oldPos);
            
        }


    } break;

    case PieceType::bishop:{
        
        if(m_playerSide == senderPiece->getPieceColor()) {
        
            if(std::abs(delta.x()/delta.x()) == 1 and std::abs(delta.y()/delta.x()) == 1 and !isPieceOnWay(oldPos, newPos)) {
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_playerKing != nullptr and isPieceChecked(m_playerKing)) {
                    
                    senderPiece->setPos(oldPos);
                    
                }
                else {
                    
                    bool captureFlag = deletePiece(pieceToDel);
                    if(isPieceChecked(m_enemyKing)){
                        m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                        m_soundPlayer.play();
                        
                    }
                    else{
                        if(!captureFlag) {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                            m_soundPlayer.play();
                        }
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                qDebug() << "bishop";
            }
            else
                senderPiece->setPos(oldPos);
        }
        else {
            
            if(std::abs(delta.x()/delta.x()) == 1 and std::abs(delta.y()/delta.x()) == 1 and !isPieceOnWay(oldPos, newPos)) {
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_enemyKing != nullptr and isPieceChecked(m_enemyKing)) {
                    
                    senderPiece->setPos(oldPos);
                    
                }
                else {
                    
                    bool captureFlag = deletePiece(pieceToDel);
                    if(isPieceChecked(m_playerKing)){
                        m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                        m_soundPlayer.play();
                        
                    }
                    else{
                        if(!captureFlag) {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                            m_soundPlayer.play();
                        }
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                qDebug() << "bishop";
            }
            else
                senderPiece->setPos(oldPos);
            
        }


    } break;

    case PieceType::rook:{
        
        if(m_playerSide == senderPiece->getPieceColor()) {

            if((delta.x() == 0 or delta.y() == 0) and !isPieceOnWay(oldPos, newPos)){
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_playerKing != nullptr and isPieceChecked(m_playerKing)) {
                    
                    senderPiece->setPos(oldPos);
                    
                }
                else {
                    
                    bool captureFlag = deletePiece(pieceToDel);
                    if(isPieceChecked(m_enemyKing)){
                        m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                        m_soundPlayer.play();
                        
                    }
                    else{
                        if(!captureFlag) {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                            m_soundPlayer.play();
                        }
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                qDebug() << "rook";
            }
            else
                senderPiece->setPos(oldPos);
        }
        else {
            
            if((delta.x() == 0 or delta.y() == 0) and !isPieceOnWay(oldPos, newPos)){
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_enemyKing != nullptr and isPieceChecked(m_enemyKing)) {
                    
                    senderPiece->setPos(oldPos);
                    
                }
                else {
                    
                    bool captureFlag = deletePiece(pieceToDel);
                    if(isPieceChecked(m_playerKing)){
                        m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                        m_soundPlayer.play();
                        
                    }
                    else{
                        if(!captureFlag) {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                            m_soundPlayer.play();
                        }
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                qDebug() << "rook";
            }
            else
                senderPiece->setPos(oldPos);
            
        }


    } break;

    case PieceType::queen:{
        
        if(m_playerSide == senderPiece->getPieceColor()) {

            if(!isPieceOnWay(oldPos, newPos) and ((std::abs(delta.x()/delta.x()) == 1 and std::abs(delta.y()/delta.x()) == 1) or
                (delta.x() == 0 or delta.y() == 0))) {
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_playerKing != nullptr and isPieceChecked(m_playerKing)) {
                    
                    senderPiece->setPos(oldPos);
                    
                }
                else {
                    
                    bool captureFlag = deletePiece(pieceToDel);
                    if(isPieceChecked(m_enemyKing)){
                        m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                        m_soundPlayer.play();
                        
                    }
                    else{
                        if(!captureFlag) {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                            m_soundPlayer.play();
                        }
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                qDebug() << "queen";
            }
            else
                senderPiece->setPos(oldPos);
        
        }
        else{
            
            if(!isPieceOnWay(oldPos, newPos) and ((std::abs(delta.x()/delta.x()) == 1 and std::abs(delta.y()/delta.x()) == 1) or
                                                   (delta.x() == 0 or delta.y() == 0))) {
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_enemyKing != nullptr and isPieceChecked(m_enemyKing)) {
                    
                    senderPiece->setPos(oldPos);
                    
                }
                else {
                    
                    bool captureFlag = deletePiece(pieceToDel);
                    if(isPieceChecked(m_playerKing)){
                        m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                        m_soundPlayer.play();
                        
                    }
                    else{
                        if(!captureFlag) {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else {
                            m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                            m_soundPlayer.play();
                        }
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                qDebug() << "queen";
            }
            else
                senderPiece->setPos(oldPos);
            
        }


    } break;

    case PieceType::whitePawn:{
        
        if(m_playerSide == senderPiece->getPieceColor()) {
        
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            
            if((delta.x() == 1 or delta.x() == -1) and delta.y() == reverseForPawn*1 and pieceToDel != nullptr){
                
                senderPiece->setPos(newPos);
                deletePiece(pieceToDel);
                if(isPieceChecked(m_enemyKing)){
                    m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                    m_soundPlayer.play();
                    
                }
                else{
                    m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                    m_soundPlayer.play();
                }
                
                return;            
                
            }
    
            if(!isPieceOnWay(oldPos, newPos) and (delta.x() == 0 and
               (delta.y() == reverseForPawn*1 or (senderPiece->getTurnsCount() == 0 and delta.y() == reverseForPawn*2)))) {
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_playerKing != nullptr and isPieceChecked(m_playerKing)) {
                    
                    senderPiece->setPos(oldPos);
                    return;
                    
                }
                else {
                    if(pieceToDel == nullptr)
                        if(isPieceChecked(m_enemyKing)){
                            m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else{
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                        }
                    else{
                        senderPiece->setPos(oldPos);
                        return;
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                senderPiece->plusOneToTurn();
                qDebug() << "white pawn";
            }
            else
                senderPiece->setPos(oldPos);
        }
        else {
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            
            if((delta.x() == 1 or delta.x() == -1) and delta.y() == reverseForPawn*1 and pieceToDel != nullptr){
                
                senderPiece->setPos(newPos);
                deletePiece(pieceToDel);
                if(isPieceChecked(m_playerKing)){
                    m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                    m_soundPlayer.play();
                    
                }
                else{
                    m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                    m_soundPlayer.play();
                }
                
                return;            
                
            }
            
            if(!isPieceOnWay(oldPos, newPos) and (delta.x() == 0 and
                                                   (delta.y() == reverseForPawn*1 or (senderPiece->getTurnsCount() == 0 and delta.y() == reverseForPawn*2)))) {
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_enemyKing != nullptr and isPieceChecked(m_enemyKing)) {
                    
                    senderPiece->setPos(oldPos);
                    return;
                    
                }
                else {
                    if(pieceToDel == nullptr)
                        if(isPieceChecked(m_playerKing)){
                            m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else{
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                        }
                    else{
                        senderPiece->setPos(oldPos);
                        return;
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                senderPiece->plusOneToTurn();
                qDebug() << "white pawn";
            }
            else
                senderPiece->setPos(oldPos);
        }


    } break;

    case PieceType::blackPawn:{
        
        if(m_playerSide == senderPiece->getPieceColor()) {
        
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            
            if((delta.x() == 1 or delta.x() == -1) and delta.y() == -reverseForPawn*1 and pieceToDel != nullptr){
                
                senderPiece->setPos(newPos);
                deletePiece(pieceToDel);
                if(isPieceChecked(m_enemyKing)){
                    m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                    m_soundPlayer.play();
                    
                }
                else{
                    m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                    m_soundPlayer.play();
                }
                
                return;            
                
            }
            
            if(!isPieceOnWay(oldPos, newPos) and (delta.x() == 0 and
            (delta.y() == -reverseForPawn*1 or (senderPiece->getTurnsCount() == 0 and delta.y() == -reverseForPawn*2)))) {
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_playerKing != nullptr and isPieceChecked(m_playerKing)) {
                    
                    senderPiece->setPos(oldPos);
                    return;
                    
                }
                else {
                    if(pieceToDel == nullptr)
                        if(isPieceChecked(m_enemyKing)){
                            m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else{
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                        }
                    else{
                        senderPiece->setPos(oldPos);
                        return;
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                senderPiece->plusOneToTurn();
                qDebug() << "white pawn";
            }
            else
                senderPiece->setPos(oldPos);
        
        }
        else {
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            
            if((delta.x() == 1 or delta.x() == -1) and delta.y() == -reverseForPawn*1 and pieceToDel != nullptr){
                
                senderPiece->setPos(newPos);
                deletePiece(pieceToDel);
                if(isPieceChecked(m_playerKing)){
                    m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                    m_soundPlayer.play();
                    
                }
                else{
                    m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
                    m_soundPlayer.play();
                }
                
                return;            
                
            }
            
            if(!isPieceOnWay(oldPos, newPos) and (delta.x() == 0 and
                                                   (delta.y() == -reverseForPawn*1 or (senderPiece->getTurnsCount() == 0 and delta.y() == -reverseForPawn*2)))) {
                
                ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
                
                senderPiece->setPos(newPos);
                if(m_enemyKing != nullptr and isPieceChecked(m_enemyKing)) {
                    
                    senderPiece->setPos(oldPos);
                    return;
                    
                }
                else {
                    if(pieceToDel == nullptr)
                        if(isPieceChecked(m_playerKing)){
                            m_soundPlayer.setSource(QUrl::fromLocalFile(checkSoundPath));
                            m_soundPlayer.play();
                            
                        }
                        else{
                            m_soundPlayer.setSource(QUrl::fromLocalFile(moveSoundPath));
                            m_soundPlayer.play();
                        }
                    else{
                        senderPiece->setPos(oldPos);
                        return;
                    }
                    
                }
                
                m_lastChosenPiece = nullptr;
                senderPiece->plusOneToTurn();
                qDebug() << "white pawn";
            }
            else
                senderPiece->setPos(oldPos);
            
        }


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
        QPointF pieceCoord =senderPiece->scenePos();
        
        ChessPiece* piece1 = findPeiceOnCoords(QPointF(pieceCoord.x() - 80, pieceCoord.y() + reverse*80));
        ChessPiece* piece2 = findPeiceOnCoords(QPointF(pieceCoord.x() + 80, pieceCoord.y() + reverse*80));
        
        if(piece1 != nullptr and piece1->getPieceColor() != PieceType::blackPawn)
            drawTipAt(pieceCoord.x() - 80, pieceCoord.y() + reverse*80);
        
        if(piece2 != nullptr and piece2->getPieceColor() != PieceType::blackPawn)
            drawTipAt(pieceCoord.x() + 80, pieceCoord.y() + reverse*80);
        
        for(int i = 1; i < moveCount; ++i){

            QPointF pieceCoord =senderPiece->scenePos();
            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + reverse*i*80));

            if(piece == nullptr) {

                drawTipAt(pieceCoord.x(), pieceCoord.y() + reverse*i*80);

            }
            else break;

        }
    }break;

    case PieceType::whitePawn:{

        int reverse = m_playerSide ? 1 : -1;
        int moveCount = (senderPiece->getTurnsCount() == 0 ? 3 : 2);
        QPointF pieceCoord =senderPiece->scenePos();
        
        ChessPiece* piece1 = findPeiceOnCoords(QPointF(pieceCoord.x() - 80, pieceCoord.y() - reverse*80));
        ChessPiece* piece2 = findPeiceOnCoords(QPointF(pieceCoord.x() + 80, pieceCoord.y() - reverse*80));
        
        if(piece1 != nullptr and piece1->getPieceColor() != PieceType::whitePawn)
            drawTipAt(pieceCoord.x() - 80, pieceCoord.y() - reverse*80);
        
        if(piece2 != nullptr and piece2->getPieceColor() != PieceType::whitePawn)
            drawTipAt(pieceCoord.x() + 80, pieceCoord.y() - reverse*80);
        
        for(int i = 1; i < moveCount; ++i){

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - reverse*i*80));

            if(piece == nullptr) {

                drawTipAt(pieceCoord.x(), pieceCoord.y() - reverse*i*80);

            }
            else break;

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
    
    // m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
    // m_soundPlayer.play();

}

bool ChessBoard::deletePiece(ChessPiece *pieceToDelete) {
    
    if(pieceToDelete == nullptr)
        return false;
    
    //TODO: write king checking, if king denied deleting and stop game;
    
    m_piecesArr.erase(std::find(m_piecesArr.begin(), m_piecesArr.end(), pieceToDelete));
    delete pieceToDelete;
    
    m_soundPlayer.setSource(QUrl::fromLocalFile(captureSoundPath));
    m_soundPlayer.play();
    
    return true;
    
}

bool ChessBoard::isPieceChecked(ChessPiece* pieceToCheck) {
    
    bool flag = false;
    ChessPiece* senderPiece = pieceToCheck;
    QPointF pieceCoord = senderPiece->scenePos();
    
    ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == (m_playerSide ? PieceType::blackPawn : PieceType::whitePawn))
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == (m_playerSide ? PieceType::blackPawn : PieceType::whitePawn))
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y()));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y()));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king)
            return true;
    
    int counter = std::min(pieceCoord.x()/m_cellSize, pieceCoord.y()/m_cellSize);
    for(int i = 1; i < counter + 1; ++i){
        
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() - i *m_cellSize));
        
        if(piece == nullptr) continue;
        if(piece->getPieceColor() != senderPiece->getPieceColor()) {
            
            if(piece->getType() == PieceType::queen or piece->getType() == PieceType::bishop)
                flag = true;
            else break;
            
        }
        else break;
    }
    
    counter = std::min((m_cellSize*7 - pieceCoord.x())/m_cellSize, pieceCoord.y()/m_cellSize);
    for(int i = 1; i < counter + 1; ++i){
        
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y() - i *m_cellSize));
        
        if(piece == nullptr) continue;
        if(piece->getPieceColor() != senderPiece->getPieceColor()) {
            
            if(piece->getType() == PieceType::queen or piece->getType() == PieceType::bishop)
                flag = true;
            else break;
            
        }
        else break;
    }
    
    counter = std::min((m_cellSize*7 - pieceCoord.x())/m_cellSize, (m_cellSize*7 - pieceCoord.y())/m_cellSize);
    for(int i = 1; i < counter + 1; ++i){
        
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y() + i *m_cellSize));
        
        if(piece == nullptr) continue;
        if(piece->getPieceColor() != senderPiece->getPieceColor()) {
            
            if(piece->getType() == PieceType::queen or piece->getType() == PieceType::bishop)
                flag = true;
            else break;
            
        }
        else break;
    }
    
    counter = std::min(pieceCoord.x()/m_cellSize, (m_cellSize*7 - pieceCoord.y())/m_cellSize);
    for(int i = 1; i < counter + 1; ++i){
        
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() + i *m_cellSize));
        
        if(piece == nullptr) continue;
        if(piece->getPieceColor() != senderPiece->getPieceColor()) {
            
            if(piece->getType() == PieceType::queen or piece->getType() == PieceType::bishop)
                flag = true;
            else break;
            
        }
        else break;
    }
    
    counter = pieceCoord.y()/m_cellSize;
    for(int i = 1; i < counter + 1; ++i){
        
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - i *m_cellSize));
        
        if(piece == nullptr) continue;
        if(piece->getPieceColor() != senderPiece->getPieceColor()) {
            
            if(piece->getType() == PieceType::queen or piece->getType() == PieceType::rook)
                flag = true;
            else break;
            
        }
        else break;
    }
    
    counter = (m_cellSize*7 - pieceCoord.x())/m_cellSize;
    for(int i = 1; i < counter + 1; ++i){
        
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y()));
        
        if(piece == nullptr) continue;
        if(piece->getPieceColor() != senderPiece->getPieceColor()) {
            
            if(piece->getType() == PieceType::queen or piece->getType() == PieceType::rook)
                flag = true;
            else break;
            
        }
        else break;
    }
    
    counter = (m_cellSize*7 - pieceCoord.y())/m_cellSize;
    for(int i = 1; i < counter + 1; ++i){
        
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + i *m_cellSize));
        
        if(piece == nullptr) continue;
        if(piece->getPieceColor() != senderPiece->getPieceColor()) {
            
            if(piece->getType() == PieceType::queen or piece->getType() == PieceType::rook)
                flag = true;
            else break;
            
        }
        else break;
    }
    
    counter = pieceCoord.x()/m_cellSize;
    for(int i = 1; i < counter + 1; ++i){
        
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y()));
        
        if(piece == nullptr) continue;
        if(piece->getPieceColor() != senderPiece->getPieceColor()) {
            
            if(piece->getType() == PieceType::queen or piece->getType() == PieceType::rook)
                flag = true;
            else break;
            
        }
        else break;
    }
    
    //1 2
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + 2 * m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            flag = true;
    
    //1 -2
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - 2 * m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            flag = true;
    
    //2 1
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() + m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            flag = true;        
    
    //2 -1
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            flag = true; 
    
    //-1 2
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + 2 * m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            flag = true; 
    
    //-1 -2
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - 2 * m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            flag = true; 
    
    //-2 1
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() + m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            flag = true; 
    
    //-2 -1
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            flag = true; 
    
    if(flag)
        qDebug() << "White king checked";
    
    return flag;
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















