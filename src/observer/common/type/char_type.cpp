/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "common/cast.hpp"
#include "common/lang/comparator.h"
#include "common/lang/exception.h"
#include "common/log/log.h"
#include "common/type/char_type.h"
#include "common/type/attr_type.h"
#include "common/value.h"

int CharType::compare(const Value &left, const Value &right) const
{
  ASSERT(left.attr_type() == AttrType::CHARS && right.attr_type() == AttrType::CHARS, "invalid type");
  return common::compare_string(
      (void *)left.value_.pointer_value_, left.length_, (void *)right.value_.pointer_value_, right.length_);
}

RC CharType::max(const Value &left, const Value &right, Value &result) const{
  string const left_str = left.get_string(), right_str = right.get_string();
  if (compare(left, right) >= 0) {
    result.set_string(left_str.c_str());
  } else {
    result.set_string(right_str.c_str());
  }
  return RC::SUCCESS;
}

RC CharType::min(const Value &left, const Value &right, Value &result) const{
  string const left_str = left.get_string(), right_str = right.get_string();
  if (compare(left, right) < 0) {
    result.set_string(left_str.c_str());
  } else {
    result.set_string(right_str.c_str());
  }
  return RC::SUCCESS;
}

RC CharType::set_value_from_str(Value &val, const string &data) const
{
  val.set_string(data.c_str());
  return RC::SUCCESS;
}

RC CharType::cast_to(const Value &val, AttrType type, Value &result) const
{
  switch (type) {
    case AttrType::INTS: {
      result.set_int(str2int(val.get_string().data()));
    } break;
    case AttrType::FLOATS: {
      result.set_float(str2float(val.get_string().data()));
    } break;
    case AttrType::DATES: {
      try {
        Date d = Date::from_string(val.get_string());
        result.set_date(d);
      } catch (exception const &ex) {
        LOG_WARN("fail to cast value. ex: %s", ex.what());
        return RC::VARIABLE_NOT_VALID;
      }
    } break;
    default: return RC::UNIMPLEMENTED;
  }
  return RC::SUCCESS;
}

int CharType::cast_cost(AttrType type)
{
  if (type == AttrType::CHARS) {
    return 0;
  }
  if (type == AttrType::INTS) {
    return 1;
  }
  if (type == AttrType::FLOATS) {
    return 2;
  }
  if (type == AttrType::DATES) {
    return 3;
  }
  return INT32_MAX;
}

RC CharType::to_string(const Value &val, string &result) const
{
  stringstream ss;
  ss << val.value_.pointer_value_;
  result = ss.str();
  return RC::SUCCESS;
}
