#pragma once

#include <iostream>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp-swagger/Controller.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>


#include "AppComponent.hpp"
#include "controllers/UserController.hpp"
#include "controllers/AddressController.hpp"
#include "controllers/PhoneController.hpp"


void run() {
  /* Create json object mapper */
  AppComponent components;


  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  oatpp::web::server::api::Endpoints docEndpoints;

  docEndpoints.append(router->addController(UserController::createShared())->getEndpoints());
  docEndpoints.append(router->addController(AddressController::createShared())->getEndpoints());
  docEndpoints.append(router->addController(PhoneController::createShared())->getEndpoints());

  router->addController(oatpp::swagger::Controller::createShared(docEndpoints));


  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Print info about server port */
  OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

  /* Run server */
  server.run();
}