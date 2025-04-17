import qbs.FileInfo

QtApplication {
    
    Depends {
        
        name: "Qt"
        submodules: ["widgets", "network", "core", "multimedia", "gui"]
        
    }
    
    cpp.defines: [
        
    ]
    
    Group{
        name: "MainWindow";
        files: [
            "choosepiecewidget.cpp",
            "choosepiecewidget.h",
            "choosepiecewidget.ui",
            "mainwindow.cpp",
            "mainwindow.h",
        ]
    }
    Group{
        name: "ChessBoard";
        files: [
            "chessboard.cpp",
            "chessboard.h",
            "chesspiece.cpp",
            "chesspiece.h",
            "customrectangle.cpp",
            "customrectangle.h",
        ]
    }
    Group{
        name: "MenuWidget";
        files: [
            "menuwidget.cpp",
            "menuwidget.h",
            "menuwidget.ui"]
    }
    Group{
        name: "ClassicalChessWidget";
        files: [
            "classicalchesswidget.cpp",
            "classicalchesswidget.h",
            "classicalchesswidget.ui",
            "historyblockwidget.cpp",
            "historyblockwidget.h",
            "historyblockwidget.ui",
        ]
    }
    
    files: [
        "main.cpp",
        "resources.qrc",
    ]
    
    
    install: true
    installDir: qbs.targetOS.contains("qnx") ? FileInfo.joinPaths("/tmp", name, "bin") : base
}
