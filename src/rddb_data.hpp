/*
 * rddb_data.hpp
 *
 *  Created on: 2013-4-3
 *      Author: wqy
 */

#ifndef RDDB_DATA_HPP_
#define RDDB_DATA_HPP_
#include <stdint.h>
#include <map>
#include <list>
#include <string>
#include "common.hpp"
#include "slice.hpp"
#include "util/buffer_helper.hpp"

namespace rddb
{
	enum KeyType
	{
		KV = 0,
		SET_META = 1,
		SET_ELEMENT = 2,
		ZSET_META = 3,
		ZSET_ELEMENT_SCORE = 4,
		ZSET_ELEMENT = 5,
		HASH_META = 6,
		HASH_FIELD = 7,
		LIST_META = 8,
		LIST_ELEMENT = 9,

	};

	struct KeyObject
	{
			KeyType type;
			Slice key;

			KeyObject(const Slice& k, KeyType T = KV) :
					type(T), key(k)
			{
			}
			virtual ~KeyObject()
			{
			}
	};

	struct ZSetKeyObject: public KeyObject
	{
			Slice value;
			double score;
			ZSetKeyObject(const Slice& k, const Slice& v, double s) :
					KeyObject(k, SET_ELEMENT), value(v), score(s)
			{
			}
	};
	struct ZSetScoreKeyObject: public KeyObject
	{
			Slice value;
			ZSetScoreKeyObject(const Slice& k, const Slice& v) :
					KeyObject(k, ZSET_ELEMENT_SCORE), value(v)
			{
			}
	};

	struct ZSetMetaValue
	{
			uint32_t size;
			ZSetMetaValue() :
					size(0)
			{
			}
	};

	struct SetKeyObject: public KeyObject
	{
			Slice value;
			SetKeyObject(const Slice& k, const Slice& v) :
					KeyObject(k, SET_ELEMENT), value(v)
			{
			}
	};

	struct SetMetaValue
	{
			uint32_t size;
			SetMetaValue() :
					size(0)
			{
			}
	};

	struct HashKeyObject: public KeyObject
	{
			Slice field;
			HashKeyObject(const Slice& k, const Slice& f) :
					KeyObject(k, HASH_FIELD), field(f)
			{
			}
	};

	struct ListKeyObject: public KeyObject
	{
			double score;
			ListKeyObject(const Slice& k, double s) :
					KeyObject(k, LIST_ELEMENT), score(s)
			{
			}
	};

	struct ListMetaValue
	{
			uint32_t size;
			double min_score;
			double max_score;
			ListMetaValue() :
					size(0), min_score(0), max_score(0)
			{
			}
	};

	enum ValueDataType
	{
		EMPTY = 0, INTEGER = 1, DOUBLE = 2, RAW = 3
	};

	enum OperationType
	{
		NOOP = 0, ADD = 1
	};

	struct ValueObject
	{
			uint8_t type;
			union
			{
					int64_t int_v;
					double double_v;
					Buffer* raw;
			} v;
			uint64_t expire;
			ValueObject() :
					type(RAW), expire(0)
			{
				v.int_v = 0;
			}
			void Clear()
			{
				if (type == RAW && NULL != v.raw)
				{
					DELETE(v.raw);
				}
				v.int_v = 0;
			}
			~ValueObject()
			{
				if (type == RAW && v.raw != NULL)
				{
					delete v.raw;
				}
			}
	};

	void encode_key(Buffer& buf, const KeyObject& key);
	KeyObject* decode_key(const Slice& key);

	void encode_value(Buffer& buf, const ValueObject& value);
	bool decode_value(Buffer& buf, ValueObject& value);
	void fill_value(const Slice& value, ValueObject& valueobject);
	int value_convert_to_raw(ValueObject& v);
	int value_convert_to_number(ValueObject& v);

}

#endif /* RDDB_DATA_HPP_ */
