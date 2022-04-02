#pragma once

#include <oatpp/core/macro/codegen.hpp>


#include OATPP_CODEGEN_BEGIN(DTO)


class UserDto : public oatpp::DTO {
  
  DTO_INIT(UserDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, firstName, "first_name");
  DTO_FIELD(String, secondName, "second_name");
  DTO_FIELD(String, addresses, "addresses");
  DTO_FIELD(String, dateOfBirth, "date_of_birth");
  DTO_FIELD(String, phoneNumbers, "phone_numbers");
  DTO_FIELD(String, personalPhoto, "photo");

};

#include OATPP_CODEGEN_END(DTO)