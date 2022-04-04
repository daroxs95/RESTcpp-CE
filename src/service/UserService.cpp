
#include <ctime>
#include <time.h>

#include "UserService.hpp"
#include "utils/validators.h"

#include "iostream"

void UserService::fillAddressAndPhones ( UserDto::Wrapper user, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection ){
  // TODO check for errors here
  auto addresses = m_database->getAddressByUserId(user->id, connection)->fetch<oatpp::Vector<oatpp::Object<AddressDto>>>();
  auto phones = m_database->getPhoneByUserId(user->id, connection)->fetch<oatpp::Vector<oatpp::Object<PhoneDto>>>();

  user->addresses = {};
  user->phoneNumbers = {};

  for(auto& i : *addresses) {
      user->addresses->push_back(i->address->c_str() );
  }
  for(auto& i : *phones) {
      user->phoneNumbers->push_back(i->phone->c_str() );
  }  
}

oatpp::Object<UserDto> UserService::createUser(const oatpp::Object<UserDto>& dto) {

  bool shallSave = true;
  shallSave = validateDate( dto->dateOfBirth ); 

  // not validating images because regex_error(error_stack)
  //if ( !validatebase64( dto->personalPhoto ) ) shallSave = false;
  
  for (size_t i = 0; i < dto->phoneNumbers->size() ; i++)
  {
    if (!validatePhone( dto->phoneNumbers[i] ) ){
      shallSave = false;
      break;
    }
  }

  if(!shallSave){
    OATPP_ASSERT_HTTP( false, Status::CODE_400, "Not a valid date, phone, or image");
  }
  
  auto dbResult = m_database->createUser(dto);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  auto userId = oatpp::sqlite::Utils::getLastInsertRowId(dbResult->getConnection());
  
  // Saving addresses to database
  for (size_t i = 0; i < dto->addresses->size() ; i++)
  {
    auto address = AddressDto::createShared();
    address->address = dto->addresses[i];
    address->userId = userId;
    auto dbResult = m_database->createAddress(address);
  }
  // Saving phone numbers to database
  for (size_t i = 0; i < dto->phoneNumbers->size() ; i++)
  {
    auto phone = PhoneDto::createShared();
    phone->phone = dto->phoneNumbers[i];
    phone->userId = userId;
    auto dbResult = m_database->createPhone(phone);
  }

  return getUserById((v_int32) userId);
}

oatpp::Object<UserDto> UserService::updateUser(const oatpp::Object<UserDto>& dto) {

  auto dbResult = m_database->updateUser(dto);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  // Saving addresses to database
  for (size_t i = 0; i < dto->addresses->size() ; i++)
  {
    auto address = AddressDto::createShared();
    address->address = dto->addresses[i];
    address->userId = dto->id;
    auto dbResult = m_database->createAddress(address);
  }
  // Saving phone numbers to database
  for (size_t i = 0; i < dto->phoneNumbers->size() ; i++)
  {
    auto phone = PhoneDto::createShared();
    phone->phone = dto->phoneNumbers[i];
    phone->userId = dto->id;
    auto dbResult = m_database->createPhone(phone);
  }

  return getUserById(dto->id);

}

oatpp::Object<UserDto> UserService::getUserById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection) {

  auto dbResult = m_database->getUserById(id, connection);
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "User not found");


  auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
  OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

  // TODO check for errors here
  auto addresses = m_database->getAddressByUserId(id, connection)->fetch<oatpp::Vector<oatpp::Object<AddressDto>>>();
  auto phones = m_database->getPhoneByUserId(id, connection)->fetch<oatpp::Vector<oatpp::Object<PhoneDto>>>();

  fillAddressAndPhones(result[0], connection);

  return result[0];

}

oatpp::Object<PageDto<oatpp::Object<UserDto>>> UserService::getAllUsers(const oatpp::UInt32& offset, 
                                                                const oatpp::UInt32& limit,
                                                                const oatpp::UInt32& minAge,
                                                                const oatpp::UInt32& maxAge,
                                                                const oatpp::String& filterText) {
  oatpp::UInt32 countToFetch = limit;

  if(limit > 10) {
    countToFetch = 10;
  }

  // Get current time as ISO-8601
  time_t now;
  time(&now);
  auto tmMaxBd = *gmtime(&now);
  auto tmMinBd = *gmtime(&now);
  tmMaxBd.tm_year -= minAge;
  tmMinBd.tm_year -= maxAge;
  char tmaxBirthDate[sizeof "2011-10-08T07:07:09Z"];
  char tminBirthDate[sizeof "2011-10-08T07:07:09Z"];
  strftime(tmaxBirthDate, sizeof tmaxBirthDate, "%FT%TZ", &tmMaxBd );
  strftime(tminBirthDate, sizeof tminBirthDate, "%FT%TZ", &tmMinBd );
  
  std::string maxBirthDate(tmaxBirthDate);
  std::string minBirthDate(tminBirthDate);

  if( minAge == -1 ) minBirthDate = "";
  if( maxAge == -1 ) maxBirthDate = "";

  // std::cout << minBirthDate <<" to " << maxBirthDate << "\n";

  auto dbResult = m_database->getUsersFilteredByNameAddressAndAge(offset, countToFetch, minBirthDate, maxBirthDate, "%"+filterText+"%");
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  auto items = dbResult->fetch<oatpp::Vector<oatpp::Object<UserDto>>>();
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  for (auto &i : *items)
  {
    fillAddressAndPhones(i);
  }
  
  auto page = PageDto<oatpp::Object<UserDto>>::createShared();
  page->offset = offset;
  page->limit = countToFetch;
  page->count = items->size();
  page->maxAge = maxAge;
  page->minAge = minAge;
  page->filterText = filterText;
  page->items = items;

  return page;

}


oatpp::Object<StatusDto> UserService::deleteUserById(const oatpp::Int32& userId) {

  
  auto transaction = m_database->beginTransaction();
  //This was me trying to enable the on delete cascade
  //m_database->executeQuery("PRAGMA foreign_keys = ON;", {} /* empty params map */, transaction.getConnection());
  m_database->deleteUserById(userId, transaction.getConnection());
  m_database->deleteAddressByUserId(userId, transaction.getConnection());
  m_database->deletePhoneByUserId(userId, transaction.getConnection());
  auto dbResult = transaction.commit();
  OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "User not found");

  //auto dbResult = m_database->deleteUserById(userId);
  //OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

  
  auto status = StatusDto::createShared();
  status->status = "OK";
  status->code = 200;
  status->message = "User was successfully deleted";
  return status;
}