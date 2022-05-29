// SEQUENCE
CREATE SEQUENCE uid;
CREATE SEQUENCE tid;
CREATE SEQUENCE bid;

// TYPE ENUM
CREATE TYPE USER_TYPE AS ENUM ('NONE', 'HOST', 'PLAYER');

CREATE TABLE Users (
              id INT DEFAULT nextval('uid') PRIMARY KEY,
              email STRING NOT NULL UNIQUE,
              password STRING DEFAULT '',
              name STRING,
              type user_type DEFAULT 'NONE',
              is_active BOOL DEFAULT true,
              timestamp INT NOT NULL
              );


// TABLE
CREATE TABLE boards
                  ( id INT DEFAULT nextval('bid') PRIMARY KEY,
                    uid INT NOT NULL REFERENCES users (id),
                    number_on_board INT[] DEFAULT '{}',
                    number_on_bucket INT[] DEFAULT '{}',
                    gid INT DEFAULT 0,
                    timestamp INT NOT NULL
                    );

CREATE TABLE public.boards (
                id INT8 NULL DEFAULT nextval('public.bid'::REGCLASS),
                uid INT8 NOT NULL,
                number_on_board INT8[] '{}',
                number_on_bucket INT8[] '{}',
                gid INT8 NULL,
                rowid INT8 NOT VISIBLE NOT NULL DEFAULT unique_rowid(),
                "timestamp" INT8 NOT NULL,
                CONSTRAINT "primary" PRIMARY KEY (rowid ASC),
                FAMILY "primary" (id, uid, number_on_board, number_on_bucket, gid, rowid, "timestamp")
            );

// GAME SESSION
CREATE TABLE game_sessions (id INT DEFAULT nextval('gid') PRIMARY KEY,
              bid INT NOT NULL REFERENCES boards (id),
              tids STRING[] DEFAULT '{}',
              timestamp INT NOT NULL);

// TICKET
CREATE TABLE tickets (id TEXT primary key not null,
                      tsid INT not null default nextval('tsid'),
                      row_0 int[], row_1 INT[], row_2 INT[],
                      crow_0 int[], crow_1 INT[], crow_2 INT[],
                      uid INT REFERENCES users (id),
                      timestamp INT,
                      gid INT REFERENCES game_sessions (id));
