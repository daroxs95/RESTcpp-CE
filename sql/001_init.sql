
CREATE TABLE Users (
    id              INTEGER PRIMARY KEY,
    first_name      VARCHAR,
    second_name      VARCHAR,
    addresses       VARCHAR,
    date_of_birth   VARCHAR,
    phone_numbers   VARCHAR,
    photo   VARCHAR
);

INSERT INTO Users
(first_name, second_name, addresses, date_of_birth, phone_numbers, photo) VALUES ('admin', 'admin@domain.com', 'admin', 'ROLE_ADMIN', 'ROLE_ADMIN', 'ROLE_ADMIN');
