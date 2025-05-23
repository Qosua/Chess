#include "chessboard.h"

ChessBoard::ChessBoard() {
    
    m_notifySound.setSource(QUrl::fromLocalFile(notifySoundPath));
    m_notifySound.setAudioOutput(&m_audioOutput1);
    m_audioOutput1.setVolume(50);
    
    m_moveSound.setSource(QUrl::fromLocalFile(moveSoundPath));
    m_moveSound.setAudioOutput(&m_audioOutput2);
    m_audioOutput2.setVolume(50);
    
    m_checkSound.setSource(QUrl::fromLocalFile(checkSoundPath));
    m_checkSound.setAudioOutput(&m_audioOutput3);
    m_audioOutput3.setVolume(50);
    
    m_captureSound.setSource(QUrl::fromLocalFile(captureSoundPath));
    m_captureSound.setAudioOutput(&m_audioOutput4);
    m_audioOutput4.setVolume(50);
    
    m_castleSound.setSource(QUrl::fromLocalFile(castleSoundPath));
    m_castleSound.setAudioOutput(&m_audioOutput5);
    m_audioOutput5.setVolume(50);
    
}

ChessBoard::~ChessBoard() {
    
    clearBoard();
    
}

void ChessBoard::setupBoard(){
    
    drawField();
    preparePieces();
    drawPieces();
    this->update();
    this->setBackgroundBrush(QBrush(QColor(38,37,34)));

    HistoryBlockWidget::resetTime();

    m_notifySound.play();
    
}

void ChessBoard::setPlayerSide(bool side) {
    m_playerSide = side;
}

bool ChessBoard::getPlayerSide() {
    return m_playerSide;
}

void ChessBoard::recheckMateBy(bool color) {
    
    ChessPiece* enemyKing = (m_playerSide == color ? m_enemyKing : m_playerKing);
    
    if(isPieceChecked(enemyKing)){
        m_checkSound.play();
        
        setAttackersPiecesFor(enemyKing);
        if(isKingMated(enemyKing))
            checkMateFor(enemyKing->getPieceColor());
        
        attackerPieces.clear();
        
    }
    
}

