#include "../inc/bc_input.h"
#include "../inc/bc_cmd.h"
#include "../inc/bc_mem_tracker.h"

static bc_memory_tracker_s bc_mem("[ BC_INPUT ]");

void bc_input::show_memory_consumtion()
{
	bc_mem.print_usage();
}

void bc_input::bind_action(ekey key, ekeystate state, ekeymod mods, const char* msg)
{

}

void bc_input::bind_axis(ekey key, ekeymod mods, const char* msg, float scale)
{

}

void bc_input::set_cmds(bc_cmd* input_cmds)
{
	cmds = input_cmds;
}

void bc_input::process()
{

}