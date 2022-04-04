
#include <ctime>
#include <time.h>

#include "UserService.hpp"
#include "utils/validators.h"

#include "iostream"


oatpp::Object<PhoneDto> UserService::updatePhone(const oatpp::Object<PhoneDto>& dto) {

  OATPP_ASSERT_HTTP( validatePhone( dto->phone) , Status::CODE_400, "Not a valid date, or phone");
  auto dbResult = m_database->updatePhone(dto);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  return getPhoneById(dto->id);
}


oatpp::Object<PhoneDto> UserService::getPhoneById(const oatpp::Int32& id ) {

  auto dbResult = m_database->getPhoneById( id );
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "Phone not found");


  auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<PhoneDto>>>();
  OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

  return result[0];
}


oatpp::Vector<oatpp::Object<PhoneDto>> UserService::getPhoneByUserId(const oatpp::Int32& id ) {

  auto dbResult = m_database->getPhoneByUserId( id );
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "User not found");


  auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<PhoneDto>>>();
  //OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

  return result;
}


oatpp::Object<StatusDto> UserService::deletePhoneById(const oatpp::Int32& userId) {
  
  auto dbResult =  m_database->deletePhoneById(userId);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "Phone not found");

  auto status = StatusDto::createShared();
  status->status = "OK";
  status->code = 200;
  status->message = "Phone was successfully deleted";
  return status;
}