void ChessBoard::drawField() {

    int order = (m_playerSide ? 0 : 1);
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            
            CustomRectangle* rectangle = new CustomRectangle(QRectF(j*m_cellSize, i*m_cellSize, m_cellSize, m_cellSize));
            rectangle->reverseSomeCell = (m_playerSide ? false : true);
            
            if((i+j)%2 == order) {
                rectangle->setBrush(QBrush(QColor(235,236,208)));
            }
            else{
                rectangle->setBrush(QBrush(QColor(115,149,82)));
            }
            
            rectangle->setPen(QPen(Qt::transparent, 0));
            
            if(m_playerSide) {
                if(j == 0) {
                    if((i+j)%2 == order) {
                        rectangle->setText(QString(QChar('8' - i)), QColor(115,149,82), false);
                    }
                    else{
                        rectangle->setText(QChar('8' - i), QColor(235,236,208), false);
                    }
                }
                if(i == 7){
                    
                    if((i+j)%2 == order) {
                        rectangle->setText(QString(QChar('a' + j)), QColor(115,149,82), true);
                    }
                    else{
                        rectangle->setText(QChar('a' + j), QColor(235,236,208), true);
                    }
                    
                }
                
                this->addItem(rectangle);
            }
            else {
                if(j == 0) {
                    if((i+j)%2 == order) {
                        rectangle->setText(QString(QChar('1' + i)), QColor(115,149,82), false);
                    }
                    else{
                        rectangle->setText(QChar('1' + i), QColor(235,236,208), false);
                    }
                }
                if(i == 7){
                    
                    if((i+j)%2 == order) {
                        rectangle->setText(QString(QChar('h' - j)), QColor(115,149,82), true);
                    }
                    else{
                        rectangle->setText(QChar('h' - j), QColor(235,236,208), true);
                    }
                    
                }
                
                this->addItem(rectangle); 
                
                
            }
            
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

    int reverseForPawn = (m_playerSide ? 1 : -1);
    ChessPiece* playerKing = (m_playerSide == senderPiece->getPieceColor() ? m_playerKing : m_enemyKing);
    ChessPiece* enemyKing = (m_playerSide == senderPiece->getPieceColor() ? m_enemyKing : m_playerKing);

    switch(senderPiece->getType()){

    case PieceType::knight:{

        if( (delta.x() == 1 and (delta.y() == 2 or delta.y() == -2)) or
            (delta.x() == 2 and (delta.y() == 1 or delta.y() == -1)) or
            (delta.x() == -1 and (delta.y() == 2 or delta.y() == -2)) or
            (delta.x() == -2 and (delta.y() == 1 or delta.y() == -1))){
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                
                senderPiece->setPos(oldPos);
                
            }
            else {
                
                if(m_isOfflineMatch)
                    changeTurnColorTo(!senderPiece->getPieceColor());
                
                turnsCounter += 1;
                
                QString name = getPieceName(pieceToDel);
                bool captureFlag = deletePiece(pieceToDel);
                
                if(isPieceChecked(enemyKing)){
                    m_checkSound.play();
                    
                    setAttackersPiecesFor(enemyKing);
                    if(isKingMated(enemyKing))
                        checkMateFor(enemyKing->getPieceColor());
                    
                    attackerPieces.clear();

                }
                else{
                    
                    if(!captureFlag)
                        m_moveSound.play();
                    else
                        m_captureSound.play();
                    
                    if(isKingStalemated(enemyKing))
                        staleMate();
                    
                    
                }
                
                PlayerAction action;
                action.actionSide = senderPiece->getPieceColor();
                action.pieceOne = senderPiece->getType();
                action.oldPos = oldPos;
                action.newPos = newPos;
                action.turnNumber = turnsCounter;
                if(captureFlag){
                    
                    action.turnType = ActionType::capture;
                    action.pieceTwoName = name;
                    
                }
                else 
                    action.turnType = ActionType::turn;
                emit turnMade(action);
                
            }
            
            m_lastChosenPiece = nullptr;
        }
        else
            senderPiece->setPos(oldPos);

    } break;

    case PieceType::king:{

        if(delta.x() == 2 and delta.y() == 0 and !isPieceOnWay(oldPos, newPos)){

            ChessPiece* checkRook = findPeiceOnCoords(QPointF(oldPos.x() + 3*m_cellSize, oldPos.y()));

            if(checkRook != nullptr and checkRook->getTurnsCount() == 0 and senderPiece->getTurnsCount() == 0) {

                senderPiece->setPos(newPos);
                if(playerKing != nullptr and isPieceChecked(playerKing)) {

                    senderPiece->setPos(oldPos);

                }
                else {
                    
                    if(m_isOfflineMatch)
                        changeTurnColorTo(!senderPiece->getPieceColor());

                    checkRook->setPos(QPointF(oldPos.x() + m_cellSize, oldPos.y()));

                    turnsCounter += 1;
                    senderPiece->plusOneToTurn();

                    if(isPieceChecked(enemyKing)){
                        m_checkSound.play();

                        setAttackersPiecesFor(enemyKing);
                        if(isKingMated(enemyKing))
                            checkMateFor(enemyKing->getPieceColor());

                        attackerPieces.clear();

                    }
                    else{

                        m_castleSound.play();

                        if(isKingStalemated(enemyKing))
                            staleMate();
                    }

                    PlayerAction action;
                    action.actionSide = senderPiece->getPieceColor();
                    action.pieceOne = senderPiece->getType();
                    action.oldPos = oldPos;
                    action.newPos = newPos;
                    action.turnNumber = turnsCounter;
                    action.turnType = ActionType::turn;
                    emit turnMade(action);

                    return;

                }

            }

            m_lastChosenPiece = nullptr;
        }
        else
            senderPiece->setPos(oldPos);

        if(delta.x() == -2 and delta.y() == 0 and !isPieceOnWay(oldPos, newPos)){

            ChessPiece* checkRook = findPeiceOnCoords(QPointF(oldPos.x() - 4*m_cellSize, oldPos.y()));

            if(checkRook != nullptr and checkRook->getTurnsCount() == 0 and senderPiece->getTurnsCount() == 0) {

                senderPiece->setPos(newPos);
                if(playerKing != nullptr and isPieceChecked(playerKing)) {

                    senderPiece->setPos(oldPos);

                }
                else {
                    
                    if(m_isOfflineMatch)
                        changeTurnColorTo(!senderPiece->getPieceColor());

                    checkRook->setPos(QPointF(oldPos.x() - m_cellSize, oldPos.y()));

                    turnsCounter += 1;
                    senderPiece->plusOneToTurn();

                    if(isPieceChecked(enemyKing)){
                        m_checkSound.play();

                        setAttackersPiecesFor(enemyKing);
                        if(isKingMated(enemyKing))
                            checkMateFor(enemyKing->getPieceColor());

                        attackerPieces.clear();

                    }
                    else{

                        m_castleSound.play();

                        if(isKingStalemated(enemyKing))
                            staleMate();
                    }

                    PlayerAction action;
                    action.actionSide = senderPiece->getPieceColor();
                    action.pieceOne = senderPiece->getType();
                    action.oldPos = oldPos;
                    action.newPos = newPos;
                    action.turnNumber = turnsCounter;
                    action.turnType = ActionType::turn;
                    emit turnMade(action);

                    return;

                }

            }

            m_lastChosenPiece = nullptr;
        }
        else
            senderPiece->setPos(oldPos);


        if(std::abs(delta.x()) <= 1 and std::abs(delta.y()) <= 1){
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                
                senderPiece->setPos(oldPos);
                
            }
            else {
                
                if(m_isOfflineMatch)
                    changeTurnColorTo(!senderPiece->getPieceColor());
                
                turnsCounter += 1;
                senderPiece->plusOneToTurn();
                
                QString name = getPieceName(pieceToDel);
                
                bool captureFlag = deletePiece(pieceToDel);
                if(isPieceChecked(enemyKing)){
                    m_checkSound.play();
                    
                    setAttackersPiecesFor(enemyKing);
                    if(isKingMated(enemyKing))
                        checkMateFor(enemyKing->getPieceColor());
                    
                    attackerPieces.clear();
                    
                }
                else{
                    
                    if(!captureFlag)
                        m_moveSound.play();
                    else
                        m_captureSound.play();
                    
                    if(isKingStalemated(enemyKing))
                        staleMate();
                }
                
                PlayerAction action;
                action.actionSide = senderPiece->getPieceColor();
                action.pieceOne = senderPiece->getType();
                action.oldPos = oldPos;
                action.newPos = newPos;
                action.turnNumber = turnsCounter;
                if(captureFlag){
                    
                    action.turnType = ActionType::capture;
                    action.pieceTwoName = name;
                    
                }
                else 
                    action.turnType = ActionType::turn;
                emit turnMade(action);
                
            }
            
            m_lastChosenPiece = nullptr;
        }
        else
            senderPiece->setPos(oldPos);

    } break;

    case PieceType::bishop:{
        
        if(std::abs(delta.x()/delta.x()) == 1 and std::abs(delta.y()/delta.x()) == 1 and !isPieceOnWay(oldPos, newPos)) {
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                
                senderPiece->setPos(oldPos);
                
            }
            else {
                
                if(m_isOfflineMatch)
                    changeTurnColorTo(!senderPiece->getPieceColor());
                
                turnsCounter += 1;
                
                QString name = getPieceName(pieceToDel);
                
                bool captureFlag = deletePiece(pieceToDel);
                if(isPieceChecked(enemyKing)){
                    m_checkSound.play();
                    
                    setAttackersPiecesFor(enemyKing);
                    if(isKingMated(enemyKing))
                        checkMateFor(enemyKing->getPieceColor());
                    
                    attackerPieces.clear();
                    
                }
                else{
                    
                    if(!captureFlag)
                        m_moveSound.play();
                    else
                        m_captureSound.play();
                    
                    if(isKingStalemated(enemyKing))
                        staleMate();
                }
                
                PlayerAction action;
                action.actionSide = senderPiece->getPieceColor();
                action.pieceOne = senderPiece->getType();
                action.oldPos = oldPos;
                action.newPos = newPos;
                action.turnNumber = turnsCounter;
                if(captureFlag){
                    
                    action.turnType = ActionType::capture;
                    action.pieceTwoName = name;
                    
                }
                else 
                    action.turnType = ActionType::turn;
                emit turnMade(action);
                
            }
            
            m_lastChosenPiece = nullptr;
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    case PieceType::rook:{

        if((delta.x() == 0 or delta.y() == 0) and !isPieceOnWay(oldPos, newPos)){
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                
                senderPiece->setPos(oldPos);
                
            }
            else {
                
                if(m_isOfflineMatch)
                    changeTurnColorTo(!senderPiece->getPieceColor());
                
                turnsCounter += 1;
                senderPiece->plusOneToTurn();
                
                QString name = getPieceName(pieceToDel);
                
                bool captureFlag = deletePiece(pieceToDel);
                if(isPieceChecked(enemyKing)){
                    m_checkSound.play();
                    
                    setAttackersPiecesFor(enemyKing);
                    if(isKingMated(enemyKing))
                        checkMateFor(enemyKing->getPieceColor());
                    
                    attackerPieces.clear();
                    
                }
                else{
                    
                    if(!captureFlag) 
                        m_moveSound.play();
                    else
                        m_captureSound.play();
                    
                    if(isKingStalemated(enemyKing))
                        staleMate();
                }
                
                PlayerAction action;
                action.actionSide = senderPiece->getPieceColor();
                action.pieceOne = senderPiece->getType();
                action.oldPos = oldPos;
                action.newPos = newPos;
                action.turnNumber = turnsCounter;
                if(captureFlag){
                    
                    action.turnType = ActionType::capture;
                    action.pieceTwoName = name;
                    
                }
                else 
                    action.turnType = ActionType::turn;
                emit turnMade(action);
                
            }
            
            m_lastChosenPiece = nullptr;
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    case PieceType::queen:{

        if(!isPieceOnWay(oldPos, newPos) and ((std::abs(delta.x()/delta.x()) == 1 and std::abs(delta.y()/delta.x()) == 1) or
            (delta.x() == 0 or delta.y() == 0))) {
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                
                senderPiece->setPos(oldPos);
                
            }
            else {
                
                if(m_isOfflineMatch)
                    changeTurnColorTo(!senderPiece->getPieceColor());
                
                turnsCounter += 1;
                
                QString name = getPieceName(pieceToDel);
                
                bool captureFlag = deletePiece(pieceToDel);
                if(isPieceChecked(enemyKing)){
                    m_checkSound.play();
                    
                    setAttackersPiecesFor(enemyKing);
                    if(isKingMated(enemyKing))
                        checkMateFor(enemyKing->getPieceColor());
                    
                    attackerPieces.clear();
                    
                }
                else{
                    
                    if(!captureFlag)
                        m_moveSound.play();
                    else
                        m_captureSound.play();
                    
                    if(isKingStalemated(enemyKing))
                        staleMate();
                    
                }
                
                PlayerAction action;
                action.actionSide = senderPiece->getPieceColor();
                action.pieceOne = senderPiece->getType();
                action.oldPos = oldPos;
                action.newPos = newPos;
                action.turnNumber = turnsCounter;
                if(captureFlag){
                    
                    action.turnType = ActionType::capture;
                    action.pieceTwoName = name;
                    
                }
                else 
                    action.turnType = ActionType::turn;
                emit turnMade(action);
                
            }
            
            m_lastChosenPiece = nullptr;
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    case PieceType::whitePawn:{
        
        ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
        
        if((delta.x() == 1 or delta.x() == -1) and delta.y() == reverseForPawn*1 and pieceToDel != nullptr){
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                senderPiece->setPos(oldPos);
                return;
            }
            else {
                
                if(m_isOfflineMatch)
                    changeTurnColorTo(!senderPiece->getPieceColor());
                
                turnsCounter += 1;
                
                if(m_playerSide){
                    if(newPos.y() == 0)
                        openPieceChoosingWidget(senderPiece);
                }
                else{
                    if(newPos.y() == 560)
                        openPieceChoosingWidget(senderPiece);
                }
                
                deletePiece(pieceToDel);
                QString name = getPieceName(pieceToDel);
                
                if(isPieceChecked(enemyKing)){
                    m_checkSound.play();
                    
                    setAttackersPiecesFor(enemyKing);
                    if(isKingMated(enemyKing))
                        checkMateFor(enemyKing->getPieceColor());
                    
                    attackerPieces.clear();
                    
                }
                else{
                    m_captureSound.play();
                    
                    if(isKingStalemated(enemyKing))
                        staleMate();
                }
                
                PlayerAction action;
                action.actionSide = senderPiece->getPieceColor();
                action.pieceOne = senderPiece->getType();
                action.oldPos = oldPos;
                action.newPos = newPos;
                action.turnNumber = turnsCounter;
                action.turnType = ActionType::capture;
                action.pieceTwoName = name;
                
                emit turnMade(action);
                
                tempIgnoredPiece = nullptr;
                senderPiece->plusOneToTurn();
                return;
            }
            tempIgnoredPiece = nullptr;
            senderPiece->plusOneToTurn();
            
        }

        if(pieceToDel == nullptr and !isPieceOnWay(oldPos, newPos) and delta.x() == 0 and (delta.y() == reverseForPawn*1 or (senderPiece->getTurnsCount() == 0 and delta.y() == reverseForPawn*2))) {
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                
                senderPiece->setPos(oldPos);
                return;
                
            }
            else {
                if(pieceToDel == nullptr) {
                    
                    if(m_isOfflineMatch)
                        changeTurnColorTo(!senderPiece->getPieceColor());
                    
                    turnsCounter += 1;
                    
                    if(m_playerSide){
                        if(newPos.y() == 0)
                            openPieceChoosingWidget(senderPiece);
                    }
                    else{
                        if(newPos.y() == 560)
                            openPieceChoosingWidget(senderPiece);
                    }
                    
                    if(isPieceChecked(enemyKing)){
                        m_checkSound.play();
                        
                        setAttackersPiecesFor(enemyKing);
                        if(isKingMated(enemyKing))
                            checkMateFor(enemyKing->getPieceColor());
                        
                        attackerPieces.clear();
                        
                    }
                    else{
                        m_moveSound.play();
                        
                        if(isKingStalemated(enemyKing))
                            staleMate();
                    }
                }
                else {
                    senderPiece->setPos(oldPos);
                    return;
                }
                
                PlayerAction action;
                action.actionSide = senderPiece->getPieceColor();
                action.pieceOne = senderPiece->getType();
                action.oldPos = oldPos;
                action.newPos = newPos;
                action.turnNumber = turnsCounter;
                action.turnType = ActionType::turn;
                emit turnMade(action);
                
            }
            
            m_lastChosenPiece = nullptr;
            senderPiece->plusOneToTurn();
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    case PieceType::blackPawn:{
        
        ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
        
        if((delta.x() == 1 or delta.x() == -1) and delta.y() == -reverseForPawn*1 and pieceToDel != nullptr){
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                senderPiece->setPos(oldPos);
                return;
            }
            else {
                
                if(m_isOfflineMatch)
                    changeTurnColorTo(!senderPiece->getPieceColor());
                
                turnsCounter += 1;
                
                if(m_playerSide){
                    if(newPos.y() == 560)
                        openPieceChoosingWidget(senderPiece);
                }
                else{
                    if(newPos.y() == 0)
                        openPieceChoosingWidget(senderPiece);
                }
                
                QString name = getPieceName(pieceToDel);
                deletePiece(pieceToDel);
                
                if(isPieceChecked(enemyKing)){
                    m_checkSound.play();
                    
                    setAttackersPiecesFor(enemyKing);
                    if(isKingMated(enemyKing))
                        checkMateFor(enemyKing->getPieceColor());
                    
                    attackerPieces.clear();
                    
                }
                else{
                    m_captureSound.play();
                    
                    if(isKingStalemated(enemyKing))
                        staleMate();
                }
                
                PlayerAction action;
                action.actionSide = senderPiece->getPieceColor();
                action.pieceOne = senderPiece->getType();
                action.oldPos = oldPos;
                action.newPos = newPos;
                action.turnNumber = turnsCounter;
                action.turnType = ActionType::capture;
                action.pieceTwoName = name;
                emit turnMade(action);
                
                tempIgnoredPiece = nullptr;
                senderPiece->plusOneToTurn();
                return;
            }
            tempIgnoredPiece = nullptr;
            senderPiece->plusOneToTurn();
            
        }
        
        if(pieceToDel == nullptr and !isPieceOnWay(oldPos, newPos) and delta.x() == 0 and (delta.y() == -reverseForPawn*1 or (senderPiece->getTurnsCount() == 0 and delta.y() == -reverseForPawn*2))) {
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                
                senderPiece->setPos(oldPos);
                return;
                
            }
            else {
                
                if(pieceToDel == nullptr){
                    
                    if(m_isOfflineMatch)
                        changeTurnColorTo(!senderPiece->getPieceColor());
                    
                    turnsCounter += 1;
                    
                    if(m_playerSide){
                        if(newPos.y() == 560)
                            openPieceChoosingWidget(senderPiece);
                    }
                    else{
                        if(newPos.y() == 0)
                            openPieceChoosingWidget(senderPiece);
                    }
                    
                    if(isPieceChecked(enemyKing)){
                        m_checkSound.play();
                        
                        setAttackersPiecesFor(enemyKing);
                        if(isKingMated(enemyKing))
                            checkMateFor(enemyKing->getPieceColor());
                        
                        attackerPieces.clear();
                        
                    }
                    else{
                        m_moveSound.play();
                        
                        if(isKingStalemated(enemyKing))
                            staleMate();
                    }
                }
                else {
                    senderPiece->setPos(oldPos);
                    return;
                }
                
                PlayerAction action;
                action.actionSide = senderPiece->getPieceColor();
                action.pieceOne = senderPiece->getType();
                action.oldPos = oldPos;
                action.newPos = newPos;
                action.turnNumber = turnsCounter;
                action.turnType = ActionType::turn;
                    
                emit turnMade(action);
                
            }
            
            m_lastChosenPiece = nullptr;
            senderPiece->plusOneToTurn();
        }
        else
            senderPiece->setPos(oldPos);


    } break;

    default:{
        senderPiece->setPos(oldPos);
    } break;

    }
    
    tempIgnoredPiece = nullptr;

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
        QPointF pieceCoord = senderPiece->scenePos();
        
        ChessPiece* piece1 = findPeiceOnCoords(QPointF(pieceCoord.x() - 80, pieceCoord.y() + reverse*80));
        ChessPiece* piece2 = findPeiceOnCoords(QPointF(pieceCoord.x() + 80, pieceCoord.y() + reverse*80));
        
        if(piece1 != nullptr and piece1->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() - 80, pieceCoord.y() + reverse*80);
        
        if(piece2 != nullptr and piece2->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() + 80, pieceCoord.y() + reverse*80);
        
        for(int i = 1; i < moveCount; ++i){
            
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
        
        if(piece1 != nullptr and piece1->getPieceColor() != senderPiece->getPieceColor())
            drawTipAt(pieceCoord.x() - 80, pieceCoord.y() - reverse*80);
        
        if(piece2 != nullptr and piece2->getPieceColor() != senderPiece->getPieceColor())
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

        if(senderPiece->getTurnsCount() == 0) {

            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + 3*m_cellSize, pieceCoord.y()));
            if(piece != nullptr and piece->getTurnsCount() == 0 and piece->getType() == PieceType::rook){

                ChessPiece* piece1 = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y()));
                ChessPiece* piece2 = findPeiceOnCoords(QPointF(pieceCoord.x() + 2*m_cellSize, pieceCoord.y()));
                if(piece1 == nullptr and piece2 == nullptr)
                    drawTipAt(pieceCoord.x() + 2*m_cellSize, pieceCoord.y());

            }
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - 4*m_cellSize, pieceCoord.y()));
            if(piece != nullptr and piece->getTurnsCount() == 0 and piece->getType() == PieceType::rook){

                ChessPiece* piece1 = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y()));
                ChessPiece* piece2 = findPeiceOnCoords(QPointF(pieceCoord.x() - 2*m_cellSize, pieceCoord.y()));
                if(piece1 == nullptr and piece2 == nullptr) {
                    drawTipAt(pieceCoord.x() - 2*m_cellSize, pieceCoord.y());
                }
            }
        }

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

    for(size_t i = 0; i < m_piecesArr.size(); ++i){

        if(pos == m_piecesArr[i]->scenePos()) {
            
            if(tempIgnoredPiece != nullptr and tempIgnoredPiece == m_piecesArr[i])
                continue;

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
    
    return ansFlag;
    
}

void ChessBoard::deletePieceAt(QPointF pos) {

    ChessPiece* piece = findPeiceOnCoords(pos);

    if(piece == nullptr)
        return;

    m_piecesArr.erase(std::find(m_piecesArr.begin(), m_piecesArr.end(), piece));
    delete piece;

}

bool ChessBoard::deletePiece(ChessPiece *pieceToDelete) {
    
    if(pieceToDelete == nullptr)
        return false;
    
    if(pieceToDelete->getType() == PieceType::king){
        
        pieceToDelete->setPixmap(QPixmap());
        return true;
        
    }
    
    m_piecesArr.erase(std::find(m_piecesArr.begin(), m_piecesArr.end(), pieceToDelete));
    delete pieceToDelete;
    
    return true;
    
}

bool ChessBoard::isPieceChecked(ChessPiece* pieceToCheck) {
    
    bool flag = false;
    ChessPiece* senderPiece = pieceToCheck;
    QPointF pieceCoord = senderPiece->scenePos();
    ChessPiece* piece = nullptr;
    
    qDebug() << "SENDER IS " << pieceToCheck->getPieceColor();
    
    if(m_playerSide) {
        
        if(senderPiece->getPieceColor()) {
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::blackPawn){
                    qDebug() << "CHECK BY PAWN 1";
                    return true;
                }
            
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::blackPawn){
                    qDebug() << "CHECK BY PAWN 2";
                    return true;
                }
        }
        else{
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::whitePawn){
                    qDebug() << "CHECK BY PAWN 3";
                    return true;
                }
            
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::whitePawn){
                    qDebug() << "CHECK BY PAWN 4";
                    return true;
                }
        }
    }
    else {
        
        if(senderPiece->getPieceColor()) {
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::blackPawn){
                    qDebug() << "CHECK BY PAWN 5";
                    return true;
                }

            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::blackPawn){
                    qDebug() << "CHECK BY PAWN 6";
                    return true;
                }
        }
        else{
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::whitePawn){
                    qDebug() << "CHECK BY PAWN 7";
                    return true;
                }
            
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::whitePawn){
                    qDebug() << "CHECK BY PAWN 8";
                    return true;
                }
        }
        
    }
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king or piece->getType() == PieceType::queen)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y()));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king or piece->getType() == PieceType::queen)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king or piece->getType() == PieceType::queen)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king or piece->getType() == PieceType::queen)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king or piece->getType() == PieceType::queen)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y()));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king or piece->getType() == PieceType::queen)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king or piece->getType() == PieceType::queen)
            return true;
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::king or piece->getType() == PieceType::queen)
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
    
    return flag;
}

