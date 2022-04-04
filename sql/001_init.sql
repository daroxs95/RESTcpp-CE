
CREATE TABLE Users (
    id              INTEGER PRIMARY KEY,
    first_name      VARCHAR,
    second_name     VARCHAR,
    date_of_birth   DATE,
    photo           BLOB
);

CREATE TABLE Addresses (
    id                      INTEGER PRIMARY KEY,
    address                 VARCHAR,
    user_id                 INTEGER,
    FOREIGN KEY(user_id)    REFERENCES Users(id) ON DELETE CASCADE
);

CREATE TABLE Phones (
    id                      INTEGER PRIMARY KEY,
    phone                   VARCHAR,
    user_id                 INTEGER,
    FOREIGN KEY(user_id)    REFERENCES Users(id) ON DELETE CASCADE
);


INSERT INTO Users
(first_name, second_name, date_of_birth, photo) VALUES ('admin', 'admin@domain.com', 'admin', 'ROLE_ADMIN' );
