#pragma once

#include "bc_ints.h"

typedef void(*action_callback_t)(void);
typedef void(*axis_callback_t)(float);

class bc_cmd
{
private:
	enum class cmd_e { action, axis };

	struct cmd
	{
		cmd* next;
		
		union
		{
			action_callback_t action = NULL;
			axis_callback_t   axis;
		};

		const char *msg;
		cmd_e       type;
	};

	cmd* list = NULL;

public:
	bc_cmd()                          = default;
	bc_cmd(const bc_cmd&)             = delete;
	bc_cmd(const bc_cmd&&)            = delete;
	bc_cmd& operator=(const bc_cmd&)  = delete;
	bc_cmd& operator=(const bc_cmd&&) = delete;
	~bc_cmd();

	void add_action    (const char* msg, action_callback_t action);
	void replace_action(const char* msg, action_callback_t action);

	void add_axis    (const char* msg, axis_callback_t axis);
	void replace_axis(const char* msg, axis_callback_t axis);

	void call(const char* msg, float scale = 0);

	static void show_memory_consumtion();
};