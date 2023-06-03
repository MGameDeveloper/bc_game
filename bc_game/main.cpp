#include "inc/bc_log.h"
#include "inc/bc_window.h"
#include "inc/bc_render.h"
#include "inc/bc_camera.h"
#include "inc/bc_shader.h"
#include "inc/bc_texture.h"
#include "inc/bc_transform.h"
#include "inc/bc_cmd.h"
#include "inc/bc_input.h"
#include "inc/bc_key.h"
#include "inc/bc_time.h"

int main(int argc, const char** argv)
{
	bc_window window("BattleCity", { 1200, 800 });
	bc_camera camera(1200, 800, -10, 1000);
	bc_shader shader("assets/shaders/default.vrtx", "assets/shaders/default.frag");
	bc_render render(2000);
	render.set_shader(&shader);
	render.set_camera(&camera);

	bc_transform tran;
	bc_texture brick_texture("assets/textures/brick.png");

	while (!window.is_closing())
	{
		bc_window::poll_events();
		
		render.clear(255, 0, 0, 255);

		render.draw_sprite(&tran, { 0, 0 }, { 8, 8 }, &brick_texture, 0);

		render.submit();

		window.swap_buffers();
	}

	return 0;
}