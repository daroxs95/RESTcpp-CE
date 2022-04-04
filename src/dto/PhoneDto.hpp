#pragma once

#include <oatpp/core/macro/codegen.hpp>


#include OATPP_CODEGEN_BEGIN(DTO)


class PhoneDto : public oatpp::DTO {
  
  DTO_INIT(PhoneDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(Int32, userId, "user_id");
  DTO_FIELD(String, phone, "phone");

};

#include OATPP_CODEGEN_END(DTO)