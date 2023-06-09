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
		ekey           key;
		ekeymod        mods;
		u32            event[3]; // 0 press, 1 release, 2 repeat
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
		i32    count = 64;
	};

	bc_key_event_queue event_queue;
	bc_key_detail      *key_detail_list = NULL;
	bc_action_key      *action_list     = NULL;
	bc_axis_key        *axis_list       = NULL;
	bc_cmd             *cmds            = NULL;


public:
	bc_input(bc_cmd* input_cmds);
	~bc_input();
	
	bc_input()                            = default;
	bc_input(const bc_input&)             = delete;
	bc_input(const bc_input&&)            = delete;
	bc_input& operator=(const bc_input&)  = delete;
	bc_input& operator=(const bc_input&&) = delete;

	static void show_memory_consumtion();

	void bind_action(ekey key, ekeystate state, ekeymod mods, const char* msg);
	void bind_axis  (ekey key, ekeymod mods, const char* msg, float scale);

	void set_cmds(bc_cmd* input_cmds);
	void process();
	
	void on_key(ekey key, ekeystate state, ekeymod mods);

	// to make all data structures contigous instead of link list
	// void alloc_keys_mem(u32 keys_count);
	// void alloc_action_mem(u32 action_count);
	// void alloc_axis_mem(u32 axis_count);
	// void alloc_event_queue_mem(u32 event_count);
};