QString ChessBoard::getPieceName(ChessPiece *piece) {
    
    if(piece == nullptr)
        return "";
    
    QString name;
    name = (piece->getPieceColor() ? "w" : "b");
    
    if(piece->getType() == PieceType::bishop)
        name += "b";
    if(piece->getType() == PieceType::queen)
        name += "q";
    if(piece->getType() == PieceType::king)
        name += "k";
    if(piece->getType() == PieceType::knight)
        name += "h";
    if(piece->getType() == PieceType::rook)
        name += "r";
    if(piece->getType() == PieceType::blackPawn or piece->getType() == PieceType::whitePawn)
        name += "p";
    
    return name;
    
}

void ChessBoard::setAttackersPiecesFor(ChessPiece *pieceToCheck) {
    
    ChessPiece* senderPiece = pieceToCheck;
    QPointF pieceCoord = senderPiece->scenePos();
    ChessPiece* piece = nullptr;

    if(m_playerSide) {

        if(senderPiece->getPieceColor()) {
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::blackPawn){
                    qDebug() << "ATTACK BY PAWN 1";
                    attackerPieces.insert(piece);
                }

            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::blackPawn){
                    qDebug() << "ATTACK BY PAWN 2";
                    attackerPieces.insert(piece);
                }
        }
        else{
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::whitePawn){
                    qDebug() << "ATTACK BY PAWN 3";
                    attackerPieces.insert(piece);
                }

            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::whitePawn){
                    qDebug() << "ATTACK BY PAWN 4";
                    attackerPieces.insert(piece);
                }
        }
    }
    else {

        if(senderPiece->getPieceColor()) {
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::blackPawn){
                    qDebug() << "ATTACK BY PAWN 5";
                    attackerPieces.insert(piece);
                }

            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::blackPawn){
                    qDebug() << "ATTACK BY PAWN 6";
                    attackerPieces.insert(piece);
                }
        }
        else{
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::whitePawn){
                    qDebug() << "ATTACK BY PAWN 7";
                    attackerPieces.insert(piece);
                }

            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize));
            if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                if(piece->getType() == PieceType::whitePawn){
                    qDebug() << "ATTACK BY PAWN 8";
                    attackerPieces.insert(piece);
                }
        }

    }
    
    {
        // piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize));
        // if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        //     if(piece->getType() == PieceType::king)
        //         attackerPieces.insert(piece);
        
        // piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y()));
        // if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        //     if(piece->getType() == PieceType::king)
        //         attackerPieces.insert(piece);
        
        // piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize));
        // if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        //     if(piece->getType() == PieceType::king)
        //         attackerPieces.insert(piece);
        
        // piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - m_cellSize));
        // if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        //     if(piece->getType() == PieceType::king)
        //         attackerPieces.insert(piece);
        
        // piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize));
        // if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        //     if(piece->getType() == PieceType::king)
        //         attackerPieces.insert(piece);
        
        // piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y()));
        // if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        //     if(piece->getType() == PieceType::king)
        //         attackerPieces.insert(piece);
        
        // piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize));
        // if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        //     if(piece->getType() == PieceType::king)
        //         attackerPieces.insert(piece);
        
        // piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + m_cellSize));
        // if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        //     if(piece->getType() == PieceType::king)
        //         attackerPieces.insert(piece);
    }
    
    int counter = std::min(pieceCoord.x()/m_cellSize, pieceCoord.y()/m_cellSize);
    for(int i = 1; i < counter + 1; ++i){
        
        ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() - i *m_cellSize));
        
        if(piece == nullptr) continue;
        if(piece->getPieceColor() != senderPiece->getPieceColor()) {
            
            if(piece->getType() == PieceType::queen or piece->getType() == PieceType::bishop)
                attackerPieces.insert(piece);
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
                attackerPieces.insert(piece);
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
                attackerPieces.insert(piece);
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
                attackerPieces.insert(piece);
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
                attackerPieces.insert(piece);
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
                attackerPieces.insert(piece);
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
                attackerPieces.insert(piece);
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
                attackerPieces.insert(piece);
            else break;
            
        }
        else break;
    }
    
    //1 2
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + 2 * m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            attackerPieces.insert(piece);
    
    //1 -2
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - 2 * m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            attackerPieces.insert(piece);
    
    //2 1
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() + m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            attackerPieces.insert(piece);
    
    //2 -1
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            attackerPieces.insert(piece);
    
    //-1 2
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + 2 * m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            attackerPieces.insert(piece);
    
    //-1 -2
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - 2 * m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            attackerPieces.insert(piece);
    
    //-2 1
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() + m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            attackerPieces.insert(piece);
    
    //-2 -1
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
        if(piece->getType() == PieceType::knight)
            attackerPieces.insert(piece);
    
}

