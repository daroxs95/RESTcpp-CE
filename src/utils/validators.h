#pragma once

#include <regex>

// function to validate date utc iso 8601
bool validateDate (const std::string& date);


// function to validate phone number
bool validatePhone (const std::string& phone);

// function to validate string as base 64 encoded
bool validatebase64 (const std::string& str);
