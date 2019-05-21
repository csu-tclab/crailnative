/*
 * CppCrail: Native Crail
 *
 * Author: Patrick Stuedi  <stu@zurich.ibm.com>
 *
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

#include "crail/client/namenode/getblock_response.h"

GetblockResponse::GetblockResponse() : NamenodeResponse() {}

GetblockResponse::~GetblockResponse() {}

int GetblockResponse::Write(NetworkStream &stream) const {
  NamenodeResponse::Write(stream);

  /*
block_info_.Write(buf);
buf.PutShort(error_);
  */

  return 0;
}

int GetblockResponse::Update(NetworkStream &stream) {
  NamenodeResponse::Update(stream);

  /*
block_info_.Update(buf);
this->error_ = buf.GetShort();
  */

  return 0;
}