bool ChessBoard::isKingStalemated(ChessPiece *pieceToCheck) {
    
    QPointF pieceCoord = pieceToCheck->scenePos();
    if(isKingCanGoOutofCheck(pieceToCheck))
        return false;
    qDebug() << "STALEMATE 2";
    
    for(auto senderPiece : m_piecesArr){
        
        if(senderPiece != nullptr and senderPiece->getPieceColor() != pieceToCheck->getPieceColor())
            continue;
        if(senderPiece->getType() == PieceType::king)
            continue;
        
        qDebug() << "STALEMATE 3";
        
        switch(senderPiece->getType()) {
            
        case PieceType::blackPawn:{
            
            int reverse = m_playerSide ? 1 : -1;
            int moveCount = (senderPiece->getTurnsCount() == 0 ? 3 : 2);
            QPointF pieceCoord =senderPiece->scenePos();
            
            ChessPiece* piece1 = findPeiceOnCoords(QPointF(pieceCoord.x() - 80, pieceCoord.y() + reverse*80));
            ChessPiece* piece2 = findPeiceOnCoords(QPointF(pieceCoord.x() + 80, pieceCoord.y() + reverse*80));
            
            bool ansFlag = false;
            if(piece1 != nullptr and piece1->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - 80, pieceCoord.y() + reverse*80), senderPiece);
            
            if(ansFlag){
                return false;
                qDebug() << "STALEMATE BP1";
            }
            
            if(piece2 != nullptr and piece2->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + 80, pieceCoord.y() + reverse*80), senderPiece);
            
            if(ansFlag){
                return false;
                qDebug() << "STALEMATE BP2";
            }
            
            for(int i = 1; i < moveCount; ++i){
                
                QPointF pieceCoord =senderPiece->scenePos();
                ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + reverse*i*80));
                
                if(piece == nullptr) {
                    
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x(), pieceCoord.y() + reverse*i*80), senderPiece);
                    
                    if(ansFlag){
                        return false;
                        qDebug() << "STALEMATE BP3";
                    }
                    
                }
                else break;
            }
        }break;
            
        case PieceType::whitePawn:{
            qDebug() << "STALEMATE WP";
            
            int reverse = m_playerSide ? 1 : -1;
            int moveCount = (senderPiece->getTurnsCount() == 0 ? 3 : 2);
            QPointF pieceCoord =senderPiece->scenePos();
            
            ChessPiece* piece1 = findPeiceOnCoords(QPointF(pieceCoord.x() - 80, pieceCoord.y() - reverse*80));
            ChessPiece* piece2 = findPeiceOnCoords(QPointF(pieceCoord.x() + 80, pieceCoord.y() - reverse*80));
            
            bool ansFlag = false;
            if(piece1 != nullptr and piece1->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - 80, pieceCoord.y() - reverse*80), senderPiece);
            
            if(ansFlag)
                return false;
            
            if(piece2 != nullptr and piece2->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + 80, pieceCoord.y() - reverse*80), senderPiece);
            
            if(ansFlag)
                return false;
            
            for(int i = 1; i < moveCount; ++i){
                
                QPointF pieceCoord =senderPiece->scenePos();
                ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - reverse*i*80));
                
                if(piece == nullptr) {
                    
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x(), pieceCoord.y() - reverse*i*80), senderPiece);
                    
                    if(ansFlag)
                        return false;
                    
                }
                else break;
            }
        }break;
            
        case PieceType::knight:{
            qDebug() << "STALEMATE N";
            
            QPointF pieceCoord = senderPiece->scenePos();
            bool ansFlag;
            
            //1 2
            ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + 2 * m_cellSize));
            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + 2 * m_cellSize), senderPiece);
            
            if(ansFlag)
                return false;
            
            //1 -2
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - 2 * m_cellSize));
            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - 2 * m_cellSize), senderPiece);
            
            if(ansFlag)
                return false;
            
            //2 1
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() + m_cellSize));
            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
               ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() + m_cellSize), senderPiece);
            
            if(ansFlag)
                return false;
            
            //2 -1
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() - m_cellSize));
            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + 2 * m_cellSize, pieceCoord.y() - m_cellSize), senderPiece);
            
            if(ansFlag)
                return false;
            
            //-1 2
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + 2 * m_cellSize));
            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + 2 * m_cellSize), senderPiece);
            
            if(ansFlag)
                return false;
            
            //-1 -2
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - 2 * m_cellSize));
            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - 2 * m_cellSize), senderPiece);
            
            if(ansFlag)
                return false;
            
            //-2 1
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() + m_cellSize));
            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() + m_cellSize), senderPiece);
            
            if(ansFlag)
                return false;
            
            //-2 -1
            piece = findPeiceOnCoords(QPointF(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() - m_cellSize));
            if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor())
                ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - 2 * m_cellSize, pieceCoord.y() - m_cellSize), senderPiece);
            
            if(ansFlag)
                return false;
            
        }break;
            
        case PieceType::bishop:{
            qDebug() << "STALEMATE B";
            
            bool ansFlag;
            QPointF pieceCoord = senderPiece->scenePos();
            
            int counter = std::min(pieceCoord.x()/m_cellSize, pieceCoord.y()/m_cellSize);
            for(int i = 1; i < counter + 1; ++i){
                
                ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() - i *m_cellSize));
                
                if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() - i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;

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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y() - i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;

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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y() + i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;

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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() + i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;

                    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                        i = counter + 1;
                }
                else
                    i = counter + 1;
            }
            
        }break;
            
        case PieceType::rook:{
            qDebug() << "STALEMATE R";
            
            bool ansFlag;
            QPointF pieceCoord = senderPiece->scenePos();
            
            int counter = pieceCoord.y()/m_cellSize;
            for(int i = 1; i < counter + 1; ++i){
                
                ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - i *m_cellSize));
                
                if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x(), pieceCoord.y() - i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;

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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y()), senderPiece);
                    if(ansFlag)
                        return false;

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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x(), pieceCoord.y() + i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;

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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y()), senderPiece);
                    if(ansFlag)
                        return false;

                    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                        i = counter + 1;
                }
                else
                    i = counter + 1;
            }
            
        }break;
            
        case PieceType::queen:{
            qDebug() << "STALEMATE Q";
            
            bool ansFlag;
            QPointF pieceCoord = senderPiece->scenePos();
            
            int counter = std::min(pieceCoord.x()/m_cellSize, pieceCoord.y()/m_cellSize);
            for(int i = 1; i < counter + 1; ++i){
                
                ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() - i *m_cellSize));
                
                if(piece == nullptr or piece->getPieceColor() != senderPiece->getPieceColor()) {
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() - i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;
                    
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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y() - i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;
                    
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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y() + i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;
                    
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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y() + i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;
                    
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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x(), pieceCoord.y() - i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;
                    
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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() + i * m_cellSize, pieceCoord.y()), senderPiece);
                    if(ansFlag)
                        return false;
                    
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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x(), pieceCoord.y() + i * m_cellSize), senderPiece);
                    if(ansFlag)
                        return false;
                    
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
                    ansFlag = isPieceAbleToMoveAt(QPointF(pieceCoord.x() - i * m_cellSize, pieceCoord.y()), senderPiece);
                    if(ansFlag)
                        return false;
                    
                    if(piece != nullptr and piece->getPieceColor() != senderPiece->getPieceColor())
                        i = counter + 1;
                }
                else
                    i = counter + 1;
            }
            
        }break;
            
        }
        
    }
    
    return true;
    
}

