#pragma once

#include <oatpp/core/macro/codegen.hpp>


#include OATPP_CODEGEN_BEGIN(DTO)


class AddressDto : public oatpp::DTO {
  
  DTO_INIT(AddressDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(Int32, userId, "user_id");
  DTO_FIELD(String, address, "address");

};

#include OATPP_CODEGEN_END(DTO)