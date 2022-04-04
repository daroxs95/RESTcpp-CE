#include "validators.h"

// function to validate date utc iso 8601
bool validateDate (const std::string& date) {
  std::regex pattern("^[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}Z$");
  std::regex patternNoTime("^[0-9]{4}-[0-9]{2}-[0-9]{2}$");
  return (std::regex_match(date, pattern) || std::regex_match(date, patternNoTime) );
};


// function to validate phone number
bool validatePhone (const std::string& phone) {
  std::regex pattern("^\\+?[0-9]{10,15}$");
  return std::regex_match(phone, pattern);
};


// function to validate string as base 64 encoded
bool validatebase64 (const std::string& str) {
  std::regex pattern("^([A-Za-z0-9+/]{4})*([A-Za-z0-9+/]{4}|[A-Za-z0-9+/]{3}=|[A-Za-z0-9+/]{2}==)$");
  return std::regex_match(str, pattern);
};