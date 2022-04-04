
#pragma once

#include "db/db.hpp"
#include "dto/UserDto.hpp"
#include "dto/PageDto.hpp"
#include "dto/StatusDto.hpp"
#include "dto/PhoneDto.hpp"
#include "dto/AddressDto.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp-sqlite/orm.hpp" //< SQLite adapter for oatpp ORM


class UserService {
  private:
    typedef oatpp::web::protocol::http::Status Status;
  private:
    OATPP_COMPONENT(std::shared_ptr<UserDb>, m_database); // Inject database component
  public:

    oatpp::Object<UserDto>                          createUser(const oatpp::Object<UserDto>& dto);
    oatpp::Object<UserDto>                          updateUser(const oatpp::Object<UserDto>& dto);
    oatpp::Object<UserDto>                          getUserById(const oatpp::Int32& id, const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr);
    oatpp::Object<PageDto<oatpp::Object<UserDto>>>  getAllUsers(const oatpp::UInt32& offset, 
                                                                const oatpp::UInt32& limit,
                                                                const oatpp::UInt32& minAge,
                                                                const oatpp::UInt32& maxAge,
                                                                const oatpp::String& filterText);
    oatpp::Object<StatusDto>                        deleteUserById(const oatpp::Int32& id);
    void                                            fillAddressAndPhones ( UserDto::Wrapper user, 
                                                                          const oatpp::provider::ResourceHandle<oatpp::orm::Connection>& connection = nullptr );

    oatpp::Vector<oatpp::Object<AddressDto>>        getAddressByUserId( const oatpp::Int32& id );
    oatpp::Object<AddressDto>                       getAddressById(const oatpp::Int32& id );
    oatpp::Object<AddressDto>                       updateAddress(const oatpp::Object<AddressDto>& dto);
    oatpp::Object<StatusDto>                        deleteAddressById(const oatpp::Int32& id);

    oatpp::Vector<oatpp::Object<PhoneDto>>          getPhoneByUserId( const oatpp::Int32& id );
    oatpp::Object<PhoneDto>                         getPhoneById(const oatpp::Int32& id );
    oatpp::Object<PhoneDto>                         updatePhone(const oatpp::Object<PhoneDto>& dto);
    oatpp::Object<StatusDto>                        deletePhoneById(const oatpp::Int32& id);
};


