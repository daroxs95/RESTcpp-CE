#pragma once

#include "oatpp/orm/SchemaMigration.hpp"
#include "oatpp/orm/DbClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "dto/UserDto.hpp"
#include "dto/AddressDto.hpp"
#include "dto/PhoneDto.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient) ///< Begin code-gen section

class UserDb : public oatpp::orm::DbClient {
public:

  UserDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
  {
    oatpp::orm::SchemaMigration migration(executor);
    migration.addFile(1 /* start from version 1 */, DATABASE_MIGRATIONS "/001_init.sql" );
    // TODO - Add more migrations here.
    migration.migrate(); // <-- run migrations. This guy will throw on error.

    auto version = executor->getSchemaVersion();
    OATPP_LOGD("UserDb", "Migration - OK. Version=%lld.", version);
  }
  
  QUERY(createUser,
        "INSERT INTO Users" 
        "(first_name, second_name, date_of_birth, photo) VALUES "
        "(:user.first_name, :user.second_name, :user.date_of_birth, :user.photo);",
        PARAM(oatpp::Object<UserDto>, user) )

  QUERY(updateUser,
        "UPDATE Users "
        "SET "
        " first_name=:user.first_name, "
        " second_name=:user.second_name, "
        " date_of_birth=:user.date_of_birth, "
        " photo=:user.photo "
        "WHERE "
        " id=:user.id;",
        PARAM(oatpp::Object<UserDto>, user))

  QUERY(getUserById,
        "SELECT Users.id, Users.first_name, Users.second_name, Users.date_of_birth, Users.photo FROM Users WHERE id=:id;",
        PARAM(oatpp::Int32, id))

  QUERY(deleteUserById,
        "DELETE FROM Users WHERE id=:id;",
        PARAM(oatpp::Int32, id))



  // Address related
  QUERY(createAddress,
      "INSERT INTO Addresses" 
      "(address, user_id ) VALUES "
      "(:address.address, :address.user_id );",
      PARAM(oatpp::Object<AddressDto>, address) )

  QUERY(getAddressById, 
        "SELECT Addresses.id, Addresses.address, Addresses.user_id FROM Addresses WHERE id=:id;", 
        PARAM(oatpp::Int32, id))

  QUERY(getAddressByUserId, 
        "SELECT Addresses.id, Addresses.address, Addresses.user_id FROM Addresses WHERE user_id=:user_id;", 
        PARAM(oatpp::Int32, user_id))

  QUERY(updateAddress,
        "UPDATE Addresses "
        "SET "
        " address=:address.address, "
        " user_id=:address.user_id "
        "WHERE "
        " id=:address.id;",
        PARAM(oatpp::Object<AddressDto>, address))

  QUERY(deleteAddressById,
        "DELETE FROM Addresses WHERE id=:id;",
        PARAM(oatpp::Int32, id))

  QUERY(deleteAddressByUserId,
        "DELETE FROM Addresses WHERE user_id=:id;",
        PARAM(oatpp::Int32, id))




  // Phone related
  QUERY(createPhone,
      "INSERT INTO Phones" 
      "(phone, user_id ) VALUES "
      "(:phone.phone, :phone.user_id );",
      PARAM(oatpp::Object<PhoneDto>, phone) )

  QUERY(getPhoneByUserId, 
        "SELECT Phones.id, Phones.phone, Phones.user_id FROM Phones WHERE user_id=:user_id;", 
        PARAM(oatpp::Int32, user_id))

  QUERY(getPhoneById, 
        "SELECT Phones.id, Phones.phone, Phones.user_id FROM Phones WHERE id=:id;", 
        PARAM(oatpp::Int32, id))

  QUERY(updatePhone,
        "UPDATE Phones "
        "SET "
        " phone=:phone.phone, "
        " user_id=:phone.user_id "
        "WHERE "
        " id=:phone.id;",
        PARAM(oatpp::Object<PhoneDto>, phone))

  QUERY(deletePhoneById,
        "DELETE FROM Phones WHERE id=:id;",
        PARAM(oatpp::Int32, id))

  QUERY(deletePhoneByUserId,
        "DELETE FROM Phones WHERE user_id=:id;",
        PARAM(oatpp::Int32, id))



  QUERY(getUsersFilteredByNameAddressAndAge,
        "SELECT DISTINCT Users.id, Users.first_name, Users.second_name, Users.date_of_birth, Users.photo FROM Users "
        "LEFT JOIN Addresses ON Users.id = Addresses.user_id "
        "WHERE Addresses.address LIKE :filter "
        "OR first_name LIKE :filter or second_name LIKE :filter; "
        "AND date_of_birth BETWEEN :min_date AND :max_date "
        "LIMIT :limit OFFSET :offset; ",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit),
        PARAM(oatpp::String, min_date),
        PARAM(oatpp::String, max_date),
        PARAM(oatpp::String, filter))

};

#include OATPP_CODEGEN_END(DbClient) ///< End code-gen section