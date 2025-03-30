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
        files: ["mainwindow.cpp",
                "mainwindow.h"]
    }
    Group{
        name: "ChessBoard";
        files: ["chessboard.cpp",
                "chessboard.h"]
    }
    Group{
        name: "MenuWidget";
        files: ["menuwidget.cpp",
                "menuwidget.h",
                "menuwidget.ui"]
    }
    Group{
        name: "ClassicalChessWidget";
        files: ["classicalchesswidget.cpp",
                "classicalchesswidget.h",
                "classicalchesswidget.ui"]
    }
    
    files: [
        "main.cpp",
        "resources.qrc"
    ]
    
    
    install: true
    installDir: qbs.targetOS.contains("qnx") ? FileInfo.joinPaths("/tmp", name, "bin") : base
}