bool ChessBoard::isKingMated(ChessPiece *pieceToCheck) {
    
    qDebug() << attackerPieces.size() << "-AttackersArrSize";
    if(attackerPieces.size() >= 2){
        
        if(isKingCanGoOutofCheck(pieceToCheck))
            return false;

    }
    if(attackerPieces.size() == 1){
        
        qDebug() << attackerPieces.size() << "-WE ARE HERE BEFORE" << pieceToCheck->getPieceColor();
        if(isKingCanGoOutofCheck(pieceToCheck))
            return false;
        qDebug() << attackerPieces.size() << "-WE ARE AFTER";
        
        QPointF pieceCoord = pieceToCheck->scenePos();
        ChessPiece* attacker = *attackerPieces.begin();
        
        switch(attacker->getType()){
            
        case PieceType::knight:{
            
            for(auto* elem : m_piecesArr) {
                
                if(elem->getPieceColor() != pieceToCheck->getPieceColor())
                    continue;
                
                if(isPieceAbleToMoveAt(attacker->scenePos(), elem))
                    return false;
                
            }
            
        }break;
            
        case PieceType::whitePawn:{
            
            for(auto* elem : m_piecesArr) {
                
                if(elem->getPieceColor() != pieceToCheck->getPieceColor())
                    continue;
                
                if(isPieceAbleToMoveAt(attacker->scenePos(), elem))
                    return false;
                
            }
            
        }break;
            
        case PieceType::blackPawn:{
            
            for(auto* elem : m_piecesArr) {
                
                if(elem->getPieceColor() != pieceToCheck->getPieceColor())
                    continue;
                
                if(isPieceAbleToMoveAt(attacker->scenePos(), elem))
                    return false;
                
            }
            
        }break;
        
        default:{
            
            qDebug() << attackerPieces.size() << "-WE ARE HERE";
            QPointF attackVector = attacker->scenePos() - pieceCoord;
            QPointF normalAttackVector = attackVector/m_cellSize;
            
            int counter = std::max(abs(normalAttackVector.x()), abs(normalAttackVector.y()));
            
            normalAttackVector /= counter;
            qDebug() << "Counter-" << counter << " normalAttackVector" << normalAttackVector;
            
            for(int i = 1; i < counter+1; ++i){
                
                pieceCoord += QPointF(normalAttackVector.x() * m_cellSize, normalAttackVector.y() * m_cellSize);
                qDebug() << pieceCoord;
                
                for(auto* elem : m_piecesArr) {
                    
                    if(elem->getPieceColor() != pieceToCheck->getPieceColor() or elem->getType() == PieceType::king)
                        continue;
                    
                    if(isPieceAbleToMoveAt(pieceCoord, elem))
                        return false;
                    
                }
                
            }
            
        } break;
            
        }

    }
    if(attackerPieces.size() == 0)
        return false;

    return true;

}

