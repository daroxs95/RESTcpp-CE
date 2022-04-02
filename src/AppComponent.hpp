#pragma once

#include <oatpp/parser/json/mapping/ObjectMapper.hpp>

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include <oatpp/core/macro/component.hpp>

#include "db/db.hpp"      //< User-declared DbClient
#include "oatpp-sqlite/orm.hpp" //< SQLite adapter for oatpp ORM
#include "DatabaseComponent.hpp"
#include "SwaggerComponent.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:


  /**
   * Database component
   */
  DatabaseComponent databaseComponent;
  SwaggerComponent swaggerComponent;


  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared({"localhost", 8000, oatpp::network::Address::IP_4});
  }());

  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  /**
   *  Create ConnectionHandler component which uses Router component to route requests
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    return oatpp::web::server::HttpConnectionHandler::createShared(router);
  }());

  /**
   *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
    return oatpp::parser::json::mapping::ObjectMapper::createShared();
  }());

};