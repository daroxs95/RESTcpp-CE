#pragma once

#include <string>

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include "dto/UserDto.hpp"
#include "db/db.hpp"
#include "service/UserService.hpp"


#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen

/**
 * Sample Api Controller.
 */
class UserController : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  UserController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
private:
  UserService m_userService; // Create user service.
public:
  
  static std::shared_ptr<UserController> createShared(
  OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) // Inject objectMapper component here as default parameter
  ){
    return std::make_shared<UserController>(objectMapper);
  }


  ENDPOINT_INFO(createUser) {
    info->summary = "Create new User";
    info->addTag("Users");

    info->addConsumes<Object<UserDto>>("application/json");

    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");
  }
  ENDPOINT("POST", "users", createUser,
           BODY_DTO(Object<UserDto>, userDto))
  {
    return createDtoResponse(Status::CODE_200, m_userService.createUser(userDto) );
  }

  ENDPOINT_INFO(putUser) {
    info->summary = "Update User by id";
    info->addTag("Users");

    info->addConsumes<Object<UserDto>>("application/json");

    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");

    info->pathParams["id"].description = "User Identifier";
  }
  ENDPOINT("PUT", "users/{userId}", putUser,
           PATH(Int32, id),
           BODY_DTO(Object<UserDto>, userDto))
  {
    userDto->id = id;
    return createDtoResponse(Status::CODE_200, m_userService.updateUser(userDto));
  }
  
  
  ENDPOINT_INFO(getUserById) {
    info->summary = "Get one User by id";
    info->addTag("Users");

    info->addResponse<Object<UserDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "text/plain");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");

    info->pathParams["userId"].description = "User Identifier";
  }
  ENDPOINT("GET", "users/{userId}", getUserById,
           PATH(Int32, userId))
  {
    return createDtoResponse(Status::CODE_200, m_userService.getUserById(userId));
  }
  

  ENDPOINT_INFO(getUsers) {
    info->summary = "Get all stored users based on criteria \n";

    info->addTag("Users");

    info->description = "<div>Accepts the optional query parameters:</div>"
                     "<div><b>offset</b> - Numbers of results to skip</div>"
                     "<div><b>limit</b> - Limit on number of results showed</div>"
                     "<div><b>minAge</b> - Minimun age of user</div>"
                     "<div><b>maxAge</b> - Maximun age of user</div>"
                     "<div><b>filterText</b> - Text to search on names and addresses</div>";

    info->addResponse<oatpp::Object<UsersPageDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");

    // TODO this info is not working
    info->queryParams["offset"].description = "Numbers of results to skip";
    info->queryParams["limit"].description = "Limit on number of results showed";
    info->queryParams["minAge"].description = "Minimun age of user";
    info->queryParams["maxAge"].description = "Maximun age of user";
    info->queryParams["filterText"].description = "Text to search on names and addresses";
  }
  ENDPOINT("GET", "users", getUsers,
          //  QUERY(UInt32, offset, "offset", &cero ),
          //  QUERY(UInt32, limit, "limit", "-1" ),
          //  QUERY(UInt32, minAge, "min_age", "-1" ),
          //  QUERY(UInt32, maxAge, "max_age", "-1" ),
          //  QUERY(String, filterText, "filter_text", "" ))
          QUERIES(QueryParams, queryParams))
  {
    int offset = 0, limit = -1, minAge = -1, maxAge = -1; 
    std::string filterText = "";

    for(auto& param : queryParams.getAll()) {
      if(param.first.std_str() == "offset") offset = std::stoi( param.second.std_str() ); 
      if(param.first.std_str() == "limit") limit = std::stoi( param.second.std_str() ); 
      if(param.first.std_str() == "minAge") minAge = std::stoi( param.second.std_str() ); 
      if(param.first.std_str() == "maxAge") maxAge = std::stoi( param.second.std_str() ); 
      if(param.first.std_str() == "maxAge") maxAge = std::stoi( param.second.std_str() ); 
      if(param.first.std_str() == "filterText") filterText = param.second.std_str() ; 
    }
    return createDtoResponse(Status::CODE_200, m_userService.getAllUsers(offset, limit, minAge, maxAge, filterText) );
  }
  
  
  ENDPOINT_INFO(deleteUser) {
    info->summary = "Delete User by id";
    info->addTag("Users");

    info->addResponse<Object<StatusDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "text/plain");

    info->pathParams["id"].description = "User Identifier";
  }
  ENDPOINT("DELETE", "users/{id}", deleteUser,
           PATH(Int32, id))
  {
    return createDtoResponse(Status::CODE_200, m_userService.deleteUserById(id) );
  }

};

#include OATPP_CODEGEN_END(ApiController) ///< End Codegen