bool ChessBoard::isKingCanGoOutofCheck(ChessPiece *pieceToCheck) {
    
    QPointF pieceCoord = pieceToCheck->scenePos();
    
    ChessPiece* piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize));
    if(piece == nullptr or piece->getPieceColor() != pieceToCheck->getPieceColor()){
        if(pieceCoord.x() + m_cellSize >= 0 and pieceCoord.x() + m_cellSize <= 560 and
        pieceCoord.y() + m_cellSize >= 0 and pieceCoord.y() + m_cellSize <= 560) {
            tempIgnoredPiece = piece;
            pieceToCheck->setPos(pieceCoord.x() + m_cellSize, pieceCoord.y() + m_cellSize);
            if(!isPieceChecked(pieceToCheck)) {
                tempIgnoredPiece = nullptr;
                pieceToCheck->setPos(pieceCoord);
                qDebug() << "1";
                return true;
            }
            else{
                pieceToCheck->setPos(pieceCoord);
                tempIgnoredPiece = nullptr;
            }
        }
    }
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y()));
    if(piece == nullptr or piece->getPieceColor() != pieceToCheck->getPieceColor()){
        if(pieceCoord.x() + m_cellSize >= 0 and pieceCoord.x() + m_cellSize <= 560 and
            pieceCoord.y() >= 0 and pieceCoord.y() <= 560) {
            tempIgnoredPiece = piece;
            pieceToCheck->setPos(pieceCoord.x() + m_cellSize, pieceCoord.y());
            if(!isPieceChecked(pieceToCheck)) {
                tempIgnoredPiece = nullptr;
                pieceToCheck->setPos(pieceCoord);
                qDebug() << "2";
                return true;
            }
            else{
                pieceToCheck->setPos(pieceCoord);
                tempIgnoredPiece = nullptr;
            }
        }
    }
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece == nullptr or piece->getPieceColor() != pieceToCheck->getPieceColor()){
        if(pieceCoord.x() + m_cellSize >= 0 and pieceCoord.x() + m_cellSize <= 560 and
            pieceCoord.y() - m_cellSize >= 0 and pieceCoord.y() - m_cellSize <= 560) {
            tempIgnoredPiece = piece;
            pieceToCheck->setPos(pieceCoord.x() + m_cellSize, pieceCoord.y() - m_cellSize);
            if(!isPieceChecked(pieceToCheck)) {
                tempIgnoredPiece = nullptr;
                pieceToCheck->setPos(pieceCoord);
                qDebug() << "3";
                return true;
            }
            else{
                pieceToCheck->setPos(pieceCoord);
                tempIgnoredPiece = nullptr;
            }
        }
    }
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() - m_cellSize));
    if(piece == nullptr or piece->getPieceColor() != pieceToCheck->getPieceColor()){
        if(pieceCoord.x()>= 0 and pieceCoord.x()<= 560 and
            pieceCoord.y() - m_cellSize >= 0 and pieceCoord.y() - m_cellSize <= 560) {
            tempIgnoredPiece = piece;
            pieceToCheck->setPos(pieceCoord.x(), pieceCoord.y() - m_cellSize);
            if(!isPieceChecked(pieceToCheck)) {
                tempIgnoredPiece = nullptr;
                pieceToCheck->setPos(pieceCoord);
                qDebug() << "4";
                return true;
            }
            else{
                pieceToCheck->setPos(pieceCoord);
                tempIgnoredPiece = nullptr;
            }
        }
    }
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize));
    if(piece == nullptr or piece->getPieceColor() != pieceToCheck->getPieceColor()){
        if(pieceCoord.x() - m_cellSize >= 0 and pieceCoord.x() - m_cellSize <= 560 and
            pieceCoord.y() - m_cellSize >= 0 and pieceCoord.y() - m_cellSize <= 560) {
            tempIgnoredPiece = piece;
            pieceToCheck->setPos(pieceCoord.x() - m_cellSize, pieceCoord.y() - m_cellSize);
            if(!isPieceChecked(pieceToCheck)) {
                tempIgnoredPiece = nullptr;
                pieceToCheck->setPos(pieceCoord);
                qDebug() << "5";
                return true;
            }
            else{
                pieceToCheck->setPos(pieceCoord);
                tempIgnoredPiece = nullptr;
            }
        }
    }
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y()));
    if(piece == nullptr or piece->getPieceColor() != pieceToCheck->getPieceColor()){
        if(pieceCoord.x() - m_cellSize >= 0 and pieceCoord.x() - m_cellSize <= 560 and
            pieceCoord.y()>= 0 and pieceCoord.y()<= 560) {
            tempIgnoredPiece = piece;
            pieceToCheck->setPos(pieceCoord.x() - m_cellSize, pieceCoord.y());
            if(!isPieceChecked(pieceToCheck)) {
                tempIgnoredPiece = nullptr;
                pieceToCheck->setPos(pieceCoord);
                qDebug() << "6";
                return true;
            }
            else{
                pieceToCheck->setPos(pieceCoord);
                tempIgnoredPiece = nullptr;
            }
        }
    }
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize));
    if(piece == nullptr or piece->getPieceColor() != pieceToCheck->getPieceColor()){
        if(pieceCoord.x() - m_cellSize >= 0 and pieceCoord.x() - m_cellSize <= 560 and
            pieceCoord.y() + m_cellSize >= 0 and pieceCoord.y() + m_cellSize <= 560) {
            tempIgnoredPiece = piece;
            pieceToCheck->setPos(pieceCoord.x() - m_cellSize, pieceCoord.y() + m_cellSize);
            if(!isPieceChecked(pieceToCheck)) {
                tempIgnoredPiece = nullptr;
                pieceToCheck->setPos(pieceCoord);
                qDebug() << "7";
                return true;
            }
            else{
                pieceToCheck->setPos(pieceCoord);
                tempIgnoredPiece = nullptr;
            }
        }
    }
    
    piece = findPeiceOnCoords(QPointF(pieceCoord.x(), pieceCoord.y() + m_cellSize));
    if(piece == nullptr or piece->getPieceColor() != pieceToCheck->getPieceColor()){
        if(pieceCoord.x()>= 0 and pieceCoord.x()<= 560 and
            pieceCoord.y() + m_cellSize >= 0 and pieceCoord.y() + m_cellSize <= 560) {
            tempIgnoredPiece = piece;
            pieceToCheck->setPos(pieceCoord.x(), pieceCoord.y() + m_cellSize);
            if(!isPieceChecked(pieceToCheck)) {
                tempIgnoredPiece = nullptr;
                pieceToCheck->setPos(pieceCoord);
                qDebug() << "8";
                return true;
            }
            else{
                pieceToCheck->setPos(pieceCoord);
                tempIgnoredPiece = nullptr;
            }
        }
    }
    
    return false;
    
}

