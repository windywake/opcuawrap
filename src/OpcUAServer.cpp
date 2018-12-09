/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. 
 *
 * Copyright (C) 2018 Tobias Klausmann 
 * <tobias.johannes.klausmann@mni.thm.de>
 */

#include "OpcUAServer.h"

namespace n_opcua {

using namespace std;

OpcServerRole OpcUAServer::getRole() {
   return role;
}

void OpcUAServer::setRole(const OpcServerRole value) {
   role = value;
   if (value == OpcServerRole::RoleServer)
      config->applicationDescription.applicationType =
            UA_APPLICATIONTYPE_SERVER;
   if (value == OpcServerRole::RoleClientServer)
      config->applicationDescription.applicationType =
            UA_APPLICATIONTYPE_CLIENTANDSERVER;
}

OpcUAServer::OpcUAServer(uint16_t sport) :
   running(true),
   port(sport),
   server(nullptr) {
   UA_ByteString *cert = nullptr;

   config = UA_ServerConfig_new_minimal(port, cert);

   role = OpcServerRole::RoleServer;
   config->applicationDescription.applicationType = UA_APPLICATIONTYPE_SERVER;
}

OpcUAServer::~OpcUAServer() {
   if (server)
      UA_Server_delete(server);
   UA_ServerConfig_delete(config);
}

void OpcUAServer::run() {
   server = UA_Server_new(config);
   UA_Server_run(server, &running);
}

void OpcUAServer::terminate() {
   running = false;
}

void OpcUAServer::setName(string sname, string slocale) {
   name = sname;
   locale = slocale;
   config->applicationDescription.applicationName =
         UA_LOCALIZEDTEXT_ALLOC((char *)(locale.c_str()),
                                (char *) name.c_str());
}

} /* namespace n_opcua */