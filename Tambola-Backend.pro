CONFIG -= qt
CONFIG += c++17

LIBS += -lpthread -lboost_json -lpqxx -lpq -lfmt

SOURCES += \
    Board/BoardHandler.cpp \
    Board/Request_Board.cpp \
    Database/CockroachDB.cpp \
    Database/PostgreSQL.cpp \
    GameSession/GameSession.cpp \
    GameSession/GameSessionHandler.cpp \
    GameSession/Request_GameSession.cpp \
    Handler/Handler.cpp \
    Handler/HandlerManager.cpp \
    HttpSession.cpp \
    RequestUtility.cpp \
    Ticket/Request_Ticket.cpp \
    Ticket/Ticket.cpp \
    Ticket/TicketHandler.cpp \
    User/Request_User.cpp \
    User/User.cpp \
    User/UsersHandler.cpp \
    User/UsersOwn.cpp \
    WsSession.cpp \
    Board/board.cpp \
    listener.cpp \
    main.cpp \
    session.cpp

HEADERS += \
    Board/BoardHandler.h \
    Board/board_db.h \
    Database/CockroachDB.h \
    Database/DBObject.h \
    Database/Database.h \
    Database/PostgreSQL.h \
    GameSession/GameSession.h \
    GameSession/GameSessionHandler.h \
    GameSession/GameSession_db.h \
    Handler/Handler.h \
    Handler/HandlerManager.h \
    HttpSession.h \
    ORM/Column.h \
    ORM/ORM_BASE.h \
    ORM/Users.h \
    RequestUtility.h \
    TException.h \
    TObject.h \
    Ticket/Ticket.h \
    Ticket/TicketHandler.h \
    Ticket/ticket_db.h \
    User/User.h \
    User/User_db.h \
    User/UsersHandler.h \
    User/UsersOwn.h \
    WsSession.h \
    Board/board.h \
    listener.h \
    session.h

DISTFILES += Data/request.json \
              Data/response.json \
              Data/update.json \
              Data/schema.sql
