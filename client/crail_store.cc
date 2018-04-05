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

#include "crail_store.h"

#include <arpa/inet.h>
#include <iostream>
#include <math.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common/crail_constants.h"
#include "directory_record.h"
#include "metadata/filename.h"
#include "storage/storage_client.h"
#include "storage/write_request.h"
#include "storage/write_response.h"

using namespace crail;

CrailStore::CrailStore() : namenode_client_(new NamenodeClient()) {}

CrailStore::~CrailStore() {}

int CrailStore::Initialize(string address, int port) {
  return this->namenode_client_->Connect((int)inet_addr(address.c_str()), port);
}

unique_ptr<CrailNode> CrailStore::Create(string &name, FileType type) {
  Filename filename(name);
  auto create_res =
      namenode_client_->Create(filename, static_cast<int>(type), 0, 0, 1);

  cout << "starging dir record writing " << endl;
  string fname = filename.name();
  if (create_res->file()->dir_offset() >= 0) {
    auto directory_stream = make_unique<CrailOutputstream>(
        this->namenode_client_, create_res->parent(),
        create_res->file()->dir_offset());
    DirectoryRecord record(1, fname);
    shared_ptr<ByteBuffer> buf = make_shared<ByteBuffer>(1024);
    record.Write(*buf);
    buf->Flip();
    cout << "sending dir record, remaining " << buf->remaining() << endl;
    directory_stream->Write(buf);
  }

  auto crail_node = make_unique<CrailNode>(create_res->file());
  return crail_node;
}

unique_ptr<CrailNode> CrailStore::Lookup(string &name) { return nullptr; }

int CrailStore::Remove(string &name) { return 0; }
