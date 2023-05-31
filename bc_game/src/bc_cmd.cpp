#include "../inc/bc_cmd.h"
#include "../inc/bc_mem_tracker.h"

static bc_memory_tracker_s bc_mem("[ BC_CMDS ]");

bc_cmd::~bc_cmd()
{
	cmd* c = list;
	for (; c; c = c->next)
		bc_mem.dealloc(c);
}

void bc_cmd::add_action(const char* msg, action_callback_t action)
{
	cmd* c = list;
	for (; c; c = c->next)
	{
		if (!c->action && !c->msg)
		{
			c->msg    = msg;
			c->action = action;
			c->type   = cmd_e::action;
			return;
		}
	}

	c         = (cmd*)bc_mem.alloc(sizeof(cmd));
	c->msg    = msg;
	c->action = action;
	c->type   = cmd_e::action;
	c->next   = list;
	list      = c;
}

void bc_cmd::replace_action(const char* msg, action_callback_t action)
{
	cmd* c = list;
	for (; c; c = c->next)
	{
		if (strcmp(c->msg, msg) == 0)
		{
			if (c->type == cmd_e::action)
				c->action = action;	
			else
				bc_log::warning("[ bc_cmd::replace_action( %s ) ]: must be action to replace", msg);

			return;
		}
	}

	bc_log::warning("[ bc_cmd::add ]: couldn't find %s cmd to replace consider bc_cmd::add instead", msg);
}

void bc_cmd::add_axis(const char* msg, axis_callback_t axis)
{
	cmd* c = list;
	for (; c; c = c->next)
	{
		if (!c->action && !c->msg)
		{
			c->msg  = msg;
			c->axis = axis;
			c->type = cmd_e::axis;
			return;
		}
	}

	c       = (cmd*)bc_mem.alloc(sizeof(cmd));
	c->msg  = msg;
	c->axis = axis;
	c->type = cmd_e::axis;
	c->next = list;
	list    = c;
}

void bc_cmd::replace_axis(const char* msg, axis_callback_t axis)
{
	cmd* c = list;
	for (; c; c = c->next)
	{
		if (strcmp(c->msg, msg) == 0)
		{
			if (c->type == cmd_e::axis)
				c->axis = axis;
			else
				bc_log::warning("[ bc_cmd::replace_axis( %s ) ]: must be axis to replace", msg);

			return;
		}
	}

	bc_log::warning("[ bc_cmd::add ]: couldn't find %s cmd to replace consider bc_cmd::add instead", msg);
}

void bc_cmd::call(const char* msg, float scale = 0)
{
	cmd* c = list;
	for (; c; c = c->next)
	{
		if (strcmp(c->msg, msg) == 0)
			break;
	}

	if (c)
	{
		if (c->action && c->type == cmd_e::action)
		{
			c->action();
		}
		else if (c->axis && c->type == cmd_e::axis)
		{
			c->axis(scale);
		}
		else
			bc_log::warning("[ bc_cmd::call( %s ) ]: doesn't have callback bound to it", msg);
	}

}

void bc_cmd::show_memory_consumtion()
{
	bc_mem.print_usage();
}