bool ChessBoard::isPieceAbleToMoveAt(QPointF coords, ChessPiece* senderPiece) {
    
    QPointF delta = coords - senderPiece->scenePos();
    delta /= 80;
    delta.setY(delta.y()*-1);
    QPointF newPos = coords;
    QPointF oldPos = senderPiece->scenePos();
    
    int reverseForPawn = m_playerSide ? 1 : -1;
    ChessPiece* playerKing = (m_playerSide == senderPiece->getPieceColor() ? m_playerKing : m_enemyKing);
    ChessPiece* enemyKing = (m_playerSide == senderPiece->getPieceColor() ? m_enemyKing : m_playerKing);
    
    switch(senderPiece->getType()){
        
    case PieceType::knight:{
        
        if( (delta.x() == 1 and (delta.y() == 2 or delta.y() == -2)) or
            (delta.x() == 2 and (delta.y() == 1 or delta.y() == -1)) or
            (delta.x() == -1 and (delta.y() == 2 or delta.y() == -2)) or
            (delta.x() == -2 and (delta.y() == 1 or delta.y() == -1))){
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                senderPiece->setPos(oldPos);
                m_lastChosenPiece = nullptr;
                return false;
                
            }
            else {
                senderPiece->setPos(oldPos);
                m_lastChosenPiece = nullptr;
                return true;
                
            }
        }
        else{
            senderPiece->setPos(oldPos);
            m_lastChosenPiece = nullptr;
            return false;
            
        }
        
    } break;
        
    case PieceType::king:{
        
        if(std::abs(delta.x()) <= 1 and std::abs(delta.y()) <= 1){
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                senderPiece->setPos(oldPos);
                m_lastChosenPiece = nullptr;
                return false;
                
            }
            else {
                senderPiece->setPos(oldPos);
                m_lastChosenPiece = nullptr;
                return true;
                
            }
        }
        else{
            senderPiece->setPos(oldPos);
            m_lastChosenPiece = nullptr;
            return false;
            
        }
        
    } break;
        
    case PieceType::bishop:{
        
        if(std::abs(delta.x()/delta.x()) == 1 and std::abs(delta.y()/delta.x()) == 1 and !isPieceOnWay(oldPos, newPos)) {
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                senderPiece->setPos(oldPos);
                m_lastChosenPiece = nullptr;
                return false;
                
            }
            else {
                senderPiece->setPos(oldPos);
                m_lastChosenPiece = nullptr;
                return true;
                
            }
        }
        else{
            senderPiece->setPos(oldPos);
            m_lastChosenPiece = nullptr;
            return false;
            
        }
        
        
    } break;
        
    case PieceType::rook:{
        
        if((delta.x() == 0 or delta.y() == 0) and !isPieceOnWay(oldPos, newPos)){
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                senderPiece->setPos(oldPos);
                m_lastChosenPiece = nullptr;
                return false;
                
            }
            else {
                senderPiece->setPos(oldPos);
                m_lastChosenPiece = nullptr;
                return true;
                
            }
        }
        else{
            senderPiece->setPos(oldPos);
            m_lastChosenPiece = nullptr;
            return false;
            
        }
        
        
    } break;
        
    case PieceType::queen:{
        
        if(!isPieceOnWay(oldPos, newPos) and ((std::abs(delta.x()/delta.x()) == 1 and std::abs(delta.y()/delta.x()) == 1) or
            (delta.x() == 0 or delta.y() == 0))) {
            
            ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                senderPiece->setPos(oldPos);
                m_lastChosenPiece = nullptr;
                return false;
                
            }
            else {
                senderPiece->setPos(oldPos);
                m_lastChosenPiece = nullptr;
                return true;
                
            }
        }
        else{
            senderPiece->setPos(oldPos);
            m_lastChosenPiece = nullptr;
            return false;
            
        }
        
        
    } break;
        
    case PieceType::whitePawn:{
        
        ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
        
        if((delta.x() == 1 or delta.x() == -1) and delta.y() == reverseForPawn*1 and pieceToDel != nullptr){
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                tempIgnoredPiece = nullptr;
                senderPiece->setPos(oldPos);
                return false;
            }
            else {
                tempIgnoredPiece = nullptr;
                senderPiece->setPos(oldPos);
                return true;
            }
        }
        
        if(pieceToDel == nullptr and !isPieceOnWay(oldPos, newPos) and delta.x() == 0 and
        (delta.y() == reverseForPawn*1 or (senderPiece->getTurnsCount() == 0 and delta.y() == reverseForPawn*2))) {
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                senderPiece->setPos(oldPos);
                tempIgnoredPiece = nullptr;
                return false;
                
            }
            else {
                if(pieceToDel == nullptr) {
                    
                    senderPiece->setPos(oldPos);
                    tempIgnoredPiece = nullptr;
                    return true;
                }
                else {
                    senderPiece->setPos(oldPos);
                    tempIgnoredPiece = nullptr;
                    return false;
                }
            }
        }
        else{
            senderPiece->setPos(oldPos);
            tempIgnoredPiece = nullptr;
            return false;
        }
        
        
    } break;
        
    case PieceType::blackPawn:{
        
        ChessPiece* pieceToDel = findPeiceOnCoords(newPos);
        
        if((delta.x() == 1 or delta.x() == -1) and delta.y() == -reverseForPawn*1 and pieceToDel != nullptr){
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                tempIgnoredPiece = nullptr;
                senderPiece->setPos(oldPos);
                return false;
            }
            else {
                tempIgnoredPiece = nullptr;
                senderPiece->setPos(oldPos);
                return true;
            }
            
        }
        
        if(pieceToDel == nullptr and !isPieceOnWay(oldPos, newPos) and delta.x() == 0 and
        (delta.y() == -reverseForPawn*1 or (senderPiece->getTurnsCount() == 0 and delta.y() == -reverseForPawn*2))) {
            
            tempIgnoredPiece = pieceToDel;
            senderPiece->setPos(newPos);
            if(playerKing != nullptr and isPieceChecked(playerKing)) {
                senderPiece->setPos(oldPos);
                tempIgnoredPiece = nullptr;
                return false;
                
            }
            else {
                if(pieceToDel == nullptr) {
                    
                    senderPiece->setPos(oldPos);
                    tempIgnoredPiece = nullptr;
                    return true;
                }
                else {
                    senderPiece->setPos(oldPos);
                    tempIgnoredPiece = nullptr;
                    return false;
                }
            }
        }
        else{
            senderPiece->setPos(oldPos);
            tempIgnoredPiece = nullptr;
            return false;
        }
        
        
    } break;
        
    default:{
        senderPiece->setPos(oldPos);
        tempIgnoredPiece = nullptr;
    } break;
        
    }
}

