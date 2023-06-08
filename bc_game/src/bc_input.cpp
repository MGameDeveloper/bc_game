#include "../inc/bc_input.h"
#include "../inc/bc_cmd.h"
#include "../inc/bc_mem_tracker.h"

static bc_memory_tracker_s bc_mem("[ BC_INPUT ]");

void bc_input::show_memory_consumtion()
{
	bc_mem.print_usage();
}

bc_input::bc_input(bc_cmd* input_cmds)
{
	cmds = input_cmds;
}

bc_input::~bc_input()
{
	bc_key_detail *key_detail = key_detail_list;
	bc_action_key *action     = action_list;
	bc_axis_key   *axis       = axis_list;

	for (; key_detail; key_detail = key_detail_list)
	{
		key_detail_list = key_detail->next;
		bc_mem.dealloc(key_detail);
	}

	for (; action; action = action_list)
	{
		action_list = action->next;
		bc_mem.dealloc(action);
	}

	for (; axis; axis = axis_list)
	{
		axis_list = axis->next;
		bc_mem.dealloc(axis);
	}
}

void bc_input::bind_action(ekey key, ekeystate state, ekeymod mods, const char* msg)
{
	bc_action_key *action = action_list;
	for (; action; action = action->next)
	{
		if (action->key.code == key &&
			action->key.state == state &&
			action->key.mods == mods)
		{
			bc_log::info("%s input msg has been replaced by %s", action->msg, msg);
			action->msg = msg;
			return;
		}
	}

	action            = (bc_action_key*)bc_mem.alloc(sizeof(bc_action_key));
	action->key.code  = key;
	action->key.state = state;
	action->key.mods  = mods;
	action->msg       = msg;
	action->next      = action_list;
	action_list       = action;

	bc_log::trace("Action added (%d, %d, %d, %s)", key, state, mods, msg);
}

void bc_input::bind_axis(ekey key, ekeymod mods, const char* msg, float scale)
{
	bc_axis_key *axis = axis_list;
	for (; axis; axis = axis->next)
	{
		if (axis->key.code == key &&
			axis->key.mods == mods)
		{
			bc_log::info("%s axis msg has been replaced by %s", axis->msg, msg);
			axis->msg   = msg;
			axis->scale = scale;
			return;
		}
	}

	axis           = (bc_axis_key*)bc_mem.alloc(sizeof(bc_axis_key));
	axis->key.code = key;
	axis->key.mods = mods;
	axis->scale    = scale;
	axis->msg      = msg;
	axis->next     = axis_list;
	axis_list      = axis;

	bc_log::trace("Axis added (%d, %d, %s, %.2f)", key, mods, msg, scale);
}

void bc_input::set_cmds(bc_cmd* input_cmds)
{
	cmds = input_cmds;
}

void bc_input::process()
{
	if (!cmds)
		return;

	struct action_event
	{
		const char *msg = NULL;
	};

	struct axis_event
	{
		const char     *msg   = NULL;
		float           scale = 0.0f;
	};

	static i32 action_idx = 0;
	static i32 axis_idx   = 0;
	static action_event actions[256];
	static axis_event   axes[256];
	bc_mem.zero(actions, sizeof(action_event) * 256);
	bc_mem.zero(axes,    sizeof(axis_event)   * 256);
	action_idx = 0;
	axis_idx   = 0;

	bc_key        *key        = NULL;
	bc_action_key *action     = action_list;
	bc_axis_key   *axis       = axis_list;
	bc_key_detail *key_detail = key_detail_list;

	// Collect Actions
	for (i32 idx = 0; idx < event_queue.idx; ++idx)
	{
		key = &event_queue.key[idx];

		for (; action; action = action->next)
		{
			if (key->code == action->key.code &&
				key->mods == action->key.mods)
			{
				if (key->state == ekeystate_Press && action->key.state == ekeystate_Repeat)
				{
					actions[action_idx].msg = action->msg;
					action_idx++;
				}
				else if (key->state == action->key.state)
				{
					actions[action_idx].msg = action->msg;
					action_idx++;
				}	
			}
		}
	}

	// Collect Axes
	// Revers the loops to minimize the iteration
	for (; key_detail; key_detail = key_detail->next)
	{
		if (key_detail->key.state == ekeystate_Unknown)
			continue;

		for (; axis; axis = axis->next)
		{
			if (key_detail->key.code == axis->key.code &&
				key_detail->key.mods == axis->key.mods &&
				key_detail->key.state > ekeystate_Unknown)
			{
				// Accumulate axis scale value
				bool axis_found = false;
				for (i32 idx = 0; idx < axis_idx; ++idx)
				{
					bc_log::warning("looking for same axis");
					if (strcmp(axis->msg, axes[idx].msg) == 0)
					{
						axis_found = true;
						axes[idx].scale += axis->scale;
						break;
					}
				}

				if (!axis_found)
				{
					if (axis_idx + 1 < 256)
					{
						axes[axis_idx].msg = axis->msg;
						axes[axis_idx].scale = axis->scale;
						axis_idx++;
					}
					else
						bc_log::error("[ bc_input::process() ]: axis pool out of range [ %s ]", axis->msg);
				}
			}
		}

		if (key_detail->key.state == ekeystate_Release)
		{
			key_detail->key.state = ekeystate_Unknown;
		}
	}


	for (i32 idx = 0; idx < action_idx; ++idx)
	{
		cmds->call(actions[idx].msg);
	}

	for (i32 idx = 0; idx < axis_idx; ++idx)
	{
		cmds->call(axes[idx].msg, axes[idx].scale);
	}

	bc_mem.zero(event_queue.key, sizeof(bc_key)* event_queue.count);
	event_queue.idx = 0;
}

void bc_input::on_key(ekey key, ekeystate state, ekeymod mods)
{
	bc_key_detail *key_detail = key_detail_list;
	for (; key_detail; key_detail = key_detail->next)
	{
		if (key_detail->key.code == key)
		{
			key_detail->key.state = state;
			key_detail->key.mods  = mods;

			if (event_queue.idx < event_queue.count)
			{
				bc_key* key_event = &event_queue.key[event_queue.idx++];
				key_event->code   = key;
				key_event->state  = state;
				key_event->mods   = mods;
			}
			else
				bc_log::warning("[ bc_input::on_key ]: event_queue out of range");

			return;
		}
	}

	key_detail            = (bc_key_detail*)bc_mem.alloc(sizeof(bc_key_detail));
	key_detail->key.code  = key;
	key_detail->key.state = state;
	key_detail->key.mods  = mods;
	key_detail->next      = key_detail_list;
	key_detail_list       = key_detail;

	if (event_queue.idx < event_queue.count)
	{
		bc_key* key_event = &event_queue.key[event_queue.idx++];
		key_event->code   = key;
		key_event->state  = state;
		key_event->mods   = mods;
	}
	else
		bc_log::warning("[ bc_input::on_key ]: event_queue out of range");
}