#pragma once

struct bc_time
{
	double elapsed_time = 0.0f;
	double start_time   = 0.0f;
	double dt           = 0.0f;

	bc_time();
	~bc_time()                          = default;
	bc_time(const bc_time&)             = delete;
	bc_time(const bc_time&&)            = delete;
	bc_time& operator=(const bc_time&)  = delete;
	bc_time& operator=(const bc_time&&) = delete;

	void update();
};