void ChessBoard::clearBoard() {
    
    for(auto elem : attackerPieces){
        
        delete elem;
        
    }
    for(auto elem : m_tipsArr){
        
        delete elem;
        
    }
    for(auto elem : m_piecesArr){
        
        delete elem;
        
    }
    
    m_playerKing = nullptr;
    m_enemyKing = nullptr;
    tempIgnoredPiece = nullptr;
    m_lastChosenPiece = nullptr;
    
    attackerPieces.clear();
    m_tipsArr.clear();
    m_piecesArr.clear();
    this->clear();
    
}

void ChessBoard::checkMateFor(bool color) {
    
    qDebug() << "<<<CHECKMATE FOR " << (color ? "WHITE>>>" : "BLACK>>>");
    emit openWinInfoWidget(true, color);
    
}

void ChessBoard::staleMate() {
    
    qDebug() << "<<<STALEMATE>>>";
    emit openWinInfoWidget(false, false);
}

void ChessBoard::changeTurnColorTo(bool color) {
    
    for(auto elem : m_piecesArr){
        
        if(elem->getPieceColor() == color)
            elem->setMoveFlag(true);
        else
            elem->setMoveFlag(false);
        
    }
    
}

void ChessBoard::setOfflineMatch() {
    m_isOfflineMatch = true;
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
        
    if(m_lastChosenPiece != nullptr) {

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















