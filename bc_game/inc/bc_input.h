#pragma once

#include "bc_ints.h"

class bc_input
{
private:
	struct bc_key
	{
		i32 key_code;
		i32 key_state;
		i32 mods;
	};

	struct bc_key_detail
	{
		bc_key_detail *next;
		bc_key         key;
	};

	struct bc_action_key
	{
		bc_action_key *next;
		const char    *msg;
		i32            key_code;
		i32            key_state;
		i32            key_mods;
	};

	struct bc_axis_key
	{
		bc_axis_key *next;
		const char  *msg;
		float        scale;
		i32          key_code;
		i32          key_mods;
	};

	struct bc_key_event_queue
	{
		bc_key key[64];
		i32    idx   = 0;
		i32    count = 0;
	};

	bc_key_event_queue event_queue;
	bc_key_detail      *key_map;
	bc_action_key      *actions;
	bc_axis_key        *axis;

public:
};