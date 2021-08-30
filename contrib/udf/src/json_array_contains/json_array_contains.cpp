// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "json_array_contains.h"
#include "gutil/strings/split.h"
#include "common/logging.h"
#include <vector>
#include <string>

namespace doris_udf {
using std::string;
using std::vector;

BooleanVal jsonArrayContains(FunctionContext* context, const BooleanVal& contains, const StringVal& origin, int num_args, const StringVal* args) {
    if (origin.is_null) {
        return BooleanVal::null();
    }
    auto in_string_vec = [] (const StringVal& arg, const std::vector<std::string>& res) {
        for (auto& str : res) {
            StringVal val((uint8_t*) str.data(), str.size());
            if (val == arg) return true;
        }
        return false;
    };
    StringPiece str((char*) origin.ptr + 1, origin.len - 2);
    vector<string> res = strings::Split(str, ",", strings::SkipWhitespace());
    for (int i = 0; i < num_args; i++) {
        if (contains.val && !in_string_vec(args[i], res)) {
            return false;
        }
        if (!contains.val && in_string_vec(args[i], res)) {
            return false;
        }
    }
    return true;
}
}

