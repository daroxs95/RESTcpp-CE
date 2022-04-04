
#include <ctime>
#include <time.h>

#include "UserService.hpp"
#include "utils/validators.h"

#include "iostream"


oatpp::Object<AddressDto> UserService::updateAddress(const oatpp::Object<AddressDto>& dto) {

  auto dbResult = m_database->updateAddress(dto);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  return getAddressById(dto->id);
}


oatpp::Object<AddressDto> UserService::getAddressById(const oatpp::Int32& id ) {

  auto dbResult = m_database->getAddressById( id );
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "Address not found");


  auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<AddressDto>>>();
  OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

  return result[0];
}


oatpp::Vector<oatpp::Object<AddressDto>> UserService::getAddressByUserId(const oatpp::Int32& id ) {

  auto dbResult = m_database->getAddressByUserId( id );
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "User not found");


  auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<AddressDto>>>();
  //OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

  return result;
}


oatpp::Object<StatusDto> UserService::deleteAddressById(const oatpp::Int32& userId) {
  
  auto transaction = m_database->beginTransaction();
  m_database->deleteAddressById(userId, transaction.getConnection());
  auto dbResult = transaction.commit();

  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "Address not found");

  auto status = StatusDto::createShared();
  status->status = "OK";
  status->code = 200;
  status->message = "Address was successfully deleted";
  return status;
}