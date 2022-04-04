#pragma once

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include "dto/AddressDto.hpp"
#include "db/db.hpp"
#include "service/UserService.hpp"


#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen

/**
 * Sample Api Controller.
 */
class AddressController : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  AddressController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
private:
  UserService m_userService; // Create user service.
public:
  
  static std::shared_ptr<AddressController> createShared(
  OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) // Inject objectMapper component here as default parameter
  ){
    return std::make_shared<AddressController>(objectMapper);
  }


  ENDPOINT_INFO(putAddress) {
    info->summary = "Update Address by id";
    info->addTag("Addresses");

    info->addConsumes<Object<AddressDto>>("application/json");

    info->addResponse<Object<AddressDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");

    info->pathParams["id"].description = "Address Identifier";
  }
  ENDPOINT("PUT", "addresses/{id}", putAddress,
           PATH(Int32, id),
           BODY_DTO(Object<AddressDto>, addressDto ))
  {
    addressDto->id = id;
    return createDtoResponse(Status::CODE_200, m_userService.updateAddress(addressDto) );
  }
  
  
  ENDPOINT_INFO(getAddressById) {
    info->summary = "Get one Address by id";
    info->addTag("Addresses");

    info->addResponse<Object<AddressDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");

    info->pathParams["id"].description = "Address Identifier";
  }
  ENDPOINT("GET", "addresses/{id}", getAddressById,
           PATH(Int32, id))
  {
    return createDtoResponse(Status::CODE_200, m_userService.getAddressById(id));
  }
  
  ENDPOINT_INFO(getAddressByUserId) {
    info->summary = "Get Addresses by User id";
    info->addTag("Addresses");
    info->addResponse<Object<AddressDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");

    info->pathParams["id"].description = "User Identifier";
  }
  ENDPOINT("GET", "addresses/by_user/{id}", getAddressByUserId,
           PATH(Int32, id))
  {
    return createDtoResponse(Status::CODE_200, m_userService.getAddressByUserId(id));
  }
  
  ENDPOINT_INFO(deleteAddress) {
    info->summary = "Delete Address by id";
    info->addTag("Addresses");

    info->addResponse<Object<StatusDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");

    info->pathParams["id"].description = "Address Identifier";
  }
  ENDPOINT("DELETE", "addresses/{id}", deleteAddress,
           PATH(Int32, id))
  {
    return createDtoResponse(Status::CODE_200, m_userService.deleteAddressById(id));
  }

};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen