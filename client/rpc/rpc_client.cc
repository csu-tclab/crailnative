/*
* Copyright (C) 2015-2018, IBM Corporation
*
* Licensed to the Apache Software Foundation (ASF) under one or more
* contributor license agreements.  See the NOTICE file distributed with
* this work for additional information regarding copyright ownership.
* The ASF licenses this file to You under the Apache License, Version 2.0
* (the "License"); you may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*    http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "rpc_client.h"

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common/byte_buffer.h"
#include "common/crail_constants.h"
#include "crail_store.h"

using namespace std;
using namespace crail;

RpcClient::RpcClient() {
  this->socket_ = socket(AF_INET, SOCK_STREAM, 0);
  this->counter_ = 1;
}

RpcClient::~RpcClient() {}

int RpcClient::Connect(int address, int port) {
  Debug(address, port);

  struct sockaddr_in addr_;
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  memset(&(addr_.sin_zero), 0, 8);
  addr_.sin_addr.s_addr = address;

  if (connect(socket_, (struct sockaddr *)&addr_, sizeof(addr_)) == -1) {
    cout << "Cannot connect to server" << endl;
    return -1;
  }
  return 0;
}

void RpcClient::Debug(int address, int port) {
  cout << "connecting to ";
  int tmp = address;
  unsigned char *_tmp = (unsigned char *)&tmp;
  for (int i = 0; i < 4; i++) {
    unsigned int ch = (unsigned int)_tmp[i];
    cout << ch << ".";
  }
  cout << ", port " << port << endl;
}

int RpcClient::issueRequest(RpcMessage &request,
                            shared_ptr<RpcMessage> response) {
  unsigned long long ticket = counter_++;
  responseMap.insert({ticket, response});
  ByteBuffer buf(1024);

  // narpc header (size, ticket)
  AddNaRPCHeader(buf, request.Size(), ticket);
  // create file request
  request.Header()->Write(buf);

  // issue request
  buf.Flip();
  cout << "rpc::sending buffer size " << buf.remaining() << endl;
  if (SendBytes(buf.get_bytes(), buf.remaining()) < 0) {
    return -1;
  }

  ByteBuffer *payload = request.Payload();
  if (payload) {
    if (SendBytes(payload->get_bytes(), payload->remaining()) < 0) {
      return -1;
    }
  }
  return 0;
}

int RpcClient::pollResponse() {
  // recv resp header
  ByteBuffer buf(1024);
  buf.Clear();
  if (RecvBytes(buf.get_bytes(), kNarpcHeader) < 0) {
    return -1;
  }
  int size = buf.GetInt();
  long long ticket = buf.GetLong();

  // recv resp obj
  buf.Clear();
  if (RecvBytes(buf.get_bytes(), size) < 0) {
    return -1;
  }

  shared_ptr<RpcMessage> response = responseMap[ticket];
  response->Header()->Update(buf);

  ByteBuffer *payload = response->Payload();
  if (payload) {
    if (payload->remaining() != buf.remaining()) {
      return -1;
    }
    if (RecvBytes(payload->get_bytes(), payload->remaining()) < 0) {
      return -1;
    }
  }

  return 0;
}

void RpcClient::AddNaRPCHeader(ByteBuffer &buf, int size,
                               unsigned long long ticket) {
  buf.PutInt(size);
  buf.PutLong(ticket);
}

long long RpcClient::RemoveNaRPCHeader(ByteBuffer &buf) {
  buf.GetInt();
  long long ticket = buf.GetLong();
  return ticket;
}

int RpcClient::SendBytes(unsigned char *buf, int size) {
  int res = send(socket_, buf, (size_t)size, (int)0);
  if (res < 0) {
    return res;
  }
  int remaining = size - res;
  while (remaining > 0) {
    int offset = size - remaining;
    res = send(socket_, buf + offset, (size_t)remaining, (int)0);
    if (res < 0) {
      return res;
    }
    remaining -= res;
  }
  return remaining;
}

int RpcClient::RecvBytes(unsigned char *buf, int size) {
  int res = recv(socket_, buf, (size_t)size, (int)0);
  if (res < 0) {
    return res;
  }
  int remaining = size - res;
  while (remaining > 0) {
    int offset = size - remaining;
    res = recv(socket_, buf + offset, (size_t)remaining, (int)0);
    if (res < 0) {
      return res;
    }
    remaining -= res;
  }
  return remaining;
}
