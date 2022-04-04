#pragma once

#include <oatpp/core/macro/codegen.hpp>


#include OATPP_CODEGEN_BEGIN(DTO)


class UserDto : public oatpp::DTO {
  
  DTO_INIT(UserDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, firstName, "first_name");
  DTO_FIELD(String, secondName, "second_name");
  DTO_FIELD(Vector<String>, addresses, "addresses");
  DTO_FIELD_INFO(dateOfBirth) {
    info->description = "Date in the format of ISO-8601, UTC ( like: \"2011-10-08T07:07:09Z\")"; //<-- Fields description is integrated with Swagger-UI.
  }
  DTO_FIELD(String, dateOfBirth, "date_of_birth");
  DTO_FIELD(Vector<String>, phoneNumbers, "phone_numbers");
  DTO_FIELD_INFO(personalPhoto) {
    info->description = "Image in the format of base 64 encoded string"; //<-- Fields description is integrated with Swagger-UI.
  }
  DTO_FIELD(String, personalPhoto, "photo");

};

#include OATPP_CODEGEN_END(DTO)