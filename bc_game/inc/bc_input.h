#pragma once

#include "bc_ints.h"
#include "bc_key.h"

struct bc_cmd;

class bc_input
{
private:
	struct bc_key
	{
		ekey      code;
		ekeystate state;
		ekeymod   mods;
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
		bc_key         key;
	};

	struct bc_axis_key
	{
		bc_axis_key *next;
		const char  *msg;
		float        scale;
		struct
		{
			ekey         code;
			ekeymod      mods;
		}key;
	};

	struct bc_key_event_queue
	{
		bc_key key[64];
		i32    idx   = 0;
		i32    count = 0;
	};

	bc_key_event_queue event_queue;
	bc_key_detail      *key_detail_list;
	bc_action_key      *action_list;
	bc_axis_key        *axis_list;
	bc_cmd             *cmds;

public:
	static void show_memory_consumtion();

	void bind_action(ekey key, ekeystate state, ekeymod mods, const char* msg);
	void bind_axis  (ekey key, ekeymod mods, const char* msg, float scale);

	void set_cmds(bc_cmd* input_cmds);
	void process();
};