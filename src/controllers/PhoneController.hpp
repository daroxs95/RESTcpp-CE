#pragma once

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include "dto/PhoneDto.hpp"
#include "db/db.hpp"
#include "service/UserService.hpp"


#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen

/**
 * Sample Api Controller.
 */
class PhoneController : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  PhoneController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
private:
  UserService m_userService; // Create user service.
public:
  
  static std::shared_ptr<PhoneController> createShared(
  OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) // Inject objectMapper component here as default parameter
  ){
    return std::make_shared<PhoneController>(objectMapper);
  }


  ENDPOINT_INFO(putPhone) {
    info->summary = "Update Phone by id";
    info->addTag("Phones");

    info->addConsumes<Object<PhoneDto>>("application/json");

    info->addResponse<Object<PhoneDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");

    info->pathParams["id"].description = "Phone Identifier";
  }
  ENDPOINT("PUT", "phones/{id}", putPhone,
           PATH(Int32, id),
           BODY_DTO(Object<PhoneDto>, phoneDto ))
  {
    phoneDto->id = id;
    return createDtoResponse(Status::CODE_200, m_userService.updatePhone(phoneDto) );
  }
  
  
  ENDPOINT_INFO(getPhoneById) {
    info->summary = "Get one Phone by id";
    info->addTag("Phones");

    info->addResponse<Object<PhoneDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");

    info->pathParams["id"].description = "Phone Identifier";
  }
  ENDPOINT("GET", "phones/{id}", getPhoneById,
           PATH(Int32, id))
  {
    return createDtoResponse(Status::CODE_200, m_userService.getPhoneById(id));
  }
  
  ENDPOINT_INFO(getPhoneByUserId) {
    info->summary = "Get Phones by User id";
    info->addTag("Phones");
    info->addResponse<Object<PhoneDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");

    info->pathParams["id"].description = "User Identifier";
  }
  ENDPOINT("GET", "phones/by_user/{id}", getPhoneByUserId,
           PATH(Int32, id))
  {
    return createDtoResponse(Status::CODE_200, m_userService.getPhoneByUserId(id));
  }
  
  ENDPOINT_INFO(deletePhone) {
    info->summary = "Delete Phone by id";
    info->addTag("Phones");

    info->addResponse<Object<StatusDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");

    info->pathParams["id"].description = "Phone Identifier";
  }
  ENDPOINT("DELETE", "phones/{id}", deletePhone,
           PATH(Int32, id))
  {
    return createDtoResponse(Status::CODE_200, m_userService.deletePhoneById(id));
  }

};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen