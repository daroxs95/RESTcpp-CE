#pragma once

#include "oatpp/orm/SchemaMigration.hpp"
#include "oatpp/orm/DbClient.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include "dto/UserDto.hpp"

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
        "(first_name, second_name, addresses, date_of_birth, phone_numbers, photo) VALUES "
        "(:user.first_name, :user.second_name, :user.addresses, :user.date_of_birth, :user.phone_numbers, :user.photo);",
        PARAM(oatpp::Object<UserDto>, user) )

  QUERY(getUserByFirstName, 
        "SELECT * FROM Users WHERE first_name=:firstName;", 
        PARAM(oatpp::String, firstName)) 

  QUERY(updateUser,
        "UPDATE Users "
        "SET "
        " first_name=:user.first_name, "
        " second_name=:user.second_name, "
        " addresses=:user.addresses, "
        " date_of_birth=:user.date_of_birth, "
        " phone_numbers=:user.phone_numbers, "
        " photo=:user.photo, "
        "WHERE "
        " id=:user.id;",
        PARAM(oatpp::Object<UserDto>, user))

  QUERY(getUserById,
        "SELECT * FROM Users WHERE id=:id;",
        PARAM(oatpp::Int32, id))

  QUERY(getAllUsers,
        "SELECT * FROM Users LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::UInt32, offset),
        PARAM(oatpp::UInt32, limit))

  QUERY(deleteUserById,
        "DELETE FROM Users WHERE id=:id;",
        PARAM(oatpp::Int32, id))

};

#include OATPP_CODEGEN_END(DbClient) ///< End code-gen section